<?php

// $Id$

function station_schedule_schedule_load($sid) {
  $result = db_query('SELECT * FROM {station_schedule} s WHERE s.sid = %d', $sid);
  if ($s = db_fetch_object($result)) {
    return $s;
  }
}

function station_schedule_schedule_load_day($day) {
  // there are 1440 minutes in a day (60minute * 24hours = 1day)
  $start = $day * 1440;
  $finish = $start + 1440;

  $ret = array();
  $result = db_query('SELECT * FROM {station_schedule} s WHERE s.finish > %d AND s.start < %d ORDER BY s.start', $start, $finish);
  while ($s = db_fetch_object($result)) {
    // if a show spans a day, limit its start and finish times to be with-in
    // the day
    if ($s->start < $start) {
      $s->start = $start;
    }
    if ($s->finish > $finish) {
      $s->finish = $finish;
    }
    $ret[] = $s;
  }
  return $ret;
}

function _station_schedule_load_program_at($minute) {
  $nid = (int) db_result(db_query('SELECT s.program_nid FROM {station_schedule} s WHERE s.start <= %d AND s.finish > %d', $minute, $minute));
  if ($nid) {
    return node_load($nid);
  }
  return FALSE;
}

function station_schedule_schedule_admin_list() {
  theme_add_style(drupal_get_path('module', 'station_schedule') .'/station_schedule.css');

  $cols = station_day_name();
  $row = array();
  for ($i = 0; $i < 7; $i++) {
    $cell = '';
    // find the minute that the day starts and finishes
    $day_start = $i * 1440;
    $day_finish = $day_start + 1440;
    $last_finish = $day_start;
    foreach (station_schedule_schedule_load_day($i) as $s) {
      // display blocks for unscheduled time periods
      if ($last_finish != $s->start) {
        $cell .= theme('station_schedule_admin_nonitem', $last_finish, $s->start);
      }
      $last_finish = $s->finish;

      // display the schedule item
      $height = ($s->finish - $s->start);
      $program = node_load($s->program_nid);
      $editurl = 'admin/schedule/edit/'. $s->sid;
      $cell .= theme('station_schedule_admin_item', $s->start, $s->finish, $program->title, $editurl);
    }
    // any remaining time during the day
    if ($last_finish < $day_finish) {
      $cell .= theme('station_schedule_admin_nonitem', $last_finish, $day_finish);
    }

    $row[$i] = $cell;
  }

  // render the tables
  return theme('table', $cols, array($row), array('id' => 'station-sch', 'class' => 'station-sch-admin'));
}

function theme_station_schedule_admin_nonitem($start, $finish, $link = NULL) {
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= "<div class='station-sch-time'>". theme('station_hour_range', $start, $finish) ."</div>";
  $output .= t('Unscheduled');
  $output .= "</div>\n";
  if ($link) {
    return l($output, $link, NULL, NULL, NULL, NULL, TRUE);
  }
  return $output;
}

function theme_station_schedule_admin_item($start, $finish, $title = '', $link = NULL) {
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= "<div class='station-sch-time'>". theme('station_hour_range', $start, $finish) ."</div>";
  $output .= $title;
  $output .= "</div>\n";
  if ($link) {
    return l($output, $link, NULL, NULL, NULL, NULL, TRUE);
  }
  return $output;
}

function station_schedule_schedule_admin_form($op, $id = NULL) {
  $post_op = $_POST['op'];
  if ($post_op == t('Cancel')) {
    drupal_goto('admin/schedule');
  }
  if ($post_op == t('Delete')) {
    drupal_goto("admin/schedule/delete/$id");
  }

  $day_options = station_day_name();
  $minute_options = array();
  $increment = variable_get('station_schedule_increment', 60);
  for ($minute = 0; $minute < 24 * 60; $minute += $increment) {
    $time = station_time_from_minute($minute);
    $minute_options[$minute] = $time['H'] .':'. $time['i'];
  }

  if ($op == 'add') {
    if ($id && $program = node_load($id)) {
      $program_title = $program->title;
    }
    $sch = new stdClass;
    $sch->sid = 0;
    $sch->start = 0;
    $sch->finish = 60;
  }
  else if ($op == 'edit') {
    if ($id && $sch = station_schedule_schedule_load($id)) {
      $program = node_load($sch->program_nid);
      $program_title = $program->title;
      $form['sid'] = array(
        '#type' => 'value',
        '#value' => $sch->sid,
      );
    }
    else {
      drupal_goto('admin/schedule');
    }
  }

  $form['start_day'] = array(
    '#type' => 'select',
    '#title' => t('Starts'),
    '#default_value' => station_day_from_minute($sch->start),
    '#options' => $day_options,
  );
  $form['start_minutes'] = array(
    '#type' => 'select',
    '#default_value' => $sch->start % 1440,
    '#options' => $minute_options,
  );
  $form['finish_day'] = array(
    '#type' => 'select',
    '#title' => t('Ends'),
    '#default_value' => station_day_from_minute($sch->finish),
    '#options' => $day_options,
    '#description' => $description,
  );
  $form['finish_minutes'] = array(
    '#type' => 'select',
    '#default_value' => $sch->finish % 1440,
    '#options' => $minute_options,
  );
  $form['program_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Program'),
    '#description' => t("Type the name of this hour's program, or if nothing is scheduled, leave it blank."),
    '#default_value' => $program_title,
    '#maxlength' => 128,
    '#autocomplete_path' => 'station/autocomplete/program'
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($sch->sid) {
    $form['buttons']['delete'] = array(
      '#type' => 'button',
      '#value' => t('Delete'),
    );
  }
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return drupal_get_form('station_schedule_schedule_admin_form', $form);
}

function station_schedule_schedule_admin_form_validate($form_id, $form_values) {
  $start = ($form_values['start_day'] * 1440) + $form_values['start_minutes'];
  $finish = ($form_values['finish_day'] * 1440) + $form_values['finish_minutes'];

  if ($start >= $finish) {
    form_set_error('finish', t("The program must start before it finishes."));
  }

  $result = db_query('SELECT count(*) AS count, min(s.start) AS start, max(s.finish) AS finish FROM {station_schedule} s WHERE s.sid <> %d AND s.finish > %d AND s.start < %d', $form_values['sid'], $start, $finish);
  if ($overlap = db_fetch_object($result)) {
    if ($overlap->count == 1) {
      form_set_error('', t("The program overlaps another scheduled item at %time.", array('%time' => theme('station_dayhour_range', $overlap->start, $overlap->finish))));
    }
    else if ($overlap->count > 1) {
      form_set_error('', t("The program overlaps %number scheduled items from %time.", array('%time' => theme('station_dayhour_range', $overlap->start, $overlap->finish), '%number' => $overlap->count)));
    }
  }

  // check for a valid program title
  if (!$prog = node_load(array('title' => $form_values['program_title'], 'type' => 'program'))) {
    form_set_error('program_title', t('You need to specify a valid program name.'));
  }
}

function station_schedule_schedule_admin_form_submit($form_id, $form_values) {
  $start = ($form_values['start_day'] * 1440) + $form_values['start_minutes'];
  $finish = ($form_values['finish_day'] * 1440) + $form_values['finish_minutes'];
  $program = node_load(array('type' => 'program', 'title' => $form_values['program_title']));

  if ($form_values['sid']) {
    db_query("UPDATE {station_schedule} SET program_nid = %d, start = %d, finish = %d WHERE sid = %d",
      $program->nid, $start, $finish, $form_values['sid']);
  }
  else {
    $sid = db_next_id('{station_schedule}_sid');
    db_query("INSERT INTO {station_schedule} (sid, program_nid, start, finish) VALUES (%d, %d, %d, %d)",
      $sid, $program->nid, $start, $finish);
  }
  drupal_goto('admin/schedule');
}

/**
 * Page to confirm the deletion of a schedule item.
 */
function station_schedule_schedule_admin_delete($sid = '') {
  $sch = station_schedule_schedule_load($sid);
  if (!$sch) {
    return 'admin/schedule';
  }

  $program = node_load($sch->program_nid);
  $title = $program->title;
  $time = theme('station_dayhour_range', $sch->start, $sch->finish);

  $form['sid'] = array('#type' => 'value', '#value' => $sch->sid);
  return confirm_form('station_schedule_schedule_admin_delete', $form,
    t('Are you sure you want to delete %title at %time?', array('%title' => $program->title, '%time' => $time)),
    $_GET['destination'] ? $_GET['destination'] : 'admin/schedule',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Delete a schedule item.
 */
function station_schedule_schedule_admin_delete_submit($formid, $form) {
  if ($form['confirm']) {
    db_query("DELETE FROM {station_schedule} WHERE sid = %d", $form['sid']);
    return 'admin/schedule';
  }
}


/**
 * Print a week's schedule page
 */
function station_schedule_schedule_week_page() {
  theme_add_style(drupal_get_path('module', 'station_schedule') .'/station_schedule.css');
  drupal_set_title(t('Weekly Schedule'));

  $today = station_today();

  // build the headers
  $cols = array('data' => t('Time'));
  foreach (station_day_name() as $name) {
    $cols[] = array('data' => $name);
  }
  // ... add a class to indicate what day it is.
  $cols[$today]['class'] = 'station-sch-now-day';

  $row = array();
  // put in a column of hours
  for ($i = 0; $i < 24; $i++) {
    $cell .= theme('station_schedule_hour', $i);
  }
  $row[] = array('data' => $cell, 'id' => 'station-sch-hours');
  // now a column for each day
  for ($i = 0; $i < 7; $i++) {
    $cell = '';
    // find the minute that the day starts and finishes
    $day_start = $i * 1440;
    $day_finish = $day_start + 1440;
    $last_finish = $day_start;
    foreach (station_schedule_schedule_load_day($i) as $s) {
      // display blocks for unscheduled time before this program
      if ($last_finish != $s->start) {
        $cell .= theme('station_schedule_spacer', $last_finish, $s->start);
      }
      $last_finish = $s->finish;
      // display the schedule item
      $program = node_load($s->program_nid);
      $cell .= theme('station_schedule_item', $s->start, $s->finish, $program);
    }
    // create an unscheduled block for any remaining time
    if ($last_finish < $day_finish) {
      $cell .= theme('station_schedule_spacer', $last_finish, $day_finish);
    }

    $row[] = array('data' => $cell);
  }
  $row[$today+1]['class'] = 'station-sch-now-day';

  // render the tables
  return theme('table', $cols, array($row), array('id' => 'station-sch'));
}

function theme_station_schedule_hour($hour) {
  $class = 'station-sch-box station-sch-hour';
  $height = 60;
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= station_hour_name($hour);
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_spacer($start, $finish) {
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= "<div class='station-sch-time'>". theme('station_hour_range', $start, $finish) ."</div>";
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_item($start, $finish, $program) {
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>\n";
  $output .= "<div class='station-sch-time'>". theme('station_hour_range', $start, $finish) ."</div>";
  $output .= $program->title;
  if ($program->djs) {
    $djs = implode(', ', _station_schedule_program_load_djs($program->nid));
    $output .= "<div class='station-sch-djs'>{$djs}</div>";
  }
#  if ($program->genre) {
#    $output .= "<div class='station-sch-genre'>{$program->genre}</div>\n";
#  }
  $output .= "</div>\n";
  return l($output, 'node/'. $program->nid, NULL, NULL, NULL, NULL, TRUE);
}


/**
 * Today's schedule. This is just a short cut for the menu system.
 */
function station_schedule_schedule_today_page() {
  return station_schedule_schedule_day_page(station_today());
}

/**
 * Print a day (or hour)'s schedule page.
 */
function station_schedule_schedule_day_page($day, $hour = NULL) {
  $day = station_valid_day($day);
  $dayname = station_day_name($day);

  if ($hour === null) {
    // one day schedule
    drupal_set_title(t('Schedule for %dayname', array('%dayname' => $dayname)));

    $schedule = station_schedule_schedule_load_day($day);
    $dayschedule = _station_schedule_schedule_view_day($schedule);
    if ($dayschedule) {
      $output = $dayschedule;
    }
    else {
      $output = t('There are no programs scheduled for %dayname', array('%dayname' => $dayname));
    }

    return '<div class="node">'. $output .'</div>';
  }
  else {
    $hour = station_valid_hour($hour);
    $hourname = station_hour_name($hour);

    // single hour
    drupal_set_title(t('Schedule for %dayname at %hourname', array('%dayname' => $dayname, '%hourname' => $hourname)));
    $breadcrumb = array(
      array('title' => t('Schedule'), 'path' => 'schedule'),
      array('title' => t($dayname),   'path' => 'schedule/'. strtolower($dayname)),
      array('title' => t($hourname),  'path' => 'schedule/'. strtolower($dayname) .'/'. strtolower($hourname)),
    );
    menu_set_location($breadcrumb);

    $minutes = station_minute_from_day_hour($day, $hour);
    $program = _station_schedule_load_program_at($minutes);
    if ($program) {
      return node_view($program, false, false, true);
    }
    else {
      return '<div>'. t("No program is scheduled.") .'</div>';
    }
  }
}

/**
 * HTML for a days schedule
 *
 * @param array $day array with one day's worth of schedule objects
 */
function _station_schedule_schedule_view_day($day) {
  $rows = array();
  $row = array();
  foreach ($day as $hour) {
    if ($hour->program_nid) {
      $program = node_load($hour->program_nid);
      $row['time'] = array('width' => 50,
        'data' => theme('station_hour_range', $hour->start, $hour->finish));
      if (station_has_archive()) {
        $links = station_listen_links($program, TRUE);
        $row['listen'] = array('width' => 50,
          'data' => $links['listen'] .' '. $links['rss']);
      }
      $row['program'] = array('width' => 240,
        'data' => l($program->title, 'node/' . $program->nid));
      $row['genre'] = array('width' => 240,
        'data' => check_plain($program->genre));
      $row['djs'] = array('width' => 200,
        'data' => implode(', ', station_schedule_program_get_themed_djs($program->nid)));

      $rows[] = $row;
    }
  }

  if ($rows) {
    if (station_has_archive()) {
      $cols = array(t('Time'), t('Listen'),  t('Program'), t('Genre'), t('DJs'));
    }
    else {
      $cols = array(t('Time'), t('Program'), t('Genre'), t('DJs'));
    }
    return theme('table', $cols, $rows, array('class'=>'station-sch'));
  }
}
