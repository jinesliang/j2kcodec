#include <iostream>
#include "jpeg2000reader.h"

int main() {
  std::cout << " Hello " << std::endl;

  Jpeg2000Reader j;
  j.CreateInfileStream("helio335.jp2");
  auto img2 = j.Decode(); // Must go first lol
  //auto img = j.DecodeTile(0);
  j.Encode("out.j2k", img2->data, 4096, 4096, 512, 512, 1, 8);


  //for (int i = 0; i < 1; i++) {
  //  std::cerr << img->data[i] << " ";
  //}

  //for (int i = 0; i < 1; i++) {
  //  std::cerr << img2->data[i] << " ";
 // }



  //j.Encode("outloders.j2k");
  // j.SetResolutionFactor(2);
  // auto tile1 = j.DecodeTile(0);
  // for (int i = 0; i < tile1->w * tile1->h; i++) {
  //   std::cout<< tile1->data[i] << " ";
  // }

//  std::cout << "Res " << tile1->w << " x " << tile1->h;
}
