<?php

/**
 * @file
 * Contains \Drupal\station_schedule\Entity\ScheduleViewBuilder.
 */

namespace Drupal\station_schedule\Entity;

use Drupal\Component\Utility\Html;
use Drupal\Core\Datetime\DateHelper;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityViewBuilderInterface;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\Core\Field\FieldItemInterface;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\station_schedule\DatetimeHelper;
use Drupal\user\EntityOwnerInterface;

/**
 * @todo.
 */
class ScheduleViewBuilder implements EntityViewBuilderInterface {

  use StringTranslationTrait;

  /**
   * {@inheritdoc}
   */
  public function view(EntityInterface $entity, $view_mode = 'full', $langcode = NULL) {
    $header[0] = ['data' => $this->t('Time')];
    $row = [];

    // First column is hours.
    $row[0] = ['id' => 'station-sch-hours', 'data' => []];
    // Load the settings.
    /** @var \Drupal\station_schedule\ScheduleInterface $entity */
    $start_hour = $entity->getStartHour();
    $end_hour = $entity->getEndHour();
    for ($hour = $start_hour; $hour < $end_hour; $hour++) {
      $row[0]['data'][] = $this->buildScheduleHour($hour);
    }

    $start_hour = $entity->getStartHour();
    $end_hour = $entity->getEndHour();
    $day_names = DateHelper::weekDays();
    foreach ($entity->getScheduledItemsByDay() as $day => $items) {
      $header[$day + 1]['data'] = $day_names[$day];
      $row[$day + 1]['data'] = [];

      // The last finish pointer starts at the beginning of the day.
      $minutes_for_day = $day * 60 * 24;
      $last_finish = $minutes_for_day + ($start_hour * 60);
      $day_finish = $minutes_for_day + ($end_hour * 60);

      /** @var \Drupal\station_schedule\ScheduleItemInterface[] $items */
      foreach ($items as $item) {
        $start = $item->getStart();
        $finish = $item->getFinish();
        // Display blocks for unscheduled time periods
        if ($last_finish != $start) {
          $row[$day + 1]['data'][] = $this->buildSpacer($last_finish, $start);
        }
        $last_finish = $finish;

        // Display the schedule item.
        $program = $item->getProgram();
        $row[$day + 1]['data'][] = $this->buildScheduledItem($start, $finish, $program);
      }
      // Display a block for any remaining time during the day.
      if ($last_finish < $day_finish) {
        $row[$day + 1]['data'][] = $this->buildSpacer($last_finish, $day_finish);
      }
    }

    // Add a class to indicate what day it is.
    $today = $this->getCurrentDateNumeral();
    $header[$today + 1]['class'][] = 'station-sch-now-day';
    $row[$today + 1]['class'][] = 'station-sch-now-day';

    return [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => [$row],
      '#attached' => ['library' => ['station_schedule/schedule']],
      '#attributes' => [
        'id' => 'station-sch',
      ],
    ];
  }

  /**
   * @todo.
   */
  protected function getCurrentDateNumeral() {
    $ts = time();
    $return = ($ts - date('Z', $ts)) + \Drupal::configFactory()->get('system.date')->get('timezone.default') ?: 0;
    return date('w', $return);
  }

  protected function buildScheduleHour($hour) {
    $time = DatetimeHelper::deriveTimeFromMinutes($hour * 60);
    $height = (60 / 24) . 'em';
    $output = [
      '#type' => 'container',
      '#attributes' => [
        'class' => ['station-sch-box', 'station-sch-hour'],
        'data-drupal-station-schedule-height' => $height,
      ],
      'children' => [
        '#plain_text' => $this->t('@time@ampm', ['@time' => $time['time'], '@ampm' => $time['a']]),
      ],
    ];
    return $output;
  }

  protected function buildScheduledItem($start, $finish, EntityInterface $program) {
    $height = (($finish - $start) / 24) . 'em';
    $link = $program->url();
    $time = DatetimeHelper::hourRange($start, $finish);

    $output = [
      '#type' => 'container',
      '#attributes' => ['class' => ['station-sch-box', 'station-sch-scheduled']],
      'children' => [
        '#prefix' => "<a href='{$link}' data-drupal-station-schedule-height='$height' title='{$time}'>",
        '#suffix' => '</a>',
        'time' => [
          '#markup' => $time,
          '#prefix' => '<span class="station-sch-time">',
          '#suffix' => '</span>',
        ],
        'title' => [
          '#markup' => $program->label(),
          '#prefix' => '<span class="station-sch-title">',
          '#suffix' => '</span>',
        ],
      ],
    ];
    $djs = [];
    if ($program instanceof FieldableEntityInterface && $program->hasField('station_program_djs')) {
      foreach ($program->get('station_program_djs') as $dj) {
        $djs[] = $dj->entity->label();
      }
    }
    if ($djs) {
      $output['children']['dj'] = [
        '#markup' => Html::escape(implode(', ', $djs)),
        '#prefix' => '<span class="station-sch-djs">',
        '#suffix' => '</span></a>',
      ];
    }
    return $output;
  }

  protected function buildSpacer($start, $finish) {
    $height = (($finish - $start) / 24) . 'em';
    $output = [
      '#type' => 'container',
      '#attributes' => [
        'class' => ['station-sch-box', 'station-sch-unscheduled'],
        'data-drupal-station-schedule-height' => $height,
      ],
      'time' => [
        '#markup' => DatetimeHelper::hourRange($start, $finish),
        '#prefix' => '<span class="station-sch-time">',
        '#suffix' => '</span>',
      ],
    ];
    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function viewMultiple(array $entities = [], $view_mode = 'full', $langcode = NULL) {
    $build = [];
    foreach ($entities as $key => $entity) {
      $build[$key] = $this->view($entity, $view_mode, $langcode);
    }
    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function resetCache(array $entities = NULL) {
    // Intentionally empty.
  }

  /**
   * {@inheritdoc}
   */
  public function getCacheTags() {
    // Intentionally empty.
  }

  /**
   * {@inheritdoc}
   */
  public function buildComponents(array &$build, array $entities, array $displays, $view_mode) {
    throw new \LogicException();
  }

  /**
   * {@inheritdoc}
   */
  public function viewField(FieldItemListInterface $items, $display_options = []) {
    throw new \LogicException();
  }

  /**
   * {@inheritdoc}
   */
  public function viewFieldItem(FieldItemInterface $item, $display_options = []) {
    throw new \LogicException();
  }

}
