#ifndef HIJOS_H
#define HIJOS_H
#include "helpers.h"
#include "pipes.h"

int crea_hijos(int N);
void ejecuta_hijo(int id, void *m_buffer, int N, int start, int numerito,
                  int *salida);

#endif