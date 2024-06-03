#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>



int main(int argc, char* argv[]){

	int fd;
	fd = open(argv[1],O_RDWR);
	int fd2;
	fd2= open(argv[2],O_RDWR | O_TRUNC);
	int buf;
	int bytes_read;
	while(bytes_read = read(fd,&buf,sizeof(int))){
		if(bytes_read!=sizeof(int)){
			break;
		}
		printf("%d\n",buf);
		write(fd2,&buf,sizeof(int));
	}



	return 0;
}
