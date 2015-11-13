<?php

/**
 * @file
 * Contains \Drupal\station_schedule\Plugin\Block\ScheduledItemBlock.
 */

namespace Drupal\station_schedule\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityViewBuilderInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * @todo.
 *
 * @Block(
 *   id = "station_schedule_item",
 *   admin_label = @Translation("Scheduled item"),
 *   context = {
 *     "schedule item" = @ContextDefinition("entity:station_schedule_item")
 *   }
 * )
 */
class ScheduledItemBlock extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * ScheduledItemBlock constructor.
   *
   * @param array $configuration
   * @param string $plugin_id
   * @param mixed $plugin_definition
   * @param \Drupal\Core\Entity\EntityViewBuilderInterface $view_builder
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityViewBuilderInterface $view_builder) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->scheduleItemViewBuilder = $view_builder;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('entity_type.manager')->getViewBuilder('station_schedule_item')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    return $this->scheduleItemViewBuilder->view($this->getContextValue('schedule item'));
  }

}
