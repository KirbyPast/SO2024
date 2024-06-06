#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

void parseazaFifo(int fdRead, int* shm_obj){
    char ch = 0;
    int freq = 0;
    int bytes_read = 0;

    int vocMax = -1;
    int consMin = __INT_MAX__;

    while(1){

        bytes_read = read(fdRead, &ch, sizeof(char));
        if(bytes_read == -1){
            perror("Eroare la citire fifo!");
            exit(2);
        }

        if(bytes_read == 0){
            //printf("Am atins eof");
            return;
        }

        bytes_read = read(fdRead, &freq, sizeof(int));

        if(bytes_read == -1){
            perror("Eroare la citire fifo!");
            exit(2);
        }

        if(bytes_read != sizeof(int) ){
            //printf("Am atins eof");
            return;
        }
        
        if(ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u'){
            if(vocMax < freq){
                vocMax = freq;
                shm_obj[0] = (int)(ch);
                shm_obj[1] = freq;
            }
        }
        else{
            if(consMin > freq){
                consMin = freq;
                shm_obj[2] = (int)(ch);
                shm_obj[3] = freq;
            }
        }
    }

}

int main(int argc, char* argv[]){

    if( -1 == mkfifo("w1_to_w2", 0600)){
        if(errno != EEXIST){
            perror("Eroare la crearea fifo-ului!");
            exit(7);
        }
    }

    int fdRF = open("w1_to_w2",O_RDONLY);

    if( -1 == fdRF ){
        perror("Eroare la deschiderea fifo-ului!");
        exit(8);
    }

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
    //sleep(3);
    parseazaFifo(fdRF,shm_obj);

}