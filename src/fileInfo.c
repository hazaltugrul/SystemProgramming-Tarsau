#include "fileInfo.h"
#include "extractFiles.h"
#include "createArchiveFile.h"

int isTextFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file) {
        int c;
        while ((c = fgetc(file)) != EOF) {
            if (c < 0 || c > 127) {
                fclose(file);
                return 0; 
            }
        }
        fclose(file);
        return 1; 
    }
    return 0; 
}


int chooseParameter(int argc, char* argv[]) {
    if ((strcmp(argv[1], "-b") == 0 && strcmp(argv[1], "-a") == 0)) {
        printf("Kullaným: %s -b giriþ_dosyalarý -o çýkýþ_dosyasý\n", argv[0]);
        printf("       %s -a arþiv_dosyasý dizin\n", argv[0]);
        return EXIT_FAILURE;
    }
    else if (strcmp(argv[1], "-b") == 0) {
        char* outputFileName = "a.sau"; 

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                if (i + 1 < argc) {
                    outputFileName = argv[i + 1];
                    break;
                }
            }
        }

        char* fileNameArray[MAX_FILES];
        int fileNameCount = 0;

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) {
                break;
            }
            else {
                int isText = isTextFile(argv[i]);
                if (!isText) {
                    printf("%s  input file format is incompatible!.\n", argv[i]);
                    return EXIT_FAILURE; 
                }
                if (fileNameCount < MAX_FILES) {
                    fileNameArray[fileNameCount++] = argv[i];
                }
                else {
                    printf("Max 32 file can be merge.\n");
                    return EXIT_FAILURE; 
                }
            }
        }

        if (fileNameCount == 0) {
            printf("There is no text file to merge.\n");
            return EXIT_FAILURE; 
        }

        getFileInfos(fileNameArray, fileNameCount, outputFileName);
    }
    else if (strcmp(argv[1], "-a") == 0)
    {
        if (argc < 3)
        {
            printf("Usage: %s -a archive_file optional[extract_directory]\n", argv[0]);
            return EXIT_FAILURE;
        }
        if (strstr(argv[2], ".sau") != NULL)
        {
            char* archiveFileName = argv[2];
            char* outputFolderName = argc == 4 ? argv[3] : "d1";
            extractArchive(archiveFileName, outputFolderName);
        }
        else
        {
            printf("Archive file is inappropriate or corrupt!\n");
        }
    }
    else {
        printf("Invalid arguments\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}