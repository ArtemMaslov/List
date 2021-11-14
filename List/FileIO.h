#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

enum FileIOErrors
{
	FILE_IO_ERR_NO_ERRORS = 0,
	FILE_IO_ERR_MEMORY    = 1 << 1,
	FILE_IO_ERR_FILE      = 1 << 2
};

struct Text
{
	size_t Size;
	char*  Data;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool DirectoryExist(const char* path);

size_t GetFileSize(FILE* file);

Text ReadFile(const char* fileName, int* state);

int TextDestructor(Text* text);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#endif // !FILE_IO_H