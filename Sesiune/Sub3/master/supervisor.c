#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

void parseazaFisier(char* filename, int fdWrite){

    int fdRead = open(filename, O_RDONLY);

    if(-1 == fdRead){
        perror("Eroare la deschiderea fisierului!");
        exit(6);
    }

    int bytes_read = 1;
    int ch = 0;

    while((bytes_read = read(fdRead,&ch,sizeof(char)))){
        if( bytes_read == 0 ){
            printf("EOFF");
            break;
        }

        if( bytes_read == -1 ){
            perror("Eroare la citire");
            exit(8);
        }

        if((ch >= 'a' && ch <='z') || (ch >= 'A' && ch <= 'Z')){
            if(-1 == write(fdWrite,&ch,sizeof(char))){
                perror("Eroare la scriere");
                exit(7);
            }
        }
    }

}

void citesteMapare(int* shm_obj){

    if(shm_obj[0] == 0){
        printf("Codul nu contine vocale!\n");
    }
    else{
        printf("Codul contine %d vocale!\n",shm_obj[0]);
    }

    if(shm_obj[1] == 0){
        printf("Codul nu contine consoane!\n");
    }
    else{
        printf("Codul contine %d consoane!\n",shm_obj[1]);
    }

}


int main(int argc, char* argv[]){


    if( argc != 2 ){
        printf("Usage: ./supervisor [path]");
        exit(1);
    }

    int sup_to_w1[2];

    if(-1 == pipe(sup_to_w1)){
        perror("Eroare la deschiderea pipe-ului!");
        exit(2);
    }

    int pid_fiu = fork();

    if(pid_fiu == -1){
        perror("Eroare la crearea fiului!");
        exit(3);
    }

    if(pid_fiu == 0){
        //ZONA EXECUTATA DE FIU

        close(sup_to_w1[1]);

        if(-1 == dup2(sup_to_w1[0],3)){
            perror("Eroare la redirectionare!");
            exit(4);
        }

        execl("slaves/worker1","worker1",NULL);
        perror("Eroare la exec!");
        exit(5);
    }
    else{
        //ZONA EXECUTATA DE TATA
        close(sup_to_w1[0]);
        char* shm_name = "comunicare_decriptie";

        int shm_fd = shm_open(shm_name, O_RDWR | O_CREAT, 0600);

        if(shm_fd == -1){
            perror("Eroare la deschiderea maparii!");
            exit(1);
        }

        if(ftruncate(shm_fd,2*sizeof(int)) == -1){
            perror("Eroare la setarea marimii!");
            exit(2);
        }

        int* shm_obj = mmap(NULL, 2*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

        if(shm_obj == MAP_FAILED){
            perror("Eroare la crearea maparii!");
            exit(3);
        } 

        parseazaFisier(argv[1],sup_to_w1[1]);
        close(sup_to_w1[1]);

        wait(NULL);
        sleep(1);

        citesteMapare(shm_obj);
        close(shm_fd);
        munmap(shm_obj, 2*sizeof(int));
    }

    return 0;
}