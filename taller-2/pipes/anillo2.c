#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pipe_read(int i, int cant_hermanos) { return i; }

int pipe_write(int i, int cant_hermanos) { return ((i + 1) % cant_hermanos); }

int pipe_read_padre(int i, int cant_hermanos) { return cant_hermanos; }

int pipe_write_padre(int i, int cant_hermanos) { return cant_hermanos + 1; }

int generar_numero_secreto(int minimo) {
  int numero;
  srand(getppid());
  numero = rand();
  while (minimo >= numero) {
    numero = rand();
  }
  return numero;
}

void cerrar_pipes_innecesarios(int posicion, int pipe_abiertos[][2],
                               int cant_hermanos) {
  for (int i = 0; i < cant_hermanos; i++) {
    if (pipe_read(posicion, cant_hermanos) != i) {
      // cierro todos los pipes de recepcion
      close(pipe_abiertos[i][0]);
    } else {
      // printf("Pipe abierto lectura proceso %d: %d \n",posicion,i);
    }
    if (pipe_write(posicion, cant_hermanos) != i) {
      // cierro todos los pipes de enviar
      close(pipe_abiertos[i][1]);
    } else {
      // printf("Pipe abierto escritura proceso %d: %d \n",posicion,i);
    }
  }
  // cierro el pipe para escribir al padre
  close(pipe_abiertos[pipe_write_padre(posicion, cant_hermanos)][0]);
  // cierro el pipe para leer desde el padre
  close(pipe_abiertos[pipe_read_padre(posicion, cant_hermanos)][1]);
}

int main(int argc, char **argv) {
  int status, pid, n;
  int buffer[1];

  if (argc != 4) {
    printf("Uso: anillo <n> <c> <s> \n");
    exit(0);
  }
  n = atoi(argv[1]);
  int start = atoi(argv[2]);

  /* COMPLETAR */
  printf(
      "Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n",
      n, buffer[0], start);
  pid_t pidchild;
  int numero_secreto;
  int pipes_totales = n + 2;
  int pipe_abiertos[pipes_totales][2];
  for (int i = 0; i < pipes_totales; i++) {
    // creo los n pipes
    pipe(pipe_abiertos[i]);
  }

  for (int i = 0; i < n; i++) {
    pidchild = fork();
    if (pidchild == 0) {
      // soy un hijo
      printf("creando hijo: %d\n", i);
      cerrar_pipes_innecesarios(i, pipe_abiertos, n);
      printf("pipes cerrados del hijo: %d\n", i);

      if (i == start) {
        // soy el unico
        int numero_recibido;
        printf("unico esperando...");
        if (read(pipe_abiertos[pipe_read_padre(i, n)][0], &numero_recibido,
                 sizeof(int)) != sizeof(int)) {
          perror("numero malo");
          exit(-1);
        }
        printf("unico: %d", i);
        numero_secreto = generar_numero_secreto(numero_recibido);
        numero_secreto = 15;
        printf("numero magico: %d\n", numero_secreto);
        while (numero_recibido < numero_secreto) {
          // printf("Numero recibido: %d",numero_recibido);
          numero_recibido++;
          if (write(pipe_abiertos[pipe_write(i, n)][1], &numero_recibido,
                    sizeof(int)) != sizeof(int)) {
            perror("numero no se quizo ir");
            exit(-1);
          }

          if (read(pipe_abiertos[pipe_read(i, n)][0], &numero_recibido,
                   sizeof(int)) != sizeof(int)) {
            perror("numero malo");
            exit(-1);
          }
        }

        if (write(pipe_abiertos[pipe_write_padre(i, n)][1], &numero_recibido,
                  sizeof(int)) != sizeof(int)) {
          perror("numero malo");
          exit(-1);
        }
        // cierro los pipes del anillo
        close(pipe_abiertos[pipe_read(i, n)][0]);
        close(pipe_abiertos[pipe_write(i, n)][1]);

        // cierro los pipes del padre
        close(pipe_abiertos[pipe_write_padre(i, n)][1]);
        // cierro el pipe para leer desde el padre
        close(pipe_abiertos[pipe_read_padre(i, n)][0]);
        exit(0);
      } else {
        // soy un simio
        printf("simio: %d", i);
        // cierro los pipes del padre
        close(pipe_abiertos[pipe_write_padre(i, n)][1]);
        // cierro el pipe para leer desde el padre
        close(pipe_abiertos[pipe_read_padre(i, n)][0]);
        int numero_recibido;
        while (1) {
          if (read(pipe_abiertos[pipe_read(i, n)][0], &numero_recibido,
                   sizeof(int)) != sizeof(int)) {
            // como no puede leer por eof, termina cerrando los pipes
            printf("cerrando pipes en cascada: %d", i);
            close(pipe_abiertos[pipe_read(i, n)][0]);
            close(pipe_abiertos[pipe_write(i, n)][1]);
            perror("pipe cerrado");
            exit(-1);
          } else {
            numero_recibido++;
            if (write(pipe_abiertos[pipe_write(i, n)][1], &numero_recibido,
                      sizeof(int)) != sizeof(int)) {
              close(pipe_abiertos[pipe_read(i, n)][0]);
              perror("numero no se quizo ir");
              exit(-1);
            }
          }
        }
      }
      exit(0);
    } else {
      // soy el padre
    }
  }
  if (pidchild != 0) {
    printf("padre enviando numero\n");
    if (write(pipe_abiertos[n][1], &n, sizeof(int)) != sizeof(int)) {
      perror("numero malo");
      exit(-1);
    }
    printf("padre esperando numero\n");
    int numero_recibido;
    if (read(pipe_abiertos[n + 1][0], &numero_recibido, sizeof(int)) !=
        sizeof(int)) {
      perror("numero malo");
      exit(-1);
    }
    printf("Numero recibido: %d", numero_recibido);
    printf("padre cerrando pipes\n");
    for (int i = 0; i < pipes_totales; i++) {
      close(pipe_abiertos[i][0]);
      close(pipe_abiertos[i][1]);
    }
  }

  /* COMPLETAR */
}