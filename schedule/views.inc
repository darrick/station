<?php

// $Id$

/**
 * Implementation of hook_views_tables().
 */
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
        'field' => 'nid'
      )
    ),
    'fields' => array(
      'streams' => array(
        'name' => t('Station Schedule: Web streams'),
        'help' => t("Web stream links"),
        'sortable' => FALSE,
        'handler' => 'station_handler_field_schedule_streams',
      ),
    ),
  );
  $tables['station_schedule_item_current'] = array(
    'name' => 'station_schedule_item',
    'join' => array(
      'left' => array(
        'table' => 'station_schedule',
        'field' => 'nid',
      ),
      'right' => array(
        'field' => 'schedule_nid'
      ),
      'extra' => array(
        'start <= ***CURRENT_STATION_MINUTE***' => NULL,
        'finish > ***CURRENT_STATION_MINUTE***' => NULL,
      ),
    ),
    'fields' => array(
      'program_nid' => array(
        'name' => t('Station Schedule: Current program'),
        'help' => t("The currently schedule program."),
        'sortable' => TRUE,
        'handler' => 'station_handler_field_schedule_current_program',
        'addlfields' => array('iid', 'start', 'finish'),
      ),
    ),
  );
  $tables['station_schedule_item'] = array(
    'name' => 'station_schedule_item',
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
        'name' => t('Station Schedule: Scheduled times'),
        'help' => t("The time a program begins."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_schedule_times',
      ),
      'archive_links' => array(
        'name' => t('Station Schedule: Archive listen and RSS links'),
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
        'name' => t('Station Schedule: Archive listen link'),
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
        'name' => t('Station Schedule: Archive RSS link'),
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

function station_handler_field_schedule_current_program($fieldinfo, $fielddata, $value, $data) {
  if (isset($data->station_schedule_item_current_program_nid)) {
    if ($program = node_load($data->station_schedule_item_current_program_nid)) {
      return l($program->title, "node/$program->nid");
    }
  }
  else {
    return check_plain(variable_get('station_block_unschedule',  t("We're on autopilot.")));
  }
}

function station_handler_field_schedule_streams($fieldinfo, $fielddata, $value, $data) {
  if ($streams = unserialize($data->station_schedule_streams)) {
    return theme('station_streams', $data->nid, $streams);
  }
}

function station_handler_field_schedule_times($fieldinfo, $fielddata, $value, $data) {
  if ($program = node_load($data->nid)) {
    $scheduled = array();
    $schedules = station_schedule_get_list();

    if (count($schedules) == 1) {
      foreach (station_schedule_program_get_themed_times($program) as $schedule_nid => $times) {
        $scheduled[] = t('@times', array('@times' => station_anded_list($times)));
      }
    }
    else {
      foreach (station_schedule_program_get_themed_times($program) as $schedule_nid => $times) {
        $scheduled[] = t('On @title @times', array('@title' => $schedules[$schedule_nid]['title'], '@times' => station_anded_list($times)));
      }
    }
    return station_anded_list($scheduled);
  }
}

function station_handler_field_schedule_djs($fieldinfo, $fielddata, $value, $data) {
  if ($program = node_load($data->nid)) {
    return station_anded_list(station_schedule_program_get_themed_djs($program));
  }
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
    $query->ensure_table('station_schedule_item');
    if ($filterdata['operator']) {
      $query->add_where('{station_schedule_item}.program_nid IS NOT NULL');
    }
    else {
      $query->add_where('{station_schedule_item}.program_nid IS NULL');
    }
    break;
  }
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
    $query->add_where('EXISTS (SELECT * FROM {station_schedule_item} WHERE {station_schedule_item}.program_nid = node.nid AND {station_schedule_item}.start <= %d AND {station_schedule_item}.finish > %d)', $minute, $minute);
    break;
  }
}
