#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

void parseazaFisier(int fdRead, int fdWrite){
    int ch = 0;
    while(read(fdRead,&ch,sizeof(char))){
        write(fdWrite,&ch,sizeof(char));
    }
}

void afiseazaRezultate(int* shm_obj){
    printf("%c: %d \n%c: %d\n",(char)(shm_obj[0]),shm_obj[1],(char)shm_obj[2],shm_obj[3]);
}


int main(int argc, char* argv[]){

    char* shm_name = "w2_to_sup";
    int shm_fd;
    if( -1 == (shm_fd = shm_open(shm_name,O_CREAT | O_RDWR, 0600))){
        perror("Eroare la deschiderea mapei!");
        exit(1);
    }

    if(-1 == ftruncate(shm_fd,4*sizeof(int))){
        perror("Eroare la ftruncate");
        exit(3);
    }

    int* shm_obj = mmap(NULL, 4*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shm_obj == MAP_FAILED){
        perror("Eroare la mapare!");
        exit(2);
    }

    int fdFis = open(argv[1],O_RDONLY);
    if(fdFis == -1){
        perror("Eroare la deschiderea fisierului!");
        exit(4);
    }

    parseazaFisier(fdFis, 4);
    close(4);

    sleep(1);
    afiseazaRezultate(shm_obj);
    return 0;

}