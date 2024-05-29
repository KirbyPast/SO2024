#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
int main(int argc, char* argv[]){

	if(argc != 2){
		printf("Usage: ./text_stats [filename]");
		exit(8);
	}

	//Creare pipe-uri:

	int pipe_0to1[2], pipe_2to0[2];

	if(-1 == pipe(pipe_0to1)){
		perror("Eroare la crearea pipe-ului intre 0 si 1!");
		exit(2);
	}

	if(-1 == pipe(pipe_2to0)){
		perror("Eroare la crearea pipe-ului intre 2 si 0!");
		exit(3);
	}

	if(-1 == mkfifo("pipe_1to2",0600)){
		if(errno != EEXIST){
			perror("Eroare la crearea pipe-ului intre 1 si 2!");
			exit(4);
		}
	}

	//Pregatim pentru fork:
	char ch;
	pid_t pid1, pid2;
	int bytes_read;
	if(-1 == (pid1 = fork())){
		perror("Eroare la crearea primului fiu!");
		exit(5);
	}

	if(pid1==0){
		//ZONA EXECUTATA DE PRIMUL FIU ( CEL CARE CA SELECTA DOAR CARACTERELE 0 ... F )
		int fdW,fdR;
		close(pipe_0to1[1]);
		close(pipe_2to0[0]);
		close(pipe_2to0[1]);

		if(-1 == (fdW =  (open("pipe_1to2",O_WRONLY)))){
			perror("Eroare la deschiderea comunicatiei!");
			exit(10);
		}

		if(-1 == (fdR = (open("pipe_1to2",O_RDONLY)))){
			perror("Eroare la test1");
			exit(11);
		}


		while(read(pipe_0to1[0],&ch,sizeof(char))>0){
			if(ch>='a' && ch<='f'){
				ch-=32;
			}
			if((ch>='0' && ch<='9') || (ch>='A' && ch<='F')){
				write(fdW,&ch,sizeof(char));
			}
		}
		close(fdR);
		close(pipe_0to1[0]);
		close(fdW);
		exit(12);

	}
	else{
		if(-1 == (pid2 = fork())){
			perror("Eroare la crearea celui de-al doilea fiu!");
			exit(6);
		}

		if(pid2 == 0){
			//ZONA EXECUTATA DOAR DE AL DOILEA FIU ( CEL CARE II VA TRANSMITE TATALUI NR DE CARACTERE SI CARE VA SCRIE IN FISIER COUNT-URILE )
			close(pipe_0to1[0]);
			close(pipe_0to1[1]);
			close(pipe_2to0[0]);
			int fdR;
			int fdDestinatie;
			int vectorAparitii[50];
			if( -1 == (fdR = open("pipe_1to2",O_RDONLY))){
				perror("Eroare la deschiderea comunicatiei!");
				exit(13);
			}

			if( -1 == (fdDestinatie = open("statistics.txt",O_WRONLY | O_TRUNC | O_CREAT, 0600))){
				perror("Eroare la deschiderea fisierului statistica!");
				exit(15);
			}
			for(int i = 0; i<=16; i++){
				vectorAparitii[i]=0;
			}

			while(read(fdR,&ch,sizeof(char))>0){
				fflush(stdout);
				if(ch>='0' && ch<='9'){
					vectorAparitii[(int)(ch-48)]++;
				}
				if(ch>='A' && ch<='F'){
					vectorAparitii[(int)(ch-55)]++;
				}
			}
			char template[2]="0:";
			int count = 0;
			char buf;
			char line = '\n';
			for(int i = 0; i<=9;i++){
				template[0] = (char)(i+48);
				write(fdDestinatie,&template,2*sizeof(char));

				//Add to counter

				if(vectorAparitii[i]) count++;

				//Convert vectorAparitii to a string
				int mirror= 0;
				while(vectorAparitii[i]){
					mirror = vectorAparitii[i]%10 + mirror*10;
					vectorAparitii[i]/=10;
				}
				if(mirror == 0) { buf = '0'; write(fdDestinatie, &buf, sizeof(char)); }
				while(mirror){
					buf = (char)(mirror%10 + 48);
					mirror/=10;
					write(fdDestinatie,&buf,sizeof(char));
				}

				write(fdDestinatie,&line,sizeof(char));
			}
			for(int i = 10; i<=15; i++){
				template[0] = (char)(i + 55);

				write(fdDestinatie,&template,2*sizeof(char));

				if(vectorAparitii[i]) count++;

				int mirror = 0;
				while(vectorAparitii[i]){
					mirror=vectorAparitii[i]%10 + mirror*10;
					vectorAparitii[i]/=10;
				}
				if(mirror == 0) { buf = '0'; write(fdDestinatie, &buf, sizeof(char)); }
				while(mirror){
					buf = (char)(mirror % 10 + 48);
					mirror/=10;
					write(fdDestinatie,&buf,sizeof(char));
				}

				write(fdDestinatie, &line, sizeof(char));
			}

			if(count == 0 ){ buf = '0'; write(pipe_2to0[1],&buf, sizeof(char)); }
			else {
				int mirror = 0;
				while(count){
					mirror=count%10 + mirror*10;
					count/=10;
				}


				while(mirror){
					buf = (char)(mirror % 10 + 48);
					mirror/=10;
					write(pipe_2to0[1], &buf, sizeof(char));
				}
			}
			close(fdR);
			close(fdDestinatie);
			close(pipe_2to0[1]);
			exit(14);
		}
		else{
			//ZONA EXECUTATA DOAR DE TATA ( CEL CARE VA CITI FISIERUL SI CARE VA AFISA NR DE CARACTERE UNICE )
			close(pipe_2to0[1]);
			close(pipe_0to1[0]);

			int fd;

			if( -1 == (fd = open(argv[1],O_RDWR))){
				perror("Eroare la deschiderea fisierului cu informatii!");
				exit(7);
			}

			while(read(fd,&ch,sizeof(char))){
				write(pipe_0to1[1],&ch,sizeof(char));
			}
			close(pipe_0to1[1]);
			char buf;
			printf("Numarul total de caractere hexadecimale unice este de: ");
			while(read(pipe_2to0[0],&buf,sizeof(char))){
				printf("%c",buf);
			}
			printf("\n");
			close(pipe_2to0[0]);
			close(pipe_2to0[1]);
			close(fd);

			wait(NULL);
			wait(NULL);

			exit(11);
		}
	}


	return 0;
}
