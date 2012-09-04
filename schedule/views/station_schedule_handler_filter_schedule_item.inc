<?php

/**
 * @file
 */

class station_schedule_handler_filter_schedule_item extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
      if (empty($this->value)) {
        $this->query->add_where($this->options['group'], "station_schedule_item_node." . $this->real_field);
      } 
      else {
        $this->query->add_where($this->options['group'], "station_schedule_item_node." . $this->real_field, 0, '<>');
      }
  }
}
