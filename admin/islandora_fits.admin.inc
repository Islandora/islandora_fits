<?php

/**
 * @file
 */

function islandora_fits_admin_form() {
  $form['islandora_fits_executable_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path to fits processor'),
    '#description' => t('Enter the location and filename (on the system) of the fits processing script.  Include the filename, not just the path.'),
    '#default_value' => variable_get('islandora_fits_executable_path'),
  );

  // add form options for what the datastream is called
  $form['islandora_fits_techmd_dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('Technical metadata datastream id'),
    '#description' => t('This is the dsid of the technical metadata on the object.'),
    '#default_value' => variable_get('islandora_fits_techmd_dsid', 'TECHMD'),
  );
  // add options to include/exclude certain mimetypes/extensions/solution packs

  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['#submit'][] = 'islandora_fits_admin_submit_form';

  return $form;
}

function islandora_fits_admin_submit_form($form, &$form_state) {
  // should probably check that the fits executable is correct
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }
}
