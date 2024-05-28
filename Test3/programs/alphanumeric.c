#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
int main(int argc, char* argv[]){

	if(argc!=2){
		exit(2);
	}
	int fd;
	if(-1==(fd=open(argv[1],O_RDONLY))){
		exit(3);
	}
	struct stat fisier;
	if(-1==(stat(argv[1],&fisier))) exit(2);
	switch(fisier.st_mode & S_IFMT){
		case S_IFREG:
			break;
		default:
			printf("Nu este fisier!");
			exit(2);
			break;
	}
	char ch;
	int ok=1;
	int err;
	while(1){
		err=read(fd,&ch,sizeof(char));
		if(err==0) break;
		if(err==-1) exit(2);
		if(!isalpha(ch)&&ch!=' '&&ch!='\n'&&!isdigit(ch)) {  ok=0; }
	}
	if(ok==1) printf("%s:alphanumeric\n",argv[1]);
	else printf("%s:notalphanumeric\n",argv[1]);
	return 0;
}
