<?php

// $Id$

function station_schedule_views_tables() {
  $tables['station_djs'] = array(
    'name' => 'station_djs',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'nid'
      )
    ),
    'fields' => array(
      'djs' => array(
        'name' => t('Station Schedule: DJs'),
        'help' => t("The program's DJs."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_schedule_djs',
      ),
    ),
  );

  $tables['station_schedule'] = array(
    'name' => 'station_schedule',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'program_nid'
      )
    ),
    'fields' => array(
      'times' => array(
        'name' => t('Station: Scheduled times'),
        'help' => t("The time a program begins."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_schedule_times',
      ),
      'archive_links' => array(
        'name' => t('Station: Archive listen and RSS links'),
        'help' => t("Links to both a page and feed for the program's archived audio."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_listen_links',
        'option' => array(
           '#type' => 'select',
           '#options' => array(
             'icon' => t('Icon'),
             'icon_text' => t('Icon and text'),
            ),
        ),
      ),
      'archive_link_page' => array(
        'name' => t('Station: Archive listen link'),
        'help' => t("Link to the program's archived audio page."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_listen_link_page',
        'option' => array(
           '#type' => 'select',
           '#options' => array(
             'icon' => t('Icon'),
             'icon_text' => t('Icon and text'),
            ),
        ),
      ),
      'archive_link_rss' => array(
        'name' => t('Station: Archive RSS link'),
        'help' => t("Link to the program's archived audio feed."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_listen_link_rss',
        'option' => array(
           '#type' => 'select',
           '#options' => array(
             'icon' => t('Icon'),
             'icon_text' => t('Icon and text'),
            ),
        ),
      ),
    ),
    'filters' => array(
      'is_scheduled' => array(
        'name' => t('Station: On the schedule'),
        'operator' => 'views_handler_operator_yesno',
        'handler' => 'station_handler_filter_is_scheduled',
        'help' => t("Filter weather the node is on the station's schedule."),
      ),
      'now_playing' => array(
        'name' => t('Station: Now playing'),
        'cacheable' => FALSE,
        'operator' => array('IS' => t('now playing')),
        'value' => '',
        'handler' => 'station_handler_filter_now_playing',
        'help' => t("Filter down to the program that is currently being broadcast."),
      ),
    ),
  );

  return $tables;
}

function station_handler_field_schedule_times($fieldinfo, $fielddata, $value, $data) {
  return station_anded_list(station_schedule_program_get_themed_times($data->nid));
}

function station_handler_field_schedule_djs($fieldinfo, $fielddata, $value, $data) {
  return station_anded_list(station_schedule_program_get_themed_djs($data->nid));
}

function station_handler_field_listen_links($fieldinfo, $fielddata, $value, $data) {
  $output = '';
  if ($fielddata['options'] == 'icon') {
    foreach(station_schedule_archive_links($data->nid, TRUE) as $link) {
      $output .= l($link['title'], $link['href'], NULL, NULL, NULL, TRUE, TRUE) .' ';
    }
  }
  else {
    $output = theme('links', station_schedule_archive_links($data->nid, FALSE));
  }
  return $output;
}
function station_handler_field_listen_link_page($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, ($fielddata['options'] == 'icon'));
  return l($links['station_archive_listen']['title'], $links['station_archive_listen']['href'], NULL, NULL, NULL, TRUE, TRUE);
}
function station_handler_field_listen_link_rss($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, ($fielddata['options'] == 'icon'));
  return l($links['station_archive_rss']['title'], $links['station_archive_rss']['href'], NULL, NULL, NULL, TRUE, TRUE);
}


function station_handler_filter_is_scheduled($op, $filterdata, $filterinfo, &$query) {
  switch ($op) {
  case 'handler':
    $query->ensure_table('node');
    $query->add_field('nid', 'node');
    if ($filterdata['operator']) {
      $query->add_where('EXISTS (SELECT * FROM {station_schedule} WHERE {station_schedule}.program_nid = node.nid)');
    }
    else {
      $query->add_where('NOT EXISTS (SELECT * FROM {station_schedule} WHERE {station_schedule}.program_nid = node.nid)');
    }
    break;
  }
  return;
}

/**
 * Substitute current time; this works with cached queries.
 */
function station_schedule_views_query_substitutions($view) {
  $timestamp = station_local_ts(time());
  $minute = station_minute_from_day_hour(date('w', $timestamp), date('G', $timestamp));
  return array('***CURRENT_STATION_MINUTE***' => $minute);
}

function station_handler_filter_now_playing($op, $filterdata, $filterinfo, &$query) {
  switch ($op) {
  case 'handler':
    $query->ensure_table('node');
    $query->add_field('nid', 'node');
    $minute = '***CURRENT_STATION_MINUTE***';
    $query->add_where('EXISTS (SELECT * FROM {station_schedule} WHERE {station_schedule}.program_nid = node.nid AND {station_schedule}.start <= %d AND {station_schedule}.finish > %d)', $minute, $minute);
    break;
  }
  return;
}
