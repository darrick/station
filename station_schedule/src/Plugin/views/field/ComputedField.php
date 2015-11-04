<?php

/**
 * @file
 * Contains \Drupal\station_schedule\Plugin\views\field\ComputedField.
 */

namespace Drupal\station_schedule\Plugin\views\field;

use Drupal\views\Plugin\views\field\FieldPluginBase;
use Drupal\views\ResultRow;

/**
 * @ViewsField("computed_field")
 */
class ComputedField extends FieldPluginBase {

  /**
   * {@inheritdoc}
   */
  public function query() {
    // Intentionally empty.
  }

  /**
   * {@inheritdoc}
   */
  public function getValue(ResultRow $values, $field = NULL) {
    $entity = $this->getEntity($values);
    return $entity ? $entity->{$this->definition['entity field']}->getValue() : NULL;
  }

}
