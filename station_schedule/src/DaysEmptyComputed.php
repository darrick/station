<?php

/**
 * @file
 * Contains \Drupal\station_schedule\DaysEmptyComputed.
 */

namespace Drupal\station_schedule;

use Drupal\Core\TypedData\TypedData;

/**
 * @todo.
 */
class DaysEmptyComputed extends TypedData {

  /**
   * @var int|null
   */
  protected $daysEmpty;

  /**
   * {@inheritdoc}
   */
  public function getValue($langcode = NULL) {
    if ($this->daysEmpty === NULL) {
      $scheduled_items_by_day = $this->getParent()->getValue()->getScheduledItemsByDay();
      $this->daysEmpty =  7 - count(array_filter($scheduled_items_by_day));
    }

    return $this->daysEmpty;
  }

  /**
   * @todo \Drupal\Core\Entity\ContentEntityBase::getTranslatedField() should
   *   check for \Drupal\Core\Field\FieldItemListInterface before calling
   *   setLangcode().
   */
  public function setLangcode($langcode) {
    // Intentionally empty.
  }

}
