<?php
// $Id$

class station_schedule_handler_field_time extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['style'] = array('default' => 'station_hour');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['style'] = array(
      '#type' => 'radios',
      '#title' => t('Style'),
      '#options' => array(
        'station_hour' => t('Hour'),
        'station_dayhour' => t('Day/hour'),
      ),
      '#default_value' => $this->options['style'],
      '#description' => t('Which time style should be used.'),
    );
  }

  function render($values) {
    return theme($this->options['style'], $values->{$this->field_alias});
  }
}
