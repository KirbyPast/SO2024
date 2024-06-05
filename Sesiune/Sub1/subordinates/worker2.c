#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

int citesteRezultate(int* shm_obj){
    int rez = 0;
    int minImp = __INT_MAX__;
    int maxPar = -9999;
    //self-explanatory
    while(read(STDIN_FILENO,&rez,sizeof(int)) == 4){
            if(abs(rez)%2 == 0 && rez > maxPar){
                maxPar = rez;
            }
            if(abs(rez)%2 == 1 && rez < minImp){
                minImp = rez;
            }
    }
    if(maxPar == -9999) maxPar = 0;
    if(minImp == __INT_MAX__) minImp = 0;


    shm_obj[0]=maxPar;
    shm_obj[1]=minImp;
    return 0;
}

int main(int argc, char* argv[]){
    //Creem maparea nepersistenta cu nume
    char shm_name[]="/w2_to_sup";
    //SHM_OPEN
    int fdWM = shm_open(shm_name, O_RDWR | O_CREAT, 0600);

    if( fdWM == -1 ){
        perror("Eroare la deschiderea mapariii-ului!");
        exit(4);
    }
    //Setam dimnesiunea
    ftruncate(fdWM ,8);

    int* shm_obj;
    //Mapam
    shm_obj = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_SHARED, fdWM, 0);

    if(shm_obj == MAP_FAILED){
        perror("Eroare la mapare!");
        exit(5);
    }


    //Citim rezultatele primite de la w1 si le scriem in sup
    citesteRezultate(shm_obj);

    //Inchidem apoi capetele
    close(STDIN_FILENO);
    close(fdWM);
    munmap(shm_obj,8);



    return 0;
}