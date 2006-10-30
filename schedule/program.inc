<?php

// $Id$

function station_schedule_program_access($op, $node) {
  global $user;

  switch ($op) {
    case 'view':
      return user_access('access content');
      break;

    case 'update':
      $djs =_station_schedule_program_load_djs($node->nid);
      // if they're a DJ, check they should be able to do it.
      if (isset($djs[$user->uid])) {
        return user_access('edit own program');
      }
      break;

    case 'delete':
    case 'create':
    default:
      return user_access('administer schedule');
  }
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for existing programs
 */
function station_schedule_program_autocomplete($string = '') {
  $matches = array();
  $result = db_query_range('SELECT DISTINCT title FROM {node} WHERE type = \'program\' AND LOWER(title) LIKE LOWER("%s%%") AND status = 1 ORDER BY title', $string, 0, 10);
  while ($program = db_fetch_object($result)) {
    $matches[$program->title] = check_plain($program->title);
  }
  print drupal_to_js($matches);
  exit();
}

/**
 * Build a form for program nodes.
 */
function station_schedule_program_form(&$node) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $node->title,
    '#required' =>  TRUE,
    '#maxlength' =>  128,
    '#description' => t('The name of the program. You should avoid making major changes to this without telling the programming director.'),
  );
  $form['genre'] = array(
    '#type' => 'textfield',
    '#title' => t('Text genre'),
    '#description' => t('A free-form description of the musical genres played on the program.'),
    '#default_value' => $node->genre,
    '#required' =>  true,
    '#maxlength' => 200,
    '#weight' => -2,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Homepage URL'),
    '#description' => t("Link to the program's website."),
    '#default_value' => $node->url,
    '#maxlength' =>  255,
    '#weight' => -1,
  );
  $form['body_filter']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#default_value' => $node->body,
    '#rows' => 10,
    '#required' => TRUE,
    '#description' => t("Free form description of the show."),
  );
  $form['body_filter']['format'] = filter_form($node->format);

  if (user_access('administer schedule')) {
    // if it's a saved, published, un-scheduled program, provide a link to add
    // it to the schedule.
    if ($node->nid && $node->status && !$node->scheduled_sids) {
      drupal_set_message(
        t("This program is unscheduled. You can <a href='%link'>add it to the schedule</a>.", 
        array('%link' => url('admin/schedule/add/'. $node->nid)))
      );
    }
    $form['djs'] = array(
      '#type' => 'fieldset',
      '#title' => t('DJs'),
      '#weight' => 1,
      '#tree' => TRUE,
      '#description' => t("Enter the names of users you'd like to add as DJs"),
    );
    if ($node->djs) {
      foreach ((array)$node->djs as $uid => $username) {
        $form['djs'][$uid]['username'] = array(
          '#type' => 'textfield', '#maxlength' => 60,
          '#autocomplete_path' => 'user/autocomplete',
          '#default_value' => $username,
        );
      }
    }
    // add some blank username textfields
    for ($i = 0; $i < 4; $i++) {
      $form['djs']['new'. $i]['username'] = array(
        '#type' => 'textfield', '#maxlength' => 60,
        '#autocomplete_path' => 'user/autocomplete',
      );
    }
  }

  return $form;
}

// use this hook to convert form elements to node values
function station_schedule_program_validate(&$node) {
  // validate the usernames
  foreach ($node->djs as $key => $dj) {
    if ($dj['username']) {
      $user = user_load(array('name' => $dj['username']));

      if (!$user->uid) {
        form_set_error("djs][$key][username", t('You need to specify a valid user name.'));
      }
    }
  }

  // check the url
  if ($node->url) {
    if (!valid_url($node->url)) {
      form_set_error('url', t('You need to provide a valid URL.'));
    }
  }
}

function station_schedule_program_submit(&$node) {
  // make a copy of the djs and put them back in the expected form
  $djs = $node->djs;
  $node->djs = array();
  foreach ($djs as $key => $dj) {
    if ($dj['username']) {
      $user = user_load(array('name' => $dj['username']));
      if ($user->uid) {
        $node->djs[$user->uid] = $user->name;
      }
    }
  }
}

/**
 * Load a program.
 */
function station_schedule_program_load($node) {
  $extras = db_fetch_object(db_query('SELECT genre, url FROM {station_program} WHERE vid = %d', $node->vid));

  // load the program's users under djs.
  $extras->djs = _station_schedule_program_load_djs($node->nid);

  // load program's schedule ids
  $result = db_query('SELECT s.sid FROM {station_schedule} s WHERE s.program_nid = %d', $node->nid);
  while ($s = db_fetch_object($result)) {
    $extras->scheduled_sids[] = $s->sid;
  }

  return $extras;
}

/**
 * Insert a new program
 */
function station_schedule_program_insert($node) {
  db_query("INSERT INTO {station_program} (nid, vid, genre, url) VALUES (%d, %d, '%s', '%s')", $node->nid, $node->vid, $node->genre, $node->url);

  // dj additions
  foreach ($node->djs as $uid => $username) {
    db_query('INSERT INTO {station_dj} (uid, program_nid) VALUES (%d, %d)', $uid, $node->nid);
  }
}

/**
 * Delete a program.
 */
function station_schedule_program_delete($node) {
  if (user_access('administer schedule')) {
    db_query('DELETE FROM {station_dj} WHERE program_nid = %d', $node->nid);
    db_query('DELETE FROM {station_program} WHERE nid = %d', $node->nid);
  }
}

/**
 * Update a program.
 */
function station_schedule_program_update($node) {
  db_query("UPDATE {station_program} SET genre = '%s', url = '%s' WHERE vid = %d", $node->genre, $node->url, $node->vid);

  // only schedule admins should be able to add or remove djs
  if (user_access('administer schedule')) {
    // dj removals
    db_query('DELETE FROM {station_dj} WHERE program_nid = %d', $node->nid);

    // dj additions
    foreach ($node->djs as $uid => $username) {
      db_query('INSERT INTO {station_dj} (uid, program_nid) VALUES (%d, %d)', $uid, $node->nid);
    }
  }
}

/**
 * Return an array of DJ info for a given program. The key is the user's uid
 * and the value is the user's name.
 */
function _station_schedule_program_load_djs($program_nid) {
  // load the dj's users users under the djs item.
  $djs = array();
  $result = db_query('SELECT d.uid, u.name FROM {station_dj} d INNER JOIN {users} u ON d.uid = u.uid WHERE d.program_nid = %d ORDER BY u.name', $program_nid);
  while ($dj = db_fetch_object($result)) {
    $djs[$dj->uid] = $dj->name;
  }
  return $djs;
}

/**
 * Returns an array of themed DJs.
 */
function station_schedule_program_get_themed_djs($program_nid) {
  // make djs into links
  $djs = array();
  foreach (_station_schedule_program_load_djs($program_nid) as $uid => $name) {
    $user = user_load(array('uid' => $uid));
    $djs[] = theme('username', $user);
  }
  return $djs;
}

/**
 * Get a program ready for viewing.
 */
function station_schedule_program_view(&$node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  if ($page) {
    $breadcrumb = array();
    $breadcrumb[] = array('path' => 'station/programs', 'title' => t('programs'));
    $breadcrumb[] = array('path' => 'node/'. $node->nid);
    menu_set_location($breadcrumb);
  }

  // make djs into links
  $djs = station_schedule_program_get_themed_djs($node->nid);

  // load the program's scheduled times
  $ranges = station_schedule_program_get_times($node->nid);

  // second pass to make pretty descriptions
  $times = array();
  foreach ($ranges as $range) {
    $times[] = theme('station_dayhour_range', $range['start'], $range['finish']);
  }

  if ($teaser) {
    $format = '%genre';
    if ($djs) {
      $format .= ' with %djs';
    }
    if ($times) {
      $format .= ', on %times.';
    }
    else {
      $format .= ', is currently unscheduled.';
    }
    $options = array(
      '%genre' => $node->genre,
      '%djs' => station_anded_list($djs),
      '%times' => station_anded_list($times),
    );
    $node->teaser = check_markup(t($format, $options));
  }
  else {
    // playlists
    $time = time();
    $time_format = 'F j, Y';

    // pull all future playlists
    $future_lists = array();
    $result = db_query('SELECT p.nid, p.timestamp FROM {station_playlist} p INNER JOIN {node} n ON p.program_nid = n.nid WHERE n.status = 1 AND p.program_nid = %d AND p.timestamp > %d ORDER BY timestamp', $node->nid, $time);
    while ($playlist = db_fetch_object($result)) {
      $future_lists[] = l(format_date($playlist->timestamp, 'custom', $time_format, 0), 'node/'. $playlist->nid);
    }
    // pull N recent playlists
    $past_lists = array();
    $result = db_query_range('SELECT p.nid, p.timestamp FROM {station_playlist} p INNER JOIN {node} n ON p.program_nid = n.nid WHERE n.status = 1 AND p.program_nid = %d AND p.timestamp < %d ORDER BY timestamp DESC', $node->nid, $time, 0, 5);
    while ($playlist = db_fetch_object($result)) {
      $past_lists[] = l(format_date($playlist->timestamp, 'custom', $time_format, 0), 'node/'. $playlist->nid);
    }

    if ($djs) {
      $dj_info = '<ul><li>'. implode('</li><li>', $djs) .'</li></ul>';
    }
    else {
      $dj_info = t('This program has no DJs');
    }

    if ($times) {
      $time_info = '<ul><li>'. implode('</li><li>', $times) .'</li></ul>';
    }
    else {
      $time_info = t('This program is currently unscheduled');
    }

    $origbody = $node->body;
    $node->body = '<p><strong>Genre:</strong> ' . check_plain($node->genre) . '</p>';
    if ($node->url) {
      $node->body .= '<p><strong>Homepage:</strong> '. l($node->url, $node->url) ."</p>\n";
    }
    $node->body .= $origbody;
    $node->body .= "<h4>DJs</h4>\n" . check_markup($dj_info);
    $node->body .= "<h4>Times</h4>\n" . check_markup($time_info);
    if ($future_lists) {
      $node->body .=  "<h4>Upcoming playlists</h4>\n" . '<ul><li>'. implode('</li><li>', $future_lists) .'</li></ul>';
    }
    if ($past_lists) {
      $node->body .= "<h4>Recent playlists</h4>\n" . '<ul><li>'. implode('</li><li>', $past_lists) .'</li></ul>';
      $node->body .= l(t('view older playlists'), 'station/playlists/'. $node->nid);
    }
  }
}

/**
 * Get the program playing at a certain time. If no time is provide, use the
 * current time.
 *
 * @param $gmt_timestamp
 *   a timestamp used to determine the day of the week an hour.
 * @return
 *   program node object if one is scheduled, an empty object if nothing is
 *   scheduled.
 */
function station_schedule_program_get_at($gmt_timestamp) {
  $ts = station_local_ts($gmt_timestamp);
  $day = date('w', $ts);
  $hour = date('G', $ts);

  // load the program id from the schedule
  $minute = station_minute_from_day_hour($day, $hour);
  $node = _station_schedule_load_program_at($minute);

  // if there's an associated program, load it
  if ($node) {
    // set this so that if the show is scheduled for multiple times the caller
    // can easily figure out which one.
    $node->current_time = $schedule->sid;
    // put this in so they can use a pretty url
    $node->node_url = url('node/'. $node->nid, NULL, NULL, TRUE);
    return $node;
  }
  return new stdClass;
}

/**
 * Returns an array of hours the program is scheduled.
 */
function station_schedule_program_get_times($nid) {
  // load the program's scheduled times
  $i = 0;
  $hours = array();
  $result = db_query('SELECT s.sid, s.start, s.finish FROM {station_schedule} s WHERE s.program_nid = %s ORDER BY s.start', $nid);
  while ($s = db_fetch_array($result)) {
    $hours[] = $s;
  }
  return $hours;
}
