#include "MurmurHash3.h"
#include <iostream>

int main() {
  const int key[4] = {1,2,3,4};
  int len = 4;
  uint32_t seed = 1;
  int out[2] = {1,2};
  MurmurHash3_x64_128  (key, len, seed, out);
  std::cout << out[0] << " " << out[1] << std::endl;
  return 0;
}