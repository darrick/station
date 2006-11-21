<?php

// $Id$

function station_schedule_views_tables() {
  $tables['station_djs'] = array(
    'name' => 'user',
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
        'name' => t('Station Program: DJs'),
        'sortable' => FALSE,
        'help' => t("The program's DJs."),
        'notafield' => TRUE,
        'sortable' => FALSE,
        'handler' => 'station_handler_field_station_program_djs',
      ),
    ),
  );

  return $tables;
}

function station_handler_field_station_program_djs($fieldinfo, $fielddata, $value, $data) {
  return implode(', ', station_schedule_program_get_themed_djs($data->nid));
}
