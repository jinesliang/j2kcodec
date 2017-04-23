#include <iostream>
#include "openjpegwrapper.h"

int main() {
  std::cout << " Hello " << std::endl;

  OpenjpegWrapper ow;
  ow.Load("test.j2k");
  auto tile1 = ow.DecodeTile(0);

  for (int i = 0; i < tile1->w * tile1->h; i++) {
    std::cout<< tile1->data[i] << " ";
  }
}
