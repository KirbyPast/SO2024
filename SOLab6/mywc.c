#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char* argv[]){
FILE* fd;
int i;
char ch,last;
int op_w=0,op_l=0,op_L=0,op_c=0;
int byte_cnt=0, line_cnt=0, word_cnt=0, len_cnt=0;
char* nume_fisier = NULL;
int len=0;
if(argc==1){
	printf("Eroare!");
	return 1;
}

for(i=1;i<argc;i++){
	if(0==strcmp(argv[i],"-w")) op_w=1;
	else if(0==strcmp(argv[i],"-l")) op_l=1;
		else if(0==strcmp(argv[i],"-L")) op_L=1;
			else if(0==strcmp(argv[i],"-c")) op_c=1;
				else if(!nume_fisier) nume_fisier=argv[i];
					else {printf("Programul accepta doar un singur fisier!"); exit(1);}
}

if(0==op_w+op_L+op_l+op_c){
op_w=1;
op_l=1;
op_c=1;
}

if(NULL == ( fd = fopen(nume_fisier,"r"))){
printf("eroare la deschidere!");
exit(2);
}

while(1){
	ch=fgetc(fd);
	if(feof(fd)) break;
	if(op_c) byte_cnt++;

	if(op_l){
		if(ch=='\n')
			line_cnt++;
	}

	if(op_w){
		if(ch==' '&&last!=' '){
			word_cnt++;
			printf("%d\n",byte_cnt);}
		else if(ch=='\n'&&last!=' '){
			word_cnt++;
			printf("%d\n",byte_cnt);}
	}

	if(op_L){
		if(ch!='\n'){
			if(ch=='\t') len+=8;
			else len++;
		}
		else{
			if(len>len_cnt) len_cnt=len;
			len=0;
		}
	}
	last=ch;
}

if(op_L){
	if(len>len_cnt) len_cnt=len;
	len=0;
}

if(op_w) printf("Numarul de cuvinte: %d\n",word_cnt);
if(op_L) printf("Lungimea maxima a unei linii: %d\n",len_cnt);
if(op_l) printf("Numarul de linii: %d\n",line_cnt);
if(op_c) printf("Numarul de biti: %d\n",byte_cnt);

return 0;

}
