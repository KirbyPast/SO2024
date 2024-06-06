#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

void numaraCaractere(int fdRead, int* shm_obj){
    int bytes_read = 1;
    char ch = 0;
    int nrVoc = 0;
    int nrCon = 0;
    while((bytes_read = read(fdRead, &ch, sizeof(char)))){
        printf("%c", ch);
        if(bytes_read == 0){
            //printf("EOF\n");
            break;
        }
        if(bytes_read == -1){
            perror("Eroare la citire!");
            exit(6);
        }

        if(strchr("aeiouAEIOU",ch) != NULL){
            nrVoc++;
        }
        else{
            nrCon++;
        }
    }
    printf("\n");
    shm_obj[0] = nrVoc;
    shm_obj[1] = nrCon;

}


int main(int argc, char* argv[]){

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

    if(-1 == mkfifo("send_data", 0600)){
        if(errno != EEXIST){
            perror("Eroare la crearea fifo-ului!");
            exit(4);
        }
    }

    int fdRF = open("send_data", O_RDONLY);

    if( -1 == fdRF ){
        perror("Eroare la deschiderea fifo-ului!");
        exit(5);
    }
    numaraCaractere(fdRF, shm_obj);

    close(fdRF);
    close(shm_fd);
    munmap(shm_obj,2*sizeof(int));
    return 0;

}