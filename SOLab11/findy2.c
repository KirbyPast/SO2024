#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){

	char* argument_list[] = {"find", "/home", "(", "-name", "*.c", "-or", "-name", "*.sh", ")", "-printf", "%p : %k KB\n", NULL};
	pid_t pid_fiu=0;
	int status=0;
	if(-1==(pid_fiu=fork())){
		perror("Eroare la fork!");
		return -1;
	}

	if(pid_fiu==0){
		execv("/bin/find", argument_list);
		perror("Eroare la exec!");

		exit(100);
	}

	wait(&status);

	if(WIFEXITED(status)){
		switch(WEXITSTATUS(status)){
			case 100: {printf("Comanda nu a mers.. \n"); break;}
			case 0: {printf("Comanda find a fost executata cu succes! \n"); break;}
			default: {printf("Comanda find a fost executata, dar a esuat cu codul: %d", WEXITSTATUS(status)); break;}
		}
	}
	else{
		printf("Comanda find a fost oprita fortat de semnalul %d \n",WTERMSIG(status));
	}
	return 0;

}
