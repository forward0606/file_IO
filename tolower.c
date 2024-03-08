#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<ctype.h>

const int MaxBuf = 4096;

void usage(char *u){
	fprintf(stderr, "make all letter in the file to  lower case\n");
	fprintf(stderr, "usage:\n");
	fprintf(stderr, "%s file\n", u);
}

int main(int argc, char **argv){
	if(argc != 2){
		usage(argv[0]);
		exit(1);
	}
	int fd = open(argv[1], O_RDWR);
	char buffer[MaxBuf];
	int result;
	while((result = read(fd, buffer, MaxBuf)) > 0){
		for(int i=0;i<result;i++){
			buffer[i] = tolower(buffer[i]);
		}
		lseek(fd, -result, SEEK_CUR);
		write(fd, buffer, result);
	}
	close(fd);
	return 0;
}
