<?php
/**
 * @file
 */

/**
 * Field handler to present a link node edit.
 */
class station_schedule_handler_field_iid_link_edit extends station_schedule_handler_field_iid_link {
  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $iid = $values->{$this->aliases['iid']};
    $schedule_nid = $values->{$this->aliases['schedule_nid']};
    return l($text, "node/$schedule_nid/schedule/$iid/edit", array('query' => drupal_get_destination()));
  }
}

