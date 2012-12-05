<?php

/**
 * @file
 */

function islandora_fits_admin_form() {
  $form['islandora_fits_executable_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path to fits.sh script?'),
    '#description' => t('Enter the location (on the system) of the fits.sh script.'),
    '#default_value' => variable_get('islandora_fits_executable_path'),
  );

  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['#submit'][] = 'islandora_fits_admin_submit_form';

  return $form;
}

function islandora_fits_admin_submit_form($form, &$form_state) {

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }

}
