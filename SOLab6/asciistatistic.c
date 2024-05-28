#include <stdio.h>
#include <linux/limits.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define ASCII_SET_SIZE 256

int main(int argc, char* argv[]){
	char nume_fisier[PATH_MAX];

	if(argc==1){
		printf("Dati numele fisierului de prelucrat: ");
		scanf("%s",nume_fisier);
	}
	else{
		strcpy(nume_fisier,argv[1]);
	}

	int vector_ap[ASCII_SET_SIZE], i;
	for(i=0;i<ASCII_SET_SIZE;i++){
		vector_ap[i]=0;
	}

	int fd;

	fd=open(nume_fisier,O_RDONLY);
	if(fd==-1){
	perror("Eroare la deschiderea fisierului!");
	exit(2);
	}

	unsigned char ch;
	int cod_r;

	do{
		cod_r=read(fd,&ch,1);

		if(cod_r==-1){
			perror("Eroare la citire");
			exit(3);
		}
		if(cod_r==0){ //EOF
			break;
		}
		vector_ap[ch]++;
	}
	while(1);

	if(close(fd)==-1){
		perror("Eroare la inchidere");
		exit(4);
	}

	for(i=0;i<ASCII_SET_SIZE;i++){
		if(vector_ap[i]!=0){
			printf("Caracterul %c cu codul ascii %d apare de %d ori in fisier \n",i,i,vector_ap[i]);
		}
	}
	return 0;
}
