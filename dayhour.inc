<?php
// $Id$

/**
 * Turn an mixed into a valid hour.
 *
 * @param   mixed   we try our hardest to convert any thing
 * @return  integer between 0 and 23.
 */
function station_valid_hour($hour) {
  if (!is_numeric($hour)) {
    $hour = strtolower(trim($hour));

    // take care of a couple of handy strings
    if ($hour == t('midnight')) {
      return 0;
    } else if ($hour == t('noon')) {
      return 12;
    }

    // try to parse out am/pm times then let it fall through to the
    // 24 hour stuff below.
    $parts = array();
    if (preg_match('/(\d+)\s*([ap]m)/', $hour, $parts)) {
      $hour = (integer) $parts[1];
      // 12am and 12pm are special cases
      if ($hour == 12) {
          $hour += 12;
      }
      if ($parts[2] == 'pm') {
          $hour += 12;
      }
    }
  }

  if ($hour < 0) {
    return ((integer) $hour % 24) + 24;
  }
  return ((integer) $hour % 24);
}

/**
 * Get a numeric day of the week from a string or integer.
 *
 * @param   mixed integer from 1-7, or a, case in-sensitive, english day name.
 * @return  integer between 1 and 7 (sunday, monday, ... ,saturday)
 */
function station_valid_day($day) {
  if (is_numeric($day)) {
    if ($day < 1) {
      return ((integer) $day % 7) + 7;
    } else if ($day > 7) {
      return ((integer) $day % 7);
    }
    return (integer) $day;
  } else {
    $dayname = station_day_name();
    $ret = array_search(ucfirst($day), $dayname, false);
    return ($ret === false) ? 1 : $ret;
  }
}

/**
 * Return an array of the names of the days of the week.
 *
 * @param integer optional day of the week
 * @return array|string array if $index isn't specified
 */
function station_day_name($day = null) {
  static $days = array(
    1 => 'Sunday',
    'Monday',
    'Tuesday',
    'Wednesday',
    'Thursday',
    'Friday',
    'Saturday'
  );

  if ($day === null) {
    return $days;
  } else {
    return $days[$day];
  }
}

/**
 * Return a formated hour string.
 *
 * @param integer $hour between 0 and 23.
 * @see station_valid_hour() to sanitize hour input.
 * @return string
 */
function station_hour_name($hour = 0) {
  if ($hour < 12) {
    $sufix = 'am';
  } else {
    $sufix = 'pm';
    $hour -= 12;
  }
  // midnight and noon
  if ($hour <= 0) {
    $hour = 12;
  }
  return $hour . $sufix;
}