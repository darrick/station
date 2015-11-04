<?php

/**
 * @file
 * Contains \Drupal\station_schedule\TypedData\ItemCountComputed.
 */

namespace Drupal\station_schedule\TypedData;

/**
 * @todo.
 */
class ItemCountComputed extends ComputedField {

  /**
   * {@inheritdoc}
   */
  protected function computeValue($langcode = NULL) {
    return count($this->getParent()->getValue()->getScheduledItems());
  }

}
