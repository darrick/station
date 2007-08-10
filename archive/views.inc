<?php

// $Id$

function station_archive_views_tables() {
  $tables['station_archive'] = array(
    'name' => 'station_archive',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'audio_nid'
      )
    ),
    'fields' => array(
      'imported' => array(
        'name' => t('Station Archive: Import Date'),
        'sortable' => TRUE,
        'handler' => views_handler_field_dates(),
        'option' => 'string',
        'help' => t('Date the audio was imported into the station archive.'),
      ),
    ),
    'filters' => array(
      'permanent' => array(
        'name' => t('Station Archive: Is in permanent archive'),
        'operator' => array('=' => 'Equals'),
        'list' => 'views_handler_operator_yesno',
        'list-type' => 'select',
        'help' => t('Filter weather the node is part of the permanent archive.'),
      ),
    ),
  );
  $tables['station_archive_program'] = array(
    'name' => 'station_archive_program',
    'join' => array(
      'left' => array(
        'table' => 'station_archive',
        'field' => 'program_nid'
      ),
      'right' => array(
        'field' => 'program_nid'
      )
    ),
    'fields' => array(
      'title' => array(
        'name' => t('Station Archive: Program Title'),
        'sortable' => TRUE,
        'handler' => array(
          'station_archive_handler_audio_program_title_field' => t("As link"),
          'station_archive_handler_audio_program_title_field_nl' => t("Without link"),
        ),
        'help' => t('The program that this audio is a recording of.'),
      ),
    ),
  );

  return $tables;
}

function station_archive_views_arguments() {
  $arguments = array(
    'station_archive_program_nid' => array(
      'name' => t('Station Archive: Audio by Program'),
      'handler' => 'station_archive_arg_program_nid_handler',
      'help' => t('The Program NID argument allows visitors to filter archived audio to those of the specified program.'),
    ),
  );
  return $arguments;
}


/*
 * Handle the program node id argument.
 */
function station_archive_arg_program_nid_handler($op, &$query, $argtype, $arg = '') {
  switch($op) {
    case 'summary':
      // add these up front to avoid a wacky bug with rewriting
      $query->ensure_table('node');
      $query->add_field('nid', 'node');
      // now onto our regularly scheduled program
      $query->ensure_table('station_archive');
      $query->ensure_table('station_archive_program');
      $query->add_field('title', 'station_archive_program');
      $query->add_field('program_nid', 'station_archive_program');
      $query->add_where('station_archive_program.program_nid IS NOT NULL');
      $fieldinfo['field'] = "station_archive_program.title";
      return $fieldinfo;
    case 'sort':
      $query->ensure_table('station_archive_program');
      $query->add_orderby('station_archive_program', 'title', $argtype);
      break;
    case 'filter':
      $query->ensure_table("station_archive");
      $query->add_where("station_archive.program_nid = %d", $arg);
      break;
    case 'link':
      return l($query->title, "$arg/" . intval($query->program_nid));
    case 'title':
      $node = db_fetch_object(db_query("SELECT sap.title FROM {station_archive_program} sap WHERE sap.program_nid=%d", $query));
      return check_plain($node->title);
  }
}

function station_archive_handler_timestamp_field($fieldinfo, $fielddata, $value, $data) {
  return l(format_date($value, 'custom', 'j-n-y'), "node/$data->nid");
}

function station_archive_handler_audio_program_title_field($fieldinfo, $fielddata, $value, $data) {
  return l($value, "node/$data->station_archive_program_nid");
}
function station_archive_handler_audio_program_title_field_nl($fieldinfo, $fielddata, $value, $data) {
  return check_plain($value);
}