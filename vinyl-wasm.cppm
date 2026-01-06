#pragma leco add_impl wasm
export module vinyl:wasm;
import casein;
import dotz;
import gelo;

namespace vinyl {
  export struct base_app_stuff {
    base_app_stuff(const char * name) {
      using namespace casein;

      fullscreen = true;
      interrupt(IRQ_FULLSCREEN);
    }
  };
  export struct base_extent_stuff {
    base_extent_stuff(const base_app_stuff * app) {}

    float aspect() const {
      auto [w, h] = casein::window_size;
      return static_cast<float>(w) / static_cast<float>(h);
    }

    int clear(dotz::vec4 colour) {
      using namespace gelo;
      auto [r, g, b, a] = colour;

      clear_color(r, g, b, a);
      gelo::clear(COLOR_BUFFER_BIT);
      viewport(0, 0, casein::window_size.x, casein::window_size.y);

      return 0;
    }

    void frame(auto && fn) { fn(); }
  };
}
