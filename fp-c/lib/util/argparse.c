#include "util/argparse.h"

#include <string.h>

bool arg_bool(int argc, const char* restrict argv[],
              const char* restrict name) {
  for (int i = 0; i < argc; ++i)
    if (strcmp(name, argv[i]) == 0) return true;
  return false;
}
