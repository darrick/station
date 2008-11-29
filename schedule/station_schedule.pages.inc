<?php
// $Id$


function theme_station_schedule_hour($hour) {
  $class = 'station-sch-box station-sch-hour';
  $height = 60;
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= theme('station_hour', $hour * 60);
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_spacer($start, $finish) {
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= "<span class='station-sch-time'>". theme('station_hour_range', $start, $finish) ."</span>";
  $output .= "</div>\n";
  return $output;
}

function theme_station_schedule_item($start, $finish, $program) {
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($finish - $start);
  $link = url('node/'. $program->nid);

  $output = "<div class='{$class}'><a href='{$link}' style='height: {$height}px;'>";
  $output .= '<span class="station-sch-time">'. theme('station_hour_range', $start, $finish) .'</span>';
  $output .= '<span class="station-sch-title">'. check_plain($program->title) .'</span>';
  if (count($program->field_station_program_dj)) {
    $djs = array();
    foreach ($program->field_station_program_dj as $entry) {
      $user = user_load($entry);
      $djs[] .= $user->name;
    }
    $output .= '<span class="station-sch-djs">'. check_plain(implode(', ', $djs)) .'</span>';
  }
  $output .= "</a></div>\n";
  return $output;
}

/**
 * Print a day's schedule page.
 */
function station_schedule_day_page($node, $day) {
  drupal_set_title(check_plain($node->title));

  if ($view = views_get_view('station_schedule_day')) {
    // Set $view->url to current page so views with exposed filters submit
    // back to the same page
    $view->url = $_GET['q'];
    $view_args = array($node->nid, $day);
    return views_build_view('embed', $view, $view_args, FALSE, NULL);
  }
  return drupal_not_found();
}


function station_schedule_item_list($node) {
  drupal_add_css(drupal_get_path('module', 'station_schedule') .'/station_schedule.css');
  drupal_set_title(check_plain($node->title));

  $header = array();
  $row = array();
  foreach ($node->schedule as $day => $items) {
    $header[$day] = station_day_name($day);
    $row[$day] = '';

    // The last finish pointer starts at the beginning of the day.
    $last_finish = $day * MINUTES_IN_DAY;
    $day_finish = ($day + 1) * MINUTES_IN_DAY;
    foreach ($items as $item) {
      // Display blocks for unscheduled time periods
      if ($last_finish != $item->start) {
        $row[$day] .= theme('station_schedule_admin_nonitem', $node, $last_finish, $item->start);
      }
      $last_finish = $item->finish;

      // Display the schedule item
      $item->program = node_load($item->program_nid);
      $row[$day] .= theme('station_schedule_admin_item', $node, $item);
    }
    // Display a block for any remaining time during the day.
    if ($last_finish < $day_finish) {
      $row[$day] .= theme('station_schedule_admin_nonitem', $node, $last_finish, $day_finish);
    }
  }

  // Render the table
  return theme('table', $header, array($row), array('id' => 'station-sch', 'class' => 'station-sch-admin'));
}

function station_schedule_item_add($schedule, $program_id = NULL, $start = 0, $finish = 60) {
  drupal_set_title(t('Add item to %title', array('%title' => $schedule->title)));

  $schedule_item = new stdClass;
  $schedule_item->schedule_nid = $schedule->nid;
  $schedule_item->iid = NULL;
  $schedule_item->program_nid = $program_id;
  $schedule_item->start = (int) $start;
  $schedule_item->finish = (int) $finish;
  $schedule_item->may_archive = TRUE;

  return drupal_get_form('station_schedule_item_edit_form', $schedule, $schedule_item, $start, $finish);
}

function station_schedule_item_edit($schedule, $schedule_item = NULL, $start = 0, $finish = 60) {
  drupal_set_title(t('Edit %title item', array('%title' => $schedule->title)));

  return drupal_get_form('station_schedule_item_edit_form', $schedule, $schedule_item, $start, $finish);
}


function station_schedule_item_edit_form(&$form_state, $node, $schedule_item = NULL, $start = 0, $finish = 60) {
  if (isset($form_state['post']['op']) && $form_state['post']['op'] == t('Cancel')) {
    drupal_goto("node/{$node->nid}/schedule", NULL, "schedule-{$start}");
  }
  $program_title = '';
  if (!empty($schedule_item->program_nid) && $program = node_load($schedule_item->program_nid)) {
    $program_title = $program->title;
  }

  $day_options = station_day_name();
  $minute_options = array();
  // Make sure the increment will advance the counter.
  $increment = ($node->settings['increment'] < 1) ? 1 : $node->settings['increment'];
  for ($minute = 0; $minute <= 24 * 60; $minute += $increment) {
    $time = station_time_from_minute($minute);
    $minute_options[$minute] = $time['time'] . $time['a'];
  }

  $form['old_schedule_item'] = array(
    '#type' => 'value',
    '#value' => $schedule_item,
  );
  $form['schedule_nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['iid'] = array(
    '#type' => 'value',
    '#value' => $schedule_item->iid,
  );
  $form['start_day'] = array(
    '#type' => 'select',
    '#title' => t('Starts'),
    '#default_value' => station_day_from_minute($schedule_item->start),
    '#options' => $day_options,
  );
  $form['start_minutes'] = array(
    '#type' => 'select',
    '#default_value' => $schedule_item->start % MINUTES_IN_DAY,
    '#options' => $minute_options,
  );
  $form['finish_day'] = array(
    '#type' => 'select',
    '#title' => t('Ends'),
    '#default_value' => station_day_from_minute($schedule_item->finish),
    '#options' => $day_options,
  );
  $form['finish_minutes'] = array(
    '#type' => 'select',
    '#default_value' => $schedule_item->finish % MINUTES_IN_DAY,
    '#options' => $minute_options,
  );
  // Make sure a range that ends on midnigh of one day gets pushed back
  // to the previous day.
  if ($form['finish_minutes']['#default_value'] == 0) {
    $form['finish_day']['#default_value']--;
    $form['finish_minutes']['#default_value'] = MINUTES_IN_DAY;
  }
  $form['program_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Program'),
    '#description' => t("Enter the name of the program."),
    '#default_value' => $program_title,
    '#maxlength' => 128,
    '#autocomplete_path' => 'station/autocomplete/program',
  );
  $form['may_archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Can be saved in Station Archive'),
    '#default_value' => $schedule_item->may_archive,
    '#description' => t('Checking this indicates that the Station Archive module can save audio recordings of the program at this timeslot to the archive.'),
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($schedule_item->iid) {
    $form['buttons']['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('station_schedule_item_edit_remove_submit'),
    );
  }
  $form['buttons']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/**
 * Button sumit function: handle the 'Remove' button on the item edit form.
 */
function station_schedule_item_edit_remove_submit($form, &$form_state) {
  $destination = '';
  if (isset($_REQUEST['destination'])) {
    $destination = drupal_get_destination();
    unset($_REQUEST['destination']);
  }
  $form_state['redirect'] = array('node/'. $form_state['values']['schedule_nid'] .'/schedule/remove/'. $form_state['values']['iid'], $destination);

}

function station_schedule_item_edit_form_validate($form, &$form_state) {
  $start = ($form_state['values']['start_day'] * MINUTES_IN_DAY) + $form_state['values']['start_minutes'];
  $finish = ($form_state['values']['finish_day'] * MINUTES_IN_DAY) + $form_state['values']['finish_minutes'];

  if ($start >= $finish) {
    form_set_error('finish', t("The program must start before it finishes."));
  }

  $result = db_query('SELECT count(*) AS count, min(s.start) AS start, max(s.finish) AS finish FROM {station_schedule_item} s WHERE s.schedule_nid = %d AND s.iid <> %d AND s.finish > %d AND s.start < %d', $form_state['values']['schedule_nid'], $form_state['values']['iid'], $start, $finish);
  if ($overlap = db_fetch_object($result)) {
    if ($overlap->count == 1) {
      form_set_error('', t("The program overlaps another scheduled item at %time.", array('%time' => theme('station_dayhour_range', $overlap->start, $overlap->finish))));
    }
    else if ($overlap->count > 1) {
      form_set_error('', t("The program overlaps %number scheduled items from %time.", array('%time' => theme('station_dayhour_range', $overlap->start, $overlap->finish), '%number' => $overlap->count)));
    }
  }

  // check for a valid program title
  if (!$prog = node_load(array('title' => $form_state['values']['program_title'], 'type' => 'station_program'))) {
    form_set_error('program_title', t('You need to specify a valid program name.'));
  }
}

function station_schedule_item_edit_form_submit($form, &$form_state) {
  $program = node_load(array('type' => 'station_program', 'title' => $form_state['values']['program_title']));

  $record = array(
    'iid' => $form_state['values']['iid'],
    'schedule_nid' => $form_state['values']['schedule_nid'],
    'program_nid' => $program->nid,
    'start' => ($form_state['values']['start_day'] * MINUTES_IN_DAY) + $form_state['values']['start_minutes'],
    'finish' => ($form_state['values']['finish_day'] * MINUTES_IN_DAY) + $form_state['values']['finish_minutes'],
    'may_archive' => $form_state['values']['may_archive'],
  );

  if ($form_state['values']['iid']) {
    drupal_write_record('station_schedule_item', $record, array('iid'));
    $old_record = (array) $form_state['values']['old_schedule_item'];
    if ($old_record['program_nid'] != $record['program_nid']) {
      _station_send_notice('schedule', 'remove', $old_record);
      _station_send_notice('schedule', 'add', $record);
    }
    elseif ($old_record['start'] != $record['start'] || $old_record['finish'] != $record['finish']) {
      _station_send_notice('schedule', 'change', $record);
    }
  }
  else {
    drupal_write_record('station_schedule_item', $record);
    _station_send_notice('schedule', 'add', $record);
  }
  $form_state['redirect'] = array('node/'. $record['schedule_nid'] .'/schedule', NULL, 'schedule-'. $record['start']);
}

/**
 * Page to confirm the removal of a schedule item.
 */
function station_schedule_item_remove_form(&$form_state, $node, $schedule_item) {
  $program = node_load($schedule_item->program_nid);

  $form['schedule_nid'] = array('#type' => 'value', '#value' => $node->nid);
  $form['program_nid'] = array('#type' => 'value', '#value' => $schedule_item->program_nid);
  $form['iid'] = array('#type' => 'value', '#value' => $schedule_item->iid);
  return confirm_form(
    $form,
    t('Are you sure you want to unschedule %title from %schedule at %time?', array('%title' => $program->title, '%schedule' => $node->title, '%time' => theme('station_dayhour_range', $schedule_item->start, $schedule_item->finish))),
    isset($_GET['destination']) ? $_GET['destination'] : "node/{$node->nid}/schedule/edit/{$schedule_item->iid}",
    t('This action cannot be undone, but you can always re-add the program to the schedule.'),
    t('Remove'),
    t('Cancel')
  );
}

/**
 * Delete a schedule item.
 */
function station_schedule_item_remove_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    db_query("DELETE FROM {station_schedule_item} WHERE iid = %d", $form_state['values']['iid']);
    _station_send_notice('schedule', 'remove', array('schedule_nid' => $form_state['values']['schedule_nid'], 'program_nid' => $form_state['values']['program_nid'], 'iid' => $form_state['values']['iid']));
    $form_state['redirect'] = 'node/'. $form_state['values'] .'/schedule';
  }
}
