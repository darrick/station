<?php

/**
 * @file
 * Contains \Drupal\station_schedule\Entity\ScheduleViewsData.
 */

namespace Drupal\station_schedule\Entity;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\views\EntityViewsData;

/**
 * @todo.
 */
class ScheduleViewsData extends EntityViewsData {

  /**
   * {@inheritdoc}
   */
  public function getViewsData() {
    $data = parent::getViewsData();

    $field_definitions = $this->entityManager->getBaseFieldDefinitions($this->entityType->id());
    /** @var \Drupal\Core\Field\FieldDefinitionInterface[] $computed_fields */
    $computed_fields = array_filter($field_definitions, function (FieldDefinitionInterface $field_definition) {
      return $field_definition->isComputed();
    });

    if ($computed_fields) {
      $computed_table = $this->entityType->getBaseTable() . '_computed';
      $data[$computed_table]['table']['group'] = $this->entityType->getLabel();
      $data[$computed_table]['table']['provider'] = $this->entityType->getProvider();
      $data[$computed_table]['table']['join'] = ['#global' => []];
      foreach ($computed_fields as $computed_field) {
        $name = $computed_field->getName();
        $data[$computed_table][$name] = [
          'title' => $computed_field->getLabel(),
          'help' => $computed_field->getDescription(),
          'field' => ['id' => 'computed_field'],
          'entity field' => $name,
        ];
      }
    }

    return $data;
  }

}
