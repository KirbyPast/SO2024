#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int fddate,fdop;

int main(int argc, char* argv[]){

char* nume_fisier1;
char* nume_fisier2;

if(argc==1){
	printf("Eroare: utilizare program: ./mycritsec4 [NUME_FISIER1] [NUME_FISIER2]");
	exit(1);
}
else{
	nume_fisier1=argv[1];
	nume_fisier2=argv[2];
}

if( -1 == (fddate = open(nume_fisier1,O_RDWR))){
	printf("Eroare la deschiderea primului fisier!");
}

if( -1 == (fdop = open(nume_fisier2,O_RDONLY))){
	printf("Eroare la deschiderea celui de-al doilea fisier");
}

int test;
int cod_r;
while(1){
cod_r=read(fddate,&test,sizeof(int));
if(-1 == cod_r){
	perror("Eroare la citire!");
	break;
}
if(0==cod_r){
	break;
}
printf("%d ",test);
}

return 0;

}
