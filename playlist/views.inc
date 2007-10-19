<?php

// $Id$

/**
 * Implementation of hook_views_tables().
 */
function station_playlist_views_tables() {
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
          'station_handler_field_playlist_timestamp_title' => t('Playlist title date format'),
          'station_handler_field_playlist_timestamp_program' => t('Program node date format'),
        ),
        'option' => array(
           '#type' => 'select',
           '#options' => array(
             'listlink' => 'As link to playlist',
             'nolink' => 'Without link'
           ),
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
        'help' => t('Filter depending on the date the program has/will air.'),
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
        'handler' => 'station_handler_field_playlist_program_title',
        'option' => array(
           '#type' => 'select',
           '#options' => array(
             'proglink' => 'As link to program',
             'listlink' => 'As link to playlist',
             'nolink' => 'Without link'
           ),
        ),
        'addlfields' => array('nid'),
        'help' => t('Title of the program.'),
      ),
    ),
  );

  return $tables;
}

/**
 * Implementation of hook_views_arguments().
 */
function station_playlist_views_arguments() {
  $arguments = array(
    'program_nid' => array(
      'name' => t('Station Playlist: Program NID'),
      'handler' => 'station_playlist_arg_program_nid_handler',
      'help' => t('The Program NID argument allows visitors to filter playlists to those attached to the specified program.'),
    ),
  );
  return $arguments;
}


/*
 * Handle the program node id argument.
 */
function station_playlist_arg_program_nid_handler($op, &$query, $argtype, $arg = '') {
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

function station_handler_field_playlist_program_title($fieldinfo, $fielddata, $value, $data) {
  if ($fielddata['options'] == 'proglink') {
    return l($value, "node/$data->station_playlist_program_node_nid");
  }
  if ($fielddata['options'] == 'listlink') {
    return l($value, "node/$data->nid");
  }
  return check_plain($value);
}

function station_handler_field_playlist_timestamp_title($fieldinfo, $fielddata, $value, $data) {
  $date = format_date($value, 'custom', variable_get('station_playlist_title_dateformat', 'm/d/Y'));
  if ($fielddata['options'] == 'listlink') {
    return l($date, "node/$data->nid");
  }
  return $date;  
}

function station_handler_field_playlist_timestamp_program($fieldinfo, $fielddata, $value, $data) {
  $date = format_date($value, 'custom', variable_get('station_playlist_program_dateformat', 'F j, Y'));
  if ($fielddata['options'] == 'listlink') {
    return l($date, "node/$data->nid");
  }
  return $date;  
}