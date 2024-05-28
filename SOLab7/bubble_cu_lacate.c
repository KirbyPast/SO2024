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
	int numar1_recitit,numar2_recitit;
	int modificare=1;

	struct flock lacat_blocaj, lacat_deblocaj;

	lacat_blocaj.l_type=F_WRLCK;
	lacat_blocaj.l_whence=SEEK_CUR;
	lacat_blocaj.l_start=0;
	lacat_blocaj.l_len=2*sizeof(int);

	lacat_deblocaj.l_type=F_UNLCK;
	lacat_deblocaj.l_whence=SEEK_CUR;
	lacat_deblocaj.l_start=-2*sizeof(int);
	lacat_deblocaj.l_len=2*sizeof(int);

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

				if(-1==lseek(fd,-2*sizeof(int),SEEK_CUR)){
					printf("Eroare la repozitionarea cursorului!");
					exit(10);
				}

				if(-1==fcntl(fd,F_SETLKW,&lacat_blocaj)){
					printf("Eroare la punerea lacatului!");
					exit(11);
				}

				if(-1==read(fd,&numar1_recitit,sizeof(int))){
					printf("Eroare la recitirea primului numar!");
					exit(12);
				}

				if(-1==read(fd,&numar2_recitit,sizeof(int))){
					printf("Eroare la recitirea celui de-al doilea numar!");
					exit(13);
				}

				if(numar1_recitit>numar2_recitit){
					modificare=1;
					if(-1==lseek(fd,-2*sizeof(int),SEEK_CUR)){
						printf("Eroare la repozitionarea cursorului!");
						exit(5);
					}

					if(-1==write(fd,&numar2_recitit,sizeof(int))){
						printf("Eroare la scrierea numarului mai mic!");
						exit(6);
					}

					if(-1==write(fd,&numar1_recitit,sizeof(int))){
						printf("Eroare la scrierea numarului mai mare!");
						exit(7);
					}
				}

				if(-1==fcntl(fd,F_SETLKW,&lacat_deblocaj)){
					printf("Eroare la eliminarea lacatului!");
					exit(14);
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
