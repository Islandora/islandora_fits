<?php

/**
 * @file
 *
 * This is a template used to display the technical metadata that
 * is present in the TECHMD datastream.
 *
 * islandora_object is a fedora tuque Object. Additional documentation
 * can be gathered from the islandora-object.tpl.php present in the Islandora
 * module.
 *
 * islandora_fits_table contains the required table definition needed
 * in rendering a table. It contains the header fields as well as the data
 * to be placed in rows.
 * 
 */
?>

<?php drupal_set_title($islandora_object->label . ' Technical Metadata'); ?>

<?php print theme('table', $islandora_fits_table); ?>