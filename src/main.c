#include "pebble.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 3

static Window *s_main_window;
static Window *topic_window;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuLayer *topics_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuSection topics_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];
static SimpleMenuItem s_second_menu_items[NUM_SECOND_MENU_ITEMS];

static void menu_select_callback(int index, void *ctx) {
  layer_mark_dirty(simple_menu_layer_get_layer(s_simple_menu_layer));
  switch(index) {
    case 0:
      window_stack_push (topic_window, true);
      break;
  }
}

static void topic_window_load(Window *window) {

  s_second_menu_items[0] = (SimpleMenuItem) {
    .title = "Food",
  };
  s_second_menu_items[1] = (SimpleMenuItem) {
    .title = "Ceremonies",
  };
  s_second_menu_items[2] = (SimpleMenuItem) {
    .title = "Tech Talks",
  };

  // Add menu items to menu section
  topics_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_second_menu_items,
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  topics_menu_layer = simple_menu_layer_create(bounds, window, topics_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(topics_menu_layer));
};

static void main_window_load(Window *window) {

  s_first_menu_items[0] = (SimpleMenuItem) {
    .title = "Timeline Update",
    .subtitle = "Add event related pins",
    .callback = menu_select_callback,

  };
  s_first_menu_items[1] = (SimpleMenuItem) {
    .title = "Final Countdown",
    .subtitle = "Displays time remaining",
    .callback = menu_select_callback,
  };
  s_first_menu_items[2] = (SimpleMenuItem) {
    .title = "Venue Hunt",
    .subtitle = "Leads you to the venue",
    .callback = menu_select_callback,

  };

  s_menu_sections[0] = (SimpleMenuSection) {
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
}

void topic_window_unload(Window *window) {
  simple_menu_layer_destroy(s_simple_menu_layer);
}

static void init() {
  s_main_window = window_create();
  topic_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load
  });
  window_set_window_handlers(topic_window, (WindowHandlers) {
    .load = topic_window_load
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
  window_destroy(topic_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}