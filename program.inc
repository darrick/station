<?php

// $Id$

function station_program_access($op, $node) {
  global $user;

  if (user_access('administer schedule')) {
    return true;
  }
  switch ($op) {
    case 'update':
      $djs =_station_program_load_djs($node->nid);
      // if they're a DJ, check they should be able to do it.
      if (isset($djs[$user->uid])) {
        return user_access('edit own program');
      }
      break;

    case 'view':
      return user_access('access content');
      break;
  }
  return false;
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for existing programs
 */
function station_program_autocomplete($string = '') {
  $matches = array();
  $result = db_query_range('SELECT title FROM {node} WHERE type = \'program\' AND LOWER(title) LIKE LOWER("%%%s%%") AND status = 1 ORDER BY title', $string, 0, 10);
  while ($program = db_fetch_object($result)) {
    $matches[$program->title] = check_plain($program->title);
  }
  print drupal_implode_autocomplete($matches);
  exit();
}

/**
 * Build a form for program nodes.
 */
function station_program_form(&$node) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $node->title,
    '#required' =>  true,
    '#size' =>  60,
    '#maxlength' =>  128,
    '#description' => t('The name of the program. You should avoid making major changes to this without telling the programming director.'),
  );
  $form['genre'] = array(
    '#type' => 'textfield',
    '#title' => t('Text genre'),
    '#description' => t('A free-form description of the musical genres played on the program.'),
    '#default_value' => $node->genre,
    '#required' =>  true,
    '#size' =>  60,
    '#maxlength' =>  200,
    '#weight' => -2,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Homepage URL'),
    '#description' => t("Link to the program's website."),
    '#default_value' => $node->url,
    '#required' =>  false,
    '#size' =>  60,
    '#maxlength' =>  255,
    '#weight' => -1,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $node->body,
    '#required' => false,
    '#cols' => 60,
    '#rows' => 5,
    '#description' => t('Freeform description of the program.'),
  );
  $form['format'] = filter_form($node->format);

  if (user_access('administer schedule')) {
    $form['users'] = array(
     '#type' => 'fieldset',
     '#title' => t('DJs'),
     '#weight' => 1,
   );
   if ($node->djs) {
     $form['users']['dj_old'] = array(
       '#type' => 'checkboxes',
       '#title' => t('Current DJs'),
       '#default_value' => $node->dj_old,
       '#options' => $node->djs,
       '#description' => t("Check the box to remove a DJ from the program."),
     );
   } else {
      $form['users']['dj_old'] = array(
        '#type' => 'item',
        '#value' => t('This program has no DJs'),
      );
   }
   $form['users']['dj_new'] = array(
     '#type' => 'textfield',
     '#title' => t('New DJ'),
     '#description' => t('Type the name of user to add as a DJ.'),
     '#maxlength' => 60,
     '#autocomplete_path' => 'user/autocomplete'
   );
  }

  return $form;
}


// use this hook to convert form elements to node values
function station_program_validate(&$node) {
  // check that dj additions are valid users
  if ($node->dj_new != '') {
    $user = user_load(array('name' => $node->dj_new));
    if ($user->uid) {
      $node->djs[$user->uid] = $user->name;
    } else {
      form_set_error('dj_new', t('You need to specify a valid user name.'));
    }
  }

  // add old users in so the preview works
  if ($node->dj_old) {
    foreach ((array)$node->dj_old as $uid => $checked) {
      if (!$checked) {
        $user = user_load(array('uid' => $uid));
        if ($user->uid) {
          $node->djs[$uid] = $user->name;
        }
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

/**
 * Load listen links for a program.
 * @param $program_nid integer node id of a program
 * @param $short bool true for short version of links
 */
function station_program_listen_links($program_nid, $short = false)  {
  $links = array();

  // TODO: remove this old URL
  $archive_url = url(variable_get('station_archive_url', 'http://archive.kpsu.org:8080/'));

  $node = node_load($program_nid);
  if ($node->nid && $archive_url) {
    // listen links
    if ($node->times) {
      if (count($node->times) > 1) {
        // program has multiple time slots, use the 'hours' url instead so they all appear in the same page.
        foreach ($node->times as $time) {
          $hour = ($time['day'] -1) * 24 + $time['hour'] + 1;
          $hours[] = $hour;
        }
        $hour = 'hours/' . implode('+', $hours);
      } else {
        // single
        $time = array_pop($node->times);
        $hour = station_day_name($time['day']) . '/' . station_hour_name($time['hour']);
      }

      $links['listen'] = theme('station_listen_link', $archive_url .'last/'. $hour, $short);
      $links['rss'] = theme('station_rss_link', $archive_url .'rss/'. $hour, $short);
    }
  }
  return $links;
}

/**
 * Load a program.
 */
function station_program_load($node) {
  $extras = db_fetch_object(db_query('SELECT genre, url FROM {station_program} WHERE vid = %d', $node->vid));

  // load the program's users users under djs.
  $extras->djs = _station_program_load_djs($node->nid);

  // load the program's schedule times users under the times.
  $schs = station_schedule_load(array('program_nid' => $node->nid), false);
  foreach ($schs as $sch) {
      $dayname = station_day_name($sch->day);
      $hourname = station_hour_name($sch->hour);

    $extras->times[$sch->sid] = array(
      'day' => $sch->day,
      'hour' => $sch->hour,
      'formatted' => t('%dayname at %hourname', array(
        '%dayname' => station_day_name($sch->day),
        '%hourname' => station_hour_name($sch->hour))
      ),
    );
  }

  return $extras;
}

/**
 * Insert a new program
 */
function station_program_insert($node) {
  db_query("INSERT INTO {station_program} (nid, vid, genre, url) VALUES (%d, %d, '%s', '%s')", $node->nid, $node->vid, $node->genre, $node->url);
}

/**
 * Delete a program.
 */
function station_program_delete($node) {
  if (user_access('administer schedule')) {
    db_query('DELETE FROM {station_dj} WHERE program_nid = %d', $node->nid);
    db_query('DELETE FROM {station_program} WHERE nid = %d', $node->nid);
  }
}

/**
 * Update a program.
 */
function station_program_update($node) {
  if ($node->revision) {
    station_program_insert($node);
  } else {
    db_query("UPDATE {station_program} SET genre = '%s', url = '%s' WHERE vid = %d", $node->genre, $node->url, $node->vid);
  }

  // only schedule admins should be able to add or remove djs
  if (user_access('administer schedule')) {
    // dj removals
    // array filter will ignore any unchecked values
    $removals = array_filter((array)$node->dj_old);
    foreach ($removals as $uid => $checked) {
      db_query('DELETE FROM {station_dj} WHERE uid = %d AND program_nid = %d', $uid, $node->nid);
    }

    // dj additions
    $user = user_load(array('name' => $node->dj_new));
    if ($user->uid) {
      db_query('INSERT INTO {station_dj} (uid, program_nid) VALUES (%d, %d)', $user->uid, $node->nid);
    }
  }
}

function _station_program_load_djs($program_nid) {
  // load the dj's users users under the djs item.
  $djs = array();
  $result = db_query('SELECT d.uid, u.name FROM {station_dj} d INNER JOIN {users} u on d.uid = u.uid WHERE d.program_nid = %d ORDER BY u.name', $program_nid);
  while ($dj = db_fetch_object($result)) {
    $djs[$dj->uid] = $dj->name;
  }
  return $djs;
}

/**
 * Print a list of programs. Display a list of un-published programs at the
 * bottom for schedule admins.
 */
function station_program_page($nid = null) {
  if ($nid) {
    $page = node_view(node_load($nid), false, true);
  } else {
    $query = "SELECT n.*, p.genre, p.url ";
    $query .= "FROM {node} n LEFT JOIN {station_program} p ON n.nid = p.nid ";
    $query .= "WHERE n.type = 'program' ORDER BY n.title";
    $queryResult = db_query($query);

    $cols = array('Program', 'Genre', 'DJs');
    $rows = array();
    $rowsOld = array();
    while ($prog = db_fetch_object($queryResult)) {
      $djs = array();
      foreach (_station_program_load_djs($prog->nid) as $uid => $name) {
        $djs[] = l($name, 'user/' . $uid);
      }

      $row = array(
        array('data' => l($prog->title, 'node/' . $prog->nid), 'width' => 240),
        array('data' => check_plain($prog->genre), 'width' => 240),
        array('data' => implode(',', $djs), 'width' => 240)
      );
      if ($prog->status == 1) {
        $rows[] = $row;
      } else {
        $rowsOld[] = $row;
      }
    }
    $page = theme('table', $cols, $rows);

    if (user_access('administer schedule')) {
      $page .= '<h2>'. t('Unpublished shows') .'</h2>';
      $page .= t('<p>These are shows that have been marked as unpublished.</p>');
      $page .= theme('table', $cols, $rowsOld);
    }
  }
  return $page;
}

/**
 * Get a program ready for viewing.
 */
function station_program_view(&$node, $teaser = false, $page = false) {
  if ($page) {
    $breadcrumb = array();
    $breadcrumb[] = array('path' => 'program', 'title' => t('programs'));
    $breadcrumb[] = array('path' => 'node/'. $node->nid);
    menu_set_location($breadcrumb);
    drupal_set_title($node->title);
  }

  // make djs into links
  $djs = array();
  if ($node->djs) {
    foreach ($node->djs as $uid => $name) {
      $djs[] = l($name, 'user/' . $uid);
    }
  }

  // make schedule time into links
  $times = array();
  if ($node->times) {
    foreach ($node->times as $nid => $time) {
      $dayname = station_day_name($time['day']);
      $hourname = station_hour_name($time['hour']);
      $times[] = l($time['formatted'], strtolower("schedule/$dayname/$hourname"));
    }
  }

  if ($teaser) {
    if ($node->genre) {
      $node->teaser = check_markup($node->genre);
    }
    if ($djs) {
      $node->teaser .= ' with '. _station_anded_list($djs);
    }
    if ($times) {
      $node->teaser .= ' on '. _station_anded_list($times);
    }
  } else {
    if ($djs) {
      $dj_info = '<ul><li>' . implode('</li><li>', $djs) . '</li></ul>';
    } else {
      $dj_info = t('This program has no DJs');
    }

    if ($times) {
      $time_info = '<ul><li>' . implode('</li><li>', $times) . '</li></ul>';
    } else {
      $time_info = t('This program is currently unscheduled');
    }

    $node->body = '<p><strong>Genre:</strong> ' . check_plain($node->genre) . '</p>'. $node->body;
    $node->body .= "<h4>DJs</h4>\n" . check_markup($dj_info);
    $node->body .= "<h4>Times</h4>\n" . check_markup($time_info);
  }
}

/**
 * Return HTML body of the block listing the current program.
 *
 * @return string
 */
function station_program_block() {
  $program = station_program_current();

  // current program
  if ($program) {
    $output = l($program->title, 'node/' .$program->nid);
  } else {
    $output = t("We're on auto pilot");
  }
  $output .= '<br />';

  // webstream links
  $high = variable_get('station_stream_high_url', '');
  $low = variable_get('station_stream_low_url', '');
  if ($high || $low) {
    $output .= t('Tune in:');
    if ($high) {
      $output .= ' <a href="'. check_url($high) .'">'. t('High') .'</a>';
    }
    if ($low) {
      $output .= ' <a href="'. check_url($low) .'">'. t('Low') .'</a>';
    }
  }

  return $output;
}

/**
 * Get the program playing at the current time.
 *
 * @return
 *   program node
 */
function station_program_current() {
  // grab the program at this time
  return station_program_get_at(time());
}

/**
 * Get the program playing at a certain time. If no time is provide, use the
 * current time.
 *
 * @param $gmt_timestamp
 *   a timestamp used to specify a day of the week an hour.
 * @return
 *   program node or FALSE if nothing is scheduled or there's an error
 */
function station_program_get_at($gmt_timestamp) {
  $ts = _station_local_ts($gmt_timestamp);

  $day = date('w', $ts);
  $hour = date('G', $ts);

  // load the schedule
  $schedules = station_schedule_load(array('day' => $day, 'hour' => $hour), false);
  $schedule = $schedules[0];

  // if there's an associated program, load it
  if ($schedule->program_nid) {
    $node = node_load($schedule->program_nid);
    // set this so that if the show is scheduled for multiple times the caller
    // can easily figure out which one.
    $node->current_time = $schedule->sid;
    return $node;
  }
  return FALSE;
}

/**
 * Implements hook_user() to associate programs with the users.
 */
function station_user($op, &$edit, &$user, $category = NULL) {
  switch ($op) {
  case 'load':
    // load the dj's programs.
    $user->programs = array();
    $result = db_query('SELECT d.program_nid AS nid, n.title FROM {station_dj} d INNER JOIN {node} n on d.program_nid = n.nid WHERE d.uid = %d ORDER BY n.title', $user->uid);
    while ($program = db_fetch_object($result)) {
      $user->programs[$program->nid] = $program->title;
    }
    break;
  case 'view':
    if ($user->programs) {
      $programlist = array();
      foreach($user->programs as $nid => $title) {
        $programlist['Programs'][] = l($title, 'node/' . $nid) . "<br />\n";
      }
      return $programlist;
    }
    break;
  case 'delete':
    db_query('DELETE FROM {station_dj} WHERE uid = %d', $user->uid);
    break;
  }
}
