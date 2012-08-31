<?php
/**
 * @file
 */

class station_schedule_handler_field_streams extends views_handler_field {
  function render($values) {
    $streams = unserialize($values->{$this->field_alias});
    if (is_array($streams)) {
      return theme('station_streams', array('streams' => $streams));
    }
  }
}
