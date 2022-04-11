#include <stdio.h>
#include <stdlib.h>

#include "matrix/matrix.h"

int main() {
  FILE* afile;

  if ((afile = fopen("A.mat", "r")) == NULL) {
    perror("damn");
    return EXIT_FAILURE;
  }

  matrix_t mat;
  if (matrix_read(afile, &mat) != 0) {
    perror("fuck");
    return EXIT_FAILURE;
  }

  if (mat.rows != 3) {
    printf("shit\n");
    return EXIT_FAILURE;
  }

  if (mat.cols != 3) {
    printf("bitch\n");
    return EXIT_FAILURE;
  }

  matrix_write(stdout, &mat);
  matrix_destroy(&mat);

  return 0;
}
