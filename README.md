# SystemProgramming-Tarsau
 Create an archive program called "tarsau" that works like tar, rar or zip, but  does not compress.
 
## Overview

The File Archiver is a command-line utility written in C that facilitates the merging of multiple text files into a single archive file and the extraction of contents from an archive file back to individual text files. This versatile tool preserves metadata such as file name, permissions, and size. The archiver imposes a maximum total size of 200 MB for merged files to maintain practicality and prevent excessive resource consumption.

## Features

- **Merge Files (-b option):** Combine multiple text files into a single archive file for efficient storage and organization. The archive file includes information about the merged files, such as file name, permissions, and file size.

- **Extract Files (-a option):** Recover the original text files from an archive, preserving metadata and file permissions.

- **Size Limitations:** Enforce a maximum total size of 200 MB for merged files.

## Usage

### Merge Files

To merge multiple text files into an archive, use the following command:

```bash
./file_archiver -b input_files -o output_file 

input_files: List of text files to be merged.
[output_file] (Optional): Set the output archive file name. If not provided, the default is a.sau.

```bash
./file_archiver -b file1.txt file2.txt -o output.sau

## Usage

### Extract Files

To extract the contents of an archive file, use the following command:

```bash
./file_archiver -a archive_file [extract_directory]

archive_file: Specify the archive file from which to extract.
[extract_directory] (Optional): Define the target directory for extracted files. Default is d1.

```bash
./file_archiver -a archive.sau extracted_files

## Function Details

### `findContents`

```c
int findContents(FileInfo *fileInfos, char *archiveFileName, int fileCount);

This function reads the contents of an archive file and populates the fileInfos array with file information, including file content.

### `extractArchive`

```c
void extractArchive(char *archiveFileName, char *outputFolderName);

archiveFileName: Specify the name of the archive file from which to extract.
outputFolderName: (Optional) Define the target directory for extracted files.

### `writeToArchive`

```c
void writeToArchive(FileInfo *fileInfos, const char *outputFileName, int fileNameCount, off_t totalSize);

This function is responsible for writing file information and content to an archive file.

fileInfos: An array containing information about the files, including file name, permissions, size, and content.
outputFileName: Specify the name of the output archive file to be created.
fileNameCount: The number of files to be included in the archive.
totalSize: The total size of the files being archived.

### `getFileInfos`

```c
void getFileInfos(char *fileNameArray[], int fileNameCount, const char *outputFileName);

This function retrieves file information, including name, permissions, and content, for the specified array of file names.

fileNameArray: An array containing the names of the files for which information will be retrieved.
fileNameCount: The number of files in the fileNameArray.
outputFileName: Specify the name of the output archive file where the retrieved information will be stored.

### `isTextFile`

```c
int isTextFile(const char *fileName);

This function checks if a file is a text file by examining its content for ASCII characters.

fileName: The name of the file to be checked.
Return Value:

1 if the file is determined to be a text file.
0 if the file is not recognized as a text file or cannot be opened.

### `chooseParameter`

```c
int chooseParameter(int argc, char *argv[]);

This function parses command-line arguments and directs the program to the appropriate operation (merge or extract).

argc: The number of command-line arguments.
argv: An array of command-line arguments.
Return Value:

EXIT_SUCCESS (0): If the operation is successfully chosen and executed.
EXIT_FAILURE (1): If an error occurs or the command-line arguments are incorrect.


### Error Handling

The program incorporates robust error handling to provide informative messages in various scenarios, such as file open failures, insufficient content, and incorrect command-line arguments.

### Compilation

Compile the program using the provided Makefile:

```bash
make

Remove compiled files using the following command:

```bash
make clean

### Dependencies

#### External Libraries

The File Archiver program utilizes the following external libraries:

- `stdio.h`: Standard Input/Output library used for input and output operations.
- `sys/stat.h`: Provides functions to retrieve information about files (e.g., file size, permissions).
- `sys/types.h`: Contains various data types used in system-level programming.
- `stdlib.h`: Standard Library functions for memory allocation, random number generation, and others.
- `string.h`: String manipulation functions like `strcpy`, `strcat`, and `strlen`.
- `fcntl.h`: File control options for low-level file manipulation.
- `dirent.h`: Directory entry functions for working with directories.
- `unistd.h`: Declares miscellaneous symbolic constants and types (e.g., `close`, `read`).
- `errno.h`: Error handling library providing a mechanism to report errors.

These libraries are standard in C programming and are used to implement various functionalities, such as file manipulation, memory allocation, and error handling in the File Archiver program.

