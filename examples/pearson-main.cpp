#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pearson.h"

const int T_MAX = 100;

Formura_Navigator navi;

float frand() {
  return rand() / float(RAND_MAX);
}

void init() {
  for(int y = navi.lower_y; y < navi.upper_y; ++y) {
    for(int x = navi.lower_x; x < navi.upper_x; ++x) {
      U[y][x] = 1;
      V[y][x] = 0;
    }
  }
  for (int y = 118; y < 138; ++y) {
    for (int x = 118; x < 138; ++x) {
      U[y][x] = 0.5+0.01*frand();
      V[y][x] = 0.25+0.01*frand();
    }
  }
}

int main (int argc, char **argv) {
  srand(time(NULL));
  MPI_Init(&argc, &argv);
  Formura_Init(&navi, MPI_COMM_WORLD);

  init();

  while(navi.time_step < T_MAX) {
    if(navi.time_step % 100 == 0) {
      char fn[256];
      sprintf(fn, "frames/%06d.txt", navi.time_step);
      FILE *fp = fopen(fn,"w");
      for(int y = navi.lower_y; y < navi.upper_y; ++y) {
        for(int x = navi.lower_x; x < navi.upper_x; ++x) {
          fprintf(fp, "%f ", U[y][x]);
        }
        fprintf(fp, "\n");
      }
      fclose(fp);
    }
    Formura_Forward(&navi);
    printf("t = %d\n", navi.time_step);
  }
  MPI_Finalize();
}
