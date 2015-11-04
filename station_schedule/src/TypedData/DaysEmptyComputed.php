<?php

/**
 * @file
 * Contains \Drupal\station_schedule\TypedData\DaysEmptyComputed.
 */

namespace Drupal\station_schedule\TypedData;

/**
 * @todo.
 */
class DaysEmptyComputed extends ComputedField {

  /**
   * {@inheritdoc}
   */
  protected function computeValue($langcode = NULL) {
    $scheduled_items_by_day = $this->getParent()->getValue()->getScheduledItemsByDay();
    return 7 - count(array_filter($scheduled_items_by_day));
  }

}
