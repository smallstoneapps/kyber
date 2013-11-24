#include <pebble.h>
#include "timestr.h"

static const char* NUMBERS[] = { "", "one", "two", "three", "four", "five", "six",
  "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "four",
  "fifteen", "sixteen", "seven", "eight", "nine"
};

static const char* TEEN = "teen";

static const char* TENS[] = { "", "", "twenty", "thirty", "forty", "fifty" };

char* stringify_hour(int hour) {
  const char* hour_str = NUMBERS[hour % 12 == 0 ? 12 : hour % 12];
  static char tmp[sizeof(hour_str)];
  strcpy(tmp, hour_str);
  return tmp;
}

char* stringify_minute_1(int minute) {
  static char minute_str[10];
  if (minute == 0) {
    strcpy(minute_str, "o'clock");
  }
  else if (minute < 20) {
    strcpy(minute_str, NUMBERS[minute]);
  }
  else {
    strcpy(minute_str, TENS[minute / 10]);
  }
  return minute_str;
}

char* stringify_minute_2(int minute) {
  static char minute_str[10];
  if (minute < 13 || minute == 15 || minute == 13 || minute == 16) {
    strcpy(minute_str, "");
  }
  else if (minute < 20) {
    strcpy(minute_str, TEEN);
  }
  else {
    strcpy(minute_str, NUMBERS[minute % 10]);
  }
  return minute_str;
}