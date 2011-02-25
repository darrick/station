#!/usr/local/bin/php -q
<?php


/**
 * This script is designed to be run by a cron job or windows scheduled task.
 * It connects to a webstream, downloads one hours worth and saves it as an
 * MP3 for the station archive module to import.
 *
 *     Y O U   D O N ' T   N E E D   T O   E D I T   T H I S   F I L E !
 *       Configuration options are now set in the ripper.ini file.
 *
 * This script requires that Streamripper (http://streamripper.sourceforge.net/)
 * version 1.61.17 or higher be installed (we use the --quite option)
 */

// Load our settings from the ripper.ini file.
$settings = parse_ini_file(dirname(__FILE__) .'/ripper.ini');

// Check that the import directory is writable.
$import_dir = $settings['import_path'];
if (!is_dir($import_dir) || !is_writable($import_dir)) {
  exit("Cannot write to the import directory '$import_dir'");
}

// Make sure we can find stream ripper. The is_executable() test might not work
// with PHP4 and Windows. Upgrade! PHP 5.1 is great.
$streamripper = $settings['streamripper_path'];
if (!file_exists($streamripper) || !is_executable($streamripper)) {
  exit("Couldn't find the stream ripper executable at '$streamripper'");
}

// Determine when we're starting, when we should end and convert that to a
// length of time in seconds.
$startTime = roundToNearestHour(time());
$endTime = $startTime + 3600;
$length = ($endTime - time()) + $settings['overlap_seconds'];

// Download the stream
$stream_url = $settings['stream_url'];
exec("{$streamripper} {$stream_url} -s -d {$import_dir} -A -l {$length} -a {$startTime}.mp3 --quiet");

// stream ripper creates the .cue file. we'll use its absence as a signal
// to the module that it's safe to import a file.
unlink("{$import_dir}/{$startTime}.cue");

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
