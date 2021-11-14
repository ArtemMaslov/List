# ListLibrary
Библиотека структуры данных двусвязаного списка.

## Описание библиотеки
```
/// Структура списка
struct List
{
    /// Массив данных.
    listType* data;

    /// Массив, хранящий индексы следующего элемента списка.
    size_t* next;
    /// Массив,  хранящий индексы предудыщего элемента списка.
    size_t* prev;

    /// Указывает на следующий свободный элемент списка.
    size_t free;
    /// Индекс начальнлого элемента списка.
    size_t head;
    /// Индекс конечного элемента списка.
    size_t tail;

    /// Размер списка. Текущее количество занятых элементов.
    size_t size;
    /// Вместимость списка. Максимальное допустимое число хранимых элементов.
    size_t capacity;

    /// Отсортирован ли список. Чтобы изменить состояние, вызовите функцию ListSort(). Если верно, то доступ к элементам списка будет происходить быстрее.
    bool   sorted;   
};


/**
 * @brief          Конструктор списка.
 * @param lst      Указатель на список.
 * @param capacity Вместимость списка. Наибольшее число элементов, которые будут храниться в списке в процессе работы с ним.
 * @return 
*/
bool ListConstructor(List* lst, size_t capacity);

/**
 * @brief     Деструктор списка.
 * @param lst Указатель на список.
*/
void ListDestructor(List* lst);

/**
 * @brief           Добавить элемент в список после элемента data[dataIndex].
 * @param lst       Указатель на список.
 * @param value     Добавляемое в список значение.
 * @param dataIndex Физический индекс в списке.
 * @return          false в случае ошибки.
*/
bool ListAddElemAfter(List* lst, listType value, size_t dataIndex);

/**
 * @brief           Добавить элемент в список перед элементом data[dataIndex].
 * @param lst       Указатель на список.
 * @param value     Добавляемое в список значение.
 * @param dataIndex Физический индекс в списке.
 * @return          false в случае ошибки.
*/
bool ListAddElemBefore(List* lst, listType value, size_t dataIndex);

/**
 * @brief           Удалить элемент data[dataIndex] из списка.
 * @param lst       Указатель на список.
 * @param dataIndex Физический индекс в списке.
 * @return          false в случае ошибки.
*/
bool ListRemoveElem(List* lst, size_t dataIndex);

/**
 * @brief               Работает быстро, если список отсортирован, иначе работает медленно. Возвращает указатель на элемент списка.
 * @param lst           Указатель на список.
 * @param logicalIndex  Логический номер элемента в списке.
 * @return              Указатель на элемент списка. nullptr, если logicalIndex > lst->size.
*/
listType* ListGetElemAt(List* lst, size_t logicalIndex);

/**
 * @brief               Работает быстро, если список отсортирован, иначе работает медленно. Возвращает физический индекс элемента.
 * @param lst           Указатель на список.
 * @param logicalIndex  Логический номер элемента в списке.
 * @return              Индекс элемента списка. 0, если logicalIndex > lst->size.
*/
size_t GetPhysicalIndex(List* lst, size_t logicalIndex);

/**
 * @brief     Отсортировать список по логическому индексу.
 * @param lst Указатель на список.
 * @return    false в случае ошибки.
*/
bool ListSort(List* lst);
```
