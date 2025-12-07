#include <pebble.h>

static Window* s_window;
static Layer* s_layer;
static GPoint POINTS[20];

typedef struct ClaySettings {
  GColor color_background;
  GColor color_hour;
  uint8_t reserved[40]; // for later growth
} __attribute__((__packed__)) ClaySettings;

ClaySettings settings;

static void default_settings() {
  settings.color_background = GColorWhite;
  settings.color_hour = GColorBlack;
}

static int fill_0(int w, int h) {
  POINTS[0]  = (GPoint){.x=w/2  , .y=h*0 };
  POINTS[1]  = (GPoint){.x=w*1  , .y=h/2 };
  POINTS[2]  = (GPoint){.x=w/2  , .y=h*1 };
  POINTS[3]  = (GPoint){.x=w*0  , .y=h/2 };
  POINTS[4]  = (GPoint){.x=w/2  , .y=h*0 };
  return 5;
}

static int fill_1(int w, int h) {
  POINTS[0]  = (GPoint){.x=w/4  , .y=h/4};
  POINTS[1]  = (GPoint){.x=w/2  , .y=h*0};
  POINTS[2]  = (GPoint){.x=w/2  , .y=h*1};
  return 3;
}

static int fill_2(int w, int h) {
  POINTS[0]  = (GPoint){.x=w/4    , .y=h/4};
  POINTS[1]  = (GPoint){.x=w/2    , .y=h*0};
  POINTS[2]  = (GPoint){.x=w*3/4  , .y=h/4};
  POINTS[3]  = (GPoint){.x=w/4    , .y=h*1};
  POINTS[4]  = (GPoint){.x=w*3/4  , .y=h*1};
  return 5;
}

static int fill_3(int w, int h) {
  POINTS[0]  = (GPoint){.x=w/4    , .y=h/4};
  POINTS[1]  = (GPoint){.x=w/2    , .y=h*0};
  POINTS[2]  = (GPoint){.x=w*3/4  , .y=h/4};
  POINTS[3]  = (GPoint){.x=w/2    , .y=h/2};
  POINTS[4]  = (GPoint){.x=w*3/4  , .y=h*3/4};
  POINTS[5]  = (GPoint){.x=w/2    , .y=h*1};
  POINTS[6]  = (GPoint){.x=w/4    , .y=h*3/4};
  return 7;
}

static int fill_4(int w, int h) {
  POINTS[0]  = (GPoint){.x=w/2    , .y=h*0};
  POINTS[1]  = (GPoint){.x=w*0    , .y=h/2};
  POINTS[2]  = (GPoint){.x=w*5/8  , .y=h/2};
  POINTS[3]  = (GPoint){.x=w/2    , .y=h*3/8};
  POINTS[4]  = (GPoint){.x=w/2    , .y=h*1};
  return 5;
}

static int fill_5(int w, int h) {
  int bw = w / 8;
  int bh = h / 8;
  POINTS[0]  = (GPoint){.x=6*bw  , .y=0*bh};
  POINTS[1]  = (GPoint){.x=2*bw  , .y=0*bh};
  POINTS[2]  = (GPoint){.x=2*bw  , .y=3*bh};
  POINTS[3]  = (GPoint){.x=4*bw  , .y=3*bh};
  POINTS[4]  = (GPoint){.x=6*bw  , .y=5*bh};
  POINTS[5]  = (GPoint){.x=6*bw  , .y=6*bh};
  POINTS[6]  = (GPoint){.x=4*bw  , .y=8*bh};
  POINTS[7]  = (GPoint){.x=2*bw  , .y=6*bh};
  return 8;
}

static int fill_6(int w, int h) {
  int bw = w / 8;
  int bh = h / 8;
  POINTS[0]  = (GPoint){.x=6*bw  , .y=2*bh};
  POINTS[1]  = (GPoint){.x=2*bw  , .y=6*bh};
  POINTS[2]  = (GPoint){.x=4*bw  , .y=8*bh};
  POINTS[3]  = (GPoint){.x=6*bw  , .y=6*bh};
  POINTS[4]  = (GPoint){.x=4*bw  , .y=4*bh};
  return 5;
}

static int fill_7(int w, int h) {
  int bw = w / 8;
  int bh = h / 8;
  POINTS[0]  = (GPoint){.x=w  , .y=h};
  POINTS[1]  = (GPoint){.x=w  , .y=h};
  POINTS[2]  = (GPoint){.x=w  , .y=h};
  return 3;
}


static int fill_8(int w, int h) {
  POINTS[0]  = (GPoint){.x=w/2  , .y=0};
  POINTS[1]  = (GPoint){.x=w    , .y=h/4};
  POINTS[2]  = (GPoint){.x=0    , .y=3*h/4};
  POINTS[3]  = (GPoint){.x=w/2  , .y=h};
  POINTS[4]  = (GPoint){.x=w    , .y=3*h/4};
  POINTS[5]  = (GPoint){.x=0    , .y=h/4};
  POINTS[6]  = (GPoint){.x=w/2  , .y=0};
  return 7;
}

static int fill_9(int w, int h) {
  POINTS[0]  = (GPoint){.x=w  , .y=h};
  POINTS[1]  = (GPoint){.x=w  , .y=h};
  POINTS[2]  = (GPoint){.x=w  , .y=h};
  POINTS[3]  = (GPoint){.x=w  , .y=h};
  POINTS[4]  = (GPoint){.x=w  , .y=h};
  return 5;
}

static void draw_digit(GContext* ctx, int digit, GRect bbox, int stroke_width) {
  int x = bbox.origin.x + stroke_width / 2;
  int y = bbox.origin.y + stroke_width / 2;
  int w = bbox.size.w - stroke_width;
  int h = bbox.size.h - stroke_width;
  int s = 0;
  switch (digit) {
    case 0: s = fill_0(w, h); break;
    case 1: s = fill_1(w, h); break;
    case 2: s = fill_2(w, h); break;
    case 3: s = fill_3(w, h); break;
    case 4: s = fill_4(w, h); break;
    case 5: s = fill_5(w, h); break;
    case 6: s = fill_6(w, h); break;
    case 7: s = fill_7(w, h); break;
    case 8: s = fill_8(w, h); break;
    case 9: s = fill_9(w, h); break;
    default: break;
  }
  graphics_context_set_stroke_width(ctx, stroke_width);
  for (int i = 1; i < s; i++) {
    GPoint* prev = POINTS + i - 1;
    GPoint* this = POINTS + i;
    graphics_draw_line(
      ctx,
      (GPoint){.x=prev->x + x, .y=prev->y + y},
      (GPoint){.x=this->x + x, .y=this->y + y}
    );
  }
}

static void update_layer(Layer* layer, GContext* ctx) {
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);
  GRect bounds = layer_get_bounds(layer);
  window_set_background_color(s_window, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  draw_digit(ctx, now->tm_sec % 10, bounds, 10);
}

static void window_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_window, settings.color_background);
  s_layer = layer_create(bounds);
  layer_set_update_proc(s_layer, update_layer);
  layer_add_child(window_layer, s_layer);
}

static void window_unload(Window* window) {
  layer_destroy(s_layer);
}

static void tick_handler(struct tm* now, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(s_window));
}

// TODO Configuration
static void inbox_received_handler(DictionaryIterator *iter, void *context) {}

static void init(void) {
  default_settings();
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
  if (s_window) window_destroy(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}