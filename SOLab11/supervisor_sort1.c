#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#define SHM_NAME_TEMPLATE "/shm_object%d"
#define MAX_FIU_COUNT 256

int main(int argc, char* argv[]){
	const int maxSize = 4096;
	if(argc!=3){
		printf("Usage: [name] [number] [file]");
		return -1;
	}

	int i = 0;
	int n = 0;
	while(argv[1][i]!=0){
		n=(int)argv[1][i]-48+n*10;
		i++;
	}

	//Crearea celor N mape.

 	char shm_name[256]="/shm_object0";
 	int shm_fd[MAX_FIU_COUNT];
	char* shm_ptr[MAX_FIU_COUNT];
	for(int i = 0; i<n;i++){
		shm_name[11]=(char)(i + 48);
		shm_fd[i]=shm_open(shm_name,O_CREAT|O_RDWR, 0666);
		if(-1==shm_fd[i]) {
			perror("Eroare la crearea unei mape!");
			return -1;
		}
		if(-1==ftruncate(shm_fd[i],maxSize)){
			perror("Eroare la truncate!");
			return -1;
		}
		shm_ptr[i]=mmap(NULL,maxSize,PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd[i],0);
	}

	//MAPAM IN MEMORIE FISIERUL DAT

	int file_fd;
	char* file_map;

	file_fd=open(argv[2],O_RDONLY);

	file_map=mmap(NULL,maxSize,PROT_READ,MAP_PRIVATE,file_fd,0);

	//NUMARAM NUMARUL DE CARACTERE NEWLINE

	int count = 0;
	for(int i = 0; file_map[i]!=0; i++){
		if(file_map[i]==10){
			count++;
		}
	}


	//Calculam lungimea fiecarui segment

	int avg_length=0;
	int last_length=0;

	if(count%n==0){
		avg_length = count/n;
	}
	else{
		avg_length = count/n;
		last_length = avg_length+count%n;
	}

	printf("%d, %d\n", avg_length, last_length);

	//Calculam offset-ul fiecarui fiu

	int offset[256];
	offset[0]=0;
	for(int i = 1; i<n; i++){
		offset[i]=offset[i-1]+avg_length;
	}

	//Pregatirea maparilor pentru cei n fii
	//Primele 3 caractere offset, urmatoarele 3 lungimea, restul numele fisierului.
	for(int i = 0; i<n; i++){
		int j = offset[i];
		int l = 2;
		//Cand offset-ul este 0 (adica in primul fiu)
		if(j==0){
			shm_ptr[i][l]='0';
		}
		while(j){
			shm_ptr[i][l]=(char)((j%10)+48);
			j/=10;
			l--;
		}

		j = avg_length;
		l = 5;
		while(j){
			shm_ptr[i][l]=(char)(j%10)+48;
			j/=10;
			l--;
		}

		if(i==n-1 && last_length!=0){
			j = last_length;
			l=5;
			while(j){
				shm_ptr[i][l]=(char)(j%10) + 48;
				j/=10;
				l--;
			}
		}

		l = 6;
		j = 0;
		while(argv[2][j]!=0){
			shm_ptr[i][l]=argv[2][j];
			j++;
			l++;
		}
		shm_ptr[i][l]=0;
	}

	//Testare Mapari pt fii

	for(int j=0;j<n;j++){
		for(int i = 0; i<maxSize; i++){
			printf("%c",shm_ptr[j][i]);
//			if(shm_ptr[j][i]==0) printf("0");
		}
	printf("\n");
	}

	//Crearea celor N fii
	pid_t pid_fiu[MAX_FIU_COUNT];
	for(int i = 0; i<n; i++){
		pid_fiu[i] = fork();
		sleep(1);
		if(pid_fiu[i] == 0){
			shm_name[11] = (char)(i + 48);
			execlp("./worker_sort1","./worker_sort1",shm_name,NULL);
			return 0;
		}
	}
	int status=0;
	while(wait(&status)!=-1);
	int indexes[MAX_FIU_COUNT];
	int nextLine;
	char compChar[maxSize];
	/*for(int j = 0; j<n; j++){
		for(int i = 0; i<maxSize; i++){
			printf("%c",shm_ptr[j][i]);
		}
	printf("\n");
	}*/
	/*while(nextLine!=-1){
		nextLine=-1;
		compChar[0]=0;
		for( i=0; i<n; i++){
			int l = indexes[i];
			while(compChar[l] == shm_ptr[i][l] && shm_ptr[i][l] != '\n') { l++; }
			if(compChar[l] < shm_ptr[i][l] && shm_ptr[i][l] != '\n'){
				l = indexes[i];
				while( shm_ptr[i][l] != '\n' ){
					compChar[l] = shm_ptr[i][l];
					l++;
				}
				nextLine = i;
			}
			while(shm_ptr[nextLine][indexes[nextLine]]!='\n' && nextLine!=-1){
				printf("%c",shm_ptr[nextLine][indexes[nextLine]]);
				indexes[nextLine]++;
			}
			printf("\n");
			indexes[nextLine]++;
		}
	}*/


	for(i=0; i<n; i++){
		for(int j=0; j<maxSize;j++){
			printf("%c",shm_ptr[i][j]);
		}
	}


	munmap(file_map,maxSize);
	close(file_fd);

	for(int i =0; i<n;i++){
		printf("a");
		for(int j = 0; j<maxSize;j++){
			shm_ptr[i][j]=0;
		}
		if(-1 == munmap(shm_ptr[i],maxSize)){
			perror("nu iese");
		}
		close(shm_fd[i]);
	}
	return 0;
}
