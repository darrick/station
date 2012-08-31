<?php
/**
 * @file
 */

/**
 * Redirect the visitor to the schedule's current day view.
 */
function station_schedule_goto_today($schedule) {
  drupal_goto('node/' . $schedule->nid . '/view/' . drupal_strtolower(station_day_name(date('w'))));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_station_schedule_hour($variables) {
  $hour = $variables['hour'];
  $class = 'station-sch-box station-sch-hour';
  $height = 60;
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= theme('station_hour', array('time' => $hour * 60));
  $output .= "</div>\n";
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_station_schedule_spacer($variables) {
  $start = $variables['start'];
  $finish = $variables['finish'];
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $output = "<div class='{$class}' style='height:{$height}px;'>";
  $output .= "<span class='station-sch-time'>" . theme('station_hour_range', array('start' => $start, 'finish' => $finish)) . "</span>";
  $output .= "</div>\n";
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_station_schedule_item($variables) {
  $start = $variables['start'];
  $finish = $variables['finish'];
  $program = $variables['program'];
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($finish - $start);
  $link = url('node/' . $program->nid);
  $time = theme('station_hour_range', array('start' => $start, 'finish' => $finish));

  $output = "<div class='{$class}'><a href='{$link}' style='height: {$height}px;' title='{$time}'>";
  $output .= "<span class='station-sch-time'>{$time}</span>";
  $output .= '<span class="station-sch-title">' . check_plain($program->title) . '</span>';
  if (!empty($program->field_station_program_dj)) {
    $djs = array();
    $station_program_djs_raw = field_get_items('node', $program, 'field_station_program_dj');
    foreach ($station_program_djs_raw as $entry) {
      // TODO Convert "user_load" to "user_load_multiple" if "$entry" is other than a uid.
      // To return a single user object, wrap "user_load_multiple" with "array_shift" or equivalent.
      // Example: array_shift(user_load_multiple(array(), $entry))
      // It should be working.
      $user = user_load($entry['uid']);
      $djs[] .= $user->name;
    }
    $output .= '<span class="station-sch-djs">' . check_plain(implode(', ', $djs)) . '</span>';
  }
  $output .= "</a></div>\n";
  return $output;
}

/**
 * Print a day's schedule page.
 */
function station_schedule_day_page($node, $day) {
  if ($view = views_get_view('station_schedule_day')) {
    $display_id = 'default';
    if ($view->access($display_id)) {
      return $view->execute_display($display_id, array($node->nid, $day));
    }
    $view->destroy();
  }
  return drupal_not_found();
}

