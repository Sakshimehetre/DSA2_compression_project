#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#define N 256
#include"huffmantree.h"

int main(int argc, char *argv[]) {
    printf("FILE COMPRESSION & DECOMPRESSION SYSTEM\n");
	if(argc == 4) {
		if (strcmp(argv[1], "-c") == 0) {
			compress(argv[2], argv[3]);
			printf("Compressed file successfully...\n");
		}
		else if(strcmp(argv[1], "-uc") == 0) {
			decompress(argv[2], argv[3]);
			printf("Decompressed file successfully...\n");
		}

	}
	else{
        printf("Please enter the file name in following format\n");
		printf("./a -c <file> <compressed file> : compression\n");
		printf("./a -uc <compressed file> <uncompressed file> : decompression\n");
	}

	return 0;
}
