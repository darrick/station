<?php

// $Id$

function station_program_views_default_views() {
  $view = new stdClass();
  $view->name = 'station_schedule_programs';
  $view->description = 'Programs';
  $view->access = array(
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
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'station/programs';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->menu = TRUE;
  $view->menu_title = 'Programs';
  $view->menu_tab = FALSE;
  $view->menu_tab_default = FALSE;
  $view->menu_weight = '';
  $view->sort = array(
    array(
      'tablename' => 'node',
      'field' => 'title',
      'sortorder' => 'ASC',
      'options' => '',
    ),
  );
  $view->field = array(
    array(
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Program',
      'handler' => 'views_handler_field_nodelink',
      'defaultsort' => 'DESC',
    ),
    array(
      'tablename' => 'station_program',
      'field' => 'genre',
      'label' => 'Genre',
    ),
    array(
      'tablename' => 'station_djs',
      'field' => 'djs',
      'label' => 'DJs',
    ),
  );
  $view->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array(
        0 => 'program',
      ),
    ),
    array(
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
  );
  $view->exposed_filter = array();
  $view->requires = array(node, station_program);
  $views[$view->name] = $view;


  return $views;
}