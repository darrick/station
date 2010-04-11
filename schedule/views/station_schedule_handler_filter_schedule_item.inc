<?php
// $Id$

class station_schedule_handler_filter_schedule_item extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "(SELECT COUNT(*) FROM {station_schedule_item} ssi WHERE ssi.program_nid = $this->table_alias.$this->real_field) " . (empty($this->value) ? '=' : '<>') . " 0");
  }
}