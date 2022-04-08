#include "helpers.h"

int gen_numero_secreto() {
  srand(time(NULL) ^ (getpid() << 16));
  return (rand() % 20) + 1;
}

void lee_entrada(int *N, int *numerito, int *start, char **argv) {
  *N = atoi(argv[1]);
  *numerito = atoi(argv[2]);
  *start = atoi(argv[3]);
}

void imprime_entrada(int N, int numerito, int start) {
  printf("Se crearán %i procesos, se enviará el caracter %i desde proceso "
         "%i \n",
         N, numerito, start);
}
