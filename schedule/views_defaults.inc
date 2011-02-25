<?php


function station_schedule_views_default_views() {
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
  $view->menu_title = 'programs';
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
      'tablename' => 'station_program',
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



  $view = new stdClass();
  $view->name = 'station_schedule_playlists';
  $view->description = 'Playlists';
  $view->access = array(
    0 => '1',
    1 => '2',
  );
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Playlists';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'teaser';
  $view->url = 'station/playlists';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '20';
  $view->menu = TRUE;
  $view->menu_title = 'playlists';
  $view->menu_tab = FALSE;
  $view->menu_tab_default = FALSE;
  $view->menu_tab_weight = '0';
  $view->block = TRUE;
  $view->block_title = 'Recent playlists';
  $view->block_header = '';
  $view->block_header_format = '1';
  $view->block_footer = '';
  $view->block_footer_format = '1';
  $view->block_empty = '';
  $view->block_empty_format = '1';
  $view->block_type = 'list';
  $view->nodes_per_block = '6';
  $view->block_more = '1';
  $view->block_use_page_header = FALSE;
  $view->block_use_page_footer = FALSE;
  $view->block_use_page_empty = FALSE;
  $view->sort = array(
    array(
      'tablename' => 'station_playlist',
      'field' => 'timestamp',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array(
    array(
      'type' => 'program_nid',
      'argdefault' => '2',
      'title' => '%1 Playlists',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array(
    array(
      'tablename' => 'node',
      'field' => 'title',
      'label' => '',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
  );
  $view->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array(
        0 => 'program_playlist',
      ),
    ),
    array(
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array(
      'tablename' => 'station_playlist',
      'field' => 'timestamp',
      'operator' => '<=',
      'options' => '',
      'value' => 'now',
    ),
  );
  $view->exposed_filter = array(
  );
  $view->requires = array(station_playlist, node);
  $views[$view->name] = $view;



  return $views;
}