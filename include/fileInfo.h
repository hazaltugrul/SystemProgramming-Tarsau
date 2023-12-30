#ifndef FILEINFO_H
#define FILEINFO_H

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_FILES 32
#define MAX_TOTAL_SIZE 200 * 1024 * 1024 // 200 MB

typedef struct {
    char fileName[20];
    mode_t permissions;
    off_t size;
    char *content;
} FileInfo;

int isTextFile(const char *fileName);
int chooseParameter(int argc, char *argv[]);

#endif





