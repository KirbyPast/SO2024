#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
int citire_fifo(int fd, int fdPipe){
    while(1){
        int t1 = 0,t2 = 0;
        char op = 0;
        int rez = 0;
        //Citim din supervisor tripletul de termen, operatie, termen
        int bytes_count = read(fd,&t1,sizeof(int));

        if(bytes_count == -1){
            perror("Eroare la citire!");
            exit(4);
        }
        if(bytes_count != sizeof(int)){
            return 3;
        }

        bytes_count = read(fd,&op,sizeof(char));
        if(bytes_count == -1){
            perror("Eroare la citire!");
            exit(4);
        }
        if(bytes_count != sizeof(char)){
            return 3;
        }

        bytes_count = read(fd,&t2,sizeof(int));
        if(bytes_count == -1){
            perror("Eroare la citire!");
            exit(4);
        }
        if(bytes_count != sizeof(int)){
            return 3;
        }
        //printf("%d %c %d",t1,op,t2);
        switch(op){
            case '+':   rez = t1+t2;    break;
            case '-':   rez = t1-t2;    break;
            case '*':   rez = t1*t2;    break;
            case '/':   rez = t1/t2;    break;
        }
        //printf(" = %d\n",rez);
        //Scriem apoi rezultatul in pipe-ul catre w2
        bytes_count = write(fdPipe,&rez,sizeof(int));
        if(-1 == bytes_count){
            perror("Eroare la scriere!");
            exit(5);
        }
        if(0 == bytes_count){
            printf("Am ajuns la final!");
            return 0;
        }
        
    }
    return 0;
}

int main(int argc, char* argv[]){
    //Initializam pipe-ul intre w1 si w2
    int w1_to_w2[2];
    if(-1 == pipe(w1_to_w2)){
        perror("Eroare la crearea pipe-ului!");
        exit(3);
    }
    //Nastem fiul
    pid_t pid_fiu = fork();

    if( pid_fiu == -1 ){
        perror("Eroare la crearea fiului!");
        exit(1);
    }

    if( pid_fiu == 0){
        //ZONA EXECUTATA DE FIU!
        //Inchidem capatul de care nu avem nevoie
        close(w1_to_w2[1]);
        //Redirectionam stdin-ul in fiu si, implicit, in exec la capatul de pipe
        if(-1 == dup2(w1_to_w2[0],STDIN_FILENO)){
                perror("Eroare la redirectare");
                exit(4);
        }
        //Dupa, inchidem capatul din fiu
        close(w1_to_w2[0]);
        //Executam worker2 (subordinates/worker2 din cauza locatiei lui starter.sh)
        execl("subordinates/worker2","worker2",NULL);
        perror("Execl esuat!:");
        exit(1);
    }
    else{
        //ZONA EXECUTATA DE TATA!

        //Creem fifo-ul intre supervisor si w1
        if(-1 == mkfifo("sup_to_w1", 0600)){
        if(errno != EEXIST){
            perror("Am belit o");
            exit(2);
            }
        }
        //Il deschidem
        int fdRF;
        if((fdRF = open("sup_to_w1",O_RDONLY)) == -1){
            perror("Eroare la deschiderea pipe-ului!");
            exit(3);
        }
        //Citim din fifo
        citire_fifo(fdRF,w1_to_w2[1]); 
        //Inchidem capatul de fifo 
        close(fdRF);
        //Inchidem apoi ambele capete neutilizate (tin capatul de read pornit ca sa pot sa scriu, chiar daca nu citesc)
        close(w1_to_w2[1]);
        close(w1_to_w2[0]);
        return 0;
    }
    return 0;
}