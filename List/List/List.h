#ifndef LIST_H
#define LIST_H

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#include "ListConfig.h"

enum ListErrors
{
    /// Ошибок нет
    LIST_ERR_NO_ERRORS    = 0,

    /// Ошибка выделения памяти через calloc / realloc.
    LIST_ERR_MEMORY       = 1 << 0,

    /// Не верный размер списка. Size > Capacity.
    /// В нормальном случае: Size <= Capacity.
    /// При попытке вставить элемент при Size == Capacity список автоматически выделяет доп. память.
    LIST_ERR_SIZE         = 1 << 1,

    /// Попытка получить пустой элемент по логическому индексу. (LogicalIndex > List.Size).
    LIST_ERR_SELECT_FREE  = 1 << 2,

    /// Попытка вставить элемент до или после свободного.
    LIST_ERR_ADD_FREE     = 1 << 3,

    /// Попытка удалить элемент из пустого списка.
    LIST_ERR_REMOVE_EMPTY = 1 << 4,

    /// Попытка удалить свободный элемент.
    LIST_ERR_REMOVE_FREE  = 1 << 5,

    /// Не правильное значение головы списка.
    LIST_ERR_HEAD         = 1 << 10,

    /// Не правильное значение хвоста списка.
    LIST_ERR_TAIL         = 1 << 11,

    /// Нарушена связность списка.
    /// - количество элементов списка не соответствует Size. 
    /// - сломана связность массивов Next и Prev.
    LIST_ERR_DATA         = 1 << 12,

    /// Ошибка создания/удаления директории.
    LIST_ERR_FOLDER       = 1 << 13,
    
    /// Ошибка открытия файла.
    LIST_ERR_FILE         = 1 << 14
};

struct ListNode
{
    // Индекс следующего элемента в списке.
    size_t   Next;
    // Индекс предыдущего элемента в списке.
    size_t   Prev;
    // Значение элемента списка.
    ListType Data;
};

/// Структура списка
struct List
{
    /// Узлы списка.
    ListNode* Nodes;

    /// Указывает на следующий свободный элемент списка.
    size_t    Free;
    /// Индекс начального элемента списка.
    size_t    Head;
    /// Индекс конечного элемента списка.
    size_t    Tail;

    /// Размер списка. Текущее количество занятых элементов.
    size_t    Size;
    /// Вместимость списка. Максимальное допустимое число хранимых элементов.
    size_t    Capacity;

    /// Отсортирован ли список. Вызвать функцию ListSort(), чтобы отсортировать.
    /// Элементы расположены в порядке возрастания физических адресов.
    /// Если верно, то доступ к элементам списка будет происходить быстрее.
    bool      Sorted;

    /// Код состояния списка
    int       Status;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief Конструктор списка.
 * 
 * @param list     Указатель на список.
 * @param capacity Вместимость списка. Наибольшее число элементов, которые будут храниться в списке.
 * 
 * @return list->Status.
*/
int ListConstructor(List* list, const size_t capacity);

/**
 * @brief Деструктор списка.
 * 
 * @param list Указатель на список.
*/
void ListDestructor(List* list);

/**
 * @brief Очищает список. Сбрасывает состояния списка для дальнейшего использования.
 * 
 * @param list Указатель на список.
*/
void ClearList(List* list);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief Добавить элемент в список после элемента data[dataIndex].
 * 
 * @param list      Указатель на список.
 * @param value     Добавляемое в список значение.
 * @param dataIndex Физический индекс в списке, после которого нужно вставить элемент.
 * 
 * @return list->Status.
*/
int ListAddElemAfter(List* list, const ListType* value, const size_t dataIndex);

/**
 * @brief Добавить элемент в список перед элементом data[dataIndex].
 * 
 * @param list      Указатель на список.
 * @param value     Добавляемое в список значение.
 * @param dataIndex Физический индекс в списке.
 * 
 * @return list->Status.
*/
int ListAddElemBefore(List* list, const ListType* value, const size_t dataIndex);

/**
 * @brief Удалить элемент data[dataIndex] из списка.
 * 
 * @param list      Указатель на список.
 * @param dataIndex Физический индекс в списке.
 * 
 * @return list->Status.
*/
int ListRemoveElem(List* list, const size_t dataIndex);

/**
 * @brief Работает быстро, если список отсортирован, иначе работает медленно.
 * 
 * @param list         Указатель на список.
 * @param logicalIndex Логический номер элемента в списке.
 * 
 * @return Указатель на элемент списка. nullptr, если logicalIndex > lst->size.
*/
ListType* ListGetElemAt(List* list, const size_t logicalIndex);

/**
 * @brief Работает быстро, если список отсортирован, иначе работает медленно.
 *
 * @param list          Указатель на список.
 * @param logicalIndex  Логический номер элемента в списке.
 *
 * @return Физические индекс элемента списка. 0, если logicalIndex > list->Size.
*/
size_t GetPhysicalIndex(List* list, const size_t logicalIndex);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief Отсортировать список по логическому индексу.
 * 
 * После сортировки получение элемента по логическому индексу совершается за O(1).
 * Без сортировки - за O(n).
 * 
 * @param list Указатель на список.
 * 
 * @return list->Status.
*/
int ListSort(List* list);

/**
 * @brief Поиск элемента в списке.
 * 
 * @param list    Указатель на список.
 * @param element Указатель на элемент списка.
 * 
 * @return Указатель на значение или nullptr, если элемент не был найден.
*/
ListNode* ListFind(const List* list, const ListType* element);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief Проверяет список на наличие ошибок и устанавливает соответствующие значения в list->Status.
 * 
 * @param list Указатель на список.
 * 
 * @return list->Status.
*/
int ListVerify(List* list);

/**
 * @brief Выводит состояние списка в файл логов.
 * 
 * @param list       Указатель на список.
 * @param dataOffset Начальный индекс, с которого нужно выводить данные (считая с 1). 0 - зарезервированный элемент.
 * @param dataSize   Количество выводимых данных в файл логов.
*/
void ListDump(List* list, size_t dataStartIndex, size_t dataEndIndex);

/**
 * @brief Выводит состояние списка в виде графа.
 * 
 * Использует graphviz. Необходима предварительная установка.
 * 
 * @param list Указатель на список.
 * 
 * @return list->Status.
*/
int ListGraphicDump(List* list);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#endif // !LIST_H