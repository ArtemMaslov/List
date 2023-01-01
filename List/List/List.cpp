#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "ListConfig.h"
#include "ListLogs.h"
#include "__private.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static int ListResize(List* list);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int ListConstructor(List* list, const size_t capacity)
{
    LOG_LIST_TRACE_CTOR;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    list->Capacity = capacity;
    list->Sorted   = true;
    list->Status   = LIST_ERR_NO_ERRORS;

    list->Head = 0;
    list->Tail = 0;
    list->Size = 0;
    list->Free = 1;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    list->Nodes = (ListNode*)calloc(capacity + 1, sizeof(ListNode));

    if (!list->Nodes)
    {
        LOG_LIST_ERR_MEMORY;

        list->Status |= LIST_ERR_MEMORY;

        return list->Status;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    ListNode* nodes = list->Nodes;

    for (size_t st = 1; st < capacity; st++)
        nodes[st].Next = st + 1;

    nodes[capacity].Next = 0;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return list->Status;
}

void ListDestructor(List* list)
{
    LOG_LIST_TRACE_CTOR;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    free(list->Nodes);

    memset(list, 0, sizeof(List));
}

void ClearList(List* list)
{
    LOG_LIST_TRACE_CTOR;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    const size_t capacity = list->Capacity;

    list->Sorted = true;

    list->Head = 0;
    list->Tail = 0;
    list->Size = 0;
    list->Free = 1;

    ListNode* nodes = list->Nodes;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    // �������� �������
    memset(nodes, 0, (capacity + 1) * sizeof(ListType));

    // ������ ������ ������ ���������.
    for (int st = 0; st <= capacity; st++)
        nodes[st].Next = st + 1;

    // �������������� ������.
    nodes[capacity].Next = 0;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int ListAddElemAfter(List* list, const ListType* value, const size_t dataIndex)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);
    assert(value);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    if (list->Size > list->Capacity)
    {
        LOG_F_LIST_ERR("������ ������ = %zd ������ ����������� = %zd", list->Size, list->Capacity);

        list->Status |= LIST_ERR_SIZE;

        return list->Status;
    }

    if (CHECK_FREE_ELEM(list, dataIndex))
    {
        LOG_F_LIST_ERR("������� �������� ������� ����� �������. dataIndex = %zd.", dataIndex);

        list->Status |= LIST_ERR_ADD_FREE;

        return list->Status;
    }

    // ������ �����, �������� �����.
    if (CHECK_LIST_FULL(list))
    {
        if (ListResize(list) != LIST_ERR_NO_ERRORS)
            return list->Status;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    ListNode* nodes = list->Nodes;

    // ����� ������ ����� � ������.
    const size_t insertElemIndex = FIND_EMPTY_PLACE(list);

    // ���� ��������� ������� �� � ����� ������, �� ��������� ����������.
    // ����� �� ���������� ������� ������.
    if (list->Tail != dataIndex) 
        list->Sorted = false;
    else
        list->Tail = insertElemIndex;

    // ���� ��������� ������� ����� ������� ������.
    if (dataIndex == 0)
        list->Head = insertElemIndex;

    // �������� ���������� ������������ ��������.
    memcpy(&nodes[insertElemIndex].Data, value, sizeof(ListType));
    
    // ���� Next ������ ��������� �������� ����������� ������� ������ ����.
    list->Free = nodes[insertElemIndex].Next;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    // �������� ���� Next � ������:

    // ���� Next ������������ ��������.
    nodes[insertElemIndex].Next = nodes[dataIndex].Next;
    // ���� Next ��������������� ��������.
    nodes[dataIndex].Next = insertElemIndex;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    // �������� ���� Prev:

    // ���� Prev ������������ ��������.
    nodes[insertElemIndex].Prev = dataIndex;
    // ���� Prev ���������� �� ����������� ���������.
    nodes[nodes[insertElemIndex].Next].Prev = insertElemIndex;

    // ������ ������ ����������.
    list->Size++;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return list->Status;
}

int ListAddElemBefore(List* list, const ListType* value, const size_t dataIndex)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);
    assert(value);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    if (list->Size > list->Capacity)
    {
        LOG_F_LIST_ERR("������ ������ = %zd ������ ����������� = %zd", list->Size, list->Capacity);

        list->Status |= LIST_ERR_SIZE;

        return list->Status;
    }

    if (CHECK_FREE_ELEM(list, dataIndex))
    {
        LOG_F_LIST_ERR("������� �������� ������� ����� ������. dataIndex = %zd.", dataIndex);

        list->Status |= LIST_ERR_ADD_FREE;

        return list->Status;
    }

    // ������ �����, �������� �����.
    if (CHECK_LIST_FULL(list))
    {
        if (ListResize(list) != LIST_ERR_NO_ERRORS)
            return list->Status;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    ListNode* nodes = list->Nodes;

    // ����� ������ ����� � ������.
    const size_t insertElemIndex = FIND_EMPTY_PLACE(list);

    // � ����� ������ ����� ������� � ������ ������ ������� ����������
    list->Sorted = false; 

    // ���� ��������� ������� ����� ������� ������, �� �� ����������� ����� �������.
    if (list->Head == dataIndex)
        list->Head = insertElemIndex;

    // �������� ���������� ������������ ��������.
    memcpy(&nodes[insertElemIndex].Data, value, sizeof(ListType));

    // ���� Next ������ ��������� �������� ����������� ������� ������ ����.
    list->Free = nodes[insertElemIndex].Next;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    // �������� ���� Prev:

    // ���� Prev ������������ ��������.
    nodes[insertElemIndex].Prev = nodes[dataIndex].Prev;
    // ���� Prev ���������� �� ����������� ���������.
    nodes[dataIndex].Prev = insertElemIndex;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    // �������� ���� Next � ������:

    // ���� Next ��������������� ��������.
    nodes[nodes[insertElemIndex].Prev].Next = insertElemIndex;
    // ���� Next ������������ ��������.
    nodes[insertElemIndex].Next = dataIndex;

    // ������ ������ ����������.
    list->Size++;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return list->Status;
}

int ListRemoveElem(List* list, const size_t dataIndex)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    if (list->Size <= 0)
    {
        LOG_LIST_ERR("������� ������� ������� �� ������� ������.");

        list->Status |= LIST_ERR_REMOVE_EMPTY;

        return list->Status;
    }

    if (CHECK_FREE_ELEM(list, dataIndex))
    {
        LOG_F_LIST_ERR("������� ������� ������ �������. dataIndex = %zd.", dataIndex);

        list->Status |= LIST_ERR_REMOVE_FREE;

        return list->Status;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    ListNode* nodes = list->Nodes;

    size_t nextIndex = nodes[dataIndex].Next;
    size_t prevIndex = nodes[dataIndex].Prev;

    // �������� �������� �� �������� ������� ����������.
    if (dataIndex != list->Tail && dataIndex != list->Head)
        list->Sorted = false;

    // ������� ������� ������.
    memset(nodes + dataIndex, 0, sizeof(ListNode));

    // �������� ����� ��������� ������
    nodes[nextIndex].Prev = prevIndex;
    nodes[prevIndex].Next = nextIndex;

    // �������� ������ � ����� ������.
    if (list->Tail == dataIndex)
        list->Tail = prevIndex;

    if (list->Head == dataIndex)
        list->Head = nextIndex;

    // �������� �������������� ������� ������.
    nodes[dataIndex].Next = list->Free;
    nodes[dataIndex].Prev = 0;

    list->Free = dataIndex;

    // ��������� ������ ������.
    list->Size--;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    
    return list->Status;
}

ListType* ListGetElemAt(List* list, const size_t logicalIndex)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    if (logicalIndex > list->Size)
    {
        LOG_F_LIST_ERR("������� �������� ������ � �� ������������� �������� ������. "
                     "logicalIndex = %zd, list->Size = %zd", logicalIndex, list->Size);

        list->Status |= LIST_ERR_SELECT_FREE;

        return nullptr;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    ListNode* nodes = list->Nodes;

    // ���� ������ ������������, �� ���������� ������ ����� �����������.
    // ����� ���� ������ �� �������� �����.
    if (list->Sorted)
    {
        return &nodes[logicalIndex + list->Head].Data;
    }
    else
    {
        size_t cur   = list->Head;
        size_t end   = list->Tail;
        size_t index = 0;

        while (cur != end)
        {
            if (index == logicalIndex)
                return &nodes[cur].Data;

            index++;
            cur = nodes[cur].Next;
        }
    }

    // ������ ��� ��������� �� ����� �����������.
    LOG_LIST_ERR("�������������� ������. ���������� �� ���������� ����. "
                 "���������� ������� �� ��� ������ � ������.");

    list->Status |= LIST_ERR_SELECT_FREE;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return nullptr;
}

size_t GetPhysicalIndex(List* list, const size_t logicalIndex)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    if (logicalIndex > list->Size)
    {
        LOG_F_LIST_ERR("������� �������� ������ � �� ������������� �������� ������. "
                       "logicalIndex = %zd, list->Size = %zd", logicalIndex, list->Size);

        list->Status |= LIST_ERR_SELECT_FREE;

        return 0;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    // ���� ������ ������������, �� ���������� ������ ��������� � ����������.
    if (list->Sorted)
        return logicalIndex + list->Head;

    // ����� ����� �� �������� �����.
    size_t cur = list->Head;

    for (size_t st = 0; st < logicalIndex; st++)
        cur = list->Nodes[cur].Next;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return cur;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

int ListSort(List* list)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    const size_t listSize     = list->Size;
    const size_t listCapacity = list->Capacity;
    size_t       currentIndex = 0;
    size_t       nextIndex    = 0;

    ListNode     tmp          = {};

    ListNode*    nodes        = list->Nodes;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    // ��������� ���� � ������ �������.
    for (size_t correctIndex = 0; correctIndex <= listSize; correctIndex++)
    {
        nextIndex = nodes[currentIndex].Next;

        // ���� ������� ��������� �� �� ���� �����, �� ��� ����� ��������� �� ���������� �����.
        if (currentIndex != correctIndex)
        {
            memcpy(&tmp, nodes + currentIndex, sizeof(ListNode));

            memcpy(nodes + currentIndex, nodes + correctIndex, sizeof(ListNode));

            memcpy(nodes + correctIndex, &tmp, sizeof(ListNode));
        }

        currentIndex = nextIndex;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    // ������������� ������ � �����.
    list->Head = 1;
    list->Tail = listSize;

    // ������������� �������� ���������� Next � Prev.

    // ������������� �������� Next � Prev ��� ����������� �����.
    for (size_t listIndex = 0; listIndex <= listSize; listIndex++)
    {
        nodes[listIndex].Next = listIndex + 1;
        nodes[listIndex].Prev = listIndex - 1;
    }

    // �������������� �������.
    nodes[0].Prev        = listSize;
    nodes[listSize].Next = 0;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    // �������������� ��������� ��������.
    if (listSize < listCapacity)
    {
        // ��������� �������� ����.
        list->Free = listSize + 1;

        // ������� ����� ������.
        memset(nodes + (listSize + 1) * sizeof(ListNode), 0, (listCapacity - listSize) * sizeof(ListNode));

        // ������������� �������� Next � Prev ��� ��������� ���������.
        for (size_t st = listSize + 1; st < listCapacity; st++)
            nodes[st].Next = st + 1;

        // ��������� ������� ��������� �� 0.
        nodes[listCapacity].Next = 0;
    }
    else
        list->Free = 0;            // ������ �����.

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return list->Status;
}

ListNode* ListFind(const List* list, const ListType* element)
{
    LOG_LIST_TRACE_FUNC_0;

    assert(list);
    assert(element);

    LIST_ASSERT_STATUS;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    
    size_t       current  = list->Head;
    const size_t listSize = list->Size;

    // �������� ����� ���������.
    for (size_t st = 0; st < listSize; st++)
    {
        if (ListComparator(element, &list->Nodes[current].Data))
            return list->Nodes + current;

        current = list->Nodes[current].Next;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    return nullptr;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static int ListResize(List* list)
{
    LOG_LIST_TRACE_FUNC_1;

    assert(list);

    LIST_ASSERT_STATUS;
    
    // ����� ������� ������� ����� ���������, ��� ������ �����.
    assert(CHECK_LIST_FULL(list));

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    
    size_t oldCapacity = list->Capacity;
    size_t newCapacity = oldCapacity * ListResizeCoef;

    if (newCapacity <= oldCapacity)
    {
        LOG_LIST_ERR_MEMORY;

        list->Status |= LIST_ERR_MEMORY;

        return list->Status;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\

    ListNode* nodes = (ListNode*)realloc(list->Nodes, (newCapacity + 1) * sizeof(ListNode));

    if (!nodes)
    {
        LOG_LIST_ERR_MEMORY;

        list->Status |= LIST_ERR_MEMORY;

        return list->Status;
    }

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    
    // ��������� ������ ��������� �������, ��������� ��������� �� ����� ���� ������.
    list->Nodes = nodes;
    
    // ������� ����� ������.
    memset(nodes + (oldCapacity + 1), 0, (newCapacity - oldCapacity) * sizeof(ListNode));

    // ��������� ���������� ������ � ������ ��������� ���������.
    for (size_t st = oldCapacity + 1; st <= newCapacity; st++)
        nodes[st].Next = st + 1;

    // ���� ���������� ������ ����������� ����� ������ ��������� �������, �� ����� �������� ����������.
    nodes[newCapacity].Next = 0;
    list->Free = oldCapacity + 1;

    list->Capacity = newCapacity;

    //***\\---//***\\-----//***\\---//***\\-----//*****\\-----//***\\---//***\\-----//***\\---//***\\
    
    return list->Status;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\