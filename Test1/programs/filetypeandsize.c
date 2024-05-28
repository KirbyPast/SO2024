#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <libgen.h>
int main(int argc, char* argv[]){

if(argc==1){
	printf("Usage: %s [nume_fisier]", argv[0]);
	exit(2);
}

struct stat data;
if(-1==stat(argv[1],&data)){
	printf("Eroare la comanda stat!");
	exit(3);
}

printf("%s are marimea:%ld\n",basename(argv[1]),data.st_size);

return S_ISDIR(data.st_mode);
}
