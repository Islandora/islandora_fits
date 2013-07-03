#### BUILD STATUS

Current build status:
[![Build Status](https://travis-ci.org/Islandora/islandora_fits.png?branch=7.x)](https://travis-ci.org/Islandora/islandora_fits)

CI Server:
http://jenkins.discoverygarden.ca

#### SUMMARY

A simple module to extend islandora solution pack install processes by adding
technical metadata extraction via the File Information Tool Set (FITS).

Behind the scenes, the module tries to get as much metadata from your file by
running:
>fits.sh -i *infile* -xc -o *outfile*

The -xc command can sometimes cause problems, so if that fails, the module tries
>fits.sh -i *infile* -x -o *outfile*

Should that fail, technical metadata extraction is aborted and the error is
logged in the watchdog. An error my be produced and logged in the apache error.log file even
if TECHMD DS extraction is successfull, as the first attempt may fail and log an error while
subsequent attempts may succeed.

The most common error printed out to the error.log file that is safe to ignore is as follows:
"Exception in thread "main" java.lang.NullPointerException
    at edu.harvard.hul.ois.fits.FitsOutput.addStandardCombinedFormat(FitsOutput.java:310)
    at edu.harvard.hul.ois.fits.Fits.outputStandardCombinedFormat(Fits.java:294)
    at edu.harvard.hul.ois.fits.Fits.outputResults(Fits.java:275)
    at edu.harvard.hul.ois.fits.Fits.main(Fits.java:186)
Error: output cannot be converted to a standard schema format for this file"

Watchdog will be updated when TECHMD DS fail's to generate.

#### REQUIREMENTS

The File Information Tool Set (http://code.google.com/p/fits/).

#### INSTALLATION

Download and unzip the fits tool which can be found at
http://code.google.com/p/fits/ (you don't need the extras.zip just the latest version
of fits-X.Y.Z.zip).  Make sure to unzip it into a location where the apache user can
get access.  Navigate to the unpacked fits folder and add executable permissions to
fits.sh so the apache user can run the script.  Clone this module into your drupal
modules folder and enable it in drupal to activate automatic technical metadata
extraction.

#### CONFIGURATION

Navigate to http://yourhostname/#overlay=admin/islandora/fits.  Here you can
specify the location of the fits script (on the file system) and the dsid you
want to use for the technical metadata datastream.

The System path to fits processor must include "fits.sh", so it would look like:
"/usr/share/fits-0.6.1/fits.sh" instead of "/usr/share/fits-0.6.1/".

#### TROUBLESHOOTING

If you run an ingest and you don't get any technical metadata, check to make
sure the permissions on the fits folder and the fits.sh script are correct and
the apache user can run the script.

Some images and audio files will cause problems during metadata extraction.
These are not fatal errors, but appear to be formats the fits script can't
understand.  In these cases, you will get some error reporting in the technical
metadata datastream that may help determine what happened.

#### FAQ

 Q: I ingested an object but I didn't get any technical metadata, what happened?

 A: Its most likely you forgot to update the permissions on the fits.sh script.
    If that is not the case, make sure your configuration points to the correct
    file.

#### CONTACT
