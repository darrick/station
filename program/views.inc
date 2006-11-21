<?php

// $Id$

function station_program_views_tables() {
  $tables['station_program'] = array(
    'name' => 'station_program',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'vid'
      ),
      'right' => array(
        'field' => 'vid'
      )
    ),
    'fields' => array(
      'genre' => array(
        'name' => t('Station Program: Genre'),
        'sortable' => TRUE,
        'help' => t('Freeform genre description.'),
      ),
      'url' => array(
        'name' => t('Station Program: URL'),
        'sortable' => FALSE,
        'help' => t('Link to external website.'),
        'handler' => array(
          'station_handler_field_station_program_url'    => t('As Link'),
          'station_handler_field_station_program_url_nl' => t('Without Link'),
        ),
      ),
    ),
  );

  return $tables;
}

function station_handler_field_station_program_url($fieldinfo, $fielddata, $value, $data) {
  return l($value, $value, NULL, NULL, TRUE);
}
function station_handler_field_station_program_url_nl($fieldinfo, $fielddata, $value, $data) {
  return check_plain($value);
}
