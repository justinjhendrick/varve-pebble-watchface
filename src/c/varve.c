#include <pebble.h>

typedef struct ClaySettings {
  GColor color_background;
  GColor color_hour;
  GColor color_minute;
  GColor color_border;
  GColor color_date;
  uint8_t reserved[40]; // for later growth
} __attribute__((__packed__)) ClaySettings;

#define SETTINGS_VERSION_KEY (1)
#define SETTINGS_KEY (2)

#define DEBUG_TIME (false)
#define DEBUG_BBOX (false)
#define FORCE_12H (false)
#define BUFFER_LEN (40)

#if PBL_DISPLAY_WIDTH >= 200
  #define DATE_FONT_KEY (RESOURCE_ID_TOMORROW_30)
  #define DATE_HEIGHT (40)
  #define HOUR_STROKE (8)
  #define HOUR_GAP (10)
  #define MINUTE_STROKE (4)
  #define MINUTE_GAP (3)
#else
  #define DATE_FONT_KEY (RESOURCE_ID_TOMORROW_22)
  #define DATE_HEIGHT (30)
  #define HOUR_STROKE (6)
  #define HOUR_GAP (7)
  #define MINUTE_STROKE (3)
  #define MINUTE_GAP (2)
#endif

static Window* window;
static Layer* layer;
static GPoint points[16];
static ClaySettings settings;
static char buffer[BUFFER_LEN];
static GFont s_font = NULL;

static void default_settings() {
  settings.color_background = COLOR_FALLBACK(GColorPictonBlue, GColorWhite);
  settings.color_hour = GColorBlack;
  settings.color_minute = GColorBlack;
  settings.color_border = GColorBlack;
  settings.color_date = COLOR_FALLBACK(GColorRajah, GColorWhite);
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
  points[1] = (GPoint){.x=0, .y=4};
  points[2] = (GPoint){.x=4, .y=4};
  points[3] = (GPoint){.x=3, .y=3};
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

static void draw_digit(GContext* ctx, int c, GRect bbox) {
  int num_points = 0;
  switch (c) {
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
    default: return;
  }
  static const int X = 4;
  static const int Y = 8;
  int x_scale = bbox.size.w / X;
  int y_scale = bbox.size.h / Y;
  for (int i = 1; i < num_points; i++) {
    GPoint* prev = points + i - 1;
    GPoint* this = points + i;
    graphics_draw_line(
      ctx,
      (GPoint){.x=bbox.origin.x + prev->x * x_scale, .y=bbox.origin.y + prev->y * y_scale},
      (GPoint){.x=bbox.origin.x + this->x * x_scale, .y=bbox.origin.y + this->y * y_scale}
    );
  }
  if (DEBUG_BBOX) {
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_rect(ctx, bbox);
  }
}

static void draw_digits(GContext* ctx, int c1, int c2, GRect bbox, int stroke, int gap) {
  // G c GG c G
  int char_width = (bbox.size.w - 4 * gap) / 2;
  GRect left;
  left.origin.x = bbox.origin.x + gap;
  left.origin.y = bbox.origin.y;
  left.size.w = char_width;
  left.size.h = bbox.size.h;
  graphics_context_set_stroke_width(ctx, stroke);
  draw_digit(ctx, c1, left);

  GRect right;
  right.origin.x = left.origin.x + char_width + 2 * gap;
  right.origin.y = left.origin.y;
  right.size.w = char_width;
  right.size.h = bbox.size.h;
  graphics_context_set_stroke_width(ctx, stroke);
  draw_digit(ctx, c2, right);
}

typedef struct Areas {
  GRect main;
  GRect lower_border;
} Areas;

static Areas make_borders(GRect in, int border_height) {
  Areas a;
  int top = 14;
  int bot = 10;
  int left = 4;
  int right = 4;

  a.main = (GRect){
    .origin = (GPoint){.x = in.origin.x + left, .y = in.origin.y + top},
    .size = (GSize){.w = in.size.w - left - right, .h = in.size.h - border_height - top - bot}
  };
  a.lower_border = (GRect){
    .origin = (GPoint){.x = in.origin.x, .y = in.size.h - border_height},
    .size = (GSize){.w = in.size.w, .h = border_height}
  };
  return a;
}

static bool is_12h() {
  return FORCE_12H || !clock_is_24h_style();
}

static void format_date(struct tm* now, char* buf, int len) {
  if (DEBUG_TIME) {
    now->tm_mday = (now->tm_sec % 31) + 1;
    now->tm_mon = (now->tm_sec % 12);
    now->tm_wday = (now->tm_sec % 7);
  }
  strftime(buf, len, "%a %d %b", now);
}

static int get_hours(struct tm* now) {
#ifdef HOUR_OVERRIDE
  return HOUR_OVERRIDE;
#else
  int hours = now->tm_hour;
  if (DEBUG_TIME) {
    hours = now->tm_sec % 24;
  }
  if (is_12h()) {
    hours = hours % 12;
    if (hours == 0) {
      hours = 12;
    }
  }
  return hours;
#endif
}

static int get_minutes(struct tm* now) {
#ifdef MINUTE_OVERRIDE
  return MINUTE_OVERRIDE;
#else
  int minutes = now->tm_min;
  if (DEBUG_TIME) {
    minutes = now->tm_sec;
  }
  return minutes;
#endif
}

static void draw_text_shifted(GContext* ctx, const char* buffer, GRect bbox, GFont font, int shift_up) {
  GRect fixed_bbox = GRect(bbox.origin.x, bbox.origin.y - shift_up, bbox.size.w, bbox.size.h);
  graphics_draw_text(ctx, buffer, font, fixed_bbox, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

static void update_layer(Layer* layer, GContext* ctx) {
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);

  GRect obstructed = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, settings.color_background);
  graphics_fill_rect(ctx, obstructed, 0, GCornerNone);

  GRect unobstructed = layer_get_unobstructed_bounds(layer);
  bool timeline_quick_view = (unobstructed.size.h < obstructed.size.h - 1);

  int lower_border_height = DATE_HEIGHT;
  if (timeline_quick_view) {
    lower_border_height = 0;
  }
  Areas areas = make_borders(unobstructed, lower_border_height);

  if (!timeline_quick_view) {
    graphics_context_set_fill_color(ctx, settings.color_border);
    graphics_fill_rect(ctx, areas.lower_border, 0, GCornerNone);
    graphics_context_set_text_color(ctx, settings.color_date);
    format_date(now, buffer, BUFFER_LEN);
    draw_text_shifted(ctx, buffer, areas.lower_border, s_font, 0);
  }

  // hours
  GRect hour_bbox;
  hour_bbox.origin.x = areas.main.origin.x;
  hour_bbox.origin.y = areas.main.origin.y;
  hour_bbox.size.w = areas.main.size.w * 3 / 4;
  hour_bbox.size.h = areas.main.size.h;
  int hours = get_hours(now);
  graphics_context_set_stroke_color(ctx, settings.color_hour);
  draw_digits(
    ctx,
    hours / 10,
    hours % 10,
    hour_bbox,
    HOUR_STROKE,
    HOUR_GAP
  );

  // minutes
  GRect minute_bbox;
  int minutes = get_minutes(now);
  minute_bbox.size.w = areas.main.size.w - hour_bbox.size.w;
  minute_bbox.size.h = hour_bbox.size.h / 4;
  minute_bbox.origin.x = hour_bbox.origin.x + hour_bbox.size.w;
  minute_bbox.origin.y = hour_bbox.origin.y + minutes * (hour_bbox.size.h - minute_bbox.size.h) / 60;
  graphics_context_set_stroke_width(ctx, 3);
  graphics_context_set_stroke_color(ctx, settings.color_minute);
  draw_digits(
    ctx,
    minutes / 10,
    minutes % 10,
    minute_bbox,
    MINUTE_STROKE,
    MINUTE_GAP
  );
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
  if (layer) layer_mark_dirty(layer);
}

static void load_settings() {
  default_settings();
  // If we need a backwards incompatible version of settings, check SETTINGS_VERSION_KEY and migrate
  persist_read_data(SETTINGS_KEY, &settings, sizeof(ClaySettings));
}

static void save_settings() {
  persist_write_int(SETTINGS_VERSION_KEY, 1);
  persist_write_data(SETTINGS_KEY, &settings, sizeof(ClaySettings));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *t;
  if ((t = dict_find(iter, MESSAGE_KEY_color_background ))) { settings.color_background = GColorFromHEX(t->value->int32); }
  if ((t = dict_find(iter, MESSAGE_KEY_color_hour       ))) { settings.color_hour       = GColorFromHEX(t->value->int32); }
  if ((t = dict_find(iter, MESSAGE_KEY_color_minute     ))) { settings.color_minute     = GColorFromHEX(t->value->int32); }
  if ((t = dict_find(iter, MESSAGE_KEY_color_border     ))) { settings.color_border     = GColorFromHEX(t->value->int32); }
  if ((t = dict_find(iter, MESSAGE_KEY_color_date       ))) { settings.color_date       = GColorFromHEX(t->value->int32); }
  save_settings();
  // Update the display based on new settings
  if (layer) { layer_mark_dirty(layer); }
}

static void init(void) {
  s_font = fonts_load_custom_font(resource_get_handle(DATE_FONT_KEY));
  load_settings();

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(1024, 64);

  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  tick_timer_service_subscribe(DEBUG_TIME ? SECOND_UNIT : MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  if (window) { window_destroy(window); }
  if (s_font) { fonts_unload_custom_font(s_font); }
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
