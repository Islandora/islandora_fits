<?php

/**
 * @file
 */

/**
 * Creates all technical metadata derivatives as datastreams for given object.
 *
 * @param FedoraObject $object
 *   The object that will be used to generate/store the derivatives.
 *
 * @return boolean
 *   TRUE if the techmd datastream was created, FALSE otherwise.
 */
function islandora_fits_create_techmd(FedoraObject $object) {
  if (!isset($object["OBJ"])) {
    drupal_set_message(t("Could not create technical derivatives for %s. No file was uploaded.", array("%s" => $object->id), "error"));
    return FALSE;
  }
  $mime_detect = new MimeDetect();
  $ext = $mime_detect->getExtension($object['OBJ']->mimeType);
  $file_name = str_replace(":", "-", $object->id);
  $file_uri = file_unmanaged_save_data($object['OBJ']->content, "temporary://{$file_name}.OBJ.{$ext}");

  if (($fits_uri = islandora_fits_create_fits($file_uri)) !== FALSE) {
    islandora_fits_add_datastream($object, variable_get('islandora_fits_techmd_dsid', 'TECHMD'), $fits_uri);
    drupal_unlink($fits_uri);
  }

  // add any more processors here - there probably won't be any

  file_unmanaged_delete($file_uri);

  // if fits_uri resolved, then we succeeded, otherwise we failed
  return ($fits_uri !== FALSE);
}

/**
 * Creates the technical metadata derivative from the given file.
 *
 * @param string $file_uri
 *   The URI to the file from which the derivative will be generated.
 *
 * @return string
 *   A URI to the generated derivative if successful, FALSE otherwise.
 */
function islandora_fits_create_fits($file_uri) {
  $output = array();
  $file = drupal_realpath($file_uri);
  $outfile = $file . ".tech.xml";
  // compose fits command
  $command = variable_get("islandora_fits_executable_path") . " -i " . $file . " -xc -o " . $outfile;
  exec($command, $output, $ret);
  if ($ret == "0") {
    return $outfile;
  }
  return FALSE;
}

/**
 * Adds the given file as a datastream to the given object using the given datastream id to identify it.
 *
 * @param FedoraObject $object
 *  The object to add the datasteam to.
 * @param string $datastream_id
 *   The datastream id of the added datastream.
 * @param string $file_uri
 *   A URI to the file containing the content for the datastream.
 */
function islandora_fits_add_datastream($object, $datastream_id, $file_uri) {
  try {
    $mime_detector = new MimeDetect();
    $ds = $object->constructDatastream($datastream_id, "M");
    $ds->label = $datastream_id;
    $ds->mimetype = $mime_detector->getMimetype($file_uri);
    $ds->setContentFromFile(drupal_realpath($file_uri));
    $object->ingestDatastream($ds);
  }
  catch (exception $e) {
    drupal_set_message(t("@message", array("@message" => $e->getMessage())));
  }
}
