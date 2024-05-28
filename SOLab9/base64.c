#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#define BUF_SIZE 3

char to_base_64(int x) {
    if (x >= 0 && x < 26) {
        return 'A' + x;
    }
    else if (x >= 26 && x < 52) {
        return 'a' + (x - 26);
    }
    else if (x >= 52 && x < 62) {
        return '0' + (x - 52);
    }
    else if(x == 62) {
        return '+';
    }
    else if(x == 63) {
        return '/';
    }
    else {
        return -1;
    }
}

int main(int argc, char* argv[]) {
    int input_fd, output_fd;
    if (argc != 3) {
        printf("Usage: src_file dest_file");
        return 1;
    }

    input_fd = open(argv[1] , O_RDONLY);
    if (input_fd == -1) {
        perror(argv [1]);
        return 2;
    }

    if (access(argv[2], F_OK) == 0) {
        // fisierul deja exista
        printf("Fisierul '%s' deja exista. Doriti sa suprascrieti? y/n: ", argv[2]);
        char answer = 0;
        scanf("%c", &answer);
        if (answer == 'y') {
            output_fd = open(argv[2] , O_RDWR | O_TRUNC , 0700);
            if (output_fd == -1) {
                perror(argv [2]);
                return 3;
            }
        }
        else if (answer == 'n') {
            printf("Umm.. ok");
            return 0;
        }
        else {
            printf("Invalid answer.");
            return 4;
        }
    }
    else {
        output_fd = open(argv[2] , O_RDWR |O_CREAT | O_TRUNC , 0700);
        if (output_fd == -1) {
            perror(argv [2]);
            return 3;
        }
    }
	struct stat sb;
	if(fstat(input_fd, &sb)==-1){
		perror("Eroare stat");
		return 4;
	}
	int length_in=sb.st_size;
	int length_out=((length_in+(length_in%3))*4)/3;
	ftruncate(output_fd,length_out);
	char* map_addr_i;
	map_addr_i=mmap(NULL,
			length_in,
			PROT_READ,
			MAP_PRIVATE,
			input_fd,
			0);

	if(map_addr_i == MAP_FAILED) {
		perror("Eroare la mapare input");
		return 4;
	}

	char* map_addr_o;
	map_addr_o=mmap(NULL,
			length_in*2,
			PROT_WRITE | PROT_EXEC,
			MAP_SHARED,
			output_fd,
			0);


	if(map_addr_o==MAP_FAILED) {
		perror("Eroare la mapare output!");
		return 4;	}
	close(input_fd);
	close(output_fd);

	int i = 0;
	//int iter = 0;
	while(i<=length_in){
		int tr = 0;
		char transform[4]={};
		int c=2;
		tr=tr<<8;
		tr+=map_addr_i[i];
		while(c){
			tr=tr<<8;
			tr+=map_addr_i[i+1];
			i++;

			c--;
		}
		i++;
	        int first = tr >> 18;
	        transform[0] = to_base_64(first);

        	int second = tr >> 12;
        	second = second & 63;
        	transform[1] = to_base_64(second);

        	int third = tr >> 6;
        	third = third & 63;
        	transform[2] = to_base_64(third);

        	int fourth = tr & 63;
        	transform[3] = to_base_64(fourth);

		strcat(map_addr_o,transform);
		//int j=0;
/*		while(j<=3){
			map_addr_o[iter]=transform[j];
			j++;
			iter++;*/
//		}
	}
	msync(map_addr_o,length_out,MS_SYNC);
	munmap(map_addr_o,length_out);
	munmap(map_addr_i,length_in);
return 0;
}
