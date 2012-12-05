<?php

/**
 * @file
 */

function islandora_fits_admin() {
  $form['islandora_fits_executable_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path to fits.sh script?'),
    '#description' => t('Enter the location (on the system) of the fits.sh script.'),
    '#default_value' => variable_get('islandora_fits_executable_path', TRUE),
  );
}
