#ifndef CREATEARCHIVEFILE_H
#define CREATEARCHIVEFILE_H

#include "fileInfo.h"

void writeToArchive(FileInfo *fileInfos, const char *outputFileName, int fileNameCount, off_t totalSize);
void getFileInfos(char *fileNameArray[], int fileNameCount, const char *outputFileName);

#endif 

