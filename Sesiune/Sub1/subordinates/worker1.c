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
        int bytes_count = read(fd,&t1,sizeof(int));

        if(bytes_count == -1){
            perror("Eroare la citire!");
            exit(4);
        }
        if(bytes_count != sizeof(int)){
            break;
        }

        bytes_count = read(fd,&op,sizeof(char));
        if(bytes_count == -1){
            perror("Eroare la citire!");
            exit(4);
        }
        if(bytes_count != sizeof(char)){
            break;
        }

        bytes_count = read(fd,&t2,sizeof(char));
        if(bytes_count == -1){
            perror("Eroare la citire!");
            exit(4);
        }
        if(bytes_count != sizeof(int)){
            break;
        }

        switch(op){
            case '+':   rez = t1+t2;    break;
            case '-':   rez = t1-t2;    break;
            case '*':   rez = t1*t2;    break;
            case '/':   rez = t1/t2;    break;
        }

        if(-1 == write(fdPipe,&rez,sizeof(int))){
            perror("Eroare la scriere!");
            exit(5);
        }
    }
}

int main(int argc, char* argv[]){

    int w1_to_w2[2];
    if(-1 == pipe(w1_to_w2)){
        perror("Eroare la crearea pipe-ului!");
        exit(3);
    }

    pid_t pid_fiu = fork();

    if( pid_fiu == -1 ){
        perror("Eroare la crearea fiului!");
        exit(1);
    }

    if( pid_fiu == 0){
        //ZONA EXECUTATA DE FIU!
        close(w1_to_w2[1]);
        if(-1 == dup2(w1_to_w2[0],STDIN_FILENO)){
                perror("Eroare la redirectare");
                exit(4);
        }

        execl("worker2","worker2",NULL);
        exit(1);
    }
    else{
        //ZONA EXECUTATA DE TATA!

        close(w1_to_w2[0]);
        if(-1 == mkfifo("sup_to_w1", 0600)){
        if(errno != EEXIST){
            perror("Am belit o");
            exit(2);
            }
        }

        int fdRF;
        if((fdRF = open("sup_to_w1",O_RDONLY)) == -1){
            perror("Eroare la deschiderea pipe-ului!");
            exit(3);
        }

        citire_fifo(fdRF,w1_to_w2[1]);

        close(fdRF);
        close(w1_to_w2[1]);
    }

}