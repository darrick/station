<?php
// $Id$

class station_schedule_handler_sort_time extends views_handler_sort {
  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();

    $min_in_day = MINUTES_IN_DAY;
    $first_day = 7 - variable_get('date_first_day', 0);
    if ($first_day != 0) {
      $formula = "(CAST($this->table_alias.$this->real_field / $min_in_day AS UNSIGNED INTEGER) + $first_day) %% 7";
    }
    else {
      $formula = "CAST($this->table_alias.$this->real_field / $min_in_day AS UNSIGNED INTEGER)";
    }

    $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias .'_'. $this->field .'_sort');
  }
}
