#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

//#include "..\..\TxLib\TX\TXLib.h"


#include "List.h"

static inline bool IsPlaceEmpty(List* lst, size_t dataIndex);

static size_t FindEmptyPlace(List* lst);

//static bool ListResize(List* lst);


void ListConstructor(List* lst)
{
    assert(lst);

    lst->head = 0;
    lst->tail = 0;
    lst->capacity = 0;
    
    lst->data = nullptr;
    lst->next = nullptr;
    lst->prev = nullptr;
}

void ListDestructor(List* lst)
{
    assert(lst);

    if (lst->data)
        free(lst->data);
}

bool AddElemAfter(List* lst, listType value, size_t dataIndex)
{
    size_t size = GetListSize(lst);

    if (size < lst->capacity - 1)
    {
        size_t emptyPlace = FindEmptyPlace(lst);

        if (lst->head == 0)
        {
            lst->data[emptyPlace] = value;
            lst->next[emptyPlace] = 0;
            lst->prev[emptyPlace] = 0;

            lst->tail = lst->head = emptyPlace;
        }
        else
        {
            lst->data[emptyPlace] = value;
            lst->next[emptyPlace] = lst->next[dataIndex];
            lst->prev[emptyPlace] = dataIndex;

            lst->next[dataIndex] = emptyPlace;

            if (dataIndex == lst->tail)
                lst->tail = emptyPlace;
        }
        lst->size++;

        return true;
    }
    else
        puts("Список полон");

    return false;
}

bool AddElemBefore(List* lst, listType value, size_t dataIndex)
{
    size_t size = GetListSize(lst);

    if (size < lst->capacity - 1)
    {
        size_t emptyPlace = FindEmptyPlace(lst);

        if (lst->head == 0)
        {
            lst->data[emptyPlace] = value;
            lst->next[emptyPlace] = 0;
            lst->prev[emptyPlace] = 0;

            lst->tail = lst->head = emptyPlace;
        }
        else
        {
            lst->data[emptyPlace] = value;
            lst->next[emptyPlace] = dataIndex;
            lst->prev[emptyPlace] = lst->prev[dataIndex];
            
            lst->prev[dataIndex] = emptyPlace;

            if (dataIndex == lst->head)
                lst->head = emptyPlace;
        }
        lst->size++;

        return true;
    }
    else
        puts("Список полон");

    return false;
}

bool RemoveElem(List* lst, size_t dataIndex)
{
    size_t size = GetListSize(lst);

    if (size > 0)
    {
        size_t nextIndex = lst->next[dataIndex];
        size_t prevIndex = lst->prev[dataIndex];

        lst->next[prevIndex] = nextIndex;

        if (lst->tail == dataIndex)
            lst->tail = prevIndex;

        if (lst->head == dataIndex)
            lst->head = nextIndex;

        lst->next[dataIndex] = 0;
        memset(lst->data + dataIndex, 0, sizeof(listType));
        
        lst->size--;

        return true;
    }
    else
        puts("Список пуст");

    return false;
}

/**
 * @brief               Работает быстро, если список отсортирован, иначе работает медленно.
 * @param lst           Указатель на список.
 * @param logicalIndex  Логический номер элемента в списке.
 * @return              Указатель на элемент списка. nullptr, если logicalIndex > lst->size
*/
listType* GetElemAt(List* lst, size_t logicalIndex)
{
    if (lst->sorted)
    {
        return lst->data + logicalIndex + 1;
    }
    else
    {
        size_t cur = lst->head;
        size_t end = lst->tail;
        size_t index = 0;

        if (logicalIndex > lst->size)
            return nullptr;

        while (cur != end)
        {
            if (index == logicalIndex)
            {
                return lst->data + cur;
            }
            index++;
            cur = lst->next[cur];
        }

        return nullptr;
    }
}

static size_t FindEmptyPlace(List* lst)
{
    for (size_t st = 0; st < lst->capacity; st++)
    {
        if (IsPlaceEmpty(lst, st))
            return st;
    }
    return 0;
}

static inline bool IsPlaceEmpty(List* lst, size_t dataIndex)
{
    if (lst->next[dataIndex] == 0 && lst->tail != dataIndex && lst->head != dataIndex)
        return true;
    return false;
}

/*static bool ListResize(List* lst)
{
    bool shouldResize = false;

    size_t size = GetListSize(lst);
    size_t oldCapacity = lst->capacity;
    
    if (size == 0)
    {
        lst->capacity = 0;
        shouldResize = true;
    }
    else if (oldCapacity == 0 && size)
    {
        lst->capacity = LIST_MIN_SIZE;
        shouldResize = true;
    }
    else
    {
        if (size >= oldCapacity)
        {
            lst->capacity *= LIST_SCALE_COEF;
            shouldResize = true;
        }
        if (size < oldCapacity / LIST_SCALE_COEF * (1 - LIST_DECREASE_COEF))
        {
            lst->capacity /= LIST_SCALE_COEF;
            shouldResize = true;
        }
    }

    if (shouldResize)
    {
        listType* resPtr = (listType*)realloc(lst->data, sizeof(listType) * lst->capacity);

        if (resPtr == nullptr)
        {
            puts("Не хватает памяти");
            return false;
        }
        lst->data = resPtr;

        memset(lst->data + oldCapacity, 0, sizeof(listType) * (lst->capacity - oldCapacity));
    }

    return true;
}*/


#ifdef LIST_DIAGNOSTIC

int ListDump(List* lst, FILE* file)
{
    fputs("ListDump\n", file);
    fprintf(file, "elemSize = %zd\n", sizeof(listType));

    int errors = ValidateList(lst);

    if (lst == nullptr)
        fputs("List is null", file);
    else
    {
        size_t capacity = lst->capacity;
        size_t size     = GetListSize(lst);

        if (size > capacity)
            puts("Size is more than capacity");

        for (int st = 0; st < capacity; st++)
        {

        }
    }

    return errors;
}

int ValidateList(List* lst)
{
    int errors = ListError::NO_ERRORS;

    if (lst != nullptr)
    {
        if (lst->data != nullptr)
        {
            size_t size     = GetListSize(lst);
            size_t capacity = lst->capacity;

            size_t notNullElemCount = 0;

            listType* data = lst->data;
            size_t*   next = lst->next;
            size_t*   prev = lst->prev;

            for (size_t st = 0; st < capacity; st++)
            {
                if (data[st] != 0 && next[st] == 0 && prev[st] == 0)
                {
                    break;
                }
            }

        }
        else
            errors |= ListError::NULL_DATA_PTR;
    }
    else
        errors = ListError::NULL_LIST_PTR;

    return errors;
}

#endif // LIST_DIAGNOSTIC
