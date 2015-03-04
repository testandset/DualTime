#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_layer, *s_day_layer, *s_dual_time_layer;


static void update_time(){
  
  //get tm structure
  time_t time_seconds = time(NULL);
  struct tm *tick_time = localtime(&time_seconds);  
  
  // create buffer
  static char time_format[5];
  static char time_buffer[] = "00:00"; // HH:MM
  static char dual_time_buffer[] = "00:00";  // HH:MM
  static char day_buffer[] = "wednesday"; // set to max, longest day name of the week.
  static char date_buffer[] = "00/00/00";  // dd/mm/yyyy
    
  // time format
  if(clock_is_24h_style() == true){
    strcpy(time_format, "%H:%M"); // 24 hour format
  } else {
    strcpy(time_format, "%I:%M"); // 12 hour format
  }
  
  // get time
  strftime(time_buffer, sizeof(time_buffer), time_format, tick_time);
  
  //get day of the week
  strftime(day_buffer, sizeof(day_buffer), "%A", tick_time);
  
  //get date
  strftime(date_buffer, sizeof(date_buffer), "%d/%m/%y", tick_time);
  
  // populate layers
  //text_layer_set_text(s_dual_time_layer, dual_time_buffer);
  text_layer_set_text(s_time_layer, time_buffer);
  text_layer_set_text(s_day_layer, day_buffer);
  text_layer_set_text(s_date_layer, date_buffer);
  
  //dual time
  time_seconds = time_seconds + 5 * 60 * 60 + 30 * 60; // +5:30 India time
  struct tm *dual_tick_time = localtime(&time_seconds);
  strftime(dual_time_buffer, sizeof(dual_time_buffer), time_format, dual_tick_time);
  text_layer_set_text(s_dual_time_layer, dual_time_buffer);
  
}

static void main_window_load(Window *window){
  
  // create time TextLayer
  s_time_layer = text_layer_create(GRect(0,62,144,50));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  // careate dual time layer
  s_dual_time_layer = text_layer_create(GRect(0,130,144,50));
  text_layer_set_background_color(s_dual_time_layer, GColorWhite);
  text_layer_set_text_color(s_dual_time_layer, GColorBlack);
  text_layer_set_font(s_dual_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_dual_time_layer, GTextAlignmentCenter);
  
  // create day textLayer
  s_day_layer = text_layer_create(GRect(0,0,144,30));
  text_layer_set_background_color(s_day_layer, GColorBlack);
  text_layer_set_text_color(s_day_layer, GColorWhite);
  text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentLeft);
  
  // create date textLayer
  s_date_layer = text_layer_create(GRect(0,30,144,30));
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);  
  
  // add it to window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_dual_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window){
  
  // destroy text layers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_dual_time_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_date_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_change){
  update_time();
}

static void init(){
  
  // create main window
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  
  // set handlers
  window_set_window_handlers(s_main_window, (WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload 
  });
  
  // register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // show window on watch
  window_stack_push(s_main_window, true);
  
  //update time
  update_time();
  
}

static void deinit(){
  // destroy window
  window_destroy(s_main_window);
}


int main(void){
  init();
  app_event_loop();
  deinit();
}