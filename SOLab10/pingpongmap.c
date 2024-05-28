#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h> 
 
void dialog_tata(char* fisier, char* shared)
{
	// TODO: de implementat bucla dialogului pentru tata
	int bytes_read=2;
	int fd2=0;
	char replica;
	if(-1==(fd2=open(fisier,O_RDONLY))){
		perror("Eroare la deschiderea fisierului! (tata) \n");
		return;
	}
 
	do{
		//message=1;
 
		//Trecem peste newline
 
		bytes_read = read(fd2,&replica,sizeof(char));
		if(-1==bytes_read){
			perror("Eroare la citirea replicii!");
			return;
		}
 
		//Daca am citit EOF, trimit mesajul 2
 
		if(0==bytes_read){
 

			shared[0]='2';
			msync(shared,sizeof(char),MS_SYNC);
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
 		/*
		}*/
		shared[0]='1';
		msync(shared,sizeof(char),MS_SYNC);
//		sleep(1);
		//Astept sa primesc semnalul de la fiu
 
		do{
		/*
		*/
//			sleep(1);
			if(shared[0]=='2') break;
		}while(shared[0]!='0');
		if(shared[0]=='2') break;
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
 
 
 
void dialog_fiu(char* fisier, char* shared)
{
	// TODO: de implementat bucla dialogului pentru tata
	int bytes_read=2;
	int fd2=0;
	char replica;
 
 
//	usleep(200000);
	if(-1==(fd2=open(fisier,O_RDONLY))){
		perror("Eroare la deschiderea fisierului! (fiu) \n");
		return;
	}
 
	do{
 
		//Incep prin a astepta mesajul de la tata
 
		do{
			if(shared[0]=='2') break;
		}while(shared[0]!='1');
		if(shared[0]=='2') break;

		//Apoi sar peste newline

		bytes_read=read(fd2,&replica,sizeof(char));
		if(-1==bytes_read){
			perror("Eroare la citirea replicii!");
			return;
		}

		//Daca gasesc EOF trimit mesajul 2
		if(0==bytes_read){
			shared[0]='2';
			msync(shared,sizeof(char),MS_SYNC);
			return;
		}

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
		shared[0]='0';
		msync(shared,sizeof(char),MS_SYNC);
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

 	char* shared;
	shared=mmap(NULL,sizeof(char),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);

	if(shared == MAP_FAILED){
		perror("Eroare la crearea mapei!");
		return -1;
	}

	if(argc!=3){
		printf("Utilizare: ./pingpong1 [Fisier_Replici_Tata] [Fisier_Replici_Fiu] \n");
		return 0;
	}

	/* Crearea unui proces fiu. */
	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
	}

	/* Ramificarea execuției în cele două procese, tată și fiu. */
	if(pid_fiu == 0)
	{   /* Zona de cod executată doar de către fiu. */

		dialog_fiu(argv[2],shared);
	}
	else
	{   /* Zona de cod executată doar de către părinte. */

		dialog_tata(argv[1],shared);
	}
	munmap(shared,sizeof(char));
	/* Zona de cod comună, executată de către ambele procese */
	printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
	return 0;
}
