<?php
// $Id$

/**
 * Implementation of hook_views_data().
 */
function station_schedule_views_data() {
  // ----------------------------------------------------------------
  // Basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['station_schedule']['table']['group']  = t('Station schedule');

  // For other base tables, explain how we join
  $data['station_schedule']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join', // this is actually optional
      'arguments' => array('station_schedule', 'node', 'nid', 'nid'),
     ),
  );

  // ----------------------------------------------------------------
  // Fields
  $data['station_schedule']['number'] = array(
    'title' => t('Scheduling increment'),
    'help' => t("The schedule's block size in minutes."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Number'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_schedule']['unscheduled_message'] = array(
    'title' => t('Unscheduled message'),
    'help' => t("Message displayed when there is no scheduled programming."),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Artist'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );



  // ----------------------------------------------------------------
  // Basic table information.

  $data['station_schedule_item']['table']['group']  = t('Station schedule item');

  // For other base tables, explain how we join
  $data['station_schedule_item']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join', // this is actually optional
      'arguments' => array('station_schedule_item', 'node', 'nid', 'nid'),
     ),
  );

  // ----------------------------------------------------------------
  // Fields
  $data['station_schedule_item']['iid'] = array(
    'title' => t('Schedule item id'),
    'help' => t("The schedule item's id."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
     ),
  );
  $data['station_schedule_item']['schedule_nid'] = array(
    'title' => t('Schedule'),
    'help' => t("The schedule the time is on."),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'field' => 'schedule_nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Schedule'),
    ),
  );
  $data['station_schedule_item']['program_nid'] = array(
    'title' => t('Program'),
    'help' => t("The scheduled program."),
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'field' => 'program_nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Program'),
    ),
  );
  $data['station_schedule_item']['start'] = array(
    'title' => t('Starting time'),
    'help' => t("Starting minute from Sunday midnight."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Number'),
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_schedule_item']['finish'] = array(
    'title' => t('Finishing time'),
    'help' => t("Finishing minute from Sunday midnight."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Number'),
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['station_schedule_item']['may_archive'] = array(
    'title' => t('May be archived'),
    'help' => t("Is this program allowed to be stored in the Station Archive."),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_boolean_operator',
       'label' => t('Active'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
