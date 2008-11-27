<?php
// $Id$

/**
 * Admin settings form.
 */
function station_program_admin_settings(&$form_state) {
  $form['station_program_redirect_on_add'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add multiple programs'),
    '#default_value' => variable_get('station_program_redirect_on_add', TRUE),
    '#description' => t('If this is checked after adding a new program node you will be redirected back to the new program form. This is much more convenient when entering multiple programs.'),
  );
  $form['module_cvs_id'] = array(
    '#type' => 'item',
    '#value' => '<pre>'. STATION_PROGRAM_CVS_ID .'</pre>',
  );

  return system_settings_form($form);
}
