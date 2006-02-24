#!/usr/local/bin/php -q
<?php

// $Id$

/**
 * This script is designed to be run by a cron job or windows scheduled task.
 * It connects to a webstream, downloads one hours worth and saves it as an
 * MP3 for the station archive module to import.
 *
 * This script requires that Streamripper (http://streamripper.sourceforge.net/)
 * version 1.61.17 or higher be installed (we use the --quite option)
 */

/**
 * Complete URL of webstream to rip
 */
$STREAM_URL ='http://127.0.0.1:8000/listen';

/**
 * Full path to the stream ripper executable. Remember, version 1.61.17 or higher.
 */
$STREAMRIPPER = '/usr/local/bin/streamripper';

/**
 * full path to station archive module's import directory (no trailing slash)
 */
$IMPORT_DIR = '/usr/local/www/drupal/modules/station/archive/import';

/**
 * the number of seconds this hour will over lap the next.
 */
$OVERLAP_SECONDS = 60;

 //////////////////////////////////////////////////////////////////////
// Y O U   D O N ' T   N E E D   T O   E D I T   B E L O W   H E R E //
//////////////////////////////////////////////////////////////////////

// Because shows tend to start early or finish late, I want some overlap
// on each end of the shows. This task should be scheduled to start one
// minute before the hour and grab an extra minute at the end. Tweak thes
// time to roll into the next hour.
$startTime = roundToNearestHour(time());
$endTime = $startTime + 3600;
$length = ($endTime - time()) + $OVERLAP_SECONDS;

// download the stream
exec("$STREAMRIPPER $STREAM_URL -s -d $IMPORT_DIR -A -l $length -a $startTime.mp3 --quiet");

// stream ripper creates the .cue file. we'll use its absence as a signal
// to the module that it's safe to import a file.
unlink("$IMPORT_DIR/$startTime.cue");

exit(0);


/**
 * Round a timestamp to the nearest hour.
 *
 * @param $time
 *   A UNIX timestamp
 */
function roundToNearestHour($time) {
  $parts = getdate($time);
  if ($parts['minutes'] > 50) {
    // advance it to the next hour
    $roundedTime = mktime($parts['hours'] + 1, 0, 0, $parts['mon'], $parts['mday'], $parts['year']);
  }
  else {
    // we're late for this hour
    $roundedTime = mktime($parts['hours'], 0, 0, $parts['mon'], $parts['mday'], $parts['year']);
  }
  return $roundedTime;
}

?>
