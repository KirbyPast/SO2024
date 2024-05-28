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
//		if( -1 == (dup2(pipe_0to1[0],0))){
//			perror("Eroare la redirectionarea input-ului!");
//			exit(10);
//		}
		int fdW;
		if(-1 == (fdW =  (open("pipe_1to2",O_WRONLY)))){
			perror("Eroare la deschiderea comunicatiei!");
			exit(10);
		}

//		while(read(pipe_0to1[0],&ch,sizeof(char))>0){
//			if(ch>='a' && ch<='f'){
//				ch-=32;
//			}
//			if((ch>='0' && ch<='9') || (ch>='A' && ch<='F')){
//				write(fdW,&ch,sizeof(char));
//			}
//		}


		close(pipe_0to1[0]);
		close(fdW);
		printf("babaababa");
		fflush(stdout);
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

//			while(read(fdR,&ch,sizeof(char))>0){
//				printf("%c",ch);
//				fflush(stdout);
//				if(ch>='0' && ch<='9'){
//					vectorAparitii[(int)(ch-30)]++;
//				}
//				if(ch>='A' && ch<='F'){
//					vectorAparitii[(int)(ch-55)]++;
//				}
//			}
			printf("Am ajuns aici");
			fflush(stdout);
			for(int i = 0; i<=9;i++){
				printf("%c:%d",(char)(i+48),vectorAparitii[i]);
			}
			for(int i = 10; i<=15; i++){
				printf("%c:%d",(char)(i+55),vectorAparitii[i]);
			}
			close(fdR);
			close(fdDestinatie);
			close(pipe_2to0[1]);
			exit(14);
		}
		else{
			//ZONA EXECUTATA DOAR DE TATA ( CEL CARE VA CITI FISIERUL SI CARE VA AFISA NR DE CARACTERE UNICE )
			int fd;
			if( -1 == (fd = open(argv[1],O_RDWR))){
				perror("Eroare la deschiderea fisierului cu informatii!");
				exit(7);
			}

//			if( -1 == (dup2(pipe_0to1[1],1))){
//				perror("Eroare la redirectionarea output-ului!");
//				exit(9);
//			}

			while(read(fd,&ch,sizeof(char))){
				write(pipe_0to1[1],&ch,sizeof(char));
			}

			close(pipe_0to1[1]);
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
