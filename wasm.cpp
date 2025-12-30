module vinyl;
import casein;
import hai;
import vaselin;

static hai::fn<void> g_callbacks[vinyl::MAX_EVENT];

void vinyl::on(event evt, hai::fn<void> callback) {
  g_callbacks[evt] = callback;
}

static void call(vinyl::event e) {
  auto & fn = g_callbacks[e];
  if (fn) fn();
}

static bool g_running {};

static void frame(void *) {
  call(vinyl::FRAME);
  if (g_running) vaselin::request_animation_frame(frame, nullptr);
}

void vinyl::init() {
  using namespace casein;

  handle(CREATE_WINDOW, [] {
    g_running = true;
    call(vinyl::START);
  });
  handle(RESIZE_WINDOW, [] { call(vinyl::RESIZE); });
  handle(QUIT,          [] {
    g_running = false;
    call(vinyl::STOP);
  });

  vaselin::request_animation_frame(frame, nullptr);
}
