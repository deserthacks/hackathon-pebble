#include "pebble.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 3

#define TOPIC_FOOD_DEFAULT false
#define TOPIC_EVENTS_DEFAULT false
#define TOPIC_TALKS_DEFAULT false
#define TOPIC_REMINDERS_DEFAULT false

static Window *s_main_window;
static Window *topic_window;
static SimpleMenuLayer *s_menu_layer;
static MenuLayer *topics_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];

// Topic subscription variables
static bool topic_food = TOPIC_FOOD_DEFAULT;
static bool topic_events = TOPIC_EVENTS_DEFAULT;
static bool topic_talks = TOPIC_TALKS_DEFAULT;
static bool topic_reminders = TOPIC_REMINDERS_DEFAULT;

enum {
  SUB_TOPIC_TALKS = 0,
  SUB_TOPIC_FOOD,
  SUB_TOPIC_EVENTS,
  SUB_TOPIC_REMINDERS,
  UNSUB_TOPIC_TALKS,
  UNSUB_TOPIC_FOOD,
  UNSUB_TOPIC_EVENTS,
  UNSUB_TOPIC_REMINDERS,
};

enum {
  TOPIC_FOOD_PKEY = 0,
  TOPIC_EVENTS_PKEY,
  TOPIC_TALKS_PKEY,
  TOPIC_REMINDERS_PKEY,
};

static void menu_select_callback(int index, void *ctx) {
  layer_mark_dirty(simple_menu_layer_get_layer(s_menu_layer));
  switch(index) {
    case 0:
      window_stack_push(topic_window, true);
      break;
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void send_to_phone(int key, int value) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SEND_TO_PHONE:");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  app_message_outbox_send();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SEND_TO_PHONE....");
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;
    case 1:
      return NUM_SECOND_MENU_ITEMS;
    default:
      return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Event types");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "One more");
      break;
  }
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->section) {
    case 0:
      switch (cell_index->row) {
        case 0:
          menu_cell_basic_draw(ctx, cell_layer, "Food", NULL, NULL);
          break;
        case 1:
          menu_cell_basic_draw(ctx, cell_layer, "Events", NULL, NULL);
          break;
        case 2:
          menu_cell_basic_draw(ctx, cell_layer, "Tech Talks", NULL, NULL);
          break;
        case 3:
          menu_cell_basic_draw(ctx, cell_layer, "Reminders", NULL, NULL);
          break;
      }
      break;
    case 1:
      switch (cell_index->row) {
        case 0:
          // There is title draw for something more simple than a basic menu item
          menu_cell_title_draw(ctx, cell_layer, "Final Item");
          break;
      }
  }
}

static void topics_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index->row) {
    case 0:
      // Food
      APP_LOG(APP_LOG_LEVEL_DEBUG, "FOOD");
      if(topic_food) {
        send_to_phone(UNSUB_TOPIC_FOOD, 0);
      } else {
        send_to_phone(SUB_TOPIC_FOOD, 1);
      }
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 1:
      // Events
      APP_LOG(APP_LOG_LEVEL_DEBUG, "EVENTS");
      if(topic_events) {
        send_to_phone(UNSUB_TOPIC_EVENTS, 0);
      } else {
        send_to_phone(SUB_TOPIC_EVENTS, 1);
      }
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 2:
      // Tech talks
      APP_LOG(APP_LOG_LEVEL_DEBUG, "TALKS");
      if(topic_talks) {
        send_to_phone(UNSUB_TOPIC_TALKS, 0);
      } else {
        send_to_phone(SUB_TOPIC_TALKS, 1);
      }
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
    case 3:
      // Reminders
      if(topic_reminders) {
        send_to_phone(UNSUB_TOPIC_REMINDERS, 0);
      } else {
        send_to_phone(SUB_TOPIC_REMINDERS, 1);
      }
      layer_mark_dirty(menu_layer_get_layer(menu_layer));
      break;
  }

}

static void topic_window_load(Window *window) {

  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  topics_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(topics_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = topics_menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(topics_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(topics_menu_layer));
}

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

  s_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_menu_layer));
}

void main_window_unload(Window *window) {
  simple_menu_layer_destroy(s_menu_layer);
}

void topic_window_unload(Window *window) {
  menu_layer_destroy(topics_menu_layer);
}

static void init() {
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);

    // Open AppMessage
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  s_main_window = window_create();
  topic_window = window_create();

  topic_food = persist_exists(TOPIC_FOOD_PKEY) ? persist_read_int(TOPIC_FOOD_PKEY) : TOPIC_FOOD_DEFAULT;
  topic_events = persist_exists(TOPIC_EVENTS_PKEY) ? persist_read_int(TOPIC_EVENTS_PKEY) : TOPIC_EVENTS_DEFAULT;
  topic_talks = persist_exists(TOPIC_TALKS_PKEY) ? persist_read_int(TOPIC_TALKS_PKEY) : TOPIC_TALKS_DEFAULT;
  topic_reminders = persist_exists(TOPIC_REMINDERS_PKEY) ? persist_read_int(TOPIC_REMINDERS_PKEY) : TOPIC_REMINDERS_DEFAULT;

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_window_handlers(topic_window, (WindowHandlers) {
    .load = topic_window_load,
    .unload = topic_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  persist_write_bool(TOPIC_TALKS_PKEY, topic_talks);
  persist_write_bool(TOPIC_EVENTS_PKEY, topic_events);
  persist_write_bool(TOPIC_FOOD_PKEY, topic_food);
  persist_write_bool(TOPIC_REMINDERS_PKEY, topic_reminders);

  window_destroy(s_main_window);
  window_destroy(topic_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}