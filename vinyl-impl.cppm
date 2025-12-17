#pragma leco add_impl impl
export module vinyl:impl;
import casein;
import dotz;
import hai;
import jute;
import sv;
import traits;
import voo;
import wagen;

namespace vinyl {
  export struct base_app_stuff {
    voo::device_and_queue dq;

    base_app_stuff(const char * name) : dq { name, casein::native_ptr } {
      casein::window_title = sv::unsafe(name);
    }
  };
  export struct nearest_texture {
    vee::descriptor_set_layout dsl = vee::create_descriptor_set_layout({
      vee::dsl_fragment_sampler(),
    });
    vee::descriptor_pool dpool = vee::create_descriptor_pool(1, {
      vee::combined_image_sampler(1),
    });
    vee::descriptor_set dset = vee::allocate_descriptor_set(*dpool, *dsl);
    vee::sampler smp = [] {
      auto info = vee::sampler_create_info {};
      info.address_mode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
      info.nearest();
      info.unnormalizedCoordinates = wagen::vk_true;
      return vee::create_sampler(info);
    }();
    voo::bound_image img {};

    explicit nearest_texture(sv name) {
      voo::load_image(name, &img, [this](auto) {
        vee::update_descriptor_set(dset, 0, 0, *img.iv, *smp);
      });
    }

    [[nodiscard]] explicit operator bool() const { return true; }
  };

  export struct frag_shader : voo::frag_shader {
    frag_shader(sv name, hai::fn<void> callback) : voo::frag_shader { jute::fmt<"%s.frag.spv">(name) } { callback(); }
  };
  export struct vert_shader : voo::vert_shader {
    vert_shader(sv name, hai::fn<void> callback) : voo::vert_shader { jute::fmt<"%s.vert.spv">(name) } { callback(); }
  };

  export using vertex_attribute_t = wagen::VkVertexInputAttributeDescription;
  export template<typename T> auto vertex_attribute(dotz::vec2 (T::*m)) {
    return vee::vertex_attribute_vec2(0, traits::offset_of(m));
  }
  export template<typename T> auto vertex_attribute(dotz::vec4 (T::*m)) {
    return vee::vertex_attribute_vec4(0, traits::offset_of(m));
  }
  export template<typename T> auto vertex_attribute(unsigned (T::*m)) {
    return vee::vertex_attribute_uint(0, traits::offset_of(m));
  }
}
