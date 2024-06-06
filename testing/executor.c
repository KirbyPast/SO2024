#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(){

    int fd = open("test1.txt", O_TRUNC | O_RDWR);
    dup2(fd,4);
    printf("balls3");
    dprintf(4,"balls2pp;");
    execl("executee","executee",NULL);
    return 0;

}