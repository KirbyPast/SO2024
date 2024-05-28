#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void bubblesort(int fd);

int main(int argc, char* argv[]){
	int fd;

	if(argc!=2){
		printf("Usage: %s [FILE_NAME]",argv[0]);
		exit(1);
	}

	if(-1==(fd=open(argv[1],O_RDWR))){
		printf("Eroare la deschiderea fisierului!");
		exit(2);
	}

	bubblesort(fd);

	close(fd);

	printf("Am terminat!");
	return 0;
}

void bubblesort(int fd){

	int rcod1,rcod2;
	int numar1,numar2;

	int modificare=1;

	while(modificare){
		modificare=0;

		while(1){
			rcod1=read(fd,&numar1,sizeof(int));
			if(rcod1==-1) { printf("Eroare la citire!"); exit(3); }
			if(rcod1==0) {break;}

			rcod2=read(fd,&numar2,sizeof(int));
			if(rcod2==-1) {printf("Eroare la citire!"); exit(4); }
			if(rcod2==0) {break;}

			if(numar1>numar2){
				modificare=1;

				if(-1==lseek(fd,-2*sizeof(int),SEEK_CUR)){
					printf("Eroare la repozitionarea cursorului!");
					exit(5);
				}

				if(-1==write(fd,&numar2,sizeof(int))){
					printf("Eroare la scrierea numarului mai mic!");
					exit(6);
				}

				if(-1==write(fd,&numar1,sizeof(int))){
					printf("Eroare la scrierea numarului mai mare!");
					exit(7);
				}
			}

			if(-1==lseek(fd,-sizeof(int),SEEK_CUR)){
				printf("Eroare la repozitionarea cursorului!");
				exit(8);
			}
		}

		if(-1==lseek(fd,0L,SEEK_SET)){
			printf("Eroare la repozitionarea cursorului!");
			exit(9);
		}
	}
}
