#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TODO: correct this return value to -1 or something then correct the chooseParameter() method
int isTextFile(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	if (file) {
		int c;
		while ((c = fgetc(file)) != EOF) {
			if (c < 0 || c > 127) {
				fclose(file);
				return 0; // not ASCII 
			}
		}
		fclose(file);
		return 1; 
	}
	return 0; // Couldn't open or not a ASCII file ?? this will be a problem.
}
void writeToArchive(FileInfo* fileInfos, const char* outputFileName, int fileNameCount, off_t totalSize) {
	FILE* archiveFile = fopen(outputFileName, "w");
	if (archiveFile == NULL) {
		printf("Arsiv dosyasi olusturulurken hata olustu!\n");
		return;
	}

	fprintf(archiveFile, "%010ld", totalSize);
	fprintf(archiveFile, "\n");

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
void getFileInfos(char* fileNameArray[], int fileNameCount, const char* outputFileName) {
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

			fileInfos[i].content = (char*)malloc((fileStat.st_size + 1) * sizeof(char));
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
		}
		else {
			printf("%s eklenemedi cunku totalSize 200MB yi gecti\n", fileNameArray[i]);
		}
	}
	for (int i = 0; i < fileNameCount; ++i) {
		printf("%s", fileInfos[i].content);
	}

	writeToArchive(fileInfos, outputFileName, fileNameCount, totalSize);
	for (int i = 0; i < fileNameCount; ++i) {
		free(fileInfos[i].content);
	}
}



int chooseParameter(int argc, char* argv[]) {

	if (argc < 4 || (strcmp(argv[1], "-b") == 0 && strcmp(argv[1], "-a") == 0))
	{
		printf("Usage: %s -b input_files -o output_file\n", argv[0]);
		printf("       %s -a archive_file directory\n", argv[0]);
		return EXIT_FAILURE;
	}
	// for -b operations
	else if (strcmp(argv[1], "-b") == 0)
	{
		char* outputFileName = (strcmp(argv[argc - 2], "-o") == 0) ? argv[argc - 1] : "a.sau";
		char* fileNameArray[MAX_FILES];
		int fileNameCount = 0;

		for (int i = 2; i < argc; i++)
		{
			if (strcmp(argv[i], "-o") == 0)
			{
				break;
			}
			else
			{
				if (isTextFile(argv[i]))
				{
					if (fileNameCount < MAX_FILES)
					{
						fileNameArray[fileNameCount++] = argv[i];
					}
					else
					{
						printf("32 den daha fazla text file i mergeleyemezsiniz\n");
						return EXIT_FAILURE;
					}
				}
				else if (isTextFile(argv[i]))
				{
					printf("%s file isn't a ASCII text file.", argv[i]);
				}
			}
		}

		if (fileNameCount == 0)
		{
			printf("Argumanlariniz arasinda text file yok\n");
			return EXIT_FAILURE;
		}

		getFileInfos(fileNameArray, fileNameCount, outputFileName); 
	}
	// for -a operation
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
			//TODO: Redirect the method to the method that has the extractArchive(archiveFileName, outputFolderName); sign or something like that.
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
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    chooseParameter(argc, argv);
	return EXIT_SUCCESS;
}