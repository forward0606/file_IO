#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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
	FILE *fpsource = fopen(argv[1], "r");
	FILE *fpcopy = fopen(argv[2], "w");
	if(fpsource == NULL){
		fprintf(stderr, "can't open file:%s\n", argv[1]);
		exit(2);
	}
	if(fpcopy == NULL){
		fprintf(stderr, "can't open file:%s\n", argv[2]);
	}
	char buffer[MaxBuffer];
	size_t result;
	while((result = fread(buffer, sizeof(char), MaxBuffer, fpsource)) > 0){
		fwrite(buffer, sizeof(char), result, fpcopy);
	}
	printf("copy finiished!\n");
	fclose(fpsource);
	fclose(fpcopy);
	return 0;
}








