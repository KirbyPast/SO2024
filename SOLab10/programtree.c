#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){
	pid_t pid_fiu;
	int level = 1;
	int ordin = 1;
	int pids[10];
	int statuses[10];
	int status=0;

	if(-1 == (pid_fiu=fork())){
		perror("Eroare la fork!");
		return -1;
	}
	else if(pid_fiu==0){
		//Primul fiu;
		ordin=0;
		level++;

		for(int j=1;j<6;j++){
			ordin++;
			if(-1 == (pid_fiu=fork())){
				perror("Eroare la fork!");
				return -1;
			}
			if(pid_fiu==0){
					level++;
					printf("(%d,%d), PID:%d, PPID:%d\n",level,ordin,getpid(),getppid());
					return 2;
			}
			waitpid(-1,&status,0);
			statuses[j]=WEXITSTATUS(status);
			pids[j]=pid_fiu;
		}
		waitpid(-1,&status,0);
		printf("(%d,%d), PID:%d, PPID:%d, PIDFII: ",level,1,getpid(),getppid());
		for(int i = 1; i<6; i++){
			printf("%d, EXITCODE:%d; ",pids[i],statuses[i]);
		}
		printf("\n");
		return 1;
	}
	pids[0]=pid_fiu;
	waitpid(-1,&status,0);
	statuses[0]=WEXITSTATUS(status);

	if(-1 == (pid_fiu=fork())){
		perror("Eroare la fork!");
		return -1;
	}
	else if(pid_fiu==0){
		//Al doilea fiu;
		ordin=5;
		level++;

		for(int j=1;j<6;j++){
			ordin++;
			if(-1 == (pid_fiu=fork())){
				perror("Eroare la fork!");
				return -1;
			}
			if(pid_fiu==0){
					level++;
					printf("(%d,%d), PID:%d, PPID:%d\n",level,ordin,getpid(),getppid());
					return 2;
			}
			waitpid(-1,&status,0);
			statuses[j]=WEXITSTATUS(status);
			pids[j]=pid_fiu;
		}
		waitpid(-1,&status,0);
		printf("(%d,%d), PID:%d, PPID:%d, PIDFII: ",level,1,getpid(),getppid());
		for(int i = 1; i<6; i++){
			printf("%d, EXITCODE:%d; ",pids[i],statuses[i]);
		}
		printf("\n");

		return 1;
	}
	pids[1]=pid_fiu;
	waitpid(-1,&status,0);
	statuses[1]=WEXITSTATUS(status);
	if(pid_fiu!=0){ //parinte
		printf("(%d,%d), PID:%d, PPID:%d, PIDFII: %d, EXITCODE:%d; %d, EXITCODE:%d \n",level,1,getpid(),getppid(),pids[0],statuses[0],pids[1],statuses[1]);
	}
	return 0;
}
