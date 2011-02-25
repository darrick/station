<?php


function station_archive_views_default_views() {
  $view = new stdClass();
  $view->name = 'station_archive_audio';
  $view->description = 'Archived audio';
  $view->access = array(
    0 => '1',
    1 => '2',
  );
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'archives';
  $view->page_header = "";
  $view->page_header_format = '1';
  $view->page_footer = "";
  $view->page_footer_format = '1';
  $view->page_empty = "Sorry, no audio could be found for this program.";
  $view->page_empty_format = '1';
  $view->page_type = 'teaser';
  $view->url = 'station/archives';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '30';
  $view->menu = TRUE;
  $view->menu_title = 'archive';
  $view->menu_tab = FALSE;
  $view->menu_tab_default = FALSE;
  $view->menu_tab_weight = '0';
  $view->sort = array(
    array(
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array(
    array(
      'type' => 'station_archive_program_nid',
      'argdefault' => '4',
      'title' => 'Archive of %1',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array(
  );
  $view->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array(
        0 => 'audio',
      ),
    ),
  );
  $view->exposed_filter = array(
  );
  $view->requires = array(node);
  $views[$view->name] = $view;




  $view = new stdClass();
  $view->name = 'station_archive_audio_all';
  $view->description = 'Archived audio';
  $view->access = array(
    0 => '1',
    1 => '2',
  );
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Full Archive';
  $view->page_header = "";
  $view->page_header_format = '1';
  $view->page_footer = theme('xml_icon', url('station/archives/rss/'));
  $view->page_footer_format = '1';
  $view->page_empty = "";
  $view->page_empty_format = '1';
  $view->page_type = 'table';
  $view->url = 'station/archives/all';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '20';
  $view->menu = TRUE;
  $view->menu_title = 'all';
  $view->menu_tab = FALSE;
  $view->menu_tab_default = FALSE;
  $view->menu_tab_weight = '0';
  $view->sort = array(
    array(
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array(
  );
  $view->field = array(
    array(
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'Title',
      'handler' => 'views_handler_field_nodelink_with_mark',
      'options' => 'link',
    ),
    array(
      'tablename' => 'node',
      'field' => 'created',
      'label' => 'Recorded',
      'handler' => 'views_handler_field_date',
      'sortable' => '1',
    ),
    array(
      'tablename' => 'audio',
      'field' => 'play_link',
      'label' => 'Play',
    ),
  );
  $view->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array(
        0 => 'audio',
      ),
    ),
  );
  $view->exposed_filter = array(
  );
  $view->requires = array(node, audio);
  $views[$view->name] = $view;



  $view = new stdClass();
  $view->name = 'station_archive_audio_rss';
  $view->description = 'Station Archive';
  $view->access = array(
    0 => '1',
    1 => '2',
  );
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Program Archive';
  $view->page_header = "";
  $view->page_header_format = '1';
  $view->page_footer = "";
  $view->page_footer_format = '1';
  $view->page_empty = "";
  $view->page_empty_format = '1';
  $view->page_type = 'views_rss';
  $view->url = 'station/archives/rss';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '20';
  $view->sort = array(
    array(
      'tablename' => 'node',
      'field' => 'created',
      'sortorder' => 'DESC',
      'options' => '',
    ),
  );
  $view->argument = array(
    array(
      'type' => 'station_archive_program_nid',
      'argdefault' => '2',
      'title' => 'Archive of %1',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array(
  );
  $view->filter = array(
    array(
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array(
        0 => 'audio',
      ),
    ),
  );
  $view->exposed_filter = array(
  );
  $view->requires = array(node);
  $views[$view->name] = $view;



  return $views;
}