#ifndef PIPES_H
#define PIPES_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void crea_pipes(void *m_buffer, int *salida, int n);
int lee_pipe(void *m_buffer, int n);
void escribe_pipe(void *m_buffer, int nMasUno, int num_temp);
void cierra_pipes_sin_uso(void *m_buffer, int nMasUno, int nMenosUno, int N,
                          int start, int *salida);
void guarda_salida(int numerito, int *salida);
void imprime_salida(int *salida);

#endif