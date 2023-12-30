#include "extractFiles.h"
#define Max_Byte 3000

char FullContent[100][Max_Byte][Max_Byte]; // 2D array for content storage

int findContents(FileInfo *fileInfos, char *archiveFileName, int fileCount) {
    FILE *file = fopen(archiveFileName, "r");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return 1;
    }

    char line[10000]; // Temporary storage for each line

    // Skip lines until (fileCount + 1) line
    for (int i = 0; i < (fileCount + 1); i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            printf("Insufficient content available.\n");
            fclose(file);
            return 1;
        }
    }

    int currentFileIndex = 0;
    int lineNumber = 0;

    while (currentFileIndex < fileCount) {
        int lineTotalSize = 0;

        while (lineTotalSize < fileInfos[currentFileIndex].size) {
            if (fgets(line, sizeof(line), file) != NULL) {
                snprintf(FullContent[currentFileIndex][lineNumber], sizeof(line), "%s", line);
                lineTotalSize += strlen(line);
                lineNumber++;
            } else {
                break; // Dosya sonuna gelindi
            }
        }

        // Assign content to FileInfo's content attribute
        fileInfos[currentFileIndex].content = (char *)malloc(lineTotalSize + 1);
        strcpy(fileInfos[currentFileIndex].content, "");

        for (int i = 0; i < lineNumber; i++) {
            strcat(fileInfos[currentFileIndex].content, FullContent[currentFileIndex][i]);
        }

        currentFileIndex++;
        lineNumber = 0;
    }

    fclose(file);
    return 0;
}


void extractArchive(char *archiveFileName, char *outputFolderName) {
    
    if (mkdir(outputFolderName, 0777) == -1) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }
    
    FILE *archiveFile = fopen(archiveFileName, "r");
    if (archiveFile == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    int lineCount = 0;
    int fileCount = 0;
    char line[1000];
    FileInfo fileInfos[MAX_FILES];
    
    int startingLine = 2;
    while (lineCount < startingLine - 1) {
        fgets(line, sizeof(line), archiveFile);
        lineCount++;
    }

    while (fgets(line, sizeof(line), archiveFile) != NULL && fileCount < MAX_FILES) {
        if (line[0] == '|') {
            char fileName[100];
            int permissions;
            long size;

            if (sscanf(line, "|%19[^,],%o,%ld|", fileName, &permissions, &size) == 3) {
               strncpy(fileInfos[fileCount].fileName, fileName, sizeof(fileInfos[fileCount].fileName));
		fileInfos[fileCount].fileName[sizeof(fileInfos[fileCount].fileName) - 1] = '\0'; // Diziyi null-terminated yapmak için

                fileInfos[fileCount].permissions = permissions;
                fileInfos[fileCount].size = size;
                fileCount++;
            } 
        }
    }


    fclose(archiveFile);
    
    
       if (findContents(fileInfos, archiveFileName, fileCount) == 0) {
        for (int i = 0; i < fileCount; i++) {
            char filePath[100];
            snprintf(filePath, sizeof(filePath), "%s/%s", outputFolderName, fileInfos[i].fileName);

            FILE *file = fopen(filePath, "w");
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }

            fprintf(file, "%s", fileInfos[i].content);
            fclose(file);

            // Set the file permissions to the original permissions
            if (chmod(filePath, fileInfos[i].permissions) != 0) {
                perror("chmod");
                // Handle error or exit as per your requirement
            }
        }
    
    // Dosya isimlerini yazdır
    for (int i = 0; i < fileCount; ++i) {
        printf("%s ", fileInfos[i].fileName);
    }

    // Klasör adını yazdır
    printf("Files opened in the %s directory.\n", outputFolderName);
   
}

}
