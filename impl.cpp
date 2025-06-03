module vinyl;

import casein;
import hai;
import mtx;
import sith;
import vee;

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

    handle(ENTER_BACKGROUND, [] { g_suspended = true; });
    handle(LEAVE_BACKGROUND, [] {
      mtx::lock l { &g_mutex };
      g_suspended = false;
      g_sus_cond.wake_all();
    });
  }
} s;
