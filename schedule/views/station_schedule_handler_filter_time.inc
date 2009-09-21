<?php
// $Id$

class station_schedule_handler_filter_time extends views_handler_filter_numeric {
  // Hide a bunch of the operators.
  function operators() {
    $operators = parent::operators();
    // BETWEEN doesn't really work with the 'now' mode.
    unset($operators['between'], $operators['not between']);
    // NULLs don't make sense since we require values for start and finish.
    unset($operators['empty'], $operators['not empty']);
    return $operators;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['value']['contains']['mode']['default'] = 'now';
    $options['value']['contains']['offset']['default'] = 0;
    return $options;
  }

  function value_form(&$form, &$form_state) {
    $form['value']['mode'] = array(
      '#type' => 'radios',
      '#title' => 'Mode',
      '#options' => array(
        'now' => t('Current time'),
        'minutes' => t('Specific time'),
      ),
      '#default_value' => $this->value['mode'],
    );

    parent::value_form($form, $form_state);

    $form['value']['offset'] = array(
      '#type' => 'textfield',
      '#title' => empty($form_state['exposed']) ? t('Minute offset') : '',
      '#size' => 10,
      '#default_value' => $this->value['offset'],
      '#description' => t('Offset in minutes, e.g. -60 for one hour ago, 1440 for one day from now.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[value][mode]' => array('now')),
    );
    $form['value']['value'] = array(
      '#type' => 'station_schedule_daytime',
      '#title' => $form['value']['value']['#title'],
      '#default_value' => $this->value['value'],
      '#process' => array('expand_station_schedule_daytime', 'views_process_dependency'),
      '#dependency' => $form['value']['value']['#dependency'] + array('radio:options[value][mode]' => array('minutes')),
      '#dependency_count' => 2,
    );
  }

  function admin_summary() {
    $options = $this->operator_options('short');
    $output = check_plain($options[$this->operator]) . ' ';

    switch ($this->value['mode']) {
      case 'now':
        if (empty($this->value['offset'])) {
          $output .= t('Now');
        }
        elseif ($this->value['offset'] < 0) {
          $output .= t('Now minus @offset minutes', array('@offset' => (int) $this->value['offset']));
        }
        else {
          $output .= t('Now plus @offset minutes', array('@offset' => (int) $this->value['offset']));
        }
        break;

      case 'minutes':
        $output .= theme('station_dayhour', $this->value['value']);
        break;

      default:
        $output .= check_plain((string) $this->value['value']);
    }
    return $output;
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    $minutes_in_week = MINUTES_IN_WEEK;
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      switch ($this->value['mode']) {
      case 'now':
        $offset = (int) $this->value['offset'];
        if (empty($offset)) {
          $this->query->add_where($this->options['group'], "$field $this->operator ***CURRENT_STATION_MINUTE***");
        }
        else {
          $this->query->add_where($this->options['group'], "$field $this->operator (***CURRENT_STATION_MINUTE*** + %d + {$minutes_in_week}) %% {$minutes_in_week}", $offset);
        }
        break;

      case 'minutes':
        $this->query->add_where($this->options['group'], "$field $this->operator %d", $this->value['value']);
        break;
      }
    }
    return;
  }
}