#include "pipes.h"

void crea_pipes(void *_m_buffer, int *salida, int n) {
  int(*m_buffer)[2] = _m_buffer;
  for (int i = 0; i < n; i++) {
    pipe(m_buffer[i]);
  }
  pipe(salida);
}

int lee_pipe(void *_m_buffer, int n) {
  int num_temp;
  int(*m_buffer)[2] = _m_buffer;
  if (read(m_buffer[n][0], &num_temp, sizeof(int)) != sizeof(int)) {
    perror("read");
    exit(1);
  }
  return num_temp;
}

void escribe_pipe(void *_m_buffer, int nMasUno, int num_temp) {
  int(*m_buffer)[2] = _m_buffer;
  if (write(m_buffer[nMasUno][1], &num_temp, sizeof(int)) != sizeof(int)) {
    perror("write");
    exit(1);
  }
}

void cierra_pipes_sin_uso(void *_m_buffer, int nMasUno, int nMenosUno, int N,
                          int start, int *salida) {
  int(*m_buffer)[2] = _m_buffer;
  for (int i = 0; i < N; i++) {
    if (i != nMasUno && i != nMenosUno) {
      close(m_buffer[i][1]);
      close(m_buffer[i][0]);
    }
  }

  if (nMasUno != start) {
    close(salida[0]);
    close(salida[1]);
  }
}

void guarda_salida(int numerito, int *salida) {
  if (write(salida[1], &numerito, sizeof(int)) != sizeof(int)) {
    perror("write");
    exit(1);
  }
}

void imprime_salida(int *salida) {
  int num_temp;
  if (read(salida[0], &num_temp, sizeof(int)) != sizeof(int)) {
    perror("read");
    exit(1);
  }
  printf("El programa termino con el numerito siendo: %d\n", num_temp);
}