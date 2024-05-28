#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


int main(int argc, char* argv[]){
char* nume_director;


if(argc==1){
	printf("Usage: ./myls [NUME_DIRECTOR]");
	exit(1);
}
nume_director=argv[1];
DIR *dd;
struct dirent *de;

if ( (dd=opendir(nume_director)) == NULL){
	printf("Eroare la deschiderea directorului!");
	exit(2);
}

while((de=readdir(dd))!=NULL)
{
	if((strcmp(de->d_name,".")==0)||(strcmp(de->d_name,"..")==0)) continue;
	printf("%s\n",de->d_name);
}

return 0;

}
