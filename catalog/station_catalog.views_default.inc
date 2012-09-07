<?php

/**
 * @file
 */

/**
 * Implements hook_views_default_views().
 */
function station_catalog_views_default_views() {
  $view = new view;
  $view->name = 'station_catalog';
  $view->description = 'Station Catalog search view';
  $view->tag = 'station';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Catalog';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view station album content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = '';
  $handler->display->display_options['style_options']['columns'] = array(
    'number' => 'number',
    'artist' => 'artist',
    'label' => 'label',
    'asin' => 'asin',
    'year' => 'year',
    'mb_release_id' => 'mb_release_id',
  );
  $handler->display->display_options['style_options']['default'] = 'number';
  $handler->display->display_options['style_options']['info'] = array(
    'number' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'artist' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'label' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'asin' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'year' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'mb_release_id' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Field: Station Catalog: Catalog Number */
  $handler->display->display_options['fields']['number']['id'] = 'number';
  $handler->display->display_options['fields']['number']['table'] = 'station_catalog';
  $handler->display->display_options['fields']['number']['field'] = 'number';
  $handler->display->display_options['fields']['number']['label'] = 'Number';
  /* Field: Station Catalog: Artist */
  $handler->display->display_options['fields']['artist']['id'] = 'artist';
  $handler->display->display_options['fields']['artist']['table'] = 'station_catalog';
  $handler->display->display_options['fields']['artist']['field'] = 'artist';
  /* Field: Station Catalog: Year */
  $handler->display->display_options['fields']['year']['id'] = 'year';
  $handler->display->display_options['fields']['year']['table'] = 'station_catalog';
  $handler->display->display_options['fields']['year']['field'] = 'year';
  /* Field: Station Catalog: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'station_catalog';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'station_album' => 'station_album',
  );
  $handler->display->display_options['filters']['type']['group'] = '0';
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Published or admin */
  $handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['table'] = 'node';
  $handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
  $handler->display->display_options['filters']['status_extra']['group'] = '0';
  $handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;
  /* Filter criterion: Station Catalog: Artist */
  $handler->display->display_options['filters']['artist']['id'] = 'artist';
  $handler->display->display_options['filters']['artist']['table'] = 'station_catalog';
  $handler->display->display_options['filters']['artist']['field'] = 'artist';
  $handler->display->display_options['filters']['artist']['group'] = '0';
  $handler->display->display_options['filters']['artist']['exposed'] = TRUE;
  $handler->display->display_options['filters']['artist']['expose']['operator_id'] = 'artist_op';
  $handler->display->display_options['filters']['artist']['expose']['label'] = 'Artist';
  $handler->display->display_options['filters']['artist']['expose']['operator'] = 'artist_op';
  $handler->display->display_options['filters']['artist']['expose']['identifier'] = 'artist';
  $handler->display->display_options['filters']['artist']['expose']['remember'] = 1;
  /* Filter criterion: Station Catalog: Label */
  $handler->display->display_options['filters']['label']['id'] = 'label';
  $handler->display->display_options['filters']['label']['table'] = 'station_catalog';
  $handler->display->display_options['filters']['label']['field'] = 'label';
  $handler->display->display_options['filters']['label']['group'] = '0';
  $handler->display->display_options['filters']['label']['exposed'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator_id'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['label'] = 'Label';
  $handler->display->display_options['filters']['label']['expose']['operator'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['identifier'] = 'label';
  $handler->display->display_options['filters']['label']['expose']['remember'] = 1;
  /* Filter criterion: Station Catalog: Year */
  $handler->display->display_options['filters']['year']['id'] = 'year';
  $handler->display->display_options['filters']['year']['table'] = 'station_catalog';
  $handler->display->display_options['filters']['year']['field'] = 'year';
  $handler->display->display_options['filters']['year']['group'] = '0';
  $handler->display->display_options['filters']['year']['exposed'] = TRUE;
  $handler->display->display_options['filters']['year']['expose']['operator_id'] = 'year_op';
  $handler->display->display_options['filters']['year']['expose']['label'] = 'Year';
  $handler->display->display_options['filters']['year']['expose']['operator'] = 'year_op';
  $handler->display->display_options['filters']['year']['expose']['identifier'] = 'year';
  $handler->display->display_options['filters']['year']['expose']['required'] = FALSE;
  $handler->display->display_options['filters']['year']['expose']['remember'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'station/catalog/search';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Catalog';
  $handler->display->display_options['menu']['weight'] = '0';
  $views[$view->name] = $view;

  return $views;
}
