#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/stat.h>
int parseazaPerechi(int fd){

    while(1){

        int t1 = 0;
        int t2 = 0;

        int bytes_read = 0;

        bytes_read = read(fd,&t1,sizeof(int));
        
        if(bytes_read != sizeof(int)){
            //printf("Am atins EOF!");
            return 0;
        }

        if(bytes_read == -1){
            perror("Eroare la citire!");
            exit(7);
        }

        bytes_read = read(fd,&t2,sizeof(int));

        if(bytes_read != sizeof(int)){
            //printf("Am atins EOF");
            return 0;
        }

        if(bytes_read == -1){
            perror("Eroare la citire!");
            exit(7);
        }

        int a = MAX(t1,t2);
        int b = MIN(t1,t2);

        while( a != b){
            if(a > b){
                a=a-b;
            }
            else b=b-a;
        }

        if(a == 1){
            if(-1 == write(STDOUT_FILENO,&t1,sizeof(int))){
                perror("Eroare la scriere");
                exit(3);
            }
            if(-1 == write(STDOUT_FILENO,&t2,sizeof(int))){
                perror("Eroare la scriere");
                exit(3);
            }
        }

    }
    return 0;
}



int main(int argc, char* argv[]){

    if(-1 == mkfifo("sup_to_w1", 0600)){
        if(errno!=EEXIST){
            perror("Eroare la crearea fifoului");
            exit(1);
        }
    }

    int fdRF;

    fdRF = open("sup_to_w1", O_RDONLY);
    if(fdRF == -1){
        perror("Eroare la deschiderea fifoului!");
        exit(2);
    }

    parseazaPerechi(fdRF);
    close(fdRF);
    close(STDOUT_FILENO);
}