#pragma leco app

import vinyl;

struct app_stuff;
struct sized_stuff;
using v = vinyl::v<app_stuff, sized_stuff>;

struct app_stuff {
  int app_scoped_data = 1;
};
struct sized_stuff {
  int extent_scoped_data = v::as()->app_scoped_data + 1;
};

const int i = [] {
  v::on_frame() = [] {};
  v::setup();

  return 0;
}();
