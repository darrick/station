<?php

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
  $result = db_query_range('SELECT title FROM {node} WHERE type = \'program\' AND LOWER(title) LIKE LOWER("%%%s%%") ORDER BY title', $string, 0, 10);
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
    '#description' => 'The name of the program.',
    '#default_value' => $node->title,
    '#required' =>  true,
    '#size' =>  60,
    '#maxlength' =>  128,
  );
  $form['genre'] = array(
    '#type' => 'textfield',
    '#title' => t('Genre'),
    '#description' => 'A freeform description of the musical genres played on the program.',
    '#default_value' => $node->genre,
    '#required' =>  true,
    '#size' =>  60,
    '#maxlength' =>  200,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => 'Link to the program\'s website.',
    '#default_value' => $node->url,
    '#required' =>  false,
    '#size' =>  60,
    '#maxlength' =>  255,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => 'Describe the program, history, whatever.',
    '#default_value' => $node->body,
    '#required' => false,
    '#cols' => 60,
    '#rows' => 5,
  );

  if (user_access('administer schedule')) {
    if ($node->djs) {
       $form['users'] = array(
        '#type' => 'fieldset',
        '#title' => t('DJs'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      if ($node->djs) {
        $form['users']['dj_old'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Current DJs'),
          '#default_value' => array_keys($node->djs),
          '#options' => $node->djs,
          '#description' => 'Uncheck the box next to a DJ\'s name to remove them from the program.',
        );
      }
      $form['users']['dj_new'] = array(
        '#type' => 'textfield',
        '#title' => t('New DJ'),
        '#description' => 'Type the name of a DJ to add to this program.',
        '#maxlength' => 60,
        '#autocomplete_path' => 'user/autocomplete'
      );
    }
  }

  return array_merge($form, filter_form($node->format));
}

// use this hook to convert form elements to node values
function station_program_validate(&$node) {
  if (isset($_POST['edit'])) {
    // only schedule admins should be able to add or remove djs
    if (user_access('administer schedule')) {
      // dj removals. remove any existing users...
      $node->djs = array();
      // ...then add the checked ones.
      if (is_array($_POST['edit']['dj_old'])) {
        foreach ($_POST['edit']['dj_old'] as $uid => $checked) {
          if ($checked) {
            $user = user_load(array('uid' => $uid));
            if ($user->uid) {
              $node->djs[$user->uid] = $user->name;
            }
          }
        }
      }
      // dj additions
      if ($_POST['edit']['dj_new'] != '') {
        $crit = array('name' => $_POST['edit']['dj_new']);
        $user = user_load($crit);

        if ($user->uid) {
          $node->djs[$user->uid] = $user->name;
        } else {
          form_set_error('dj_new', t('You need to specify a valid user name.'));
        }
      }
    }
  }
}

/**
 * Load listen links for a program.
 * @param integer node id of a program
 * @param bool short version of links?
 */
function station_program_listen_links($program_nid, $short = false)  {
  $links = array();
  $node = node_load(array('type' => 'program', 'nid' => $program_nid));
  if ($node->nid) {
    // listen links
    if ($node->times) {
      if (count($node->times) > 1) {
        foreach ($node->times as $id => $time) {
          $hour = ($time['day'] -1) * 24 + $time['hour'] + 1;
          assert($id == $hour);
          $hours[] = $hour;
        }
        $hour = 'hours/' . implode('+', $hours);
      } else {
        $time = array_pop($node->times);
        $hour = station_day_name($time['day']) . '/' . station_hour_name($time['hour']);
      }
      $links['listen'] = theme('station_listen_link', $hour, $short);
      $links['rss'] = theme('station_rss_link', $hour, $short);
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
    $extras->times[$sch->sid] = array(
      'day' => $sch->day,
      'hour' => $sch->hour,
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
  db_query('DELETE FROM {station_dj} WHERE program_nid = %d', $node->nid);
  db_query('DELETE FROM {station_program} WHERE nid = %d', $node->nid);
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

  // dj changes...
  $old_djs = array_keys(_station_program_load_djs($node->nid));
  $new_djs = array_keys($node->djs);

  // ...removals
  foreach (array_diff($old_djs, $new_djs) as $uid) {
    db_query('DELETE FROM {station_dj} WHERE uid = %d AND program_nid = %d', $uid, $node->nid);
  }

  // ...additions
  foreach (array_diff($new_djs, $old_djs) as $uid) {
    db_query('INSERT INTO {station_dj} (uid, program_nid) VALUES (%d, %d)', $uid, $node->nid);
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

function station_program_page($nid = null) {
  if ($nid) {
    $page = node_view(node_load($nid), false, true);
  } else {
    $query = "SELECT {node}.*, {station_program}.genre, {station_program}.url ";
    $query .= "FROM {node} LEFT JOIN {station_program} ON {node}.nid = {station_program}.nid  ";
    $query .= "WHERE {node}.type = 'program' ORDER BY {node}.title";
    $queryResult = db_query($query);

    $cols = array('Program', 'Genre', 'DJs');
    $rows = array();
    while ($prog = db_fetch_object($queryResult)) {
      $djs = array();
      foreach (_station_program_load_djs($prog->nid) as $uid => $name) {
        $djs[] = l($name, 'user/' . $uid);
      }

      $rows[] = array(
        array('data' => l($prog->title, 'node/' . $prog->nid), 'width' => 240),
        array('data' => $prog->genre, 'width' => 240),
        array('data' => implode(',', $djs), 'width' => 240)
      );
    }
    $page = theme('table', $cols, $rows);
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

  if ($page) {
    if ($node->djs) {
      $dj_info = '<ul>';
      foreach ($node->djs as $uid => $name) {
          $dj_info .= '<li>' . l($name, 'user/' . $uid) . '</li>';
      }
      $dj_info .= '</ul>';
    } else {
      $dj_info = 'This program has no DJs';
    }

    if ($node->times) {
      $time_info = '<ul>';
      foreach ($node->times as $nid => $time) {
        $dayname = station_day_name($time['day']);
        $hourname = station_hour_name($time['hour']);
        $time_info .= '<li>' . l($dayname.' '.$hourname, strtolower('schedule/'.$dayname.'/'.$hourname)) . '</li>';
      }
      $time_info .= '</ul>';
    } else {
      $time_info = 'This program is not currently scheduled';
    }

    $body = '<p><strong>Genre:</strong> ' . check_markup($node->genre) . '</br>';
    if ($node->body) {
      $body .= '<div>' . check_markup($node->body) . '</div>';
    }
    $body .= "<h4>DJs</h4>\n" . check_markup($dj_info);
    $body .= "<h4>Times</h4>\n" . check_markup($time_info);
    $node->body = $body;
  } else if ($teaser) {
    $node->teaser = check_markup($node->genre);
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
    $output = l($program->title, 'node/' .$program->nid) . '<br />';
  } else {
    $output = t('We\'re on auto pilot') . '<br />';
  }

  // webstream links
  $high = variable_get('station_stream_high_url', '');
  $low = variable_get('station_stream_low_url', '');
  if ($high || $low) {
    $output .= t('Tune in:');
    if ($high) {
      $output .= ' ' . l(t('High'), $high);
    }
    if ($low) {
      $output .= ' ' . l(t('Low'), $low);
    }
  }

  return $output;
}


function station_program_current() {
  // this is -1 hour because portland is one hour behind the timezone telana's
  // server in montana.
  $date = getdate(time() - variable_get('station_tzoffset', 60 * 60 * 1));
  // getdate returns the day with a 0-offset, we need an offset of 1.
  $day = _station_valid_day($date['wday'] + 1);
  $hour = _station_valid_hour($date['hours']);

  // load the schedule
  $schedules = station_schedule_load(array('day' => $day, 'hour' => $hour), false);
  $schedule = $schedules[0];

  // if there's an associated program, load it
  if ($schedule->program_nid) {
    return node_load($schedule->program_nid);
  } else {
    return array();
  }
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
  case 'edit':
    var_dump($edit);
    break;
  case 'delete':
    db_query('DELETE FROM {station_dj} WHERE uid = %d', $user->uid);
    break;
  }
}
