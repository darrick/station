<?php

/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 */
function station_playlist_views_default_views() {
  /*
   * View 'station_playlist_tracks'
   */
  $view = new view;
  $view->name = 'station_playlist_tracks';
  $view->description = 'Listing of tracks on a playlist node.';
  $view->tag = 'station_playlist';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station playlist content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Station Playlist Track: Artist */
  $handler->display->display_options['fields']['artist']['id'] = 'artist';
  $handler->display->display_options['fields']['artist']['table'] = 'station_playlist_track';
  $handler->display->display_options['fields']['artist']['field'] = 'artist';
  $handler->display->display_options['fields']['artist']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['artist']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['artist']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['artist']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['artist']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['artist']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['artist']['alter']['html'] = 0;
  /* Field: Station Playlist Track: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'station_playlist_track';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  /* Field: Station Playlist Track: Album */
  $handler->display->display_options['fields']['album']['id'] = 'album';
  $handler->display->display_options['fields']['album']['table'] = 'station_playlist_track';
  $handler->display->display_options['fields']['album']['field'] = 'album';
  $handler->display->display_options['fields']['album']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['album']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['album']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['album']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['album']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['album']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['album']['alter']['html'] = 0;
  /* Field: Station Playlist Track: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'station_playlist_track';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['label']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['label']['alter']['html'] = 0;
  /* Field: Station Playlist Track: Link */
  $handler->display->display_options['fields']['link']['id'] = 'link';
  $handler->display->display_options['fields']['link']['table'] = 'station_playlist_track';
  $handler->display->display_options['fields']['link']['field'] = 'link';
  $handler->display->display_options['fields']['link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link']['alter']['html'] = 0;
  /* Sort criterion: Station Playlist Track: Weight */
  $handler->display->display_options['sorts']['weight']['id'] = 'weight';
  $handler->display->display_options['sorts']['weight']['table'] = 'station_playlist_track';
  $handler->display->display_options['sorts']['weight']['field'] = 'weight';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_playlist' => 'station_playlist',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  $views[$view->name] = $view;

  /*
   * View 'station_program_playlists'
   */
  $view = new view;
  $view->name = 'station_program_playlists';
  $view->description = 'Station Program\'s associated Playlists';
  $view->tag = 'station_playlist';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Playlists';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station playlist content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  /* Sort criterion: Content: Date (field_station_playlist_date) */
  $handler->display->display_options['sorts']['field_station_playlist_date_value']['id'] = 'field_station_playlist_date_value';
  $handler->display->display_options['sorts']['field_station_playlist_date_value']['table'] = 'field_data_field_station_playlist_date';
  $handler->display->display_options['sorts']['field_station_playlist_date_value']['field'] = 'field_station_playlist_date_value';
  $handler->display->display_options['sorts']['field_station_playlist_date_value']['order'] = 'DESC';
  /* Contextual filter: Content: Program (field_station_program) */
  $handler->display->display_options['arguments']['field_station_program_nid']['id'] = 'field_station_program_nid';
  $handler->display->display_options['arguments']['field_station_program_nid']['table'] = 'field_data_field_station_program';
  $handler->display->display_options['arguments']['field_station_program_nid']['field'] = 'field_station_program_nid';
  $handler->display->display_options['arguments']['field_station_program_nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_station_program_nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['field_station_program_nid']['title_enable'] = 1;
  $handler->display->display_options['arguments']['field_station_program_nid']['title'] = '%1 Past Playlists:';
  $handler->display->display_options['arguments']['field_station_program_nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_station_program_nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_station_program_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_station_program_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_station_program_nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_station_program_nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['field_station_program_nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_station_program_nid']['validate_options']['types'] = array(
    'station_program' => 'station_program',
  );
  $handler->display->display_options['arguments']['field_station_program_nid']['validate_options']['access'] = '1';
  $handler->display->display_options['arguments']['field_station_program_nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_station_program_nid']['not'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_playlist' => 'station_playlist',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  /* Filter criterion: Content: Date (field_station_playlist_date) */
  $handler->display->display_options['filters']['field_station_playlist_date_value']['id'] = 'field_station_playlist_date_value';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['table'] = 'field_data_field_station_playlist_date';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['field'] = 'field_station_playlist_date_value';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['default_date'] = 'now';

  /* Display: Future */
  $handler = $view->new_display('block', 'Future', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Upcoming shows';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_station_playlist_date']['id'] = 'field_station_playlist_date';
  $handler->display->display_options['fields']['field_station_playlist_date']['table'] = 'field_data_field_station_playlist_date';
  $handler->display->display_options['fields']['field_station_playlist_date']['field'] = 'field_station_playlist_date';
  $handler->display->display_options['fields']['field_station_playlist_date']['label'] = '';
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['path'] = 'node/[nid] ';
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_station_playlist_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_station_playlist_date']['field_api_classes'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_playlist' => 'station_playlist',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  /* Filter criterion: Content: Date (field_station_playlist_date) */
  $handler->display->display_options['filters']['field_station_playlist_date_value']['id'] = 'field_station_playlist_date_value';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['table'] = 'field_data_field_station_playlist_date';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['field'] = 'field_station_playlist_date_value';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['operator'] = '>';
  $handler->display->display_options['filters']['field_station_playlist_date_value']['default_date'] = 'now';
  $handler->display->display_options['block_description'] = 'Program\'s upcoming playlists';

  /* Display: Past */
  $handler = $view->new_display('block', 'Past', 'block_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Previous shows';
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Date */
  $handler->display->display_options['fields']['field_station_playlist_date']['id'] = 'field_station_playlist_date';
  $handler->display->display_options['fields']['field_station_playlist_date']['table'] = 'field_data_field_station_playlist_date';
  $handler->display->display_options['fields']['field_station_playlist_date']['field'] = 'field_station_playlist_date';
  $handler->display->display_options['fields']['field_station_playlist_date']['label'] = '';
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_station_playlist_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_station_playlist_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_station_playlist_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_station_playlist_date']['field_api_classes'] = 0;
  $handler->display->display_options['block_description'] = 'Program\'s previous playlists';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = 1;
  $handler->display->display_options['row_options']['comments'] = 0;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Program (field_station_program) */
  $handler->display->display_options['arguments']['field_station_program_nid']['id'] = 'field_station_program_nid';
  $handler->display->display_options['arguments']['field_station_program_nid']['table'] = 'field_data_field_station_program';
  $handler->display->display_options['arguments']['field_station_program_nid']['field'] = 'field_station_program_nid';
  $handler->display->display_options['arguments']['field_station_program_nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['field_station_program_nid']['title_enable'] = 1;
  $handler->display->display_options['arguments']['field_station_program_nid']['title'] = '%1 Future Playlists:';
  $handler->display->display_options['arguments']['field_station_program_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_station_program_nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_station_program_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_station_program_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_station_program_nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_station_program_nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['field_station_program_nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_station_program_nid']['validate_options']['types'] = array(
    'station_program' => 'station_program',
  );
  $handler->display->display_options['arguments']['field_station_program_nid']['validate_options']['access'] = '1';
  $handler->display->display_options['arguments']['field_station_program_nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_station_program_nid']['not'] = 0;
  $handler->display->display_options['path'] = 'station/playlists';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Playlists';
  $handler->display->display_options['menu']['weight'] = '0';
  $views[$view->name] = $view;

  return $views;
}
