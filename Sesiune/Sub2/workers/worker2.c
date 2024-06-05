#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

int selectarePereche(int fdR, int* shm_obj){
    int difMax = -1;
    while(1){

        int t1 = 0;
        int t2 = 0;

        int bytes_read = 0;

        bytes_read = read(fdR,&t1,sizeof(int));
        
        if(bytes_read != sizeof(int)){
            //printf("Am atins EOF!");
            return 0;
        }

        if(bytes_read == -1){
            perror("Eroare la citire!");
            exit(7);
        }

        bytes_read = read(fdR,&t2,sizeof(int));

        if(bytes_read != sizeof(int)){
            //printf("Am atins EOF");
            return 0;
        }

        if(bytes_read == -1){
            perror("Eroare la citire!");
            exit(7);
        }

        
        if(abs(t1-t2) > difMax){
            difMax = abs(t1-t2);
            shm_obj[0] = t1;
            shm_obj[1] = t2;
        }

    }
    return 0;
}


int main(int argc, char* argv[]){



    int w1_to_w2[2];
    if(-1 == pipe(w1_to_w2)){
        perror("Eroare la crearea piep-ului");
        exit(3);
    }

    pid_t pid_fiu = fork();
    if(pid_fiu == -1){
        perror("Eroare la crearea unui fiu!");
        exit(4);
    }

    if(pid_fiu == 0){
        //ZONA EXECUTATA DE FIU
        close(w1_to_w2[0]);

        if(-1 == dup2(w1_to_w2[1],STDOUT_FILENO)){
            perror("Eroare la redirectionarea output-ului!");
            exit(5);
        }
        close(w1_to_w2[1]);
        execl("workers/worker1","worker1",NULL);
        perror("Eroare la exec");
        exit(6);
    }
    else{
        //ZONA EXECUTATA DE TATA
        //Initializare shared map
        close(w1_to_w2[1]);
        char* shm_name = "w2_to_sup";
        int shm_fd = shm_open(shm_name,O_RDWR | O_CREAT, 0600);

        if( -1 == shm_fd ){
            perror("Eroare la deschiderea mapei!");
            exit(1);
        }

        if(-1 == ftruncate(shm_fd,2*sizeof(int))){
            perror("Eroare la setarea marimii!");
            exit(6);
        }

        int* shm_obj = mmap(NULL, 2*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        close(shm_fd);
        if(shm_obj == MAP_FAILED){
            perror("Eroare la maparea mapei!");
            exit(2);
        }

        selectarePereche(w1_to_w2[0],shm_obj);
        close(w1_to_w2[0]);

        munmap(shm_obj, 2*sizeof(int));
    }


}