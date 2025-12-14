export module vinyl;
import hai;

export namespace vinyl {
  enum event {
    START,
    RESIZE,
    FRAME,
    STOP,
    MAX_EVENT,
  };
  void on(event evt, hai::fn<void> callback);

  template<typename A, typename S> struct v {
    static auto & as() { static A * i; return i; };
    static auto & ss() { static S * i; return i; };

    static void setup(hai::fn<void> on_frame) {
      using namespace vinyl;

      on(START, [] { as() = new A {}; });
      on(RESIZE, [] {
        delete ss();
        ss() = nullptr;
      });
      on(FRAME, [=] mutable {
        if (!ss()) ss() = new S {};
        on_frame();
      });
      on(STOP, [] {
        delete ss();
        delete as();
      });
    }
  };
}

#ifndef LECO_TARGET_WASM
#pragma leco add_impl impl
#else
#pragma leco add_impl wasm
#endif
