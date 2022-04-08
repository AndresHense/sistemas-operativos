#ifndef HELPERS_H
#define HELPERS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int gen_numero_secreto();
void lee_entrada(int *N, int *numerito, int *start, char **argv);
void imprime_entrada(int N, int numerito, int start);

#endif