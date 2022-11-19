#include "ketemine.hpp"

using namespace ktp;

int main(void) {

  if (!keteMine::init()) return 1;

  keteMine::run();

  return 0;
}
