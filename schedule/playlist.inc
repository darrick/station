<?php

// $Id$

function station_schedule_playlist_access($op, $node) {
  global $user;

  if ($op == 'create') {
    return user_access('create playlists');
  }

  if ($op == 'update' || $op == 'delete') {
    if (user_access('edit own playlists') && ($user->uid == $node->uid)) {
      return TRUE;
    }
  }

  return true;
}

/**
 * Build a form for playlist nodes.
 */
function station_schedule_playlist_form(&$node) {
  if (!isset($node->program_title) && isset($node->program_nid)) {
    $program = node_load($node->program_nid);
    $node->program_title = $program->title;
  }

  $form['program_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Program'),
    '#default_value' => $node->program_title,
    '#maxlength' => 128,
    '#autocomplete_path' => 'program/autocomplete',
    '#required' =>  TRUE,
    '#description' => t('The name of the program. If you filled in for someone else, enter the name of their show.'),
  );

  $form['program_date'] = array(
    '#type' => 'date',
    '#title' => t('Program date'),
    '#default_value' => $node->program_date,
    '#required' =>  TRUE,
    '#description' => t('The date the program aired.'),
  );

  $form['body_filter']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#required' => FALSE,
  );
  $form['body_filter']['format'] = filter_form($node->format);

  $form['tracks'] = array(
    '#type' => 'fieldset', '#title' => t('Tracks'),
    '#tree' => TRUE, '#collapsible' => FALSE, '#weight'=>1,
    '#theme' => 'station_schedule_playlist_track_form',
    '#description' => t('Enter the tracks played on the show, the artist and title are required. The weight is used to set the order: lower weights rise, heavier sink. If you want to remove a track just clear the artist and title text boxes.'),
  );

  $weight = 0;
  // load current tracks
  foreach ((array)$node->tracks as $key => $track) {
    $form['tracks'][$weight]['artist'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 128,
      '#default_value' => $track['artist'],
      '#required' => TRUE,
    );
    $form['tracks'][$weight]['album'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 128,
      '#default_value' => $track['album'],
    );
    $form['tracks'][$weight]['title'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 128,
      '#default_value' => $track['title'],
      '#required' => TRUE,
    );
    $form['tracks'][$weight]['weight'] = array(
      '#type' => 'weight', '#value' => $weight,
    );
    $weight++;
  }
  // add some empty tracks
  for ($i = 0; $i < 4; $i++) {
    $form['tracks'][$weight]['artist'] = array('#type'=>'textfield', '#size' => 30, '#maxlength' => 128);
    $form['tracks'][$weight]['album'] = array('#type'=>'textfield', '#size' => 30, '#maxlength' => 128);
    $form['tracks'][$weight]['title'] = array('#type'=>'textfield', '#size' => 30, '#maxlength' => 128);
    $form['tracks'][$weight]['weight'] = array('#type' => 'weight', '#value' => $weight);
    $weight++;
  }
  return $form;
}

function theme_station_schedule_playlist_track_form($form) {
  $output = '';
  $header = array(t('Artist'), t('Album'), t('Title'), t('Weight'));

  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = form_render($form[$key]['artist']);
    $row[] = form_render($form[$key]['album']);
    $row[] = form_render($form[$key]['title']);
    $row[] = form_render($form[$key]['weight']);
    $rows[] = $row;
  }

  if (count($rows) > 0) {
    $output .= theme('table', $header, $rows);
  }

  return $output . form_render($form);
}

// use this hook to convert form elements to node values
function station_schedule_playlist_validate(&$node) {
  // convert the programs title into a node id
  if (isset($node->program_title)) {
    $prog = node_load(array('type' => 'program', 'title' => $node->program_title));
    if ($prog) {
      $node->program_nid = $prog->nid;
    }
  }

  if (!isset($node->program_nid)) {
    form_set_error('program_title', t('A program must be selected.'));
  }

  $node->title = $node->program_title .' playlist for '. implode('-', $node->program_date);
}

function station_schedule_playlist_submit(&$node) {
  // convert the programs title into a node id
  if (isset($node->program_title)) {
    if ($prog = node_load(array('type' => 'program', 'title' => $node->program_title))) {
      $node->program_nid = $prog->nid;
    }
  }

  // title
  $node->title = $node->program_title .' playlist for '. implode('-', $node->program_date);

  // convert the date into a timestamp
  $node->timestamp = mktime(0, 0, 0, $node->program_date['month'], $node->program_date['day'], $node->program_date['year']);
}

/**
 * Load a playlist.
 */
function station_schedule_playlist_load($node) {
  // id and time
  $extras = db_fetch_object(db_query('SELECT p.program_nid, p.timestamp FROM {station_playlist} p WHERE p.nid = %d', $node->nid));

  // program title
  $prog = node_load($extras->program_nid);
  $extras->program_title = $prog->title;

  // program date
  $extras->program_date = array(
    'day' => format_date($extras->timestamp, 'custom', 'j'),
    'month' => format_date($extras->timestamp, 'custom', 'n'),
    'year' => format_date($extras->timestamp, 'custom', 'Y')
  );

  // tracks
  $extras->tracks = array();
  $result = db_query('SELECT i.weight, i.album, i.artist, i.title FROM {station_playlist_track} i WHERE i.nid = %d ORDER BY i.weight', $node->nid);
  while ($track = db_fetch_array($result)) {
    $extras->tracks[] = $track;
  }

  return $extras;
}

/**
 * Insert a new playlist
 */
function station_schedule_playlist_insert($node) {
  db_query("INSERT INTO {station_playlist} (nid, program_nid, timestamp) VALUES (%d, %d, %d)", $node->nid, $node->program_nid, $node->timestamp);
  foreach ($node->tracks as $track) {
    if ($track['artist'] || $track['title']) {
      db_query("INSERT INTO {station_playlist_track} (nid, weight, artist, album, title) VALUES (%d, %d, '%s', '%s', '%s')", $node->nid, $track['weight'], $track['artist'], $track['album'], $track['title']);
    }
  }
}

/**
 * Delete a playlist.
 */
function station_schedule_playlist_delete($node) {
  db_query('DELETE FROM {station_playlist} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {station_playlist_track} WHERE nid = %d', $node->nid);
}

/**
 * Update a playlist.
 */
function station_schedule_playlist_update($node) {
  db_query("UPDATE {station_playlist} SET program_nid = %d, timestamp= %d WHERE nid = %d", $node->program_nid, $node->timestamp, $node->nid);
  // delete and re-add tracks
  db_query('DELETE FROM {station_playlist_track} WHERE nid = %d', $node->nid);
  foreach ($node->tracks as $track) {
    if ($track['artist'] || $track['title']) {
      db_query("INSERT INTO {station_playlist_track} (nid, weight, artist, album, title) VALUES (%d, %d, '%s', '%s')", $node->nid, $track['weight'], $track['artist'], $track['album'], $track['title']);
    }
  }
}

/**
 * Get a playlist ready for viewing.
 */
function station_schedule_playlist_view(&$node, $teaser = false, $page = false) {
  if ($page) {
    $breadcrumb = array(
      array('path' => 'program', 'title' => t('programs')),
      array('path' => 'node/'. $node->program_nid, 'title' => $node->program_title),
      array('path' => 'node/'. $node->program_nid .'/playlists', 'title' => t('playlists')),
      array('path' => 'node/'. $node->nid, 'title' => t('playlist')),
    );
    menu_set_location($breadcrumb);
  }

  $header = array(t('Artist'), t('Album'), t('Title'));
  $rows = array();
  foreach ((array) $node->tracks as $track) {
    if ($track['artist'] || $track['title']) {
      $rows[] = array($track['artist'], $track['album'], $track['title']);
    }
  }
  if (count($rows) > 0) {
    $node->body .= theme('table', $header, $rows);
  }
}

// list recent playlists
function station_schedule_playlist_page() {
  $result = pager_query(db_rewrite_sql("SELECT n.nid, n.created FROM {node} n WHERE n.type = 'program_playlist' AND n.status = 1 ORDER BY n.created DESC"), variable_get('default_nodes_main', 10), 0, NULL, $node->nid);
  $output = '';
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node->nid), 1);
  }
  $output .= theme('pager', NULL, variable_get('default_nodes_main', 10));

  print theme('page', $output);
}

// list a program's playlists
function station_schedule_playlist_program_page($nid = 0) {
  $node = node_load(array('nid' => $nid, 'type' => 'program'));
  if (!$node->nid) {
    drupal_not_found();
  }

  drupal_set_title(t("%program's playlists", array('%program' => $node->title)));

  $result = pager_query(db_rewrite_sql("SELECT n.nid, n.created FROM {node} n INNER JOIN {station_playlist} p ON n.nid = p.nid WHERE p.program_nid = %d AND n.type = 'program_playlist' AND n.status = 1 ORDER BY n.created DESC"), variable_get('default_nodes_main', 10), 0, NULL, $node->nid);
  $output = '';
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node->nid), 1);
  }
  $output .= theme('pager', NULL, variable_get('default_nodes_main', 10));

  print theme('page', $output);
}

// list a user's playlists
function station_schedule_playlist_user_page($uid = 0) {
  $user = user_load(array('uid' => $uid));
  if (!$user->uid) {
    drupal_not_found();
  }

  drupal_set_title(t("%username's playlists", array('%username' => $user->name)));

  $result = pager_query(db_rewrite_sql("SELECT n.nid, n.created FROM {node} n WHERE n.uid = %d AND n.type = 'program_playlist' AND n.status = 1 ORDER BY n.created DESC"), variable_get('default_nodes_main', 10), 0, NULL, $user->uid);
  $output = '';
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node->nid), 1);
  }
  $output .= theme('pager', NULL, variable_get('default_nodes_main', 10));

  print theme('page', $output);
}
