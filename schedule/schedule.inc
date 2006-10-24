<?php

// $Id$

function _station_make_hour_compatible(&$s) {
  $s->start_day = (int) ($s->start / 1440);
  $s->start_hour = (int) (($s->start % 1440) / 60);
  $s->finish_day = (int) ($s->finish / 1440);
  $s->finish_hour = (int) (($s->finish % 1440) / 60);
  $s->day = $s->start_day;
  $s->hour = $s->start_hour;
}

function station_schedule_schedule_load($sid) {
  $result = db_query('SELECT * FROM {station_schedule} s WHERE s.sid = %d', $sid);
  if ($s = db_fetch_object($result)) {
    // FIXME this is a hack to get the schedule working...
    _station_make_hour_compatible($s);
    return $s;
  }
}

function station_schedule_schedule_load_day($day) {
  // there are 1440 minutes in a day (60minute * 24hours = 1day)
  $start = $day * 1440;
  $finish = $start + 1439;

  $ret = array();
  $result = db_query('SELECT * FROM {station_schedule} s WHERE s.start BETWEEN %d AND %d ORDER BY s.start', $start, $finish);
  while ($s = db_fetch_object($result)) {
    // FIXME this is a hack to get the schedule working...
    _station_make_hour_compatible($s);
    $ret[] = $s;
  }
  return $ret;
}

function station_schedule_schedule_load_week() {
  $ret = array();
  $result = db_query($query = 'SELECT * FROM {station_schedule} s ORDER BY s.start');
  while ($s = db_fetch_object($result)) {
    // FIXME this is a hack to get the schedule working...
    _station_make_hour_compatible($s);
    $ret[$s->day][$s->hour] = $s;
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
    $result = db_query($query = 'SELECT * FROM {station_schedule} s WHERE s.finish > %d AND s.start < %d ORDER BY s.start', $day_start, $day_finish);
    while ($s = db_fetch_object($result)) {
      // display blocks for unscheduled time periods
      if ($last_finish != $s->start) {
        $cell .= theme('station_schedule_admin_nonitem', $last_finish, $s->start);
      }
      $last_finish = $s->finish;

      // display the schedule item
      $height = ($s->finish - $s->start);
      $program = node_load($s->program_nid);
      $editurl = 'admin/schedule/edit/'. $s->sid;
      $cell .= theme('station_schedule_admin_item', $s->start, $s->finish, $program->title, TRUE, $editurl);
    }
    // any remaining time during the day
    if ($last_finish < $day_finish) {
      $cell .= theme('station_schedule_admin_nonitem', $last_finish, $day_finish);
    }

    $row[$i] = $cell;
  }

  // render the tables
  return theme('table', $cols, array($row), array('id' => 'station-schedule'));
}

function theme_station_schedule_admin_nonitem($start, $finish, $link = NULL) {
  $height = ($finish - $start);
  $output = "<div class='station-schedule-box station-schedule-unscheduled' style='height:{$height}px;'>";
  $output .= "<span class='station-schedule-time'>". theme('station_hour_range', $start, $finish) ."</span><br />";
  $output .= t('Unscheduled');
  $output .= "</div>\n";
  if ($link) {
    return l($output, $link, NULL, NULL, NULL, NULL, TRUE);
  }
  return $output;
}

function theme_station_schedule_admin_item($start, $finish, $title = '', $scheduled = FALSE, $link = NULL) {
  $height = ($finish - $start);
  $output = "<div class='station-schedule-box station-schedule-scheduled' style='height:{$height}px;'>";
  $output .= "<span class='station-schedule-time'>". theme('station_hour_range', $start, $finish) ."</span><br />";
  $output .= $title;
  $output .= "</div>\n";
  if ($link) {
    return l($output, $link, NULL, NULL, NULL, NULL, TRUE);
  }
  return $output;
}

function station_schedule_schedule_admin_form($sid = NULL) {
  $op = $_POST['op'];
  if ($op == t('Cancel')) {
    drupal_goto('admin/schedule');
  }
  if ($op == t('Delete')) {
    drupal_goto("admin/schedule/delete/$sid");
  }

  $day_options = station_day_name();
  $hour_options = array();
  for ($i = 0; $i < 24; $i++) {
    $hour_options[$i] = station_hour_name($i);
  }

  if ($sch = station_schedule_schedule_load($sid)) {
    $program = node_load($sch->program_nid);
    $program_title = $program->title;

    $form['sid'] = array(
      '#type' => 'value',
      '#value' => $sid,
    );
  }
  else {
    $sch = new stdClass;
    $sch->start_day = 0;
    $sch->finish_day = 0;
    $sch->start_hour = 0;
    $sch->finish_hour = 1;
  }

  $form['start_day'] = array(
    '#type' => 'select',
    '#title' => t('Starts'),
    '#default_value' => $sch->start_day,
    '#options' => $day_options,
  );
  $form['start_hour'] = array(
    '#type' => 'select',
    '#default_value' => $sch->start_hour,
    '#options' => $hour_options,
  );
  $form['finish_day'] = array(
    '#type' => 'select',
    '#title' => t('Ends'),
    '#default_value' => $sch->finish_day,
    '#options' => $day_options,
    '#description' => $description,
  );
  $form['finish_hour'] = array(
    '#type' => 'select',
    '#default_value' => $sch->finish_hour,
    '#options' => $hour_options,
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
  $start = station_minute_from_day_hour($form_values['start_day'], $form_values['start_hour']);
  $finish = station_minute_from_day_hour($form_values['finish_day'], $form_values['finish_hour']);

  if ($start >= $finish) {
    form_set_error('finish', t("The program has to finish after it starts."));
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
  $start = station_minute_from_day_hour($form_values['start_day'], $form_values['start_hour']);
  $finish = station_minute_from_day_hour($form_values['finish_day'], $form_values['finish_hour']);
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

  // full week schedule
  drupal_set_title(t('Weekly Schedule'));

  $cols = array(t('Time'));
  $cols = array_merge($cols, station_day_name());
  $row = array();

  for ($i = 0; $i < 24; $i++) {
    $cell .= theme('station_schedule_hour', $i);
  }
  $row[] = array('data' => $cell, 'id'=>'hour');

  for ($i = 0; $i < 7; $i++) {
    $cell = '';
    // find the minute that the day starts and finishes
    $day_start = $i * 1440;
    $day_finish = $day_start + 1440;
    $last_finish = $day_start;
    $result = db_query($query = 'SELECT * FROM {station_schedule} s WHERE s.finish > %d AND s.start < %d ORDER BY s.start', $day_start, $day_finish);
    while ($s = db_fetch_object($result)) {
      // display blocks for unscheduled time periods
      if ($last_finish != $s->start) {
        $cell .= theme('station_schedule_spacer', $last_finish, $s->start);
      }
      $last_finish = $s->finish;

      // display the schedule item
      $height = ($s->finish - $s->start);
      $program = node_load($s->program_nid);
      $cell .= theme('station_schedule_item', $s->start, $s->finish, $program);
    }
    // any remaining time during the day
    if ($last_finish < $day_finish) {
      $cell .= theme('station_schedule_spacer', $last_finish, $day_finish);
    }

    $row[] = array('data' => $cell);
  }

  // render the tables
  return theme('table', $cols, array($row), array('id' => 'station-schedule'));
}

function theme_station_schedule_hour($hour) {
  $height = 60;
  $output .= "<div class='station-schedule-box station-schedule-hour' style='height:{$height}px;'>";
  $output .= station_hour_name($hour);
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_spacer($start, $finish) {
  $height = ($finish - $start);
  $output = "<div class='station-schedule-box station-schedule-unscheduled' style='height:{$height}px;'></div>\n";
  return $output;
}

function theme_station_schedule_item($start, $finish, $program) {
  $height = ($finish - $start);
  $output = "<div class='station-schedule-box station-schedule-scheduled' style='height:{$height}px;'>\n";
  $output .= $program->title ."\n";
  if ($program->djs) {
    $djs = implode(', ', _station_schedule_program_load_djs($program->nid));
    $output .= "  <div class='station-schedule-djs'>{$djs}</div>\n";
  }
#  if ($program->genre) {
#    $output .= "  <div class='station-schedule-genre'>{$program->genre}</div>\n";
#  }
  $output .= "</div>";
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
    return theme('table', $cols, $rows, array('class'=>'station-schedule'));
  }
}
