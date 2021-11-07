#ifndef LIST_H_
#define LIST_H_


/// @brief Тип элементов списка
/// Заменить на тип элементов, которые будет использовать список
typedef int listType;
/// Нулевой элемент списка
const   listType nullVal = -1;

/// Включает ListValidate, ListDump
#define LIST_DIAGNOSTIC

/*#define LIST_SCALE_COEF 2
#define LIST_DECREASE_COEF 0.1
#define LIST_MIN_SIZE   256*/

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
    /// Размер списка. Текущее количество занятых элементов.
    size_t size;
    /// Вместимость списка. Максимальное допустимое число хранимых элементов.
    size_t capacity;
    /// Отсортирован ли список. Чтобы изменить состояние, вызовите функцию SortList(). Если верно, то доступ к элементам списка будет происходить быстрее.
    bool   sorted;   
    /// Индекс начальнлого элемента списка.
    size_t head;
    /// Индекс конечного элемента списка.
    size_t tail;
};

/// Коды ошибок списка
enum ListError
{
    NO_ERRORS          = 0,      /// Ошибок нет
    NULL_DATA_PTR      = 1 << 2, /// Указатель на массив данных равен nullptr
    NULL_LIST_PTR      = 1 << 3, /// Указатель на список равен nullptr
    NEXT_CORRUPTED     = 1 << 4, /// В массиве next были обнаружены поврежденные индексы.
    PREV_CORRUPTED     = 1 << 5, /// В массиве prev были обнаружены поврежденные индексы.
    DATA_CORRUPTED     = 1 << 6, /// В массиве data были обнаружены поврежденные данные.
    RESERVED_FAULT     = 1 << 7, /// 
    HEAD_FAULT         = 1 << 8, /// 
    TAIL_FAULT         = 1 << 9, /// 
};

/// Конструктор списка
void ListConstructor(List* lst);

/// Деструктор списка
void ListDestructor(List* lst);

/// Размер списка. Количество используемых элементов
inline size_t GetListSize(List* lst);

/// Добавить элемент в список после элемента data[dataIndex]
bool AddElemAfter(List* lst, listType value, size_t dataIndex);

/// Добавить элемент в список перед элементом data[dataIndex]
bool AddElemBefore(List* lst, listType value, size_t dataIndex);

#ifdef LIST_DIAGNOSTIC

int ListDump(List* lst, FILE* file);

int ValidateList(List* lst);

#endif // LIST_DIAGNOSTIC


#endif