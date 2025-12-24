#pragma leco add_impl wasm
export module vinyl:wasm;

namespace vinyl {
  export struct base_app_stuff {
    base_app_stuff(const char * name) {}
  };
  export struct base_extent_stuff {
    base_extent_stuff(const base_app_stuff * app) {}

    void frame(auto && fn) { fn(); }
  };
}
