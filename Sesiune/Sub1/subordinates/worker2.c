#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

int citesteRezultate(int* shm_obj){

    int rez = 0;
    int minImp = __INT_MAX__;
    int maxPar = __INT_MAX__ + 1;

    while(read(STDIN_FILENO,&rez,sizeof(int)) == 4){
            if(rez%2 == 0 && rez > maxPar){
                maxPar = rez;
            }
            if(rez%2 == 1 && rez < minImp){
                minImp = rez;
            }
    }

    shm_obj[0]=maxPar;
    shm_obj[1]=minImp;
}

int main(int argc, char* argv[]){

    char shm_name[]="/w2_to_sup";

    int fdWM = open(shm_name, O_RDONLY | O_CREAT, 0600);

    if( fdWM == -1 ){
        perror("Eroare la deschiderea maparii-ului!");
        exit(4);
    }

    int* shm_obj;

    shm_obj = mmap(NULL, 8, PROT_WRITE, MAP_SHARED, fdWM, 0);

    if(shm_obj == MAP_FAILED){
        perror("Eroare la mapare!");
        exit(5);
    }



    citesteRezultate(shm_obj);

    close(fdWM);
    munmap(shm_obj,8);



    return 0;
}