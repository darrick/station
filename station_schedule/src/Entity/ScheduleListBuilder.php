<?php

/**
 * @file
 * Contains \Drupal\station_schedule\Entity\ScheduleListBuilder.
 */

namespace Drupal\station_schedule\Entity;

use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityListBuilder;
use Drupal\Core\Url;

/**
 * @todo.
 */
class ScheduleListBuilder extends EntityListBuilder {

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build = parent::render();
    $build['table']['#empty'] = $this->t('No schedules available. <a href=":link">Add schedule</a>.', [
      ':link' => Url::fromRoute('entity.station_schedule.add_form')->toString()
    ]);
    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header = [
      'title' => $this->t('Title'),
      'item_count' => $this->t('Scheduled item count'),
      'days_empty' => $this->t('Days empty'),
      'unfilled_time' => $this->t('Unfilled time'),
    ] + parent::buildHeader();
    return $header;
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    /** @var \Drupal\station_schedule\ScheduleInterface $entity */
    $row['title']['data'] = [
      '#type' => 'link',
      '#title' => $entity->label(),
      '#url' => $entity->urlInfo(),
    ];

    $row['item_count'] = $entity->item_count->getValue();

    $row['days_empty'] = $entity->days_empty->getValue();

    $hours_per_week = ($entity->getEndHour() - $entity->getStartHour()) * 7;
    $duration = 0;
    $scheduled_items_by_day = $entity->getScheduledItemsByDay();
    foreach ($scheduled_items_by_day as $schedule_items) {
      /** @var \Drupal\station_schedule\ScheduleItemInterface[] $schedule_items */
      foreach ($schedule_items as $schedule_item) {
        $duration += $schedule_item->getFinish() - $schedule_item->getStart();
      }
    }
    $filled_hours = $duration / 60;
    $unfilled_hours = $hours_per_week - $filled_hours;
    $row['unfilled_time'] = $this->t('@hours hours / @percentage%', ['@hours' => $unfilled_hours, '@percentage' => round($unfilled_hours * 100 / $hours_per_week)]);

    return $row + parent::buildRow($entity);
  }

}
