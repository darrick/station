<?php

/**
 * @file
 * Contains \Drupal\station_schedule\TypedData\ComputedField.
 */

namespace Drupal\station_schedule\TypedData;

use Drupal\Core\TypedData\TypedData;

/**
 * Provides a helper class to extend for computed fields.
 */
abstract class ComputedField extends TypedData {

  /**
   * The computed value.

   * @var mixed
   */
  protected $value;

  /**
   * Computes the value.
   *
   * @return mixed
   *   The value for this field.
   */
  abstract protected function computeValue();

  /**
   * {@inheritdoc}
   */
  public function getValue() {
    if ($this->value === NULL) {
      $this->value = $this->computeValue();
    }
    return $this->value;
  }

  /**
   * @todo Remove once https://www.drupal.org/node/2608468 is committed.
   */
  public function setLangcode($langcode) {
    // Intentionally empty.
  }

}
