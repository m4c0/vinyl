#pragma leco add_impl impl
export module vinyl:impl;
import casein;
import dotz;
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
    voo::bound_image depth;
    voo::swapchain_and_stuff sw;

    base_extent_stuff(const base_app_stuff * app) :
      rp { voo::single_att_depth_render_pass(app->dq) }
    , depth { voo::bound_image::create_depth(app->dq.extent_of()) }
    , sw { app->dq, *rp, *depth.iv }
    {}

    float aspect() const { return sw.aspect(); }

    [[nodiscard]] auto clear(dotz::vec4 colour) {
      auto rp = sw.cmd_render_pass({
        .clear_colours { 
          vee::clear_colour(colour), 
          vee::clear_depth(1.0),
        },
      });
      auto cb = sw.command_buffer();
      auto ext = sw.extent();
      vee::cmd_set_viewport_flipped(cb, ext);
      vee::cmd_set_scissor(cb, ext);
      return rp;
    }

    void frame(auto && fn) {
      sw.acquire_next_image();
      sw.queue_one_time_submit(fn);
      sw.queue_present();
    }
  };
}
