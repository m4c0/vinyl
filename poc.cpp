#pragma leco app

import casein;
import hai;
import silog;
import sith;
import voo;

namespace vinyl {
  enum event {
    START,
    FRAME,
    STOP,
    MAX_EVENT,
  };
  void on(event evt, hai::fn<void> callback);
}

//

struct app_stuff {
  voo::device_and_queue dq { "poc-vinyl", casein::native_ptr };
  voo::swapchain_and_stuff sw { dq };
};
static hai::uptr<app_stuff> gs {};

static struct app_init {
  app_init() {
    using namespace vinyl;
    on(START, [] { gs.reset(new app_stuff {}); });
    on(FRAME, [] {});
    on(STOP,  [] { gs.reset(nullptr); });
  }
} i;

//

static hai::fn<void> g_callbacks[vinyl::MAX_EVENT];

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
    call(vinyl::FRAME);
  }

  vee::device_wait_idle();
  call(vinyl::STOP);
}

static class init {
  sith::stateless_thread m_thr { run };
  sith::run_guard m_guard {};

public:
  init() {
    using namespace casein;

    handle(CREATE_WINDOW, [this] { m_guard = sith::run_guard { &m_thr }; });
    handle(QUIT,          [this] { m_guard = {}; });
  }
} s;
