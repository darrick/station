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
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'artist' => array(
      'label' => 'Artist',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'artist',
      'table' => 'station_playlist_track',
      'field' => 'artist',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'station_playlist_track',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'album' => array(
      'label' => 'Album',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'album',
      'table' => 'station_playlist_track',
      'field' => 'album',
      'relationship' => 'none',
    ),
    'label' => array(
      'label' => 'Label',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'label',
      'table' => 'station_playlist_track',
      'field' => 'label',
      'relationship' => 'none',
    ),
    'link' => array(
      'label' => 'Link',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'link',
      'table' => 'station_playlist_track',
      'field' => 'link',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'weight' => array(
      'order' => 'ASC',
      'id' => 'weight',
      'table' => 'station_playlist_track',
      'field' => 'weight',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'summary asc',
      'style_plugin' => 'default_summary',
      'style_options' => array(
        'count' => 1,
        'override' => 0,
        'items_per_page' => '25',
      ),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'node',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'station_playlist' => 'station_playlist',
        'station_program' => 0,
        'station_schedule' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 1,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'station_playlist' => 'station_playlist',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view station playlist content',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'table');
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
  $handler->display->display_options['arguments']['field_station_program_nid']['title'] = '%1 Playlists';
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
  $handler->display->display_options['arguments']['field_station_program_nid']['title'] = '%1 Playlists';
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

  return $views;
}
