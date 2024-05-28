#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define BLOCK_SIZE 4096

int main(int argc, char* argv[])
{
	int i, flag_opt_b = 0, flag_opt_s = 0;
	char *flag_opt_s_string = NULL;
	char *nume_fisier =NULL;
	printf("b");
	if(argc<2)
	{
		printf("Usage: %s, [-b] [-s STRING] fisier \n", argv[0]);
		exit(1);
	}

	for(i=1;i<argc;i++)
	{
		printf("a");
		if( strcmp(argv[i],"-b") == 0) {  flag_opt_b = 1; continue; }
		if( strcmp(argv[i],"-s") == 0) {
			flag_opt_s = 1;
			flag_opt_s_string = argv[i+1];
			i++;
			continue;
		}
		if( nume_fisier == NULL ) { nume_fisier = argv[i]; continue; }
		
		printf("Eroare: nu ati respectat formatul impus pentru comanda \n");
	}
	printf("%s",nume_fisier);
	int input_fd = open(nume_fisier,O_RDONLY);
	long y = fseek(input_fd,0,SEEK_END);
	printf("%ld",y);
return 0;
}
