#ifndef EXTRACTFILES_H
#define EXTRACTFILES_H

#include "fileInfo.h"

int findContents(FileInfo *fileInfos, char *archiveFileName, int fileCount);
void extractArchive(char *archiveFileName, char *outputFolderName);

#endif

