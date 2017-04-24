#include <iostream>
#include "jpeg2000reader.h"

int main() {
  std::cout << " Hello " << std::endl;

  Jpeg2000Reader j;
  j.Load("test.j2k");
  j.SetResolutionFactor(2);
  auto tile1 = j.DecodeTile(0);
  for (int i = 0; i < tile1->w * tile1->h; i++) {
    std::cout<< tile1->data[i] << " ";
  }

  std::cout << "Res " << tile1->w << " x " << tile1->h;
}
