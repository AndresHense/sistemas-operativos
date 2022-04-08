#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int dado(){
	srand(time(NULL) ^ (getpid()<<16));
	return (rand() % 6) +1;
}

int main(int argc, char const *argv[]){
	srand(time(0));
	write(1,"Hola soy Homero\n",16);
	int pipes_bart[2];
	int pipes_lisa[2];
	if (pipe(pipes_bart) == -1) {
		perror("pipe");
		exit(1);
	}
	if (pipe(pipes_lisa) == -1) {
		perror("pipe");
		exit(1);
	}
	int pid= fork();
		if(pid==0){
			//sleep(1);
			write(1,"Hola soy Bart\n",14);
			int mi_dado = dado();
			if (write(pipes_bart[1], &mi_dado, sizeof(int)) != sizeof(int)) {
				perror("write");
				exit(1);
			}
			printf("Soy Bart y mi dado fue %d\n", mi_dado);
			return 0;
		}
		pid= fork();
		if(pid==0){
			//sleep(1);
			write(1,"Hola soy Lisa\n",14);

			int mi_dado = dado();
			if (write(pipes_lisa[1], &mi_dado, sizeof(int)) != sizeof(int)) {
				perror("write");
				exit(1);
			}
			printf("Soy Lisa y mi dado fue %d\n", mi_dado);
			return 0;
		}
		wait(NULL);

		int valor_bart;
		int valor_lisa;
		//Este codigo solo lo corre el padre
		if (read(pipes_bart[0], &valor_bart, sizeof(int)) != sizeof(int)) {
				perror("read");
				exit(1);
			}
		if (read(pipes_lisa[0], &valor_lisa, sizeof(int)) != sizeof(int)) {
				perror("read");
				exit(1);
		}
		if(valor_bart<valor_lisa){
			printf("Ganó Lisa!\n");
		}else{
			printf("Ganó Bart!\n");
		}
	return 0 ;
}
