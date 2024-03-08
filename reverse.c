#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>


void usage(char *u){
	fprintf(stderr, "make the file reverse\n");
	fprintf(stderr, "usage:\n");
	fprintf(stderr, "%s file\n", u);
}

int main(int argc, char **argv){
	if(argc != 2){
		usage(argv[0]);
		exit(1);
	}
	int fd = open(argv[1], O_RDWR);
	if(fd < 0){
		printf("can't open file %s\n", argv[1]);
	}
	int filesize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char *buf = (char *)malloc(sizeof(char ) *filesize);
	read(fd, buf, filesize);
	lseek(fd, 0, SEEK_SET);
	char temp;
	for(int i=0;i<filesize/2;i++){
		temp = buf[i];
		buf[i] = buf[filesize-1-i];
		buf[filesize-1-i] = temp;
	}
	write(fd, buf, filesize);
	close(fd);
	return 0;
}
