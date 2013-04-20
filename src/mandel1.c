#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x27, 0x7D, 0x90, 0x5C, 0x09, 0x35, 0x40, 0x5D, 0x96, 0xF3, 0x27, 0x96, 0x69, 0x26, 0x17, 0x9B }
PBL_APP_INFO(MY_UUID,
             "Mandelbrot v0.1", "Gabe Simon",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

Layer graphicsLayer;

int MAX_ITERS = 20;
int ESCAPE_VALUE = 10;
double xmin = -2.3, xmax = 1, ymin = -1.5, ymax = 1.5;
double xwid, ywid, xdel, ydel;
double xpos, ypos;
double cx, cy;
int xPixelMin = 0;
int xPixelMax = 144;
int yPixelMin = 0;
int yPixelMax = 168;
int mandel();

void update_layer_callback(Layer *me, GContext* ctx) {
  (void)me;

  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);

  int x,y;
  xwid = xmax - xmin;
  ywid = ymax - ymin;
  xdel = xwid / xPixelMax;
  ydel = ywid / yPixelMax;

  for (x = xPixelMin; x < xPixelMax; ++x) {
    xpos = xmin + (x * xdel);
    for (y = yPixelMin; y < yPixelMax; ++y) {
      ypos = ymin + (y * ydel);

      GPoint mandelPoint = {x,y};

      if (mandel(xpos, ypos) % 2 == 0) {
	graphics_draw_pixel(ctx, mandelPoint);
      }
    }
  }
  
}

int mandel (double cx, double cy) {
  int k = 0;
  double zx = 0, zy = 0, zxtemp = 0, zytemp = 0;
  while (( k < MAX_ITERS) && (zx*zx + zy*zy < ESCAPE_VALUE)) {
    ++k;
    zxtemp = zx * zx - zy * zy + cx;
    zytemp = 2 * zx * zy + cy;
    zx = zxtemp;
    zy = zytemp;
  }
  return k;
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Graphics layer");
  window_stack_push(&window, true /* Animated */);

  layer_init(&graphicsLayer, window.layer.frame);
  graphicsLayer.update_proc = update_layer_callback;
  layer_add_child(&window.layer, &graphicsLayer);

  layer_mark_dirty(&graphicsLayer);


}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
