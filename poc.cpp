#pragma leco app

import casein;
import hai;
import mtx;
import silog;
import sith;
import voo;

namespace vinyl {
  enum event {
    START,
    RESIZE,
    FRAME,
    STOP,
    MAX_EVENT,
  };
  void on(event evt, hai::fn<void> callback);
}

//

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

static hai::fn<void> g_callbacks[vinyl::MAX_EVENT];
static mtx::mutex g_mutex {};
static mtx::cond g_sus_cond {};
static volatile bool g_suspended = false;
static volatile bool g_resized = false;

void vinyl::on(event evt, hai::fn<void> callback) {
  g_callbacks[evt] = callback;
}

static void call(vinyl::event e) {
  auto & fn = g_callbacks[e];
  if (fn) fn();
}

static void run(sith::thread * t) {
  call(vinyl::START);

  while (!t->interrupted()) {
    g_resized = false;
    call(vinyl::RESIZE);

    while (!t->interrupted() && !g_resized) {
      if (g_suspended) {
        mtx::lock l { &g_mutex };
        g_sus_cond.wait(&l);
      } else call(vinyl::FRAME);
    }

    vee::device_wait_idle();
  }

  call(vinyl::STOP);
}

static class init {
  sith::stateless_thread m_thr { run };
  sith::run_guard m_guard {};

public:
  init() {
    using namespace casein;

    handle(CREATE_WINDOW, [this] { m_guard = sith::run_guard { &m_thr }; });
    handle(RESIZE_WINDOW, [] { if (!casein::window_live_resize) g_resized = true; });
    handle(QUIT,          [this] { m_guard = {}; });

    handle(casein::ENTER_BACKGROUND, [] { g_suspended = true; });
    handle(casein::LEAVE_BACKGROUND, [] {
      mtx::lock l { &g_mutex };
      g_suspended = false;
      g_sus_cond.wake_all();
    });
  }
} s;
