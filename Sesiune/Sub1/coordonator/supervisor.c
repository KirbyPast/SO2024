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
    //Deschidem fisierul primit ca argument
    int fdFis = open(filename, O_RDONLY);

    if( -1 == fdFis){
        perror("Eroare la deschiderea fisierului!\n");
        exit(4);
    }
    //Citim din el
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
        char ch = op[0];
        //Scriem datele in fifo
        write(fd,&T1,sizeof(int));
    
        write(fd,&ch,sizeof(char));
                           
        write(fd,&T2,sizeof(int));

    }
    return 0;
}


void primesteDate(int* shm_obj){
    //Preluam datele de la w2
    int sum = shm_obj[0] + shm_obj[1];
    printf("%d + ",shm_obj[0]);
    printf("%d = ",shm_obj[1]);
    printf("%d\n",sum);
    
}

int main(int argc, char* argv[]){

    //Verificam argumentele
    if (argc != 2){
        printf("Usage: ./supervisor [path]");
        exit(1);
    }
  
    //Creem fisierul fifo pentru comunicare intre sup si w1
    if(-1 == mkfifo("sup_to_w1", 0600)){
        if(errno != EEXIST){
            perror("Am belit o");
            exit(2);
        }
    }

    //Deschidem fisierul fifo
    int fdWF;
    if((fdWF = open("sup_to_w1",O_WRONLY)) == -1){
        perror("Eroare la deschiderea pipe-ului!");
        exit(3);
    }

    //Incepem initializarea maparii nepersistente cu nume
    char shm_name[]="/w2_to_sup";

    //SHM_OPEN, nu OPEN
    int fdRM = shm_open(shm_name, O_RDWR | O_CREAT, 0600);
  
    if( fdRM == -1 ){
        perror("Eroare la deschiderea maparii-ului!\n");
        exit(4);
    }

    //Setam marimea (2*sizeof(int)), ptca maparea va tine doar 2 nr inauntru
    ftruncate(fdRM ,8);

    int* shm_obj;
    //Mapam fisierul
    shm_obj = mmap(NULL, 8, PROT_WRITE | PROT_READ, MAP_SHARED, fdRM, 0);

    if(shm_obj == MAP_FAILED){
        perror("Eroare la crearea unei mape!");
        exit(5);
    }
    //Parsam fisierul primit ca argument
    parseazaFisier(argv[1],fdWF);
    //Inchidem fifo-ul deoarece nu mai avem nevoie de el
    close(fdWF);
    //Pt a-i lasa timp lui worker2 sa scrie
    sleep(1);
    //Primim datele de la worker2
    primesteDate(shm_obj);
    //Inchidem restul lucrurilor
    close(fdRM);
    munmap(shm_obj,8);

    return 0;
}