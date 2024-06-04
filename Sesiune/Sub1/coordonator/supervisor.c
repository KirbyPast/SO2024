#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

int parseazaFisier(char* filename, int fd){

    int fdFis = open(filename, O_RDONLY);

    if( -1 == fdFis){
        perror("Eroare la deschiderea fisierului!\n");
        exit(4);
    }


    while(1){
        int line_length = 0;
        char buf[128] = {0};
        printf("TesT");

        while(read(fdFis, &buf[line_length], sizeof(char)) > 0){
            if(buf[line_length] == '\n'){
                buf[line_length] = '\0';
                break;
            }
            line_length++;
        }

        char* t1 = NULL, *t2 = NULL , *op = NULL;
        char* p = strtok(buf," ");
        while(p){
            if(t1 == NULL){
                t1 = p;
            }
            else if (op == NULL){
                op = p;
            }
            else if (t2 == NULL){
                t2 = p;
            }
            p=strtok(NULL, " ");
        }
        
        int T1, T2;

        T1 = atoi(t1);
        T2 = atoi(t2);

        write(fd,&T1,sizeof(int));
        write(fd,&op,sizeof(char));
        write(fd,&T2,sizeof(int));

    }
    return 0;
}


void primesteDate(int* shm_obj){
    int sum = shm_obj[0] + shm_obj[1];
    printf("%d\n",sum);
    
}

int main(int argc, char* argv[]){
    printf("merg");
    if (argc != 2){
        printf("Usage: ./supervisor [path]");
        exit(1);
    }
  
    if(-1 == mkfifo("sup_to_w1", 0600)){
        if(errno != EEXIST){
            perror("Am belit o");
            exit(2);
        }
    }

    int fdWF;
    if((fdWF = open("sup_to_w1",O_WRONLY)) == -1){
        perror("Eroare la deschiderea pipe-ului!");
        exit(3);
    }

    if( fdWF == -1 ){
        perror("Eroare la deschiderea pipe-ului!");
        exit(3);
    }

    char shm_name[]="/w2_to_sup";
    sleep(3);
    int fdRM = open(shm_name, O_RDONLY | O_CREAT, 0600);

    if( fdRM == -1 ){
        perror("Eroare la deschiderea maparii-ului!");
        exit(4);
    }

    int* shm_obj;

    shm_obj = mmap(NULL, 8, PROT_READ, MAP_SHARED, fdRM, 0);

    if(shm_obj == MAP_FAILED){
        perror("Eroare la crearea unei mape!");
        exit(5);
    }

    parseazaFisier(argv[1],fdWF);

    primesteDate(shm_obj);

    close(fdRM);
    munmap(shm_obj,8);

    return 0;
}