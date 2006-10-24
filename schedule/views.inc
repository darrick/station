<?php

// $Id$

function station_schedule_views_tables() {
  $tables['station_playlist'] = array(
    'name' => 'station_playlist',
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
      'timestamp' => array(
        'name' => t('Station Playlist: Date'),
        'sortable' => TRUE,
        'handler' => array(
          'station_handler_field_station_playlist_timestamp' => t("As link"),
          'station_handler_field_station_playlist_timestamp_nl' => t("Without link"),
        ),
        'help' => t('Date of the program.'),
      ),
    ),
    'sorts' => array(
      'timestamp' => array(
        'name' => t('Station Playlist: Date'),
        'help' => t("Sort by the playlist's air date."),
       ),
    ),
    'filters' => array(
      'timestamp' => array(
        'name' => t('Station Playlist: Date'),
        'operator' => 'views_handler_operator_gtlt',
        'value' => views_handler_filter_date_value_form(),
        'handler' => 'views_handler_filter_timestamp',
        'help' => t('Filter depending on the date the program has/will air. '),
      ),
    ),

  );
  $tables['station_playlist_program_node'] = array(
    'name' => 'node',
    'provider' => 'internal',
    'join' => array(
      'left' => array(
        'table' => 'station_playlist',
        'field' => 'program_nid'
      ),
      'right' => array(
        'field' => 'nid'
      )
    ),
    'fields' => array(
      'title' => array(
        'name' => t('Station Playlist: Program Title'),
        'sortable' => TRUE,
        'handler' => array(
          'station_handler_field_station_playlist_program_title'    => t('As Link'),
          'station_handler_field_station_playlist_program_title_nl' => t('Without Link'),
        ),
        'addlfields' => array('nid'),
        'help' => t('Title of the program.'),
      ),
    ),
  );


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

function station_schedule_views_arguments() {
  $arguments = array(
    'program_nid' => array(
      'name' => t('Station Playlist: Program NID'),
      'handler' => 'station_schedule_playlist_arg_program_nid_handler',
      'help' => t('The Program NID argument allows visitors to filter playlists to those attached to the specified program.'),
    ),
  );
  return $arguments;
}


/*
 * Handle the program node id argument.
 */
function station_schedule_playlist_arg_program_nid_handler($op, &$query, $argtype, $arg = '') {
  switch($op) {
    case 'summary':
      $query->ensure_table('station_playlist', true);
      $query->ensure_table('station_playlist_program_node', true);
      $query->add_field('program_nid', 'station_playlist');
      $query->add_field('title', 'station_playlist_program_node');
      $fieldinfo['field'] = "station_playlist_program_node.title";
      return $fieldinfo;
    case 'sort':
      $query->add_orderby('station_playlist_program_node', 'title', $argtype);
      break;
    case 'filter':
      $query->ensure_table("station_playlist");
      $query->add_where("station_playlist.program_nid = %d", $arg);
      break;
    case 'link':
      return l($query->title, "$arg/" . intval($query->program_nid));
    case 'title':
      $node = db_fetch_object(db_query("SELECT title FROM {node} WHERE nid=%d", $query));
      return check_plain($node->title);
  }
}


function station_handler_field_station_program_url($fieldinfo, $fielddata, $value, $data) {
  return l($value, $value, NULL, NULL, TRUE);
}
function station_handler_field_station_program_url_nl($fieldinfo, $fielddata, $value, $data) {
  return check_plain($value);
}

function station_handler_field_station_playlist_program_title($fieldinfo, $fielddata, $value, $data) {
  return l($value, "node/$data->station_playlist_program_node_nid");
}
function station_handler_field_station_playlist_program_title_nl($fieldinfo, $fielddata, $value, $data) {
  return check_plain($value);
}

function station_handler_field_station_playlist_timestamp($fieldinfo, $fielddata, $value, $data) {
  return l(format_date($value, 'custom', 'j-n-y'), "node/$data->nid");
}
function station_handler_field_station_playlist_timestamp_nl($fieldinfo, $fielddata, $value, $data) {
  return format_date($value, 'custom', 'j-n-y');
}

function station_handler_field_station_program_djs($fieldinfo, $fielddata, $value, $data) {
  return implode(', ', station_schedule_program_get_themed_djs($data->nid));
}