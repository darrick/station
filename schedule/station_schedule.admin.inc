<?php
// $Id$

/**
 * Admin settings form.
 */
function station_schedule_admin_settings() {
  $form['djs'] = array(
    '#type' => 'fieldset',
    '#title' => t('DJ settings'),
  );
  $form['djs']['station_schedule_dj_title'] = array(
    '#type' => 'textfield',
    '#title' => t('DJ title'),
    '#default_value' => variable_get('station_schedule_dj_title', 'DJs'),
    '#description' => t("This setting lets you determine what you'd like to call the users associated with programs. It should be plural."),
    '#required' => TRUE,
  );
  $form['djs']['station_schedule_dj_role'] = array(
    '#type' => 'select',
    '#title' => t('DJs role'),
    '#default_value' => variable_get('station_schedule_dj_role', DRUPAL_AUTHENTICATED_RID),
    '#options' => user_roles(TRUE),
    '#description' => t("This lets you restrict DJs to members of a specific role. If you don't want any restrictions, select <em>authenticated user</em>s."),
  );

  $form['bc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Backwards compatibility'),
  );
  $schedules = array();
  foreach (station_get_schedules() as $nid => $schedule) {
    $schedules[$nid] = $schedule['title'];
  }
  $form['bc']['station_schedule_redirect_old_urls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect old links'),
    '#default_value' => variable_get('station_schedule_redirect_old_urls', 0),
    '#options' => $schedules,
    '#description' => t("Checking this will ensure that the station module continues to redirect old <code>/schedule</code> and <code>/station/schedule</code> links."),
  );
  $form['bc']['station_schedule_default'] = array(
    '#type' => 'select',
    '#title' => t('Default schedule'),
    '#default_value' => variable_get('station_schedule_default', 0),
    '#options' => $schedules,
    '#description' => t("This you select the schedule that will be used as the default for backwards compatibility."),
  );

  $form['station_schedule_program_node_type'] = array(
    '#type' => 'item',
    '#title' => t('Scheduled node type'),
    '#value' => t('The schedule is using the %type node provided by the %module module.', array('%type' => 'station_program', '%module' => node_get_types('module', 'station_program'))),
  );

  $form['module_cvs_id'] = array(
    '#type' => 'item',
    '#value' => '<pre>'. STATION_SCHEDULE_CVS_ID .'</pre>',
  );

  return system_settings_form($form);
}

function theme_station_schedule_admin_nonitem($node, $start, $finish) {
  $class = 'station-sch-box station-sch-unscheduled';
  $height = ($finish - $start);
  $link = url("node/{$node->nid}/schedule/add/0/{$start}/{$finish}");

  $output = "<div class='{$class}'><a id='schedule-{$start}' href='{$link}' style='height: {$height}px;'>";
  $output .= '<span class="station-sch-time">'. theme('station_hour_range', $start, $finish) .'</span>';
  $output .= '<span class="station-sch-title">'. t('<em>Unscheduled</em>') .'</span>';
  $output .= "</a></div>\n";

  return $output;
}

function theme_station_schedule_admin_item($node, $item) {
  $class = 'station-sch-box station-sch-scheduled';
  $height = ($item->finish - $item->start);
  $link = url("node/{$node->nid}/schedule/edit/{$item->iid}");

  $output = "<div class='{$class}'><a id='schedule-{$item->start}' href='{$link}' style='height: {$height}px;'>";
  $output .= '<span class="station-sch-time">'. theme('station_hour_range', $item->start, $item->finish) .'</span>';
  $output .= '<span class="station-sch-title">'. check_plain($item->program->title) .'</span>';
  $output .= "</a></div>\n";

  return $output;
}