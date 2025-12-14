#pragma leco app

import vinyl;

struct app_stuff;
struct sized_stuff;
using v = vinyl::v<app_stuff, sized_stuff>;

struct app_stuff {
};
struct sized_stuff {
};

const int i = [] {
  v::on_frame() = [] {};
  v::setup();

  return 0;
}();
