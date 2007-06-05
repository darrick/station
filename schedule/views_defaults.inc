<?php

// $Id$

function station_schedule_views_default_views() {
  $view = new stdClass();
  $view->name = 'station_scheduled_programs';
  $view->description = 'Programs';
  $view->access = array (
  0 => '1',
  1 => '2',
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Programs';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'There are no currently no scheduled programs.';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'station/programs/scheduled';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->menu = TRUE;
  $view->menu_title = 'Programs';
  $view->menu_tab = TRUE;
  $view->menu_tab_weight = '0';
  $view->menu_tab_default = TRUE;
  $view->menu_tab_default_parent = NULL;
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
    array (
      'tablename' => 'station_schedule',
      'field' => 'times',
      'label' => 'Times',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
  0 => 'program',
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
      'tablename' => 'station_schedule',
      'field' => 'is_scheduled',
      'operator' => '1',
      'options' => '',
      'value' => '',
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node, station_program, station_djs, station_schedule);
  $views[$view->name] = $view;



  $view = new stdClass();
  $view->name = 'station_unscheduled_programs';
  $view->description = 'Unscheduled programs';
  $view->access = array (
  0 => '1',
  1 => '2',
);
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Unscheduled programs';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = 'There are no currently no unscheduled programs.';
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
  0 => 'program',
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
      'tablename' => 'station_schedule',
      'field' => 'is_scheduled',
      'operator' => '0',
      'options' => '',
      'value' => '',
    ),
  );
  $view->exposed_filter = array (
  );
  $view->requires = array(node, station_program, station_djs, station_schedule);
  $views[$view->name] = $view;


  return $views;
}