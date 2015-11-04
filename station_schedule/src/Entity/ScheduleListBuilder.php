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

    $unfilled_hours = $entity->unfilled_time->getValue();
    $unfilled_percentage = round($unfilled_hours * 100 / $entity->hours_per_week->getValue());
    $row['unfilled_time'] = $this->t('@hours hours / @percentage%', ['@hours' => $unfilled_hours, '@percentage' => $unfilled_percentage]);

    return $row + parent::buildRow($entity);
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultOperations(EntityInterface $entity) {
    $operations = parent::getDefaultOperations($entity);
    if ($entity->access('update') && $entity->hasLinkTemplate('schedule')) {
      $operations['alter-schedule'] = array(
        'title' => $this->t('Alter schedule'),
        'weight' => 15,
        'url' => $entity->urlInfo('schedule'),
      );
    }
    return $operations;
  }

}
