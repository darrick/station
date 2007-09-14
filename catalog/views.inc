<?php
// $Id$

/**
 * Implementation of hook_views_tables().
 */
function station_catalog_views_tables() {
  $tables['station_catalog'] = array(
    'name' => 'station_catalog',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'field' => 'nid'
      )
    ),
    'fields' => array(
      'number' => array(
        'name' => t('Station Catalog: Number'),
        'help' => t("Album's log number."),
        'sortable' => TRUE,
      ),
      'artist' => array(
        'name' => t('Station Catalog: Artist'),
        'help' => t("Album artist name."),
        'sortable' => TRUE,
      ),
      'album' => array(
        'name' => t('Station Catalog: Album title'),
        'help' => t("Album time."),
        'sortable' => TRUE,
      ),
      'year' => array(
        'name' => t('Station Catalog: Year'),
        'help' => t("Year the album was released."),
        'sortable' => TRUE,
      ),
      'label' => array(
        'name' => t('Station Catalog: Label'),
        'help' => t("Label that released the album."),
        'sortable' => TRUE,
      ),
      'mb_release_id' => array(
        'name' => t('Station Catalog: Music Brainz ID'),
        'help' => t("MusicBrainz ID for this release."),
        'sortable' => TRUE,
      ),
      'asin' => array(
        'name' => t('Station Catalog: ASIN'),
        'help' => t("Amazon product ID for this album."),
        'sortable' => TRUE,
      ),
    ),
  );

  return $tables;
}
