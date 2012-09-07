<?php

/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 */
function station_schedule_views_default_views() {
  /*
   * View 'station_djs_programs'
   */
  $view = new view;
  $view->name = 'station_djs_programs';
  $view->description = 'Listing of programs by DJ.';
  $view->tag = 'station_schedule';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Programs by DJ';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station program content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['group'] = '0';
  $handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'station/dj';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'DJ';
  $handler->display->display_options['menu']['weight'] = '0';
  $views[$view->name] = $view;

  /*
   * View 'station_schedule_day'
   */
  $view = new view;
  $view->name = 'station_schedule_day';
  $view->description = 'Station Schedule\'s day view';
  $view->tag = 'station_schedule';
  $view->base_table = 'station_schedule_item';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station schedule content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'start' => 'start',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'start' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'area';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = 'There are no programs scheduled for this day.';
  $handler->display->display_options['empty']['text']['format'] = 'plain_text';
  /* Relationship: Station Schedule Item: Program */
  $handler->display->display_options['relationships']['program_nid']['id'] = 'program_nid';
  $handler->display->display_options['relationships']['program_nid']['table'] = 'station_schedule_item';
  $handler->display->display_options['relationships']['program_nid']['field'] = 'program_nid';
  $handler->display->display_options['relationships']['program_nid']['required'] = 1;
  /* Relationship: Station Schedule Item: Schedule */
  $handler->display->display_options['relationships']['schedule_nid']['id'] = 'schedule_nid';
  $handler->display->display_options['relationships']['schedule_nid']['table'] = 'station_schedule_item';
  $handler->display->display_options['relationships']['schedule_nid']['field'] = 'schedule_nid';
  $handler->display->display_options['relationships']['schedule_nid']['required'] = 1;
  /* Field: Station Schedule Item: Start */
  $handler->display->display_options['fields']['start']['id'] = 'start';
  $handler->display->display_options['fields']['start']['table'] = 'station_schedule_item';
  $handler->display->display_options['fields']['start']['field'] = 'start';
  $handler->display->display_options['fields']['start']['label'] = 'Time';
  $handler->display->display_options['fields']['start']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['start']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['start']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['start']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['start']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['start']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['start']['alter']['html'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'program_nid';
  $handler->display->display_options['fields']['title']['label'] = 'Program';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Station Schedule Item: Start */
  $handler->display->display_options['sorts']['start']['id'] = 'start';
  $handler->display->display_options['sorts']['start']['table'] = 'station_schedule_item';
  $handler->display->display_options['sorts']['start']['field'] = 'start';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'schedule_nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  /* Contextual filter: Station Schedule Item: Time range */
  $handler->display->display_options['arguments']['time']['id'] = 'start';
  $handler->display->display_options['arguments']['time']['table'] = 'station_schedule_item';
  $handler->display->display_options['arguments']['time']['field'] = 'time';
  $handler->display->display_options['arguments']['time']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['time']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['time']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['time']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['time']['specify_validation'] = 1;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['relationship'] = 'program_nid';
  $handler->display->display_options['filters']['status_extra']['group'] = '0';
  $handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;
  $views[$view->name] = $view;

  /*
   * View 'station_schedule_items'
   */
  $view = new view;
  $view->name = 'station_schedule_items';
  $view->description = 'Station schedule items';
  $view->tag = 'station_schedule';
  $view->base_table = 'station_schedule_item';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'iid' => 'iid',
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'iid' => array(
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Relationship: Station Schedule Item: Program */
  $handler->display->display_options['relationships']['program_nid']['id'] = 'program_nid';
  $handler->display->display_options['relationships']['program_nid']['table'] = 'station_schedule_item';
  $handler->display->display_options['relationships']['program_nid']['field'] = 'program_nid';
  $handler->display->display_options['relationships']['program_nid']['required'] = 0;
  /* Relationship: Station Schedule Item: Schedule */
  $handler->display->display_options['relationships']['schedule_nid']['id'] = 'schedule_nid';
  $handler->display->display_options['relationships']['schedule_nid']['table'] = 'station_schedule_item';
  $handler->display->display_options['relationships']['schedule_nid']['field'] = 'schedule_nid';
  $handler->display->display_options['relationships']['schedule_nid']['required'] = 0;
  /* Field: Station Schedule Item: Start */
  $handler->display->display_options['fields']['start']['id'] = 'start';
  $handler->display->display_options['fields']['start']['table'] = 'station_schedule_item';
  $handler->display->display_options['fields']['start']['field'] = 'start';
  $handler->display->display_options['fields']['start']['label'] = 'Time';
  $handler->display->display_options['fields']['start']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['start']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['start']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['start']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['start']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['start']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['start']['alter']['html'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'program_nid';
  $handler->display->display_options['fields']['title']['label'] = 'Program';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Station Schedule Item: Start */
  $handler->display->display_options['sorts']['start']['id'] = 'start';
  $handler->display->display_options['sorts']['start']['table'] = 'station_schedule_item';
  $handler->display->display_options['sorts']['start']['field'] = 'start';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'schedule_nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['argument'] = '30';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  $views[$view->name] = $view;

  /*
   * View 'station_schedule_programs'
   */
  $view = new view;
  $view->name = 'station_schedule_programs';
  $view->description = 'Scheduled programs';
  $view->tag = 'station_schedule';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Programs';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station program content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_station_program_genre_value' => 'field_station_program_genre_value',
    'field_station_program_dj_uid' => 'field_station_program_dj_uid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_station_program_genre_value' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'field_station_program_dj_uid' => array(
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  /* Relationship: Station Program: Program Scheduling */
  $handler->display->display_options['relationships']['station_program_iid']['id'] = 'station_program_iid';
  $handler->display->display_options['relationships']['station_program_iid']['table'] = 'node';
  $handler->display->display_options['relationships']['station_program_iid']['field'] = 'station_program_iid';
  $handler->display->display_options['relationships']['station_program_iid']['required'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Station Schedule Item: Time range */
  $handler->display->display_options['fields']['time']['id'] = 'time';
  $handler->display->display_options['fields']['time']['table'] = 'station_schedule_item';
  $handler->display->display_options['fields']['time']['field'] = 'time';
  $handler->display->display_options['fields']['time']['relationship'] = 'station_program_iid';
  $handler->display->display_options['fields']['time']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['time']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['time']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['time']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['time']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['time']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['time']['alter']['html'] = 0;
  $handler->display->display_options['fields']['time']['style'] = 'station_dayhour_range';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_program' => 'station_program',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  /* Filter criterion: Station Program: Is scheduled */
  $handler->display->display_options['filters']['station_program_iid']['id'] = 'station_program_iid';
  $handler->display->display_options['filters']['station_program_iid']['table'] = 'node';
  $handler->display->display_options['filters']['station_program_iid']['field'] = 'station_program_iid';
  $handler->display->display_options['filters']['station_program_iid']['value'] = '1';
  $handler->display->display_options['filters']['station_program_iid']['group'] = '0';
  $handler->display->display_options['filters']['station_program_iid']['expose']['operator'] = FALSE;

  /* Display: Scheduled Page */
  $handler = $view->new_display('page', 'Scheduled Page', 'page_1');
  $handler->display->display_options['path'] = 'station/programs/scheduled';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Scheduled';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Programs';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Unscheduled Page */
  $handler = $view->new_display('page', 'Unscheduled Page', 'page_2');
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_station_program_genre_value']['id'] = 'field_station_program_genre_value';
  $handler->display->display_options['fields']['field_station_program_genre_value']['table'] = 'node_data_field_station_program_genre';
  $handler->display->display_options['fields']['field_station_program_genre_value']['field'] = 'field_station_program_genre_value';
  $handler->display->display_options['fields']['field_station_program_genre_value']['label'] = '';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_station_program_dj_uid']['id'] = 'field_station_program_dj_uid';
  $handler->display->display_options['fields']['field_station_program_dj_uid']['table'] = 'node_data_field_station_program_dj';
  $handler->display->display_options['fields']['field_station_program_dj_uid']['field'] = 'field_station_program_dj_uid';
  $handler->display->display_options['fields']['field_station_program_dj_uid']['label'] = 'DJs';
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_station_program_dj_uid']['alter']['html'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_program' => 'station_program',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  /* Filter criterion: Station Program: Is scheduled */
  $handler->display->display_options['filters']['station_program_iid']['id'] = 'station_program_iid';
  $handler->display->display_options['filters']['station_program_iid']['table'] = 'node';
  $handler->display->display_options['filters']['station_program_iid']['field'] = 'station_program_iid';
  $handler->display->display_options['filters']['station_program_iid']['value'] = '0';
  $handler->display->display_options['filters']['station_program_iid']['group'] = '0';
  $handler->display->display_options['filters']['station_program_iid']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'station/programs/unscheduled';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Unscheduled';
  $handler->display->display_options['menu']['weight'] = '0';
  $views[$view->name] = $view;

  /*
   * View 'station_schedules'
   */
  $view = new view;
  $view->name = 'station_schedules';
  $view->description = 'Station Schedules';
  $view->tag = 'station_schedule';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Schedule listing';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station schedule content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_schedule' => 'station_schedule',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['group'] = '0';
  $handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'station/schedules';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Schedules';
  $handler->display->display_options['menu']['weight'] = '0';
  $views[$view->name] = $view;

  return $views;
}
