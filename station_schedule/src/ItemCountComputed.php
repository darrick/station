<?php

/**
 * @file
 * Contains \Drupal\station_schedule\ItemCountComputed.
 */

namespace Drupal\station_schedule;

use Drupal\Core\TypedData\TypedData;

/**
 * @todo.
 */
class ItemCountComputed extends TypedData {

  /**
   * @var int|null
   */
  protected $itemCount;

  /**
   * {@inheritdoc}
   */
  public function getValue($langcode = NULL) {
    if ($this->itemCount === NULL) {
      $this->itemCount = count($this->getParent()->getValue()->getScheduledItems());
    }

    return $this->itemCount;
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
