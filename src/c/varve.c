#include <pebble.h>

typedef struct ClaySettings {
  GColor color_background;
  GColor color_hour;
  uint8_t reserved[40]; // for later growth
} __attribute__((__packed__)) ClaySettings;

static Window* window;
static Layer* layer;
static GPoint points[20];
static ClaySettings settings;

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

static void draw_digit(GContext* ctx, int digit, GPoint tl_corner, int w) {
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
  int b = w / 4;  // block size
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

static void update_layer(Layer* layer, GContext* ctx) {
  time_t temp = time(NULL);
  struct tm* now = localtime(&temp);
  GRect bounds = layer_get_bounds(layer);
  window_set_background_color(window, settings.color_background);
  graphics_context_set_stroke_color(ctx, settings.color_hour);
  int w = bounds.size.w / 7;
  int gap = bounds.size.w / 20;
  int digit = 0;
  int h = w * 2;
  for (int iy = 0; iy < 2; iy++) {
    for (int ix = 0; ix < 5; ix++) {
      int x = gap + ix * (gap + w);
      int y = gap + iy * (gap + h);
      draw_digit(ctx, digit, (GPoint){.x=x, .y=y}, w);
      digit++;
    }
  }
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
static void default_settings() {
  settings.color_background = GColorWhite;
  settings.color_hour = GColorBlack;
}
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
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
  if (window) window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}