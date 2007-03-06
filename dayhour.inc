<?php

// $Id$

// there are 1440 minutes in a day (60minute * 24hours = 1day)
define('MINUTES_IN_DAY', 1440);

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
    }
    else if ($hour == t('noon')) {
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
 * @param   mixed integer from 0-6, or a, case in-sensitive, english day name.
 * @return  integer between 0 and 6 (sunday, monday, ... ,saturday)
 */
function station_valid_day($day) {
  if (is_numeric($day)) {
    if ($day < 0) {
      return ((integer) $day % 7) + 7;
    }
    else if ($day > 6) {
      return ((integer) $day % 7);
    }
    return (integer) $day;
  }
  else {
    $dayname = station_day_name();
    $ret = array_search(ucfirst($day), $dayname, false);
    return ($ret === false) ? 0 : $ret;
  }
}

/**
 * Return an array of the names of the days of the week.
 *
 * @param integer optional day of the week. Sunday = 0
 * @return array|string array if $index isn't specified
 */
function station_day_name($day = null) {
  static $days;
  if (!isset($days)) {
    $days = array(
      0 => t('Sunday'),
      t('Monday'),
      t('Tuesday'),
      t('Wednesday'),
      t('Thursday'),
      t('Friday'),
      t('Saturday')
    );
  }

  if ($day === null) {
    return $days;
  }
  else {
    return $days[$day % 7];
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
  }
  else {
    $sufix = 'pm';
    $hour -= 12;
  }
  // midnight and noon
  if ($hour <= 0) {
    $hour = 12;
  }
  return $hour . $sufix;
}

/**
 * Convert a day hour tuple into minutes.
 * @param $day 1-7
 * @param $hour 0-23
 * @return integer minutes.
 */
function station_minute_from_day_hour($day, $hour) {
  return (($day * 24) + $hour) * 60;
}

/**
 * Day of the week from minutes.
 * @param $minutes since midnight sunday
 * @return day of the week
 */
function station_day_from_minute($minute) {
  return (int) (($minute) / MINUTES_IN_DAY);
}

/**
 * Return an array of time information for the minutes since midnight sunday.
 * 'w' = Day of week (0-6)
 * 'G' = 24 hour
 * 'g' = 12 hour
 * 'H' = 24 hour, 0 padded
 * 'h' = 12 hour, 0 padded
 * 'i' = minutes, 0 padded
 * 'time' = h:i
 * 'minutes' = minutes since midnight sunday
 * 'a' = am/pm
 */
function station_time_from_minute($minutes) {
  $min = $minutes % 60;
  $day = (int) (($minutes) / MINUTES_IN_DAY);
  $hour24 = (int) (($minutes % MINUTES_IN_DAY) / 60);
  if (!($hour12 = $hour24 % 12)) {
    $hour12 = 12;
  }
  $i = str_pad($min, 2, '0', STR_PAD_LEFT);
  $h = str_pad($hour12, 2, '0', STR_PAD_LEFT);
  return array(
    'w' => $day,
    'G' => $hour24,
    'g' => $hour12,
    'H' => str_pad($hour24, 2, '0', STR_PAD_LEFT),
    'h' => $h,
    'i' => $i,
    'time' => ($min == 0) ? $hour12 : "$hour12:$i",
    'minutes' => $minutes,
    'a' => ($hour24 > 11) ? 'pm' : 'am',
  );
}

/**
 * Format a range of minutes into a day hour string, i.e. Sunday 11pm - Monday 1am
 * @param $start
 *   minutes
 * @param $finish
 *   minutes
 * @return formatted string
 */
function theme_station_dayhour_range($start, $finish) {
  $start = station_time_from_minute($start);
  $finish = station_time_from_minute($finish);
  $format_params = array(
    '@sday' => station_day_name($start['w']), '@shour' => $start['g'],
    '@stime' => $start['time'], '@sampm' => $start['a'],
    '@fday' => station_day_name($finish['w']), '@fhour' => $finish['g'],
    '@ftime' => $finish['time'], '@fampm' => $finish['a'],
  );

  // same day
  if ($start['w'] == $finish['w']) {
    // same am pm
    if ($start['a'] == $finish['a']) {
      $format = '@sday @stime-@ftime@sampm';
    }
    else {
      $format = '@sday @stime@sampm-@ftime@fampm';
    }
  }
  else {
    $format = '@sday @stime@sampm-@fday @ftime@fampm';
  }
  return t($format, $format_params);
}

/**
 * Format a range of minutes into a hour string, i.e. 1am-3pm
 * @param $start
 *   minutes
 * @param $finish
 *   minutes
 * @return formatted string
 */
function theme_station_hour_range($start, $finish) {
  $start = station_time_from_minute($start);
  $finish = station_time_from_minute($finish);
  $format_params = array(
    '@stime' => $start['time'], '@sampm' => $start['a'],
    '@ftime' => $finish['time'], '@fampm' => $finish['a'],
  );

  if ($start['a'] == $finish['a']) {
    $format = '@stime-@ftime@sampm';
  }
  else {
    $format = '@stime@sampm-@ftime@fampm';
  }
  return t($format, $format_params);
}
