# file IO
###### tags: `programing` `c`
了解
- [ ] fopen
- [ ] fread
- [ ] fwrite
- [ ] open
- [ ] read
- [ ] write
- [ ] lseek
有f和沒有f主要差異在於是否有buffer，f開頭有內建buffer
## fopen
```c
#include <stdio.h>
FILE *fopen(const char *restrict pathname, const char *restrict mode);
```
pathname:檔案的名稱、路徑
### mode
共有六種
* r : 讀，指標指在最前方
* r+: 讀加寫，指標指在最前方
* w : 寫，無檔案建立新檔案，有檔案直接覆蓋，指標指在最前方
* w+: 同 w 但可讀，指標指在最前方
* a : 在檔案尾端新增，無檔案建立新檔案，指標指在檔案尾端
* a+: 同 a 但可讀，指標位置因不同系統而不同
### restrict 
restrict是 C 的關鍵字，只能用來修飾 pointer，目的是協助 compiler 做最佳化
是告知 compiler ，這一個 restrict pointer 是唯一指向 data 的 pointer，
意即程式當中不會透過其他變數 ( pointer,或者直接存取 ) 來 access 此 data，
ex:
```c
void* memcpy (void* restrict destination, const void* restrict source, size_t n);
```
代表destination和source區段是不可重疊的

## fread and fwrite
```c
size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
```
* ptr: buffer
* size: 一個資料的大小 sizeof(type)
* count: 最大讀取量
* stream: file pointer
## example
COPY file1 to file2
```c
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
	return 0;
}

```
## open
```c
#include <sys/types.h>
#include <sys/stat.h>    
#include <fcntl.h>

int open(const char * pathname, int flags);
int open(const char * pathname, int flags, mode_t mode);
```
pathname:檔案名稱、路徑
### flags
有以下幾種
* O_RDONLY 以只讀方式開啟檔案
* O_WRONLY 以只寫方式開啟檔案
* O_RDWR 以可讀寫方式開啟檔案.
上述三種旗標是互斥的, 也就是不可同時使用, 但可與下列的旗標利用OR(|)運算子組合.
* O_CREAT 若欲開啟的檔案不存在則自動建立該檔案.
* O_TRUNC 若檔案存在並且以可寫的方式開啟時, 此旗標會令檔案長度清為0, 而原來存於該檔案的資料也會消失.
* O_APPEND 當讀寫檔案時會從檔案尾開始移動, 也就是所寫入的資料會以附加的方式加入到檔案後面.
### mode
-rw——- (600) 只有擁有者有讀寫許可權
[請見](https://codertw.com/%E7%A8%8B%E5%BC%8F%E8%AA%9E%E8%A8%80/609512/)
## read and write
```c
#include<unistd.h>

ssize_t read(int fd,void*buf,size_t count);
ssize_t write(int fd,const void*buf,size_t count);
```
* fd: file id (open 後的回傳值)
* buffer: 資料存取位置
* count:
    * read: 最大存取byte數量
    * write: 寫入byte數量
## example
COPY file1 to file2
```c=
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
	return 0;
}
```
## lseek
```c
 #include <sys/types.h>
 #include <unistd.h>
 
 int lseek(int fd, int offset, whence);
 off_t lseek(int fd, off_t offset, int whence);
 
```
每一個已開啟的檔案都有一個讀寫位置, 當read()或write()時, 讀寫位置會隨之增加,lseek()便是用來控制該檔案的讀寫位置
回傳檔案位置
fd: file index
offset: 移動位移量
### whence
* SEEK_SET 引數offset 即為新的讀寫位置.
* SEEK_CUR 以目前的讀寫位置往後增加offset 個位移量.
* SEEK_END 將讀寫位置指向檔案尾後再增加offset 個位移量.
當whence 值為SEEK_CUR 或 SEEK_END 時, 引數offet 允許負值的出現.
### 找行尾用法
```c
int fileSize = lseek(fd, 0, SEEK_END);
lseek(fd, 0, SEEK_SET);
```
### example
#### lower case
make the content in file to lower case
```c=
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

	return 0;
}
```
#### reverse
rever the file data
```c=
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
	return 0;
}
```

# [github連結](https://github.com/forward0606/file_IO)
