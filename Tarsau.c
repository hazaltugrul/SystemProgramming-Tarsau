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

		//TODO: Redirect the method to the method that has the  getFileInfos(fileNameArray, fileNameCount, outputFileName); sign or something like that.
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