#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <bits/types/sigset_t.h>

int n = 1;
int i = 0;
int ok = 0;
int quitok = 0;

void handle_chld(int nr_sem){
    printf("Am primit SIGCHLD!\n");
}

void handle_usr2(int nr_sem){
    printf("Am primit USR1!\n");
}

void handle_alarm(int nr_sem){
    i++;
    if(!quitok && i == 5){
        printf("Am iesit din cauza inactivitatii!\n");
        exit(2);
    }
    else if(!ok){
        printf("Au trecut %d secunde...\n",n);
    }
    ok = 0;
    alarm(n);
}
//Seconds count

int main(int argc, char* argv[]){
    char ch;
    int fd;

    if(-1 == (fd = (open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0600)))){
        perror("Eroare la deschiderea/crearea fisierului!");
        exit(1);
    }

    n=atoi(argv[2]);

    signal(SIGALRM, handle_alarm);

    alarm(n);
    while(read(0,&ch,sizeof(char))>0){
            ok = 1;
            quitok = 1;
            write(fd,&ch,sizeof(char));
    }

    close(fd);

    pid_t pid_fiu;

    if(-1 == (pid_fiu = fork())){
        perror("Eroare la crearea unui fiu!");
        exit(2);
    }

    if(pid_fiu == 0){
        //Zona executata de fiu
        signal(SIGUSR2,handle_usr2);
        
        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask,SIGUSR2);
        sigsuspend(&mask);

        if(-1 == ( fd = open(argv[1],O_RDONLY))){
            perror("Eroare la deschiderea fisierului!");
            exit(4);
        }

        while(read(fd,&ch,sizeof(char))>0){
            printf("%c",ch);
        }

        close(fd);


        printf("Fiul a terminat executia. \n");
        exit(2);
    }
    else{
        //Zona executata de tata
        signal(SIGCHLD,handle_chld);
        sleep(1);
        if(-1 == kill(pid_fiu,SIGUSR2)){
            perror("Eroare la trimiterea semnalului!");
            exit(3);
        }
        //pause();

        sigset_t mask;
        sigfillset(&mask);
        sigdelset(&mask,SIGCHLD);
        sigsuspend(&mask);

        printf("Tatal a terminat executia. \n");

        exit(2);
    }



    return 0;
}