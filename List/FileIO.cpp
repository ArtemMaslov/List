#include <assert.h>
#include <stdlib.h>

#include <windows.h>

#include "Logs/Logs.h"

#include "FileIO.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool DirectoryExist(const char* path)
{
	DWORD attr = GetFileAttributesA(path);

	if (attr == INVALID_FILE_ATTRIBUTES)
		return false;

	return (attr & FILE_ATTRIBUTE_DIRECTORY);
}

/**
 * @brief      �������� ������ �����.
 *
 * @param file ��������� �� �������� ����.
 *
 * @return	   ������ �����.
*/
size_t GetFileSize(FILE* file)
{
	assert(file);

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	return fileSize;
}

/**
 * @brief          ��������� ����.
 *
 * @param fileName ��� �����.
 *
 * @return         ��������� �� ����� ������.
 *				   ��� ���������� ������� ������� � ����� ������.
 *				   nullptr � ������ ������.
*/
Text ReadFile(const char* fileName, int* state)
{
	assert(state);
	assert(fileName);

	Text text = {};

	FILE* file = fopen(fileName, "r");

	if (!file)
	{
		LOG_F_ERR("������ �������� ����� \"%s\"", fileName);
		*state = FILE_IO_ERR_FILE;
		return text;
	}

	const size_t fileSize = GetFileSize(file);

	char* buffer = (char*)calloc(fileSize + 1, sizeof(char));

	if (!buffer)
	{
		LOG_ERR("������ ��������� ������.");
		*state = FILE_IO_ERR_MEMORY;
		return text;
	}

	size_t readed = fread(buffer, sizeof(char), fileSize, file);

	fclose(file);

	*state = FILE_IO_ERR_NO_ERRORS;

	text.Data = buffer;
	text.Size = fileSize;

	return text;
}

int TextDestructor(Text* text)
{
	assert(text);

	text->Size = 0;
	free(text->Data);

	return FILE_IO_ERR_NO_ERRORS;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\