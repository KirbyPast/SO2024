#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

void prelucreazaDate(int fdRead, int fdWrite){

    int bytes_read = 1;
    char ch = 0;

    while((bytes_read = read(fdRead,&ch,sizeof(char)))){
        if(bytes_read == 0){
            //printf("Eof");
            break;
        }

        if(bytes_read == -1){
            perror("Eroare la citire!");
            exit(6);
        }

        if(ch >= 'a' && ch <= 'z'){
            ch = (ch - 'a' + 14) % 26 + 'a';
        }

        if(ch >= 'A' && ch <= 'Z'){
            ch = (ch - 'A' + 14) % 26 + 'A';
        }

        if( -1 == write(fdWrite, &ch, sizeof(char))){
            perror("Eroare la scriere!");
            exit(7);
        }
    }

}

int main(int argc, char* argv[]){

    if(-1 == mkfifo("send_data", 0600)){
        if(errno != EEXIST){
            perror("Eroare la crearea fifo-ului!");
            exit(4);
        }
    }

    int fdWF = open("send_data", O_WRONLY);

    if( -1 == fdWF ){
        perror("Eroare la deschiderea fifo-ului!");
        exit(5);
    }

    prelucreazaDate(3,fdWF);
    close(3);
    return 0;
}