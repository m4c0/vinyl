#pragma leco add_impl wasm
export module vinyl:wasm;
import gelo;
import hai;
import silog;
import sires;
import stubby;
import sv;

namespace vinyl {
  export struct nearest_texture {
    int id = gelo::create_texture();

    explicit nearest_texture(sv name) {
      sires::read(name, nullptr, [this,name](auto, hai::cstr & bits) {
        using namespace gelo;

        silog::log(silog::info, "[%*s] loaded", static_cast<int>(name.size()), name.begin());

        auto img = stbi::load(bits);
        auto & [ w, h, ch, data ] = img;
        active_texture(TEXTURE0);
        bind_texture(TEXTURE_2D, id);
        tex_image_2d(TEXTURE_2D, 0, RGBA, w, h, 0, RGBA, UNSIGNED_BYTE, *data, w * h * 4);
        tex_parameter_i(TEXTURE_2D, TEXTURE_WRAP_S, CLAMP_TO_EDGE);
        tex_parameter_i(TEXTURE_2D, TEXTURE_WRAP_T, CLAMP_TO_EDGE);
        tex_parameter_i(TEXTURE_2D, TEXTURE_MIN_FILTER, NEAREST);
        tex_parameter_i(TEXTURE_2D, TEXTURE_MAG_FILTER, NEAREST);
      });
    }

    [[nodiscard]] explicit operator bool() const { return id != 0; }
  };
}
