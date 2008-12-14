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
  if (!empty($program->field_station_program_dj)) {
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
  if ($view = views_get_view('station_schedule_items')) {
    $display_id = 'default';
    if ($view->access($display_id)) {
      return $view->execute_display($display_id);
    }
    $view->destroy();
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
