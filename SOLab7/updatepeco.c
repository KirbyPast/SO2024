#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

	int fd, i;
	float valoare, rezervor;

	struct flock block, unblock;

	block.l_type=F_WRLCK;
	block.l_whence=SEEK_CUR;
	block.l_len=sizeof(float);
	block.l_start=0;

	unblock.l_type=F_UNLCK;
	unblock.l_whence=SEEK_CUR;
	unblock.l_len=sizeof(float);
	unblock.l_start=0;

	if(argc==1){
		printf("Usage: ./updatepeco [lista_de_numere]");
		exit(1);
	}

	fd=open("peco.bin",O_RDWR);

	for(i=1;i<argc;i++){
		sscanf(argv[i], "%f", &valoare);
		lseek(fd,0L,SEEK_SET);
		fcntl(fd,F_SETLKW,&block);
		read(fd,&rezervor,sizeof(float));
		rezervor+=valoare;
		if(rezervor<0){
			printf("Eroare! nu exista destul combustibil in rezervor! \n");
			rezervor-=valoare;
		}
		lseek(fd,0L,SEEK_SET);
		write(fd,&rezervor,sizeof(float));
		fcntl(fd,F_SETLKW,&unblock);
		//sleep(1);
	}

	lseek(fd,0L,SEEK_SET);
	read(fd,&rezervor,sizeof(float));
	printf("%f \n",rezervor);
return 0;
}
