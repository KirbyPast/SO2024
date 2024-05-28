#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
int main(int argc, char* argv[]){
	const int maxSize = 4096;
	//argv[1][0]='3';
	int shm_fd;

	if(-1 == (shm_fd = shm_open(argv[1],O_RDWR,0666))){
		perror("Eroare la deschiderea mapei");
		return -1;
	}

	char* shm_ptr;

	if(MAP_FAILED == (shm_ptr = mmap(NULL,maxSize,PROT_READ | PROT_WRITE ,MAP_SHARED,shm_fd,0))){
		perror("Eroare la crearea mapei");
		return -1;
	}

	int lines_count = 0;
	int offset = 0;
	char filename[256];

	//Calculam offset-ul (primii 3 biti din mapare)

	int p = 100;
	int i = 0;
	while(p){
		if(shm_ptr[i] != 0){
			offset=(int)(shm_ptr[i]-48)*p+offset;
		}
		i++;
		p/=10;
	}

	//Calculam lungimea segmentului (bitii 3-5 din mapare)

	p=100;
	i=3;
	while(p){
		if(shm_ptr[i] != 0){
			lines_count = (int)(shm_ptr[i]-48)*p+lines_count;
		}
		i++;
		p/=10;
	}

	//Aflam numele fisierului ce trebuie sortat (restul de biti)

	i=6;
	while(shm_ptr[i]!=0){
		filename[i-6]=shm_ptr[i];
		//printf("%c",shm_ptr[i]);
		i++;
	}

	char text[100][256];
	int fd;

	if(-1 == (fd = open(filename,O_RDONLY))){
		perror("Eroare la deschiderea fisierului!");
		return -1;
	}
	char ch = 0;
	int count = 0;
	int no_offset = 0;
	int no_lines = 0;
	//ne mutam la offset in fisier

	while(count!=offset){
		if(-1 == read(fd,&ch,sizeof(char))){
			perror("Eroare la citire!");
			return -1;
		}
		if(ch==10){
			count++;
		}
		no_offset++;
	}

	//numaram linescount linii si le salvam

	i=0;
	int j = 0;
	while(count!=offset+lines_count){
		if(-1 == read(fd,&ch,sizeof(char))){
			perror("Eroare la citire!");
			return -1;
		}
		text[i][j++]=ch;
		if(ch==10){
			j=0;
			i++;
			count++;
		}
		no_lines++;
	}

	//mapam fisierul primit ca argument in memorie, pentru a incepe sortarea


	printf("%d, %d\n", no_offset, no_lines);

	lseek(fd, no_offset, SEEK_SET);


	long pageSize = sysconf(_SC_PAGE_SIZE);
	int no_offset_pages = (no_offset / pageSize) * pageSize;

	char* file_part = mmap(NULL,no_lines , PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, no_offset_pages);
	if(file_part == MAP_FAILED) {
		perror("Eroare la maparea fisierului!");
		return -1; 
	}

	char lines[256][256];
	j = 0;
	int k = 0;
	for(i = no_offset; i<= no_offset + no_lines; i++){
		lines[j][k]=file_part[i];
		k++;
		if(file_part[i] == '\n'){
			lines[j][k-1]='\0';
			k=0;
			j++;
		}
	}


	for(int i = 0; i<lines_count - 1; i++){
		for(int j = i+1; j<lines_count; j++){
			if(strcmp(lines[i],lines[j])<0){
				char temp[256];
				strcpy(temp, lines[i]);
				strcpy(lines[i],lines[j]);
				strcpy(lines[j],temp);
			}
		}
	}
	j=0;
	k=0;
	for(i = no_offset; i<= no_offset + no_lines; i++){
		file_part[i]=lines[j][k];
		if(lines[j][k] == '\0'){
			file_part[i] = '\n';
			k=0;
			j++;
		}
		else{
			k++;
		}
	}
	j=0;
	for(i = no_offset; i<=no_offset + no_lines; i++){
		shm_ptr[j++] = file_part[i];
	}
/*	for(i = 0; i <= no_offset + no_lines; i++){
		printf("%c",shm_ptr[i]);
	}
*/

	for(i =0; i<lines_count;i++){
		printf("%s\n",lines[i]);
	}



	printf("%d,%d,%d,%s\n",getpid(),offset,lines_count,filename);

	return 0;
}
