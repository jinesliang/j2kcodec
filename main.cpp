#include <iostream>
#include "openjpegwrapper.h"

int main() {
  std::cout << " Hello " << std::endl;

  OpenjpegWrapper ow;
  ow.Load("test.j2k");
  ow.SetResolutionFactor(1);
  auto tile1 = ow.DecodeTile(0);
  for (int i = 0; i < tile1->w * tile1->h; i++) {
    std::cout<< tile1->data[i] << " ";
  }

  std::cout << "Res " << tile1->w << " x " << tile1->h;
}
