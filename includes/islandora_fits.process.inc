/**
 * @file
 */

/**
 * Creates all technical metadata derivatives as datastreams for given object.
 *
 * @param FedoraObject $object
 * The object that will be used to generate/store the derivatives.
 *
 * @return boolean
 * TRUE if successful, FALSE otherwise.
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

  if (($exif_uri = islandora_fits_create_exif($file_uri)) !== FALSE) {
    islandora_audio_add_datastream($object, 'TECHMD', $exif_uri);
  }

  file_unmanaged_delete($file_uri);
  islandora_audio_add_datastream($object, 'TN', drupal_get_path('module', 'islandora_audio') . '/images/audio-TN.jpg');
  return TRUE;
}

/**
 * Creates the exif derivative from the given file.
 *
 * @param string $file_uri
 * The URI to the file from which the derivative will be generated.
 *
 * @return string
 * A URI to the generated derivative if successful, FALSE otherwise.
 */
function islandora_fits_create_exif($file_uri) {
  $output = array();
  $file = drupal_realpath($file_uri);
  $outfile = $file . '_exif.xml';
  $command = "exiftool -X $file > $outfile";
  $ret = '0';
  exec($command, $output, $ret);
  if ($ret == '0') { // success
    return $outfile;
  }
  return FALSE;
}
