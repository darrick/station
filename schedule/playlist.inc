<?php

// $Id$

function station_schedule_playlist_access($op, $node) {
  global $user;

  if (user_access('administer schedule')) {
    return TRUE;
  }

  switch ($op) {
    case 'view':
      return user_access('access content');
      break;

    case 'create':
      return user_access('create playlists');
      break;

    case 'update':
    case 'delete':
      if (user_access('edit own playlists') && ($user->uid == $node->uid)) {
        return TRUE;
      }
      break;
  }
  return FALSE;
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
    '#autocomplete_path' => 'station/autocomplete/program',
    '#required' => TRUE,
    '#description' => t('The name of the program. If you filled in for someone else, enter the name of their show.'),
  );

  $form['program_date'] = array(
    '#type' => 'date',
    '#title' => t('Program date'),
    '#default_value' => $node->program_date,
    '#required' => TRUE,
    '#description' => t('The date the program aired.'),
  );

  $form['tracks'] = array(
    '#type' => 'fieldset', '#title' => t('Tracks'),
    '#tree' => TRUE, '#collapsible' => FALSE, '#weight'=>1,
    '#theme' => 'station_schedule_playlist_track_form',
    '#description' => t('Enter the tracks played on the show, the artist and title are required. The weight is used to set the order: lower weights rise, heavier sink. If you want to remove a track just clear the artist and title text boxes. If you need more tracks, just save, more will be added. '),
  );

  $blanks = 10;
  $weight = 0;
  $max = max(20, $blanks + count($node->tracks) + 1);
  for($i = -1; $i < $max; $i++) {
    $weights[$i] = $i;
  }

  // load current tracks
  foreach ((array)$node->tracks as $key => $track) {
    $form['tracks'][$weight]['artist'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 255,
      '#default_value' => $track['artist'],
      '#autocomplete_path' => 'station/autocomplete/playlist/artist',
    );
    $form['tracks'][$weight]['album'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 255,
      '#default_value' => $track['album'],
      '#autocomplete_path' => 'station/autocomplete/playlist/album',
    );
    $form['tracks'][$weight]['title'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 255,
      '#default_value' => $track['title'],
    );
    $form['tracks'][$weight]['label'] = array(
      '#type' => 'textfield', '#size' => 30, '#maxlength' => 255,
      '#default_value' => $track['label'],
      '#autocomplete_path' => 'station/autocomplete/playlist/label',
    );
    $form['tracks'][$weight]['weight'] = array(
      '#type' => 'select', '#options' => $weights, '#default_value' => $weight,
    );
    $weight++;
  }
  // add some empty tracks
  for ($i = 0; $i < $blanks; $i++) {
    $form['tracks'][$weight]['artist'] = array('#type'=>'textfield',
      '#size' => 30, '#maxlength' => 255,
      '#autocomplete_path' => 'station/autocomplete/playlist/artist',
    );
    $form['tracks'][$weight]['album'] = array('#type'=>'textfield',
      '#size' => 30, '#maxlength' => 255,
      '#autocomplete_path' => 'station/autocomplete/playlist/album',
    );
    $form['tracks'][$weight]['title'] = array('#type'=>'textfield',
      '#size' => 30, '#maxlength' => 255,
    );
    $form['tracks'][$weight]['label'] = array('#type'=>'textfield',
      '#size' => 30, '#maxlength' => 255,
      '#autocomplete_path' => 'station/autocomplete/playlist/label',
    );
    $form['tracks'][$weight]['weight'] = array('#type' => 'select',
      '#options' => $weights, '#default_value' => $weight,
    );
    $weight++;
  }

  $form['body_filter']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $node->body,
    '#rows' => 5,
    '#required' => FALSE,
  );
  $form['body_filter']['format'] = filter_form($node->format);

  return $form;
}

function theme_station_schedule_playlist_track_form($form) {
  $header = array(t('Artist'), t('Title'), t('Album'), t('Label'), t('Weight'));

  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = form_render($form[$key]['artist']);
    $row[] = form_render($form[$key]['title']);
    $row[] = form_render($form[$key]['album']);
    $row[] = form_render($form[$key]['label']);
    $row[] = form_render($form[$key]['weight']);
    $rows[] = $row;
  }

  if (count($rows) == 0) {
    $output = '';
  }
  else {
    $output = theme('table', $header, $rows);
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

  // check for track errors
  foreach ($node->tracks as $track) {
    if (($track['album'] || $track['label']) && !($track['artist'] || $track['title'])) {
      form_set_error('tracks]['. $track['weight'] . '][artist', "You've got to provide an artist or a title.");
      form_set_error('tracks]['. $track['weight'] . '][title');
    }
  }
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
  $extras = db_fetch_object(db_query('SELECT program_nid, timestamp FROM {station_playlist} WHERE nid = %d', $node->nid));

  // program title
  $prog = node_load($extras->program_nid);
  $extras->program_title = $prog->title;

  // program date
  $extras->program_date = array(
    'day' => date('j', $extras->timestamp),
    'month' => date('n', $extras->timestamp),
    'year' => date('Y', $extras->timestamp)
  );

  // tracks
  $extras->tracks = array();
  $result = db_query('SELECT weight, album, artist, title, label FROM {station_playlist_track} WHERE nid = %d ORDER BY weight', $node->nid);
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
      db_query("INSERT INTO {station_playlist_track} (nid, weight, artist, album, title, label) VALUES (%d, %d, '%s', '%s', '%s', '%s')",
        $node->nid, trim($track['weight']), trim($track['artist']), trim($track['album']), trim($track['title']), trim($track['label']));
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
      db_query("INSERT INTO {station_playlist_track} (nid, weight, artist, album, title, label) VALUES (%d, %d, '%s', '%s', '%s', '%s')",
        $node->nid, trim($track['weight']), trim($track['artist']), trim($track['album']), trim($track['title']), trim($track['label']));
    }
  }
}

/**
 * Get a playlist ready for viewing.
 */
function station_schedule_playlist_view(&$node, $teaser = false, $page = false) {
  $node = node_prepare($node, $teaser);

  if ($page) {
    $breadcrumb = array(
      array('path' => 'station/programs', 'title' => t('programs')),
      array('path' => 'node/'. $node->program_nid, 'title' => $node->program_title),
      array('path' => 'station/playlists/'. $node->program_nid, 'title' => t('playlists')),
      array('path' => 'node/'. $node->nid, 'title' => t('playlist')),
    );
    menu_set_location($breadcrumb);
  }

  if ($teaser) {
    if (!$node->body) {
      $node->teaser = t('%trackcount tracks', array('%trackcount' => count($node->tracks)));
    }
  } else {
    if (count($node->tracks) > 0) {
      // make one pass to figure out what fields we've got...
      $hasAlbum = $hasLabel = FALSE;
      foreach ((array) $node->tracks as $track) {
        $hasAlbum |= (bool) ($track['album']);
        $hasLabel |= (bool) ($track['label']);
      }
      $header = array(t('Artist'), t('Title'));
      if ($hasAlbum) {
        $header[] = t('Album');
      }
      if ($hasLabel) {
        $header[] = t('Label');
      }
      // ...then put the into the table
      $rows = array();
      foreach ((array) $node->tracks as $track) {
        if ($track['artist'] || $track['title']) {
          $row = array(
            check_plain($track['artist']),
            check_plain($track['title']),
          );
          if ($hasAlbum) {
            $row[] = check_plain($track['album']);
          }
          if ($hasLabel) {
            $row[] = check_plain($track['label']);
          }
          $rows[] = $row;
        }
      }

      $node->body .= theme('table', $header, $rows);
    }
  }
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for playlist
 * items.
 */
function station_schedule_playlist_autocomplete($field = '', $string = '') {
  $matches = array();
  if (in_array($field, array('artist', 'title', 'album', 'label'))) {
    $result = db_query_range('SELECT DISTINCT %s AS val FROM {station_playlist_track} WHERE LOWER(%s) LIKE LOWER("%s%%") ORDER BY val', $field, $field, $string, 0, 10);
    while ($item = db_fetch_object($result)) {
      $matches[$item->val] = check_plain($item->val);
    }
  }
  print drupal_to_js($matches);
  exit();
}
