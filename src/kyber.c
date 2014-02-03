#include <pebble.h>

#include "libs/pebble-assist/pebble-assist.h"
#include "timestr.h"

typedef struct {
  int start_hour;
  int start_minute;
  int duration;
  char* message_1;
  char* message_2;
  char* message_3;
} TimeMessage;

void handle_init();
void handle_deinit();
void handle_tick(struct tm *tick_time, TimeUnits units_changed);
void update_time();

Window* window;
TextLayer* layer_hour;
TextLayer* layer_minutes_1;
TextLayer* layer_minutes_2;

char hour_str[10];
char minute_str_1[10];
char minute_str_2[10];

/**
 * To change whe the messages appear, you'll only need to modify the lines below.
 * The first number is the hour (in 24 hour time) that the message begins.
 * The second number is the minute past the hour that the message begins.
 * The third number is the duration of the message in minutes.
 * N.B. If you want messages to go through midnight, you'll need to have two
 *      entries, one that starts at midnight and one that finishes at midnight.
 * The first string is the top (bold) part of the message.
 * The second string is the second part of the message.
 * N.B. You must provide both parts of the message, if one of them should be 
 *      blank then you should use an empty string.
 *
 * VERY IMPORTANT
 *
 * If you change the number of messages, you must update the NUM_MESSAGES
 * value below as well to match.
 **/

#define NUM_MESSAGES 10

TimeMessage messages[NUM_MESSAGES] = {
  {  0,  0, 360, "sleep", "", "" },
  {  6,  0,  30, "wake", "up", "" },
  {  7, 30,  15, "start", "work", "" },
  {  8, 30,  15, "break", "fast", "" },
  { 10,  0,  15, "tea", "break", "" },
  { 13,  0,  40, "lunch", "", "" },
  { 15,  0,  15, "tea", "break", "" },
  { 16, 40,  10, "nearly", "home", "time" },
  { 17,  0,  10, "home", "time", "" },
  { 23,  0,  60, "sleep", "", "" }
};

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

void handle_init(void) {

  window = window_create();
  window_stack_push(window, true);

  window_set_background_color(window, GColorBlack);

  layer_hour = text_layer_create(GRect(2, 18, 144, 50));
  text_layer_set_system_font(layer_hour, FONT_KEY_BITHAM_42_BOLD);
  text_layer_set_text_color(layer_hour, GColorWhite);
  text_layer_set_background_color(layer_hour, GColorClear);
  text_layer_set_text_alignment(layer_hour, GTextAlignmentLeft);
  text_layer_add_to_window(layer_hour, window);

  layer_minutes_1 = text_layer_create(GRect(2, 60, 144, 50));
  text_layer_set_system_font(layer_minutes_1, FONT_KEY_BITHAM_42_LIGHT);
  text_layer_set_text_color(layer_minutes_1, GColorWhite);
  text_layer_set_background_color(layer_minutes_1, GColorClear);
  text_layer_set_text_alignment(layer_minutes_1, GTextAlignmentLeft);
  text_layer_add_to_window(layer_minutes_1, window);

  layer_minutes_2 = text_layer_create(GRect(2, 94, 144, 50));
  text_layer_set_system_font(layer_minutes_2, FONT_KEY_BITHAM_42_LIGHT);
  text_layer_set_text_color(layer_minutes_2, GColorWhite);
  text_layer_set_background_color(layer_minutes_2, GColorClear);
  text_layer_set_text_alignment(layer_minutes_2, GTextAlignmentLeft);
  text_layer_add_to_window(layer_minutes_2, window);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);

  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  update_time(current_time);
}

void handle_deinit() {
  text_layer_destroy(layer_minutes_2);
  text_layer_destroy(layer_minutes_1);
  text_layer_destroy(layer_hour);
  window_destroy(window);
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

TimeMessage* message_from_time(struct tm *tick_time) {
  for (int m = 0; m < NUM_MESSAGES; m += 1) {
    TimeMessage* msg = &messages[m];
    if (tick_time->tm_hour >= msg->start_hour) {
      int min_into_period = ((tick_time->tm_hour - msg->start_hour) * 60 + ((tick_time->tm_min < msg->start_minute ? 60 : 0) + tick_time->tm_min - msg->start_minute));
      if (min_into_period < msg->duration) {
        return msg;
      }
    }
  }
  return NULL;
}

void update_time(struct tm* tick_time) {
  TimeMessage* msg = message_from_time(tick_time);
  if (msg == NULL) {
    strcpy(hour_str, stringify_hour(tick_time->tm_hour));
    strcpy(minute_str_1, stringify_minute_1(tick_time->tm_min));
    strcpy(minute_str_2, stringify_minute_2(tick_time->tm_min));
  }
  else {
    strcpy(hour_str, msg->message_1);
    strcpy(minute_str_1, msg->message_2);
    strcpy(minute_str_2, msg->message_3);
  }
  text_layer_set_text(layer_hour, hour_str);
  text_layer_set_text(layer_minutes_1, minute_str_1);
  text_layer_set_text(layer_minutes_2, minute_str_2);
}