<?php
// $Id$

/**
 * Give a string with hours return an array of vocabulary term ids for the
 * given hours.
 *
 * @param $vid
 *   integer vocabulary id
 * @param $hour_string
 *   string with hours values, integers from 1 to 168 separated by spaces
 */
function _station_archive_get_hour_tids($vid, $hour_string = '') {
  $ids = array();
  foreach (explode(' ', $hour_string) as $id) {
    $id = (integer) $id;
    if ($id > 0 && $id <= 24*7) {
      $ids[] = $id;
    }
  }
  sort($ids);

  $tids = array();
  foreach ($ids as $id) {
    $day = (integer) (($id - 1) / 24);
    $hour = ($id - 1) % 24;

    if ($dayterm = _station_archive_get_day_term($vid, $day)) {
      if ($hourterm = _station_archive_get_hour_term($dayterm, $hour)) {
        $tids[] = $hourterm['tid'];
      }
    }
  }

  return $tids;
}

/**
 * Find the taxonomy term for a day.
 *
 * @param $vid
 *   Our vocabulary ID.
 * @param $day
 *   Integer day between 0 and 6.
 * @return
 *   Array with a day's taxonomy term, FALSE if it doesn't exist.
 */
function _station_archive_get_day_term($vid, $day) {
  $dayname = station_day_name($day);
  $result = db_query(db_rewrite_sql("SELECT t.tid, t.* FROM {term_data} t WHERE t.vid = %d AND LOWER('%s') LIKE LOWER(name)", 't', 'tid'), array($vid, trim($dayname)));
  return db_fetch_array($result);
}

/**
 * Find or create taxonomy term for a day.
 *
 * @param $dayterm
 *   Array day taxonomy term.
 * @param $hour
 *   Integer hour between 0 and 23.
 * @return
 *   Array with an hour's taxonomy term, false if it doesn't exist.
 */
function _station_archive_get_hour_term($dayterm, $hour) {
  $minute = station_minute_from_day_hour(0, $hour);
  $time = station_time_from_minute($minute);
  $hourname = $time['time'] . $time['a'];
  $result = db_query(db_rewrite_sql("SELECT d.tid, d.* FROM {term_data} d INNER JOIN {term_hierarchy} h ON d.tid = h.tid WHERE d.vid = %d AND h.parent = %d AND LOWER('%s') LIKE LOWER(name)", 't', 'tid'), array($dayterm['vid'], $dayterm['tid'], trim($hourname)));
  return db_fetch_array($result);
}

function station_archive_view_html($dayname = NULL, $hourname = NULL) {
  if (!is_null($dayname)) {
    $day = station_valid_day($dayname);
    $vid = _station_archive_get_vid();
    if ($dayterm = _station_archive_get_day_term($vid, $day)) {
      $tid = $dayterm['tid'];

      if (!is_null($hourname)) {
        $hour = station_valid_hour($hourname);
        if ($hourterm = _station_archive_get_hour_term($dayterm, $hour)) {
          $tid = $hourterm['tid'];
        }
      }
      drupal_goto('taxonomy/term/'. $tid);
    }
  }
  drupal_not_found();
}

function station_archive_view_rss($dayname = NULL, $hourname = NULL) {
  if (!is_null($dayname)) {
    $day = station_valid_day($dayname);
    $vid = _station_archive_get_vid();
    if ($dayterm = _station_archive_get_day_term($vid, $day)) {
      $tid = $dayterm['tid'];

      if (!is_null($hourname)) {
        $hour = station_valid_hour($hourname);
        if ($hourterm = _station_archive_get_hour_term($dayterm, $hour)) {
          $tid = $hourterm['tid'];
        }
      }

      drupal_goto('taxonomy/term/'. $tid .'/0/feed');
    }
  }
  drupal_not_found();
}

function station_archive_view_hours_html($hour_string) {
  $title = t('Shows on...');
  $rss_url = url('rss/hours/'. str_replace(' ', '+', $hour_string));

  $attributes = array(
    'rel' => 'alternate',
    'type' => 'application/rss+xml',
    'title' => t('RSS - %title', array('%title' => $title)),
    'href' => $rss_url,
  );
  drupal_add_link($attributes);
  drupal_set_title($title);

  $vid = _station_archive_get_vid();
  $tids = _station_archive_get_hour_tids($vid, $hour_string);
  $result = taxonomy_select_nodes($tids, 'or', 0, FALSE);
  while ($node = db_fetch_object($result)) {
    $output .= node_view(node_load($node), 1);
  }
  $output .= theme('feed_icon', $rss_url);

  return $output;
}

function station_archive_view_hours_rss($hour_string) {
  $title = t('Shows on...');
  $desc = t('Description of shows...');

  $vid = _station_archive_get_vid();
  $tids = _station_archive_get_hour_tids($vid, $hour_string);
  $result = taxonomy_select_nodes($tids, 'or', 'all', FALSE);

  $channel['title'] = $title;
  $channel['description'] = $desc;

  return node_feed($result, $channel);
}