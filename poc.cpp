#pragma leco app

import casein;
import hai;
import vinyl;
import voo;

struct app_stuff {
  voo::device_and_queue dq { "poc-vinyl", casein::native_ptr };
};
static hai::uptr<app_stuff> gas {};

struct sized_stuff {
  voo::swapchain_and_stuff sw { gas->dq };
};
static hai::uptr<sized_stuff> gss {};

static void frame() {
  if (!gss) gss.reset(new sized_stuff {});
}

static struct app_init {
  app_init() {
    using namespace vinyl;
    on(START,  [] { gas.reset(new app_stuff {}); });
    on(RESIZE, [] { gss.reset(nullptr); });
    on(FRAME,  &frame);
    on(STOP,   [] { 
      gss.reset(nullptr);
      gas.reset(nullptr);
    });
  }
} i;

//

