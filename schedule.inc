<?php

// $Id$

function station_schedule_access($op, $sch) {
  // there are a fixed number of schedule slots, only updates are allowed.
  switch ($op) {
    case 'update':
      return user_access('administer schedule');

    case 'view':
      return user_access('access content');

    default:
      return false;
  }
}

/**
 * Grab the schedule (joined with program info).
 *
 * @param $crit
 *   An array where the key is the field name, value is a value to match.
 * @param boolean $nested should the result be nested arrays?
 * @return Array. An array (indexed by day) of arrays of schedule objects
 *   (indexed by hour).
 */
function station_schedule_load($crit = array(), $nested = true)  {
  // table
  $query = 'SELECT * FROM {station_schedule} s';

  // where clause
  if (is_array($crit)) {
    $where = array();
    foreach ($crit as $field => $value) {
      $where[] = 's.' . db_escape_string($field) . "='" . db_escape_string($value) . "'";
    }
    // put it together
    if ($where = implode(' AND ', $where)) {
      $query .= ' WHERE ' . $where;
    }
  }

  // order by clause
  $query .= ' ORDER BY s.day, s.hour';

  $ret = array();
  $queryResult = db_query($query);
  while ($sch = db_fetch_object($queryResult)) {
    $sch->dayname = station_day_name($sch->day);
    $sch->hourname = station_hour_name($sch->hour);
    if ($nested) {
      $ret[$sch->day][$sch->hour] = $sch;
    } else {
      $ret[] = $sch;
    }
  }
  return $ret;
}



/**
 * Get a schedule node ready for viewing.
 */
function station_schedule_view(&$node, $teaser = false, $page = false) {
  if ($page) {
    $breadcrumb = array();
    $breadcrumb[] = array('path' => 'schedule', 'title' => t('Schedule'));
    $breadcrumb[] = array('path' => 'schedule/' . strtolower($node->dayname), 'title' => t($node->dayname));
    $breadcrumb[] = array('path' => 'schedule/' . strtolower($node->dayname) . '/' . $node->hourname, 'title' => t($node->hourname));
    menu_set_location($breadcrumb);
  }

  if (isset($node->program_nid)) {
    $p = node_load($node->program_nid);
    $program = l($p->title, 'node/' . $p->nid);
  } else {
    $program = 'No program is scheduled at this time.';
  }

  if ($teaser) {
    $node->teaser = check_markup($program);
  } else {
    $node->body = check_markup($program);
  }
}


/**
 * Implementation of hook_form().
 *
 * Now it's time to describe the form for collecting the information
 * specific to this node type. This hook requires us to return some HTML
 * that will be later placed inside the form.
 */
function station_schedule_admin_form($sid) {
  $schs = station_schedule_load(array('sid' => $sid), false);
  $sch = array_pop($schs);

  drupal_set_title($sch->dayname . ' ' . $sch->hourname);
  if (isset($sch->program_nid)) {
    $program = node_load($sch->program_nid);
    $program_title = $program->title;
  }

  $form['#type'] = 'form';
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $sch->sid,
  );
  // used to redirect back to the right day
  $form['dayname'] = array(
    '#type' => 'value',
    '#value' => strtolower($sch->dayname),
  );
  $form['program_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Program'),
    '#description' => t("Type the name of this hour's program, or if nothing is scheduled, leave it blank."),
    '#default_value' => $program_title,
    '#maxlength' => 128,
    '#autocomplete_path' => 'program/autocomplete'
  );
  $form['onam'] = array(
    '#type' => 'checkbox',
    '#title' => t('On AM'),
    '#description' => t('Is this program broadcast on KBPS?'),
    '#default_value' => $sch->onam,
    '#return_value' => 1,
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  return drupal_get_form('station_schedule_admin_form', $form);
}

function station_schedule_admin_form_validate($form_id, $form_values) {
  if ($form_values['program_title']) {
    // convert the programs title into a node id
    $prog = node_load(array('type' => 'program', 'title' => $form_values['program_title']));
    if (!$prog) {
      form_set_error('program_title', t('You need to specify a valid program name.'));
    }
  }
}

function station_schedule_admin_form_submit($form_id, $form_values) {
  $prog = node_load(array('type' => 'program', 'title' => $form_values['program_title']));
  db_query("UPDATE {station_schedule} SET program_nid = %d, onam = %d WHERE sid = %d",
    $prog->nid, $form_values['onam'], $form_values['sid']);
  drupal_goto('admin/schedule#' . $form_values['dayname']);
}

function station_schedule_admin_list() {
  // create two separate tables, one for am and one for pm...
  // column titles: hour then days of the week
  $cols = array_merge(array('Hour'), station_day_name());

  // ...first column in both are hour titles
  foreach (range(0,11) as $hourid) {
    $amrows[$hourid][0] = station_hour_name($hourid);
  }
  foreach (range(12,23) as $hourid) {
    $pmrows[$hourid][0] = station_hour_name($hourid);
  }

  // fill in the table bodies
  $schs = station_schedule_load(array(), true);
  foreach ($schs as $dayid => $hour) {
    foreach ($hour as $hourid => $sch) {
      $disp = '';
      if ($sch->program_nid) {
        $prog = node_load($sch->program_nid);
        $disp = l($prog->title, 'node/'. $sch->program_nid) .'<br />';
      }
      $disp .= '<small>' . l(t('edit'), 'admin/schedule/edit/'. $sch->sid) . '</small>';
      if ($hourid < 12) {
        $amrows[$hourid][$dayid + 1] = $disp;
      } else {
        $pmrows[$hourid][$dayid + 1] = $disp;
      }
    }
  }

  // render the tables
  $output = theme('table', $cols, $amrows);
  $output .= "<br />\n";
  $output .= theme('table', $cols, $pmrows);
  return $output;

  return $output;
}

/**
 * Print a week's schedule page
 */
function station_schedule_week_page() {
  $daynames = station_day_name();

  $breadcrumb = array();
  $breadcrumb[] = array('path' => 'schedule', 'title' => t('Schedule'));
  menu_set_location($breadcrumb);

  // full week schedule
  drupal_set_title(t('Weekly Schedule'));

  foreach (station_schedule_load() as $d => $day) {
    $todayname = strtolower($daynames[$d]);

    $output = l('', '', array('id' => $todayname, 'name' => $todayname), null, null, false, true);
    $output .= '<h2>' . ucfirst($todayname) . '</h2>';

    // output the links to other days
    $output .= '<div class="schedule-hournav">';
    foreach ($daynames as $dayname) {
      $output .= l($dayname, 'schedule/week', null, null, strtolower($dayname), false, true) . ' ';
    }
    $output .= "</div>\n";
    // output the
    $output .= _station_schedule_view_day($day);

    $page .= theme('station_schedule_day', $output);
  }

  return $page;
}

/**
 * Today's schedule. This is just a short cut for the menu system.
 */
function station_schedule_today_page($hour = null) {
  return station_schedule_day_page(_station_today(), $hour);
}

/**
 * Print a day (or hour)'s schedule page.
 */
function station_schedule_day_page($day, $hour = null) {
  $day = station_valid_day($day);

  $breadcrumb = array();
  $breadcrumb[] = array('path' => 'schedule', 'title' => t('Schedule'));

  $daynames = station_day_name();
  $dayname = $daynames[$day];

  if ($hour === null) {
    // one day schedule
    drupal_set_title(t('Schedule for %dayname', array('%dayname' => $dayname)));

    $schedule = station_schedule_load(array('day' => $day), false);
    $output .= _station_schedule_view_day($schedule);

    $page = theme('station_schedule_day', $output);
  } else {
    $hour = station_valid_hour($hour);
    $hourname = station_hour_name($hour);

    // single hour
    drupal_set_title(t('Schedule for %dayname %hourname', array('%dayname' => $dayname, '%hourname' => $hourname)));
    $breadcrumb[] = array('path' => 'schedule/' . strtolower($dayname) . '/' . $hourname, 'title' => t($hourname));

    $schedules = station_schedule_load(array('day' => $day, 'hour' => $hour), false);
    $page = _station_schedule_view_hour($schedules[0]);
  }

  menu_set_location($breadcrumb);

  return $page;
}

/**
 * HTML for a days schedule
 *
 * @param array $day array with one day's worth of schedule objects
 */
function _station_schedule_view_day($day) {
  $cols = array('Time', 'Listen', 'Program', 'Genre', 'DJs');
  $rows = array();

  foreach ($day as $hour) {
    if ($hour->program_nid) {
      $program = node_load($hour->program_nid);

      $djs = array();
      foreach ($program->djs as $uid => $name) {
        $djs[] = l($name, 'user/' . $uid);
      }

      $links = station_program_listen_links($hour->program_nid, true);

      $rows[] = array(
        array(
          'data' => l($hour->hourname, 'schedule/' . strtolower($hour->dayname) . '/' . strtolower($hour->hourname)),
          'width' => 20),
        array(
          'data' => $links['listen'] . ' ' . $links['rss'],
          'width' => 40),
        array(
          'data' => l($program->title, 'node/' . $hour->program_nid),
          'width' => 240),
        array(
          'data' => check_plain($program->genre),
          'width' => 240),
        array(
          'data' => implode(',', $djs),
          'width' => 200),
      );
    }
  }
  return theme('station_schedule_table', $cols, $rows);
}

/**
 * Print a page view for a scheduled hour
 *
 * @param object $schedule schedule data object
 */
function _station_schedule_view_hour($schedule) {
  if ($schedule->program_nid) {
    $program = node_load($schedule->program_nid);

    $page .= node_view($program, false, false, true);
  } else {
    $page .= '<div>No program is scheduled.</div>';
  }

  return $page;
}
