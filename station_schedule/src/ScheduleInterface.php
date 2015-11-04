<?php

/**
 * @file
 * Contains \Drupal\station_schedule\ScheduleInterface.
 */

namespace Drupal\station_schedule;

use Drupal\Core\Entity\ContentEntityInterface;

/**
 * @todo.
 */
interface ScheduleInterface extends ContentEntityInterface {
  public function getStartHour();
  public function getEndHour();
  public function getIncrement();
  public function getUnscheduledMessage();

  /**
   * @return \Drupal\station_schedule\ScheduleItemInterface[][]
   */
  public function getScheduledItems();

  /**
   * @param \Drupal\station_schedule\ScheduleItemInterface[][] $scheduled_items
   *
   * @return $this
   */
  public function setScheduledItems(array $scheduled_items);

}
