/*
*  Program: com-1.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    /* Scriem un mesaj incomplet, pe fluxul stdout. */
    printf("Salut...\n");

//    fflush(stdout);  /* De-comentați acest apel fflush(), pentru a obține mesajul întreg pe ecran! */

    /* Reacoperim cu programul com-2 */
    execl("com-2","com-2",NULL);

    /* Se va executa numai dacă eșuează apelul exec ! */
    perror("Eroare la apelul execl");
    return 1;
}
