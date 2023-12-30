#include "createArchiveFile.h"

void writeToArchive(FileInfo *fileInfos, const char *outputFileName, int fileNameCount, off_t totalSize) {
    FILE *archiveFile = fopen(outputFileName, "w");
    if (archiveFile == NULL) {
        printf("Arsiv dosyasi olusturulurken hata olustu!\n");
        return;
    }

    fprintf(archiveFile, "%010ld", totalSize);
    fprintf(archiveFile,"\n");

    // Dosya bilgilerini yaz
    for (int i = 0; i < fileNameCount; i++) {
        fprintf(archiveFile, "|%s,%o,%ld|\n", fileInfos[i].fileName, fileInfos[i].permissions, fileInfos[i].size);
    }

    for (int i = 0; i < fileNameCount; i++) {
        int fd = open(fileInfos[i].fileName, O_RDONLY);
        if (fd < 0) {
            printf("Dosya acilirken hata olustu!\n");
            continue;
        }

        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, sizeof(char), bytesRead, archiveFile);
        }
        close(fd);
    }

    fclose(archiveFile);
    printf("The files have been merged.\n");
}




void getFileInfos(char *fileNameArray[], int fileNameCount, const char *outputFileName) {
    FileInfo fileInfos[MAX_FILES];
    off_t totalSize = 0;

    for (int i = 0; i < fileNameCount; ++i) {
        struct stat fileStat;
        if (stat(fileNameArray[i], &fileStat) == -1) {
            perror("Dosya boyutunu alirken bir hata olustu");
            continue;
        }

        if (totalSize + fileStat.st_size <= MAX_TOTAL_SIZE) {
            totalSize += fileStat.st_size;

            strncpy(fileInfos[i].fileName, fileNameArray[i], sizeof(fileInfos[i].fileName) - 1);
            fileInfos[i].fileName[sizeof(fileInfos[i].fileName) - 1] = '\0';

            fileInfos[i].permissions = fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
            fileInfos[i].size = fileStat.st_size;

            int fd = open(fileNameArray[i], O_RDONLY);
            if (fd < 0) {
                perror("Couldn't open the file");
                continue;
            }

            fileInfos[i].content = (char *)malloc((fileStat.st_size + 1) * sizeof(char));
            if (fileInfos[i].content == NULL) {
                perror("Bellek tahsis edilirken bir hata olustu");
                close(fd);
                continue;
            }

            ssize_t bytesRead = read(fd, fileInfos[i].content, fileStat.st_size);
            if (bytesRead != fileStat.st_size) {
                perror("Dosya okunurken bir hata olustu");
                close(fd);
                free(fileInfos[i].content);
                continue;
            }

            fileInfos[i].content[fileStat.st_size] = '\0';

            close(fd);
        } else {
            printf("%s eklenemedi cunku totalSize 200MB yi gecti\n", fileNameArray[i]);
        }
    }

    writeToArchive(fileInfos, outputFileName, fileNameCount, totalSize);
    for (int i = 0; i < fileNameCount; ++i) {
        free(fileInfos[i].content);
    }
}


