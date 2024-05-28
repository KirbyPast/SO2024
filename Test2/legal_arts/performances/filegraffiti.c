#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	if(argc!=3) {
		printf("Argumente gresite!");
		exit(2);
	}
	char* nume_fisier;
	nume_fisier = argv[1];
	char* cuvant_graffiti;
	cuvant_graffiti = argv[2];
	int fd, cod_r;
	if(-1==(fd=open(nume_fisier,O_RDWR))){
		printf("Eroare la deschiderea fisierului %s! \n", nume_fisier);
		exit(3);
	}

	cod_r=lseek(fd,-strlen(cuvant_graffiti),SEEK_END);
	if(cod_r==-1){
		printf("CANVAS TOO SMALL \n");
		exit(4);
	}
	if(-1==(write(fd,cuvant_graffiti,strlen(cuvant_graffiti)))){
		printf("Eroare la vandalizare! \n");
		exit(5);
	}
	else{
		printf("FILE VANDALIZED: %s \n",nume_fisier);
	}
	return 0;
}
