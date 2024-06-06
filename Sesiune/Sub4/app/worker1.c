#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

void calculeazaFrecventa(int readFD, int writeFD){
    char ch = 0;
    int bytes_read = 1;
    int frecv[30] = {0};
    while(1){
        bytes_read = read(readFD,&ch,sizeof(char));
        if(bytes_read == 0){
            //printf("EOF");
            break;
        }
        if(bytes_read == -1){
            perror("Eroare la citire din pipe");
            exit(10);
        }
        if(ch >= 'a' && ch <= 'z'){
            frecv[(int)(ch - 97)]++;
        }
        if(ch >= 'A' && ch <= 'Z'){
            frecv[(int)(ch - 65)]++;
        }
    }
    
    for(int i = 0; i<25; i++){
        if(frecv[i] != 0){
            ch = (char)(i+97);
            if(-1 == write(writeFD,&ch,sizeof(char))){
                perror("Eroare la scrierea in fifo");
                exit(11);
            }
            
            if(-1 == write(writeFD,&frecv[i],sizeof(int))){
                perror("Eroare la scrierea in fifo");
                exit(12);
            }
        }
    }
}




int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: ./worker1 [path]");
        exit(1);
    }

    int sup_to_w1[2];
    if( -1 == pipe(sup_to_w1)){
        perror("Eroare la crearea pipe-ului!");
        exit(2);
    }

    pid_t pid_fiu = fork();

    if(pid_fiu == -1){
        perror("Eroare la crearea fiului!");
        exit(3);
    }

    if(pid_fiu == 0){
        //ZONA EXECUTATA DE FIU

        if( close(sup_to_w1[0]) == -1 ){
            perror("Eroare la inchiderea capatului de citire");
            exit(4);
        }

        if( -1 == dup2(sup_to_w1[1], 4)){
            perror("Eroare la setarea pipe-ului!");
            exit(5);
        }
        //close(sup_to_w1[1]);
        execl("app/supervisor","supervisor",argv[1],NULL);
        perror("Eroare la exec!");
        exit(6);
    }
    else{
        //ZONA EXECUTATA DE TATA
        if(-1 == close(sup_to_w1[1])){
            perror("Eroare la inchiderea capatului!");
            exit(9);
        }
        if( -1 == mkfifo("w1_to_w2", 0600)){
            if(errno != EEXIST){
                perror("Eroare la crearea fifo-ului!");
                exit(7);
            }
        }

        int fdWF = open("w1_to_w2",O_WRONLY);

        if( -1 == fdWF ){
            perror("Eroare la deschiderea fifo-ului!");
            exit(8);
        }

        int fdRF = open("w1_to_w2",O_RDONLY);

        calculeazaFrecventa(sup_to_w1[0],fdWF);

        close(sup_to_w1[0]);
        close(fdWF);
        close(fdRF);
    }
    sleep(2);
    return 0;
}