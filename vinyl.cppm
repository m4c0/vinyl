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
}

#pragma leco add_impl impl
