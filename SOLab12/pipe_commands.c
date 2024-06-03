#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
	pid_t pid1, pid2, pid3;
	int Pipe1to2[2], Pipe2to3[2], Pipe3to4[2];
	if(-1 == pipe(Pipe1to2)){
		perror("Eroare la crearea pipe-ului!");
		exit(2);
	}

	if(-1 == pipe(Pipe2to3)){
		perror("Eroare la crearea pipe-ului!");
		exit(3);
	}

	if(-1 == pipe(Pipe3to4)){
		perror("Eroare la crearea pipe-ului!");
		exit(4);
	}

	if(-1 == (pid1 = fork())){
		perror("Eroare la crearea primului fiu!");
		exit(5);
	}

	if(pid1==0){
		//Secventa executata de primul fiu

		if( -1 == dup2(Pipe1to2[1],1)){
			perror("Eroare la redirectionarea iesirii standard!");
			exit(8);
		}

		execlp("w", "w", "-h", NULL);
		perror("Eroare la comanda w");
		exit(9);

	}
	else{
		if(-1 == (pid2 = fork())){
			perror("Eroare la crearea celui de-al doilea fiu!");
			exit(6);
		}

		if(pid2==0){
			//Secventa executata de al doilea fiu
			close(Pipe1to2[1]);
			if( -1 == dup2(Pipe2to3[1],1)){
				perror("Eroare la redirectionarea iesirii standard!");
				exit(10);
			}

			if( -1 == dup2(Pipe1to2[0],0)){
				perror("Eroare la redirectionarea intrarii standard!");
				exit(11);
			}

			execlp("tr", "tr", "-s", "' '", NULL);
			perror("Eroare la tr!");
			exit(12);
		}
		else{
			if(-1 == (pid3 = fork())){
				perror("Eroare la crearea celui de-al treilea fiu!");
				exit(7);
			}

			if(pid3==0){
				//Secventa executata de al treilea fiu
				close(Pipe1to2[1]);
				close(Pipe2to3[1]);
				if( -1 == dup2(Pipe3to4[1],1)){
					perror("Eroare la redirectionarea iesirii standard!");
					exit(13);
				}

				if( -1 == dup2(Pipe2to3[0],0)){
					perror("Eroare la redirectionarea intrarii standard!");
					exit(14);
				}

				execlp("cut", "cut", "-d ", "-f1,8", NULL);
				perror("Eroare la cut!");
				exit(15);
			}
			else{
				//Secventa executata de tata.

				close(Pipe1to2[1]);
				close(Pipe2to3[1]);
				close(Pipe3to4[1]);

				if(-1 == dup2(Pipe3to4[0],0)){
					perror("Eroare la redirectionarea intrarii standard!");
					exit(16);
				}

				execlp("sort", "sort", "-t:" , "-k", "1", NULL);
				perror("Eroare la comanda sort!");
				exit(17);
			}
		}
	}




	return 0;
}

	
