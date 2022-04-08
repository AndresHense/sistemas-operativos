#include "helpers.h"
#include "hijos.h"
#include "pipes.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    printf("Uso: anillo <n> <c> <s> \n");
    exit(0);
  }

  int N, numerito, start;
  lee_entrada(&N, &numerito, &start, argv);

  int m_buffer[atoi(argv[1])][2];
  int salida[2];

  imprime_entrada(N, numerito, start);

  crea_pipes(m_buffer, salida, N);

  int hijo_id = crea_hijos(N);

  ejecuta_hijo(hijo_id, m_buffer, N, start, numerito, salida);

  wait(NULL);

  imprime_salida(salida);
  cierra_pipes_sin_uso(m_buffer, -1, -1, N, start, salida);
}
