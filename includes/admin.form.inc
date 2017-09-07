<?php

/**
 * @file
 * The admin form for the islandora fits module.
 */

/**
 * Admin form for Islandora FITS configuration.
 *
 * @see drupal_get_form
 */
function islandora_fits_admin_form($form, $form_state) {
  if (isset($form_state['triggering_element'])) {
    // Textfield AJAX callback.
    if ($form_state['triggering_element']['#name'] == 'islandora_fits_path_textfield') {
      $fits_path = $form_state['input']['islandora_fits_path_textfield'];
    }
  }
  else {
    $fits_path = variable_get('islandora_fits_executable_path', 'fits.sh');
  }
  $conf_message = islandora_fits_get_path_message($fits_path);
  $form['islandora_fits_do_derivatives'] = array(
    '#type' => 'checkbox',
    '#title' => t('Run Islandora FITS derivatives locally'),
    '#name' => 'islandora_fits_do_derivatives',
    '#description' => t('Uncheck if FITS is not installed on this system, or if technical metadata extraction is being done elsewhere.'),
    '#return_value' => 1,
    '#default_value' => variable_get('islandora_fits_do_derivatives', 1),
  );
  $form['islandora_fits_wrapper'] = array(
    '#prefix' => '<div id="islandora_fits_wrapper">',
    '#suffix' => '</div>',
  );
  $form['islandora_fits_wrapper']['islandora_fits_executable_path'] = array(
    '#type' => 'textfield',
    '#title' => t('System path to FITS processor'),
    '#name' => 'islandora_fits_path_textfield',
    '#description' => t('Enter the location and filename (on the system) of the FITS processing script. Include the filename, not just the path.'),
    '#default_value' => $fits_path,
    '#ajax' => array(
      'callback' => 'islandora_fits_path_ajax',
      'wrapper' => 'islandora_fits_wrapper',
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_fits_do_derivatives"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['islandora_fits_wrapper']['islandora_fits_path_check'] = array(
    '#type' => 'item',
    '#markup' => $conf_message,
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_fits_do_derivatives"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Add form options for what the datastream is called.
  $form['islandora_fits_techmd_dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('Technical metadata datastream ID'),
    '#description' => t("The DSID to use for an object's technical metadata."),
    '#default_value' => variable_get('islandora_fits_techmd_dsid', 'TECHMD'),
  );
  // Add options to include/exclude certain mimetypes/extensions/solution packs.
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save Configuration'));
  $form['#submit'][] = 'islandora_fits_admin_submit_form';

  return $form;
}

/**
 * Validation handler for Fits admin form.
 */
function islandora_fits_admin_form_validate($form, &$form_state) {
  if ($form_state['values']['islandora_fits_do_derivatives']) {
    if (!islandora_fits_path_check($form_state['values']['islandora_fits_executable_path'])) {
      form_set_error('islandora_fits_path_textfield', "Path to FITS is not valid. Provide a valid path or disable FITS derivatives.");
    }
  }
  else {
    // TODO: Clear the fits executable path?
  }
}

/**
 * Form submit handler.
 */
function islandora_fits_admin_submit_form($form, &$form_state) {
  $variables = array(
    'islandora_fits_do_derivatives',
    'islandora_fits_executable_path',
    'islandora_fits_techmd_dsid',
  );
  foreach ($variables as $variable) {
    variable_set($variable, $form_state['values'][$variable]);
  }
  drupal_set_message(t('The settings have been updated!'));
}

/**
 * Ajax handler for admin form.
 */
function islandora_fits_path_ajax($form, $form_state) {
  return $form['islandora_fits_wrapper'];
}

/**
 * Gets a user-friendly message to show whether the path is valid.
 */
function islandora_fits_get_path_message($fits_path) {
  if (islandora_fits_path_check($fits_path)) {
    $confirmation_message = '<img src="' . url('misc/watchdog-ok.png') . '"/>' . t('File Information Tool Set found at @fitspath', array('@fitspath' => $fits_path));
  }
  else {
    $confirmation_message = '<img src="' . url('misc/watchdog-error.png') . '"/>' . t('Unable to locate the File Information Tool Set at @fitspath', array('@fitspath' => $fits_path));
  }
  return $confirmation_message;
}

/**
 * Checks fits path for an executable.
 *
 * Returns true if the path is an executable, false otherwise.
 */
function islandora_fits_path_check($fits_path) {
  if (is_executable($fits_path)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
