#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

const int MaxBuffer = 4096;

void usage(char *u){
	fprintf(stderr, "usage :\n %s file1 fil2\n", u);
	fprintf(stderr, "try to copy file1 content to file2\n");
}

int main(int argc, char **argv){
	if(argc != 3){
		usage(argv[0]);
		exit(1);
	}
	int f1 = open(argv[1], O_RDONLY);
	int f2 = open(argv[2], O_WRONLY | O_CREAT, 0600);
	if(f1 < 0){
		fprintf(stderr, "can't open the file %s\n", argv[1]);
		exit(1);
	}
	if(f2 < 0){
		fprintf(stderr, "can't open the file %s\n", argv[2]);
		exit(1);
	}
	char buffer[MaxBuffer];
	int result;
	while((result = read(f1, buffer, MaxBuffer * sizeof(char))) >0){
		write(f2, buffer, result);
	}
	printf("file Copy finished\n");
	close(f1);
	close(f2);
	return 0;
}
