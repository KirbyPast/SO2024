#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void dialog_tata(char* fisier, int fdbin)
{
	// TODO: de implementat bucla dialogului pentru tata


	struct flock lacat_blocaj_r, lacat_blocaj_w, lacat_deblocaj;

	lacat_blocaj_r.l_type=F_WRLCK;
	lacat_blocaj_r.l_whence=SEEK_SET;
	lacat_blocaj_r.l_start=0;
	lacat_blocaj_r.l_len=sizeof(int);

	lacat_blocaj_w.l_type=F_RDLCK;
	lacat_blocaj_w.l_whence=SEEK_SET;
	lacat_blocaj_w.l_start=0;
	lacat_blocaj_w.l_len=sizeof(int);

	lacat_deblocaj.l_type=F_UNLCK;
	lacat_deblocaj.l_whence=SEEK_SET;
	lacat_deblocaj.l_start=0;
	lacat_deblocaj.l_len=sizeof(int);

	int bytes_read=2;
	int flag=1;
	int doneflag=2;
	int fd2=0;
	int message=1;
	char replica;
	if(-1==(fd2=open(fisier,O_RDONLY))){
		perror("Eroare la deschiderea fisierului! (tata) \n");
		return;
	}

	do{
		message=1;

		//Trecem peste newline

		bytes_read = read(fd2,&replica,sizeof(char));
		if(-1==bytes_read){
			perror("Eroare la citirea replicii!");
			return;
		}

		//Daca am citit EOF, trimit mesajul 2

		if(0==bytes_read){

			if(-1==fcntl(fdbin,F_SETLKW,&lacat_blocaj_w)){
				perror("Eroare la setarea lacatului! \n");
				return;
			}

			if(-1==lseek(fdbin,0,SEEK_SET)){
				perror("Eroare la repozitionarea cursorului! \n");
				return;
			}

			if(-1==write(fdbin,&doneflag,sizeof(int))){
				perror("Eroare la actualizarea flag-ului! \n");
				return;
			}

			if(-1==fcntl(fdbin,F_SETLKW,&lacat_deblocaj)){
				perror("Eroare la scoaterea lacatului! \n");
				return;
			}
			return;

		}

		//Altfel scriu replica

		while(replica!=10){
			printf("%c",replica);
			if(-1==(read(fd2,&replica,sizeof(char)))){
				perror("Eroare la citirea replicii! (tata) \n");
				return;
			}
		}
		printf("\n");

		//Dupa ce am scris replica, pun un lacat pe fisier si actualizez mesajul

		if(-1==fcntl(fdbin,F_SETLKW,&lacat_blocaj_w)){
			perror("Eroare la punerea lacatului! \n");
			return;
		}

		if(-1==lseek(fdbin,0,SEEK_SET)){
			perror("Eroare la repozitionarea cursorului! \n");
			return;
		}

		if(-1==write(fdbin,&flag,sizeof(int))){
			perror("Eroare la actualizarea flag-ului! \n");
			return;
		}

		if(-1==fcntl(fdbin,F_SETLKW,&lacat_deblocaj)){
			perror("Eroare la scoaterea lacatului! \n");
			return;
		}
//		sleep(1);
		//Astept sa primesc semnalul de la fiu

		do{
			if(-1==fcntl(fdbin,F_SETLKW,&lacat_blocaj_r)){
				perror("Eroare la punerea lacatului! \n");
				return;
			}

			if(-1==lseek(fdbin,0,SEEK_SET)){
				perror("Eroare la repozitionarea cursorului! \n");
				return;
			}
			if(-1==read(fdbin,&message,sizeof(int))){
				perror("Eroare la citirea din fisierul bin! \n");
				return;
			}

			if(-1==fcntl(fdbin,F_SETLKW,&lacat_deblocaj)){
				perror("Eroare la scoaterea lacatului! \n");
				return;
			}
//			sleep(1);
			if(message==2) break;
		}while(message!=0);
		if(message==2) break;
	}while(1);

	//Daca am iesit si mai am replici de zis, le zic

	while(bytes_read){
		printf("%c",replica);
		bytes_read=read(fd2,&replica,sizeof(char));
		if(-1==bytes_read){
			perror("Eroare la citirea replicii! (tata) \n");
			return;
		}
	}
	close(fd2);
	return;
	 // Scrieți două bucle imbricate, conform indicațiilor cuprinse în diagramă!
}



void dialog_fiu(char* fisier, int fdbin)
{
	// TODO: de implementat bucla dialogului pentru tata
	int bytes_read=2;
	int flag=0;
	int doneflag=2;
	int fd2=0;
	int message=0;
	char replica;


	struct flock lacat_blocaj_r, lacat_blocaj_w, lacat_deblocaj;

	lacat_blocaj_r.l_type=F_WRLCK;
	lacat_blocaj_r.l_whence=SEEK_SET;
	lacat_blocaj_r.l_start=0;
	lacat_blocaj_r.l_len=sizeof(int);

	lacat_blocaj_w.l_type=F_RDLCK;
	lacat_blocaj_w.l_whence=SEEK_SET;
	lacat_blocaj_w.l_start=0;
	lacat_blocaj_w.l_len=sizeof(int);

	lacat_deblocaj.l_type=F_UNLCK;
	lacat_deblocaj.l_whence=SEEK_SET;
	lacat_deblocaj.l_start=0;
	lacat_deblocaj.l_len=sizeof(int);

//	usleep(200000);
	if(-1==(fd2=open(fisier,O_RDONLY))){
		perror("Eroare la deschiderea fisierului! (fiu) \n");
		return;
	}

	do{
		message=0;

		//Incep prin a astepta mesajul de la tata

		do{
			if(-1==fcntl(fdbin,F_SETLKW,&lacat_blocaj_r)){
				perror("Eroare la punerea lacatului! \n");
				return;
			}
			if(-1==lseek(fdbin,0,SEEK_SET)){
				perror("Eroare la repozitionarea cursorului! \n");
				return;
			}
			if(-1==read(fdbin,&message,sizeof(int))){
				perror("Eroare la citirea din fisierul bin! \n");
				return;
			}

			if(-1==fcntl(fdbin,F_SETLKW,&lacat_deblocaj)){
				perror("Eroare la scoaterea lacatului! \n");
				return;
			}
//			sleep(1);
			if(message==2) break;
		}while(message!=1);
		if(message==2) break;

		//Apoi sar peste newline

		bytes_read=read(fd2,&replica,sizeof(char));
		if(-1==bytes_read){
			perror("Eroare la citirea replicii!");
			return;
		}

		//Daca gasesc EOF trimit mesajul 2
		if(0==bytes_read){
			if(-1==fcntl(fdbin,F_SETLKW,&lacat_blocaj_w)){
				perror("Eroare la setarea lacatului! \n");
				return;
			}

			if(-1==lseek(fdbin,0,SEEK_SET)){
				perror("Eroare la repozitionarea cursorului! \n");
				return;
			}
			if(-1==write(fdbin,&doneflag,sizeof(int))){
				perror("Eroare la actualizarea flag-ului! \n");
				return;
			}
			if(-1==fcntl(fdbin,F_SETLKW,&lacat_deblocaj)){
				perror("Eroare la scoaterea lacatului! \n");
				return;
			}

			return;
		}
//		sleep(1);

		//Scriu replica

		while(replica!=10){
			printf("%c",replica);
			if(-1==(read(fd2,&replica,sizeof(char)))){
				perror("Eroare la citirea replicii! (fiu) \n");
				return;
			}
		}
		printf("\n");

		//Actualizez flag-ul pentru tata

		if(-1==fcntl(fdbin,F_SETLKW,&lacat_blocaj_w)){
			perror("Eroare la punerea lacatului! \n");
			return;
		}

		if(-1==lseek(fdbin,0,SEEK_SET)){
			perror("Eroare la repozitionarea cursorului! \n");
			return;
		}

		if(-1==write(fdbin,&flag,sizeof(int))){
			perror("Eroare la actualizarea flag-ului! \n");
			return;
		}

		if(-1==fcntl(fdbin,F_SETLKW,&lacat_deblocaj)){
			perror("Eroare la scoaterea lacatului! \n");
			return;
		}


	}while(1);


	while(bytes_read){
		printf("%c",replica);
		bytes_read=read(fd2,&replica,sizeof(char));
		if(-1==bytes_read){
			perror("Eroare la citirea replicii! (tata) \n");
			return;
		}
	}
	close(fd2);
	return;
}

int main(int argc, char* argv[])
{
	pid_t pid_fiu;

	// TODO: de inițializat cu 0 fișierul flag.bin

	int fd;

	if(argc!=3){
		printf("Utilizare: ./pingpong1 [Fisier_Replici_Tata] [Fisier_Replici_Fiu] \n");
		return 0;
	}

	if(-1==(fd=open("flag.bin",O_RDWR))){
		perror("Eroare la deschiderea fisierului!");
		return -1;
	}

	int flag=0;

	if(-1==(write(fd,&flag,sizeof(int)))){
		perror("Eroare la initializarea fisierului!");
		return -2;
	}

	/* Crearea unui proces fiu. */
	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
	}

	/* Ramificarea execuției în cele două procese, tată și fiu. */
	if(pid_fiu == 0)
	{   /* Zona de cod executată doar de către fiu. */

		//printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid());
		dialog_fiu(argv[2],fd);
	}
	else
	{   /* Zona de cod executată doar de către părinte. */

		//printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());
		dialog_tata(argv[1],fd);
	}
	close(fd);
	/* Zona de cod comună, executată de către ambele procese */
	//printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
	return 0;
}



