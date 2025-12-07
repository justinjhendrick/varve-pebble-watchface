#include <pebble.h>

typedef struct ClaySettings {
  GColor color_background;
  GColor color_hour;
  GColor color_minute;
  GColor color_border;
  GColor color_date;
  uint8_t reserved[40]; // for later growth
} __attribute__((__packed__)) ClaySettings;

#define DEBUG (false)
#define BUFFER_LEN (40)

static Window* window;
static Layer* layer;
static GPoint points[16];
static ClaySettings settings;
static char buffer[BUFFER_LEN];

static void default_settings() {
  settings.color_background = GColorWhite;
  settings.color_hour = GColorBlack;
  settings.color_minute = GColorBlack;
  settings.color_border = GColorBlack;
  settings.color_date = GColorWhite;
}

static int fill_0() {
  points[0]  = (GPoint){.x=2, .y=0};
  points[1]  = (GPoint){.x=4, .y=2};
  points[2]  = (GPoint){.x=4, .y=6};
  points[3]  = (GPoint){.x=2, .y=8};
  points[4]  = (GPoint){.x=0, .y=6};
  points[5]  = (GPoint){.x=0, .y=2};
  points[6]  = (GPoint){.x=2, .y=0};
  return 7;
}

static int fill_1() {
  points[0]  = (GPoint){.x=1, .y=2};
  points[1]  = (GPoint){.x=3, .y=0};
  points[2]  = (GPoint){.x=3, .y=8};
  return 3;
}

static int fill_2() {
  points[0]  = (GPoint){.x=0, .y=2};
  points[1]  = (GPoint){.x=2, .y=0};
  points[2]  = (GPoint){.x=4, .y=2};
  points[3]  = (GPoint){.x=4, .y=4};
  points[4]  = (GPoint){.x=0, .y=8};
  points[5]  = (GPoint){.x=4, .y=8};
  return 6;
}

static int fill_3() {
  points[0]  = (GPoint){.x=0, .y=2};
  points[1]  = (GPoint){.x=2, .y=0};
  points[2]  = (GPoint){.x=4, .y=2};
  points[3]  = (GPoint){.x=2, .y=4};
  points[4]  = (GPoint){.x=4, .y=6};
  points[5]  = (GPoint){.x=2, .y=8};
  points[6]  = (GPoint){.x=0, .y=6};
  return 7;
}

static int fill_4() {
  points[0]  = (GPoint){.x=3, .y=0};
  points[1]  = (GPoint){.x=0, .y=3};
  points[2]  = (GPoint){.x=4, .y=3};
  points[3]  = (GPoint){.x=3, .y=2};
  points[4]  = (GPoint){.x=3, .y=8};
  return 5;
}

static int fill_5() {
  points[0]  = (GPoint){.x=4 , .y=0};
  points[1]  = (GPoint){.x=0 , .y=0};
  points[2]  = (GPoint){.x=0 , .y=3};
  points[3]  = (GPoint){.x=2 , .y=3};
  points[4]  = (GPoint){.x=4 , .y=4};
  points[5]  = (GPoint){.x=4 , .y=7};
  points[6]  = (GPoint){.x=2 , .y=8};
  points[7]  = (GPoint){.x=0 , .y=7};
  return 8;
}

static int fill_6() {
  points[0]  = (GPoint){.x=4 , .y=2};
  points[1]  = (GPoint){.x=2 , .y=0};
  points[2]  = (GPoint){.x=0 , .y=2};
  points[3]  = (GPoint){.x=0 , .y=6};
  points[4]  = (GPoint){.x=2 , .y=8};
  points[5]  = (GPoint){.x=4 , .y=6};
  points[6]  = (GPoint){.x=2 , .y=4};
  points[7]  = (GPoint){.x=0 , .y=6};
  return 8;
}

static int fill_7() {
  points[0]  = (GPoint){.x=0 , .y=0};
  points[1]  = (GPoint){.x=4 , .y=0};
  points[2]  = (GPoint){.x=0 , .y=8};
  return 3;
}


static int fill_8() {
  points[0]  = (GPoint){.x=2, .y=0};
  points[1]  = (GPoint){.x=4, .y=2};
  points[2]  = (GPoint){.x=0, .y=6};
  points[3]  = (GPoint){.x=2, .y=8};
  points[4]  = (GPoint){.x=4, .y=6};
  points[5]  = (GPoint){.x=0, .y=2};
  points[6]  = (GPoint){.x=2, .y=0};
  return 7;
}

static int fill_9() {
  points[0]  = (GPoint){.x=4, .y=3};
  points[1]  = (GPoint){.x=2, .y=4};
  points[2]  = (GPoint){.x=0, .y=3};
  points[3]  = (GPoint){.x=0, .y=1};
  points[4]  = (GPoint){.x=2, .y=0};
  points[5]  = (GPoint){.x=4, .y=1};
  points[6]  = (GPoint){.x=4, .y=4};
  points[7]  = (GPoint){.x=2, .y=8};
  return 8;
}

static void draw_digit(GContext* ctx, int digit, int w, GPoint tl_corner) {
  int num_points = 0;
  switch (digit) {
    case 0: num_points = fill_0(); break;
    case 1: num_points = fill_1(); break;
    case 2: num_points = fill_2(); break;
    case 3: num_points = fill_3(); break;
    case 4: num_points = fill_4(); break;
    case 5: num_points = fill_5(); break;
    case 6: num_points = fill_6(); break;
    case 7: num_points = fill_7(); break;
    case 8: num_points = fill_8(); break;
    case 9: num_points = fill_9(); break;
    default: break;
  }
  int b = w / 4;
  graphics_context_set_stroke_width(ctx, b);
  for (int i = 1; i < num_points; i++) {
    GPoint* prev = points + i - 1;
    GPoint* this = points + i;
    graphics_draw_line(
      ctx,
      (GPoint){.x=tl_corner.x + b * prev->x, .y=tl_corner.y + b * prev->y},
      (GPoint){.x=tl_corner.x + b * this->x, .y=tl_corner.y + b * this->y}
    );
  }
}

static void make_borders(GRect* out, GRect in, int xborder, int yborder) {
  int text_vertical_fudge = 4;
  int y = in.origin.y;
  out[0] = (GRect){
    .origin = (GPoint){.x = in.origin.x, .y = y - text_vertical_fudge},
    .size = (GSize){.w = in.size.w, .h = yborder}
  };
  y += yborder;
  int main_h = in.size.h - 2 * yborder;
  out[1] = (GRect){
    .origin = (GPoint){.x = in.origin.x + xborder, .y = y},
    .size = (GSize){.w = in.size.w - 2 * xborder, .h = main_h}
  };
  y += main_h;
  out[2] = (GRect){
    .origin = (GPoint){.x = in.origin.x, .y = y - text_vertical_fudge},
    .size = (GSize){.w = in.size.w, .h = yborder}
  };
}

static void format_day_of_week(struct tm* now, char* buf, int len) {
  strftime(buf, len, "%A", now);
}

static void format_day_and_month(struct tm* now, char* buf, int len) {
  strftime(buf, len, "%B %e", now);
}

static int get_hours(struct tm* now) {
  int hours = now->tm_hour;
  if (DEBUG) {
    hours = now->tm_sec % 24;
  }
  if (!clock_is_24h_style()) {
    hours = hours % 12;
    if (hours == 0) {
      hours = 12;
    }
  }
  return hours;
}
  
static int get_minutes(struct tm* now) {
  int minutes = now->tm_min;
  if (DEBUG) {
    minutes = now->tm_sec;
  }
  return minutes;
}

static void update_layer(Layer* layer, GContext* ctx) {
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);

  GRect boxes[3];
  make_borders(boxes, layer_get_bounds(layer), 0, 26);
  GRect upper_border = boxes[0];
  GRect bounds = boxes[1];
  GRect lower_border = boxes[2];

  window_set_background_color(window, settings.color_border);
  graphics_context_set_fill_color(ctx, settings.color_background);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24);

  format_day_of_week(now, buffer, BUFFER_LEN);
  graphics_draw_text(ctx, buffer, font, upper_border, GTextOverflowModeFill, GTextAlignmentCenter, NULL);
  format_day_and_month(now, buffer, BUFFER_LEN);
  graphics_draw_text(ctx, buffer, font, lower_border, GTextOverflowModeFill, GTextAlignmentCenter, NULL);

  graphics_context_set_stroke_color(ctx, settings.color_hour);
  int hour_width = bounds.size.w * 3 / 4;
  int hour_digit_gap = 7;
  if (bounds.size.w > 160) {
    hour_digit_gap = 8; // emery
  }
  int hour_digit_width = (hour_width - 4 * hour_digit_gap) / 2;
  int hour_digit_height = hour_digit_width * 2;
  int extra_hour_space = bounds.size.h - hour_digit_height;
  int hour_digit_y = bounds.origin.y + extra_hour_space / 2;
  int hours = get_hours(now);
  draw_digit(ctx, hours / 10, hour_digit_width, (GPoint){.x=bounds.origin.x + 1 * hour_digit_gap,                    .y=hour_digit_y});
  draw_digit(ctx, hours % 10, hour_digit_width, (GPoint){.x=bounds.origin.x + 3 * hour_digit_gap + hour_digit_width, .y=hour_digit_y});

  graphics_context_set_stroke_color(ctx, settings.color_minute);
  int minute_width = bounds.size.w - hour_width;
  int minute_digit_gap = 2;
  if (bounds.size.w > 160) {
    minute_digit_gap = 3; // emery
  }
  int minute_digit_width = (minute_width - 4 * minute_digit_gap) / 2;
  int minute_digit_height = minute_digit_width * 2;
  int r = bounds.origin.x + bounds.size.w;
  int minutes = get_minutes(now);
  int minute_y = hour_digit_y + minutes * (hour_digit_height - minute_digit_height) / 60;
  draw_digit(ctx, minutes % 10, minute_digit_width, (GPoint){.x=r - 1 * minute_digit_gap - 1 * minute_digit_width, .y=minute_y});
  draw_digit(ctx, minutes / 10, minute_digit_width, (GPoint){.x=r - 3 * minute_digit_gap - 2 * minute_digit_width, .y=minute_y});
}

static void window_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, settings.color_background);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, update_layer);
  layer_add_child(window_layer, layer);
}

static void window_unload(Window* window) {
  if (layer) layer_destroy(layer);
}

static void tick_handler(struct tm* now, TimeUnits unitchanged) {
  layer_mark_dirty(window_get_root_layer(window));
}

// TODO Configuration
static void inbox_received_handler(DictionaryIterator *iter, void *context) {}

static void init(void) {
  default_settings();
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  tick_timer_service_subscribe(DEBUG ? SECOND_UNIT : MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  if (window) window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}