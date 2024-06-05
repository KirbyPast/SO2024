#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int parseazaFisier(char* filename, int fd){

    int fdFis = open(filename, O_RDONLY);
    if(fdFis == -1){
        perror("Eroare la deschiderea fisierului!");
        exit(3);
    }

     while(1){
        int line_length = 0;
        char buf[128] = {0};

        int bytes_count = 0;
        //Citim pana la newline
        while((bytes_count = read(fdFis, &buf[line_length], sizeof(char)))){
            if(bytes_count == -1 || bytes_count > 1){
                perror("Eroare la citire!");
                exit(3);
            }
            //Cand intampinam newline il schimbam cu un NULL
            if(buf[line_length] == '\n'){
                buf[line_length] = '\0';
                break;
            }
            line_length++;
        }
        //Daca nu mai este ce sa citim din fisier, iesim din while
        if(bytes_count == 0){
            return 4;
        }
        //Tokenizam sirul si il impartim in termen 1, operatie, termen 2
        char* t1 = NULL, *t2 = NULL;
        char* p = strtok(buf," ");
        while(p){
            if(t1 == NULL){
                t1 = p;
            }
            else if (t2 == NULL){
                t2 = p;
            }
            p=strtok(NULL, " ");
        }

        int T1, T2;

        T1 = atoi(t1);
        T2 = atoi(t2);
        //Scriem datele in fifo
        if(-1 == write(fd,&T1,sizeof(int))){
            perror("Eroare la write!");
            exit(6);
        }
                           
        if(-1 == write(fd,&T2,sizeof(int))){
            perror("Eroare la write1");
            exit(7);
        }

    }

}

void citesteRezultate(int* shm_obj){
    printf("%d * %d = %d\n",shm_obj[0],shm_obj[1],shm_obj[0] * shm_obj[1]);
}


int main(int argc, char* argv[]){

    if(argc != 2){
        printf("Usage: ./supervisor [path]");
        exit(1);
    }

    if(-1 == mkfifo("sup_to_w1", 0600)){
        if(errno!=EEXIST){
            perror("Eroare la crearea fifoului");
            exit(1);
        }
    }

    int fdRF;

    fdRF = open("sup_to_w1", O_WRONLY);
    if(fdRF == -1){
        perror("Eroare la deschiderea fifoului!");
        exit(2);
    }

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

    parseazaFisier(argv[1],fdRF);
    close(fdRF);
    sleep(1);
    citesteRezultate(shm_obj);
    munmap(shm_obj,2*sizeof(int));
}