#ifndef LIST_CONFIG_H
#define LIST_CONFIG_H

#include <string.h>

#include "List.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/// @brief Тип элементов списка
/// Заменить на тип элементов, которые будет использовать список
typedef int ListType;

#define LIST_LOG_IMAGE_FOLDER "_listImgs\\"

#define LIST_ENABLE_ASSERT_STATUS true

#if LIST_ENABLE_ASSERT_STATUS
#define LIST_ASSERT_STATUS assert(list->Status == LIST_ERR_NO_ERRORS)
#else
#define LIST_ASSERT_STATUS ((void)0)
#endif

const double ListResizeCoef = 1.5;

/**
 * @brief      Сравнивает два элемента списка.
 * @param val1 Первый элемент.
 * @param val2 Второй элемент.
 * @return     true, если элементы равны. false, если элементы различны.
*/
static inline bool ListComparator(const ListType* val1, const ListType* val2)
{
	return *val1 == *val2;
}

/// Размер буфера преобразования элемента в строку.
const size_t ListToStringSize = 128;

/**
 * @brief        Преобразовать в строку элемент списка.
 * @param buffer Буфер выходной строки.
 * @param value  Элемент списка.
*/
static inline void ListElementToString(char* buffer, const size_t bufferSize, const ListType* value)
{
	snprintf(buffer, bufferSize, "%d", *value);
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#endif // !LIST_CONFIG_H