<?php

/**
 * @file
 */

function islandora_fits_admin_form($form, $form_state) {
  if (isset($form_state['triggering_element'])) {
    // Textfield AJAX callback
    if ($form_state['triggering_element']['#name'] == 'islandora_fits_path_textfield') {
       $fits_path = $form_state['input']['islandora_fits_path_textfield'];
    }
  }
  else {
    $fits_path = variable_get('islandora_fits_executable_path', 'fits.sh');
  }
  $conf_message = islandora_fits_path_check($fits_path);
  $form['islandora_fits_wrapper'] = array(
    '#prefix' => '<div id="islandora_fits_wrapper">',
    '#suffix' => '</div>'
  );
  $form['islandora_fits_wrapper']['islandora_fits_executable_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path to FITS processor'),
    '#name' => 'islandora_fits_path_textfield',
    '#description' => t('Enter the location and filename (on the system) of the FITS processing script. Include the filename, not just the path.'),
    '#default_value' => $fits_path,
    '#ajax' => array(
      'callback' => 'islandora_fits_path_ajax',
      'wrapper' => 'islandora_fits_wrapper'
    ),
  );
  $form['islandora_fits_wrapper']['islandora_fits_path_check'] = array(
    '#markup' => $conf_message
  );
  // add form options for what the datastream is called
  $form['islandora_fits_techmd_dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('Technical metadata datastream ID'),
    '#description' => t('The DSID to use for an object\'s technical metadata.'),
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
  drupal_set_message(t('The settings have been updated!'));
}

function islandora_fits_path_ajax($form, $form_state) {
  return $form['islandora_fits_wrapper'];
}

function islandora_fits_path_check($fits_path) {
  $output = array();
  $return_value = '';
  exec('stat ' . $fits_path, $output, $return_value);

  if ($return_value !== 0) {
    $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/>' . t('Unable to locate fits.sh at the above location!');
  }
  else {
    $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>' . t('Found fits.sh at the above location!');
  }
  return $confirmation_message;
}