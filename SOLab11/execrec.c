/*
*  Program: exec-rec.c  (exemplu de apel recursiv prin exec)
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char* argv[], char* env[])
{
    char **r, *s;
	
    /* Afișăm ID-urile, linia de comandă și environment-ul cu care a fost apelată instanța curentă de execuție a acestui program. */
    printf("PID=%d, PPID=%d, OWNER=%d\n", getpid(), getppid(), getuid());
    printf("COMMANDLINE:\n");
    r = argv;
    while(1)
    { 
         s = *r++;
         if(s == NULL) break;
         printf("%s\n",s);
    }
    printf("ENVIRONMENT:\n");
    r = env;
    while(1)
    { 
         s = *r++;
         if(s == NULL) break;
         printf("%s\n",s);
    }
    putchar('\n');

    /* Modificăm environment-ul și linia de comandă, pentru apelul recursiv. */
    char *new_argv[3], *new_env[2];

    new_env[0] = "Salut!";
    new_env[1] = NULL;

    new_argv[0] = argv[0]; /* Acesta e numele executabilului ! */
    new_argv[1] = "2nd call";   // Vom folosi acest caracter '2' pentru o detecție simplă a faptului
    new_argv[2] = NULL;         // că NU mai suntem în prima instanță de execuție a acestui program.


    /* Putem folosi oricare dintre valorile modificate ale environment-ului sau liniei de comandă (și anume,
       vom folosi modificarea argv[1] indicată mai sus), pentru a evita recursia la infinit !!! */
    if( (argv[1] != NULL) && (argv[1][0] == '2') )
    {
         exit(0); /* Astfel, oprim recursia la al doilea apel ! */
    }
    else
    {
        printf("\033[31;47m");
        /* '\033[31;47m' reprezintă o "secvență escape" ce va reconfigura terminalul pentru a afișa textul scris cu roșu pe fundal alb.
           Important: efectul reconfigurărilor prin "secvențe escape" este permanent în cadrul procesului respectiv (se păstrează inclusiv după exec),
           până la finalul execuției sale, sau până la o nouă reconfigurare, ori până la "resetarea" cu "secvența escape" '\033[0m', care va anula
           efectul reconfigurărilor atributelor textului efectuate anterior "printării" ei. */
        printf("Urmeaza apelul primitivei execve..."); /* Astfel, acest text va fi scris cu culoarea roșie pe fundal alb. */

        printf("\033[0m\n");  /* '\033[0m' reprezintă o "secvență escape" ce va reseta terminalul la atributele implicite (deci refacem fundalul negru). */
        printf("\033[36m\n"); /* '\033[36m' reprezintă o "secvență escape" ce va reconfigura terminalul pentru a afișa textul cu culoarea cyan. */

        execve(argv[0], new_argv, new_env);
        /* Prin reconfigurarea de mai sus, toate mesajele afișate pe ecran de execuția noii instanțe a programului, vor fi colorate în cyan. */

        perror("Eroare la apelul execve");
        exit(1);
    }
    return 0;
}
