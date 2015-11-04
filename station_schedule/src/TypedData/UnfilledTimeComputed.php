<?php

/**
 * @file
 * Contains \Drupal\station_schedule\TypedData\UnfilledTimeComputed.
 */

namespace Drupal\station_schedule\TypedData;

/**
 * @todo.
 */
class UnfilledTimeComputed extends ComputedField {

  /**
   * {@inheritdoc}
   */
  protected function computeValue($langcode = NULL) {
    $entity = $this->getParent()->getValue();
    $hours_per_week = $entity->hours_per_week->getValue();

    $duration = 0;
    foreach ($entity->getScheduledItemsByDay() as $schedule_items) {
      /** @var \Drupal\station_schedule\ScheduleItemInterface[] $schedule_items */
      foreach ($schedule_items as $schedule_item) {
        $duration += $schedule_item->getFinish() - $schedule_item->getStart();
      }
    }

    return $hours_per_week - ($duration / 60);
  }

}
