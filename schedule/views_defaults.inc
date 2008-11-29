<?php

// $Id$

/**
 * Implementation of hook_views_default_views().
 */
function station_schedule_views_default_views() {


  $view = new stdClass();
  $view->name = 'station_schedule_day';
  $view->description = 'What\'s on the schedule on a given day.';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Day Schedule';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'There are no programs scheduled for this day.';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = '';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->sort = array (
    array (
      'tablename' => 'station_schedule_item',
      'field' => 'start',
      'sortorder' => 'ASC',
      'options' => '',
    ),
  );
  $view->argument = array (
    array (
      'type' => 'station_schedule_argument_schedule_nid',
      'argdefault' => '4',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
    array (
      'type' => 'station_schedule_argument_schedule_item_day',
      'argdefault' => '4',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'station_schedule_item',
      'field' => 'start',
      'label' => 'Time',
      'defaultsort' => 'DESC',
      'options' => 'hour',
    ),
    array (
      'tablename' => 'station_schedule_item',
      'field' => 'archive_links',
      'label' => 'Listen',
      'options' => 'icon',
    ),
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Program',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array (
      'tablename' => 'station_program',
      'field' => 'genre',
      'label' => 'Genre',
    ),
    array (
      'tablename' => 'station_djs',
      'field' => 'djs',
      'label' => variable_get('station_schedule_dj_title', 'DJs'),
    ),
  );
  // If there's no archive remove the listen links.
  if (!station_has_archive()) {
    unset($view->field[1]);
  }
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'station_program',
),
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(station_schedule_item, node, station_program, station_djs);
  $views[$view->name] = $view;



  $view = new stdClass();
  $view->name = 'station_scheduled_programs';
  $view->description = 'Scheduled programs';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Programs';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'There are currently no scheduled programs.';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'station/programs/scheduled';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->menu = TRUE;
  $view->menu_title = 'Scheduled';
  $view->menu_tab = TRUE;
  $view->menu_tab_weight = '0';
  $view->menu_tab_default = TRUE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_tab_default_parent_type = 'normal';
  $view->menu_parent_tab_weight = '0';
  $view->menu_parent_title = 'Programs';
  $view->sort = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'sortorder' => 'ASC',
      'options' => '',
    ),
  );
  $view->argument = array (
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Title',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array (
      'tablename' => 'station_program',
      'field' => 'genre',
      'label' => 'Genre',
    ),
    array (
      'tablename' => 'station_djs',
      'field' => 'djs',
      'label' => variable_get('station_schedule_dj_title', 'DJs'),
    ),
    array (
      'tablename' => 'station_schedule_item',
      'field' => 'times',
      'label' => 'Times',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'distinct',
      'operator' => '=',
      'options' => '',
      'value' => array (
  0 => 'distinct',
),
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'station_program',
),
    ),
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'station_schedule_item',
      'field' => 'is_scheduled',
      'operator' => '1',
      'options' => '',
      'value' => '',
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node, station_program, station_djs, station_schedule_item);
  $views[$view->name] = $view;



  $view = new stdClass();
  $view->name = 'station_unscheduled_programs';
  $view->description = 'Unscheduled programs';
  $view->access = array (
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Programs';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'There are currently no unscheduled programs.';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'station/programs/unscheduled';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->menu = TRUE;
  $view->menu_title = 'Unscheduled';
  $view->menu_tab = TRUE;
  $view->menu_tab_weight = '1';
  $view->menu_tab_default = FALSE;
  $view->menu_tab_default_parent = NULL;
  $view->menu_tab_default_parent_type = 'tab';
  $view->menu_parent_tab_weight = '0';
  $view->menu_parent_title = '';
  $view->sort = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'sortorder' => 'ASC',
      'options' => '',
    ),
  );
  $view->argument = array (
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Title',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array (
      'tablename' => 'station_program',
      'field' => 'genre',
      'label' => 'Genre',
    ),
    array (
      'tablename' => 'station_djs',
      'field' => 'djs',
      'label' => variable_get('station_schedule_dj_title', 'DJs'),
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'station_program',
),
    ),
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'station_schedule_item',
      'field' => 'is_scheduled',
      'operator' => '0',
      'options' => '',
      'value' => '',
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node, station_program, station_djs, station_schedule_item);
  $views[$view->name] = $view;


  return $views;
}
