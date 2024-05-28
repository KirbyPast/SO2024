#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/limits.h>


char nume_folder[PATH_MAX];



void parcurgere(char nume_folder[]){
	
}




int main(int argc, char* argv[]){

if(argc==1){
	printf("Introduceti path-ul catre folder: ");
	scanf("%s",nume_folder);
}
else{
	strcpy(nume_folder,argv[1]);
}

parcurgere(nume_folder)

}
