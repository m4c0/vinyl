#pragma leco add_impl impl
export module vinyl:impl;
import casein;
import sv;
import voo;

namespace vinyl {
  export struct base_app_stuff {
    voo::device_and_queue dq;

    base_app_stuff(const char * name) : dq { name, casein::native_ptr } {
      casein::window_title = sv::unsafe(name);
    }
  };

  export struct base_extent_stuff {
    vee::render_pass rp; 
    voo::swapchain_and_stuff sw;

    base_extent_stuff(const base_app_stuff * app) :
      rp { voo::single_att_render_pass(app->dq) }
    , sw { app->dq, *rp }
    {}
  };
}
