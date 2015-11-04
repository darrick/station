<?php

/**
 * @file
 * Contains \Drupal\station_schedule\Entity\Schedule.
 */

namespace Drupal\station_schedule\Entity;

use Drupal\Core\Datetime\DateHelper;
use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityStorageInterface;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\BaseFieldDefinition;
use Drupal\station_schedule\ScheduleInterface;

/**
 * Defines the station schedule entity class.
 *
 * @ContentEntityType(
 *   id = "station_schedule",
 *   label = @Translation("Schedule"),
 *   base_table = "station_schedule",
 *   data_table = "station_schedule_field_data",
 *   admin_permission = "administer station schedule",
 *   field_ui_base_route = "entity.station_schedule.collection",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "title",
 *     "uuid" = "uuid",
 *     "uid" = "uid",
 *   },
 *   handlers = {
 *     "list_builder" = "\Drupal\station_schedule\Entity\ScheduleListBuilder",
 *     "view_builder" = "\Drupal\station_schedule\Entity\ScheduleViewBuilder",
 *     "route_provider" = {
 *       "html" = "\Drupal\station_schedule\Entity\ScheduleRouteProvider",
 *     },
 *     "form" = {
 *       "add" = "\Drupal\station_schedule\Entity\Form\ScheduleAddForm",
 *       "edit" = "\Drupal\station_schedule\Entity\Form\ScheduleEditForm",
 *       "delete" = "\Drupal\station_schedule\Entity\Form\ScheduleDeleteForm",
 *     },
 *   },
 *   links = {
 *     "canonical" = "/station/schedule/{station_schedule}",
 *     "add-form" = "/station/schedule/add",
 *     "edit-form" = "/station/schedule/{station_schedule}/edit",
 *     "schedule" = "/station/schedule/{station_schedule}/schedule",
 *     "delete-form" = "/station/schedule/{station_schedule}/delete",
 *     "collection" = "/admin/station/schedule",
 *   }
 * )
 */
class Schedule extends ContentEntityBase implements ScheduleInterface {

  /**
   * Runtime only.
   *
   * @var \Drupal\station_schedule\ScheduleItemInterface[][]
   */
  protected $scheduledItems = [];

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields['id'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Station schedule ID'))
      ->setDescription(t('The station schedule ID.'))
      ->setReadOnly(TRUE)
      ->setSetting('unsigned', TRUE);

    $fields['uuid'] = BaseFieldDefinition::create('uuid')
      ->setLabel(t('UUID'))
      ->setDescription(t('The station schedule UUID.'))
      ->setReadOnly(TRUE);

    $fields['title'] = BaseFieldDefinition::create('string')
      ->setLabel(t('Title'))
      ->setRequired(TRUE)
      ->setTranslatable(TRUE)
      ->setRevisionable(TRUE)
      ->setSetting('max_length', 255)
      ->setDisplayOptions('view', [
        'label' => 'hidden',
        'type' => 'string',
        'weight' => -5,
      ])
      ->setDisplayOptions('form', [
        'type' => 'string_textfield',
        'weight' => -5,
      ])
      ->setDisplayConfigurable('form', TRUE);

    $fields['uid'] = BaseFieldDefinition::create('entity_reference')
      ->setLabel(t('Authored by'))
      ->setDescription(t('The username of the content author.'))
      ->setRevisionable(TRUE)
      ->setSetting('target_type', 'user')
      ->setDefaultValueCallback('Drupal\node\Entity\Node::getCurrentUserId')
      ->setTranslatable(TRUE)
      ->setDisplayOptions('view', [
        'label' => 'hidden',
        'type' => 'author',
        'weight' => 0,
      ])
      ->setDisplayOptions('form', [
        'type' => 'entity_reference_autocomplete',
        'settings' => [
          'match_operator' => 'CONTAINS',
          'size' => '60',
          'placeholder' => '',
        ],
      ])
      ->setDisplayConfigurable('form', TRUE);

    $fields['increment'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Time increment'))
      ->setDescription(t('Increment of the schedule block size in minutes.'))
      ->setSetting('unsigned', TRUE)
      ->setDefaultValue(0)
      ->setDisplayOptions('form', [
        'type' => 'station_schedule_increment',
      ])
      ->setDisplayConfigurable('form', TRUE);

    $fields['start_hour'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('Start hour'))
      ->setDescription(t('Schedule start time in hours.'))
      ->setSetting('unsigned', TRUE)
      ->setDefaultValue(0)
      ->setSetting('hour_type', 'start')
      ->setDisplayOptions('form', [
        'type' => 'station_schedule_hour',
      ])
      ->setDisplayConfigurable('form', TRUE);

    $fields['end_hour'] = BaseFieldDefinition::create('integer')
      ->setLabel(t('End hour'))
      ->setDescription(t('Schedule end time in hours.'))
      ->setSetting('unsigned', TRUE)
      ->setDefaultValue(24)
      ->setDisplayOptions('form', [
        'type' => 'station_schedule_hour',
      ])
      ->setSetting('hour_type', 'end')
      ->setDisplayConfigurable('form', TRUE);

    $fields['unscheduled_message'] = BaseFieldDefinition::create('string')
      ->setLabel(t("'No scheduled item' message"))
      ->setDescription('Message to display during unscheduled periods.')
      ->setRequired(TRUE)
      ->setSetting('max_length', 255)
      ->setDisplayOptions('form', [
        'type' => 'string_textfield',
      ])
      ->setDisplayConfigurable('form', TRUE);

    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function getStartHour() {
    return $this->get('start_hour')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getEndHour() {
    return $this->get('end_hour')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getIncrement() {
    return $this->get('increment')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getUnscheduledMessage() {
    return $this->get('unscheduled_message')->value;
  }

  /**
   * {@inheritdoc}
   */
  public function getScheduledItems() {
    return $this->scheduledItems;
  }

  /**
   * {@inheritdoc}
   */
  public function setScheduledItems(array $scheduled_items) {
    $this->scheduledItems = $scheduled_items;
    return $this;
  }

  /**
   * {@inheritdoc}
   *
   * @param \Drupal\station_schedule\ScheduleInterface[] $entities
   */
  public static function postLoad(EntityStorageInterface $storage, array &$entities) {
    $day_names = DateHelper::weekDaysOrdered(DateHelper::weekDays(TRUE));

    /** @var \Drupal\Core\Entity\EntityStorageInterface $schedule_item_storage */
    $schedule_item_storage = \Drupal::service('entity_type.manager')->getStorage('station_schedule_item');
    $minutes_per_day = 60 * 24;
    foreach ($entities as $entity) {
      /** @var \Drupal\station_schedule\ScheduleItemInterface[] $schedule_items */
      $schedule_items = $schedule_item_storage->loadByProperties(['schedule' => $entity->id()]);

      $schedule = [];
      foreach ($day_names as $day => $name) {
        $schedule[$day] = [];

        // Find shows that start before the end of the day anf finish after the
        // beginning of the day.
        $start = $day * $minutes_per_day + ($entity->getStartHour() * 60);
        $finish = $day * $minutes_per_day + ($entity->getEndHour() * 60);
        $ids = $schedule_item_storage->getQuery()
          ->condition('schedule', $entity->id())
          ->condition('start', $finish, '<')
          ->condition('finish', $start, '>')
          ->sort('start')
          ->execute();

        foreach ($ids as $id) {
          $scheduled_item = clone $schedule_items[$id];
          // If a show spans a day, limit its start and finish times to be
          // within the day.
          if ($scheduled_item->getStart() < $start) {
            $scheduled_item->get('start')->setValue($start);
          }
          if ($scheduled_item->getFinish() > $finish) {
            $scheduled_item->get('finish')->setValue($finish);
          }
          $schedule[$day][] = $scheduled_item;
        }
      }
      $entity->setScheduledItems($schedule);
    }
  }

}
