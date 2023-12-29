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




int main(int argc, char* argv[])
{
	// There will be a method here somethhing like that chooseParameter(argc, argv);
	return EXIT_SUCCESS;
}