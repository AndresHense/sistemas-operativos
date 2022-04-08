#include "hijos.h"

int crea_hijos(int N) {
  for (int i = 0; i < N; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      printf("se creo el hijo: %d\n", i);
      return i;
    }
  }
  return -1;
}

void ejecuta_starter(void *_m_buffer, int numerito, int *start_flag,
                     int *numero_secreto, int n, int nMenosUno, int *salida) {
  int(*m_buffer)[2] = _m_buffer;

  if (*start_flag == 1) {
    *numero_secreto = gen_numero_secreto() + numerito;
    printf("el numero secreto es: %d\n", *numero_secreto);

    escribe_pipe(m_buffer, nMenosUno, numerito);

    *start_flag = 0;

  } else {
    int num_temp = lee_pipe(m_buffer, nMenosUno);
    printf("el start_flag anduvo y el numerito es: %d\n", num_temp);

    if (num_temp >= *numero_secreto) {
      printf("entre a la guarda de salida\n");
      guarda_salida(num_temp, salida);

      close(m_buffer[n][1]);
      close(m_buffer[n][0]);

      exit(0);
    }

    escribe_pipe(m_buffer, nMenosUno, num_temp);
  }
}

void ejecuta_hijo(int id, void *_m_buffer, int N, int start, int numerito,
                  int *salida) {
  if (id == -1)
    return;

  int(*m_buffer)[2] = _m_buffer;

  int n = id;
  int nMenosUno = (id == 0) ? N - 1 : ((id - 1) % N);

  int start_flag = 1;
  int numero_secreto;

  cierra_pipes_sin_uso(m_buffer, n, nMenosUno, N, start, salida);

  while (1) {
    if (id == start) {
      ejecuta_starter(m_buffer, numerito, &start_flag, &numero_secreto, n,
                      nMenosUno, salida);
    }

    int num_temp = lee_pipe(m_buffer, nMenosUno);
    printf("el hijo es %d y el numerito es: %d\n", id, num_temp);

    num_temp++;

    escribe_pipe(m_buffer, n, num_temp);
  }
}