#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


//PUNE UN STRUCT.
struct produs{
	int cod, cantitate;
}baza, alt;


int main(int argc, char* argv[]){

	if(argc==1){
		printf("Usage: %s [fisier1.bin] [fisier2.bin]",argv[0]);
		exit(1);
	}

	baza.cod = -1; baza.cantitate=-1;
	alt.cod=-1; alt.cantitate=-1;

	int cod_r;
	int fdbase, fdalt;

	fdbase=open(argv[1],O_RDWR);
	fdalt=open(argv[2],O_RDONLY);

	struct flock lock,unlock;

	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_CUR;
	lock.l_len=sizeof(int);
	lock.l_start=-sizeof(int);

	unlock.l_type=F_UNLCK;
	unlock.l_whence=SEEK_CUR;
	unlock.l_len=sizeof(int);
	unlock.l_start=-sizeof(int);



	while(1){
		cod_r=read(fdalt,&alt.cod,sizeof(int));
		if(cod_r==0) break; //EOF
		cod_r=read(fdalt,&alt.cantitate,sizeof(int));
		if(cod_r==0) break;

		while(baza.cod!=alt.cod){
			read(fdbase,&baza.cod,sizeof(int));
			read(fdbase,&baza.cantitate,sizeof(int));
		}

		if(baza.cod!=alt.cod) printf("Eroare! codul %d nu a fost gasit in baza de date!", alt.cod);
		if(baza.cod==alt.cod){
			fcntl(fdbase,F_SETLKW,&lock);

			lseek(fdbase,-sizeof(int),SEEK_CUR);
			read(fdbase,&baza.cantitate,sizeof(int));

			baza.cantitate+=alt.cantitate;

			lseek(fdbase,-sizeof(int),SEEK_CUR);
			write(fdbase,&baza.cantitate,sizeof(int));

			fcntl(fdbase,F_SETLKW,&unlock);
		}
		lseek(fdbase,0L,SEEK_SET);
	}


	return 0;
}
