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
        'name' => t('Station Schedule: Times'),
        'help' => t("The time a program begins."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => 'station_handler_field_schedule_times',
      ),
      'archive_links' => array(
        'name' => t('Station Schedule: Archive listen page and RSS feed links'),
        'help' => t("Links to both a page and feed for the program's archived audio."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => array(
          'station_handler_field_listen_links_short' => t('Icon'),
          'station_handler_field_listen_links_long' => t('Icon and text'),
        ),
      ),
      'archive_link_page' => array(
        'name' => t('Station Schedule: Archive listen page link'),
        'help' => t("Link to the program's archived audio page."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => array(
          'station_handler_field_listen_link_page_short' => t('Icon'),
          'station_handler_field_listen_link_page_long' => t('Icon and text'),
        ),
      ),
      'archive_link_rss' => array(
        'name' => t('Station Schedule: Archive RSS feed link'),
        'help' => t("Link to the program's archived audio feed."),
        'sortable' => FALSE,
        'notafield' => TRUE,
        'handler' => array(
          'station_handler_field_listen_link_rss_short' => t('Icon'),
          'station_handler_field_listen_link_rss_long' => t('Icon and text'),
        ),
      ),
    ),
    'filters' => array(
      'is_scheduled' => array(
        'cacheable' => FALSE, //FOR DEBUGGING
        'name' => t('Station Schedule: Is currently scheduled'),
        'operator' => 'views_handler_operator_yesno',
        'handler' => 'station_handler_filter_is_scheduled',
        'help' => t("Filter weather the node is on the station's schedule."),
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

function station_handler_field_listen_links_short($fieldinfo, $fielddata, $value, $data) {
  $output = '';
  foreach(station_schedule_archive_links($data->nid, TRUE) as $link) {
    $output .= l($link['title'], $link['href'], NULL, NULL, NULL, TRUE, TRUE) .' ';
  }
  return $output;
}
function station_handler_field_listen_link_page_short($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, TRUE);
  return l($links['station_archive_listen']['title'], $links['station_archive_listen']['href'], NULL, NULL, NULL, TRUE, TRUE);
}
function station_handler_field_listen_link_rss_short($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, TRUE);
  return l($links['station_archive_rss']['title'], $links['station_archive_rss']['href'], NULL, NULL, NULL, TRUE, TRUE);
}

function station_handler_field_listen_links_long($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, FALSE);
  return theme('links', $links);
}
function station_handler_field_listen_link_page_long($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, FALSE);
  return l($links['station_archive_listen']['title'], $links['station_archive_listen']['href'], NULL, NULL, NULL, TRUE, TRUE);
}
function station_handler_field_listen_link_rss_long($fieldinfo, $fielddata, $value, $data) {
  $links = station_schedule_archive_links($data->nid, FALSE);
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
