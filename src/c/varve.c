#include <pebble.h>

typedef struct ClaySettings {
  GColor color_background;
  GColor color_hour;
  GColor color_minute;
  GColor color_ampm;
  GColor color_border;
  GColor color_date;
  uint8_t reserved[40]; // for later growth
} __attribute__((__packed__)) ClaySettings;

#define DEBUG (false)
#define FORCE_12H (false)
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
  settings.color_ampm = GColorBlack;
  settings.color_border = GColorBlack;
  settings.color_date = GColorWhite;
}

static int fill_0() {
  points[0] = (GPoint){.x=2, .y=0};
  points[1] = (GPoint){.x=4, .y=2};
  points[2] = (GPoint){.x=4, .y=6};
  points[3] = (GPoint){.x=2, .y=8};
  points[4] = (GPoint){.x=0, .y=6};
  points[5] = (GPoint){.x=0, .y=2};
  points[6] = (GPoint){.x=2, .y=0};
  return 7;
}

static int fill_1() {
  points[0] = (GPoint){.x=1, .y=2};
  points[1] = (GPoint){.x=3, .y=0};
  points[2] = (GPoint){.x=3, .y=8};
  return 3;
}

static int fill_2() {
  points[0] = (GPoint){.x=0, .y=2};
  points[1] = (GPoint){.x=2, .y=0};
  points[2] = (GPoint){.x=4, .y=2};
  points[3] = (GPoint){.x=4, .y=4};
  points[4] = (GPoint){.x=0, .y=8};
  points[5] = (GPoint){.x=4, .y=8};
  return 6;
}

static int fill_3() {
  points[0] = (GPoint){.x=0, .y=2};
  points[1] = (GPoint){.x=2, .y=0};
  points[2] = (GPoint){.x=4, .y=2};
  points[3] = (GPoint){.x=2, .y=4};
  points[4] = (GPoint){.x=4, .y=6};
  points[5] = (GPoint){.x=2, .y=8};
  points[6] = (GPoint){.x=0, .y=6};
  return 7;
}

static int fill_4() {
  points[0] = (GPoint){.x=3, .y=0};
  points[1] = (GPoint){.x=0, .y=3};
  points[2] = (GPoint){.x=4, .y=3};
  points[3] = (GPoint){.x=3, .y=2};
  points[4] = (GPoint){.x=3, .y=8};
  return 5;
}

static int fill_5() {
  points[0] = (GPoint){.x=4 , .y=0};
  points[1] = (GPoint){.x=0 , .y=0};
  points[2] = (GPoint){.x=0 , .y=3};
  points[3] = (GPoint){.x=2 , .y=3};
  points[4] = (GPoint){.x=4 , .y=4};
  points[5] = (GPoint){.x=4 , .y=7};
  points[6] = (GPoint){.x=2 , .y=8};
  points[7] = (GPoint){.x=0 , .y=7};
  return 8;
}

static int fill_6() {
  points[0] = (GPoint){.x=4 , .y=2};
  points[1] = (GPoint){.x=2 , .y=0};
  points[2] = (GPoint){.x=0 , .y=2};
  points[3] = (GPoint){.x=0 , .y=6};
  points[4] = (GPoint){.x=2 , .y=8};
  points[5] = (GPoint){.x=4 , .y=6};
  points[6] = (GPoint){.x=2 , .y=4};
  points[7] = (GPoint){.x=0 , .y=6};
  return 8;
}

static int fill_7() {
  points[0] = (GPoint){.x=0 , .y=0};
  points[1] = (GPoint){.x=4 , .y=0};
  points[2] = (GPoint){.x=0 , .y=8};
  return 3;
}


static int fill_8() {
  points[0] = (GPoint){.x=2, .y=0};
  points[1] = (GPoint){.x=4, .y=2};
  points[2] = (GPoint){.x=0, .y=6};
  points[3] = (GPoint){.x=2, .y=8};
  points[4] = (GPoint){.x=4, .y=6};
  points[5] = (GPoint){.x=0, .y=2};
  points[6] = (GPoint){.x=2, .y=0};
  return 7;
}

static int fill_9() {
  points[0] = (GPoint){.x=4, .y=3};
  points[1] = (GPoint){.x=2, .y=4};
  points[2] = (GPoint){.x=0, .y=3};
  points[3] = (GPoint){.x=0, .y=1};
  points[4] = (GPoint){.x=2, .y=0};
  points[5] = (GPoint){.x=4, .y=1};
  points[6] = (GPoint){.x=4, .y=4};
  points[7] = (GPoint){.x=2, .y=8};
  return 8;
}

static int fill_A() {
  points[0] = (GPoint){.x=0, .y=8};
  points[1] = (GPoint){.x=0, .y=4};
  points[2] = (GPoint){.x=2, .y=0};
  points[3] = (GPoint){.x=4, .y=4};
  points[4] = (GPoint){.x=0, .y=4};
  points[5] = (GPoint){.x=4, .y=4};
  points[6] = (GPoint){.x=4, .y=8};
  return 7;
}

static int fill_P() {
  points[0] = (GPoint){.x=0, .y=3};
  points[1] = (GPoint){.x=2, .y=4};
  points[2] = (GPoint){.x=4, .y=3};
  points[3] = (GPoint){.x=4, .y=1};
  points[4] = (GPoint){.x=2, .y=0};
  points[5] = (GPoint){.x=0, .y=1};
  points[6] = (GPoint){.x=0, .y=8};
  return 7;
}

static int fill_M() {
  points[0] = (GPoint){.x=0, .y=8};
  points[1] = (GPoint){.x=0, .y=0};
  points[2] = (GPoint){.x=2, .y=4};
  points[3] = (GPoint){.x=4, .y=0};
  points[4] = (GPoint){.x=4, .y=8};
  return 5;
}

static void draw_char(GContext* ctx, char c, int block_size, GPoint tl_corner) {
  int num_points = 0;
  switch (c) {
    case '0': num_points = fill_0(); break;
    case '1': num_points = fill_1(); break;
    case '2': num_points = fill_2(); break;
    case '3': num_points = fill_3(); break;
    case '4': num_points = fill_4(); break;
    case '5': num_points = fill_5(); break;
    case '6': num_points = fill_6(); break;
    case '7': num_points = fill_7(); break;
    case '8': num_points = fill_8(); break;
    case '9': num_points = fill_9(); break;
    case 'A': num_points = fill_A(); break;
    case 'P': num_points = fill_P(); break;
    case 'M': num_points = fill_M(); break;
    default: break;
  }
  int sw = block_size - 2;
  if (sw < 1) sw = 1;
  graphics_context_set_stroke_width(ctx, sw);
  for (int i = 1; i < num_points; i++) {
    GPoint* prev = points + i - 1;
    GPoint* this = points + i;
    graphics_draw_line(
      ctx,
      (GPoint){.x=tl_corner.x + prev->x * block_size, .y=tl_corner.y + prev->y * block_size},
      (GPoint){.x=tl_corner.x + this->x * block_size, .y=tl_corner.y + this->y * block_size}
    );
  }
}

typedef struct Dimensions {
  int actual_width;
  int block_size;
  int xborder;
  int gap;
  int char_width;
  int height;
} Dimensions;

static Dimensions get_str_dimensions(int combined_width, bool include_xborder) {
  Dimensions d;
  int total_blocks = (9 + include_xborder);
  d.block_size = combined_width / total_blocks;
  d.actual_width = combined_width - (combined_width % total_blocks);
  d.xborder = include_xborder ? (d.block_size / 2) : 0;
  d.gap = d.block_size;
  d.char_width = 4 * d.block_size;
  d.height = 2 * d.char_width;
  return d;
}

static void draw_str(GContext* ctx, char c1, char c2, Dimensions d, GPoint tl_corner) {
  draw_char(ctx, c1, d.block_size, (GPoint){.x=tl_corner.x + d.xborder,                        .y=tl_corner.y});
  draw_char(ctx, c2, d.block_size, (GPoint){.x=tl_corner.x + d.xborder + d.char_width + d.gap, .y=tl_corner.y});
}

static void make_borders(GRect* out, GRect in, int xborder, int yborder) {
  int text_yfudge = 4;
  int text_xfudge = 2;
  int y = in.origin.y;
  out[0] = (GRect){
    .origin = (GPoint){.x = in.origin.x + text_xfudge, .y = y - text_yfudge},
    .size = (GSize){.w = in.size.w / 2, .h = yborder}
  };
  out[1] = (GRect){
    .origin = (GPoint){.x = in.origin.x + in.size.w / 2, .y = y - text_yfudge},
    .size = (GSize){.w = in.size.w / 2 - text_xfudge, .h = yborder}
  };
  y += yborder;
  int main_h = in.size.h - 2 * yborder;
  out[2] = (GRect){
    .origin = (GPoint){.x = in.origin.x + xborder, .y = y},
    .size = (GSize){.w = in.size.w - 2 * xborder, .h = main_h}
  };
  y += main_h;
  out[3] = (GRect){
    .origin = (GPoint){.x = in.origin.x, .y = y - text_yfudge},
    .size = (GSize){.w = in.size.w, .h = yborder}
  };
}

static bool is_12h() {
  return FORCE_12H || !clock_is_24h_style();
}

static void format_day_of_week(struct tm* now, char* buf, int len) {
  strftime(buf, len, "%A", now);
}

static void format_am_pm(struct tm* now, char* buf, int len) {
  strftime(buf, len, "%p", now);
}

static void format_day_and_month(struct tm* now, char* buf, int len) {
  strftime(buf, len, "%B %e", now);
}

static int get_hours(struct tm* now) {
  int hours = now->tm_hour;
  if (DEBUG) {
    hours = now->tm_sec % 24;
  }
  if (is_12h()) {
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

static char to_char(int digit) {
  return digit + 0x30;
}

static void update_layer(Layer* layer, GContext* ctx) {
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);

  GRect boxes[4];
  make_borders(boxes, layer_get_bounds(layer), 0, 26);
  GRect lupper_border = boxes[0];
  GRect rupper_border = boxes[1];
  GRect main_area = boxes[2];
  GRect lower_border = boxes[3];

  window_set_background_color(window, settings.color_border);
  graphics_context_set_fill_color(ctx, settings.color_background);
  graphics_fill_rect(ctx, main_area, 0, GCornerNone);
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24);

  format_day_of_week(now, buffer, BUFFER_LEN);
  graphics_draw_text(ctx, buffer, font, lupper_border, GTextOverflowModeFill, GTextAlignmentLeft, NULL);
  format_am_pm(now, buffer, BUFFER_LEN);
  graphics_draw_text(ctx, buffer, font, rupper_border, GTextOverflowModeFill, GTextAlignmentRight, NULL);
  format_day_and_month(now, buffer, BUFFER_LEN);
  graphics_draw_text(ctx, buffer, font, lower_border, GTextOverflowModeFill, GTextAlignmentCenter, NULL);

  // hours
  int desired_hour_width = main_area.size.w * 3 / 4;
  Dimensions hour_dims = get_str_dimensions(desired_hour_width, true);
  int extra_hour_space = main_area.size.h - hour_dims.height;
  int hour_digit_y = main_area.origin.y + extra_hour_space / 2;
  int hours = get_hours(now);
  graphics_context_set_stroke_color(ctx, settings.color_hour);
  draw_str(
    ctx,
    is_12h() && hours < 10 ? '\0' : to_char(hours / 10),
    to_char(hours % 10),
    hour_dims,
    (GPoint){.x=main_area.origin.x, .y=hour_digit_y}
  );

  // minutes & ampm
  int minute_width = main_area.size.w - hour_dims.actual_width;
  Dimensions minute_dims = get_str_dimensions(minute_width, true);
  //if (is_12h()) {
  //  Dimensions ampm_dims = get_str_dimensions(minute_width / 2, true);
  //  char ap = now->tm_hour < 12 ? 'A' : 'P';
  //  graphics_context_set_stroke_color(ctx, settings.color_ampm);
  //  draw_str(ctx, ap, 'M', ampm_dims, (GPoint){.x=main_area.origin.x + main_area.size.w - ampm_dims.actual_width, .y=lower_border.origin.y - ampm_dims.height});
  //}
  int minutes = get_minutes(now);
  int minute_y = hour_digit_y + minutes * (hour_dims.height - minute_dims.height) / 60;
  graphics_context_set_stroke_color(ctx, settings.color_minute);
  draw_str(ctx, to_char(minutes / 10), to_char(minutes % 10), minute_dims, (GPoint){.x=main_area.origin.x + hour_dims.actual_width, .y=minute_y});
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