#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
int main(int argc, char* argv[]) {

  	pid_t child=fork();
  	if(child==0){
  		for(int i=0;i<5;i++){
			puts("sup!");
			kill(child,SIGURG);
			puts("ya va!");
			sleep(1);
		}
		
  	}
  	wait(NULL);
  	if(child!=0)
	system(argv[1]);
  	
	return 0;
}
*/
int  i=0;
int j=0;

void urgHandler(){
	puts("ya va!");
	//i++;
}

void intHandler(){
	j++;
}

int main(int argc, char* argv[]) {
	
	pid_t child=fork();
	if(child==0){
		signal(SIGURG,&urgHandler);
		signal(SIGINT,&intHandler);
		while(j==0){;
		}
		char* args[argc-1];
		for(int k=0;k<argc-1;k++){
			args[k]=argv[k+1];
			printf("%s ",args[k]);
		}
		//execvp(args[0],args);
		execvp(argv[1], argv+1);
		
		perror("ERROR child exec(...)"); exit(1);
  	}
 	else{
  		while(i<5){
			puts("sup!");
			kill(child,SIGURG);
			//while(magicBool);
			//wait(NULL);	//espera a q el hijo muera
			sleep(1);
			i++;
		}
	}
  	kill(child,SIGINT);
  	wait(NULL);
	return 0;
}
