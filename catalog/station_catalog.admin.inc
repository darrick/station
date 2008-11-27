<?php
// $Id$

/**
 * Admin settings form.
 */
function station_catalog_admin_settings(&$form_state) {
  $form['station_catalog_redirect_on_add'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add multiple albums'),
    '#default_value' => variable_get('station_catalog_redirect_on_add', TRUE),
    '#description' => t('If this is checked after adding a new album you will be redirected back to the new album form. This is much more convienient when entering multiple albums.'),
  );
  $form['module_cvs_id'] = array(
    '#type' => 'item',
    '#value' => '<pre>'. STATION_CATALOG_CVS_ID .'</pre>',
  );

  return system_settings_form($form);
}
