#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define ASCII_SET_SIZE 256
#define BLOCK_SIZE 4096

unsigned char buffer[BLOCK_SIZE];

char nume_fisier[256];

int fd, bytes_read, i, count[ASCII_SET_SIZE];

int main(int argc, char* argv[]){

if(argc==1){
	//perror("Specificati fisierul din care trebuie facuta citirea!");
	//return 1;
	printf("Specificati fisierul din care trebuie facuta citirea: ");
	scanf("%s",nume_fisier);
}
else{
	strcpy(nume_fisier,argv[1]);
}

if(-1 == (fd = open(nume_fisier,O_RDONLY))){
	perror("Eroare la deschidere!");
	return 2;
}

do{

	bytes_read=read(fd,buffer,BLOCK_SIZE);

	if(bytes_read==0) break; //EOF

	if(bytes_read==-1) {
		perror("Eroare la citire!");
		return 3;
	}

	for(i=0;i<bytes_read;i++){
		count[buffer[i]]++;
	}
}
while(1);

for(i=0;i<ASCII_SET_SIZE;i++){
	if(count[i]!=0&&i!=10){
		printf("Caracterul %c apare de %d ori \n",i,count[i]);
	}
}
return 0;
}
