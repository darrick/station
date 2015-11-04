<?php

/**
 * @file
 * Contains \Drupal\station_schedule\TypedData\HoursPerWeekComputed.
 */

namespace Drupal\station_schedule\TypedData;

/**
 * @todo.
 */
class HoursPerWeekComputed extends ComputedField {

  /**
   * {@inheritdoc}
   */
  protected function computeValue($langcode = NULL) {
    $entity = $this->getParent()->getValue();
    return ($entity->getEndHour() - $entity->getStartHour()) * 7;
  }

}
