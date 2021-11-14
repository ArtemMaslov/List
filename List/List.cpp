#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


#include "List.h"

static inline bool IsPlaceEmpty(List* lst, size_t dataIndex);

static inline size_t FindEmptyPlace(List* lst);

static void ClearList(List* lst);


#ifdef LIST_DIAGNOSTIC

static void PrintElem(List* lst, size_t index, FILE* file);

static void CreateHtmlGraphicLog();

static int graphicDumpCallsCount = 1;

#endif


bool ListConstructor(List* lst, size_t capacity)
{
    assert(lst);

    lst->head = 0;
    lst->tail = 0;
    lst->capacity = capacity;
    lst->size = 0;
    
    lst->data = (listType*)calloc(capacity + 1, sizeof(listType));
    lst->next = (size_t*)calloc(capacity + 1, sizeof(size_t));
    lst->prev = (size_t*)calloc(capacity + 1, sizeof(size_t));

    if (lst->data == nullptr || lst->next == nullptr || lst->prev == nullptr)
    {
        puts("Не хватает памяти для списка.");
        ClearList(lst);
        return false;
    }

    memset(lst->data, nullVal, (capacity + 1) * sizeof(listType));

    lst->free = 1;
    lst->sorted = true;
    for (int st = 1; st <= capacity; st++)
        lst->next[st] = st + 1;
    lst->next[capacity] = 0;
    
    ListDump(lst, stdout);
    return true;
}

void ListDestructor(List* lst)
{
    assert(lst);

    ClearList(lst);
}

bool ListAddElemAfter(List* lst, listType value, size_t dataIndex)
{
    assert(lst);

    ListDump(lst, stdout);

    size_t emptyPlace = FindEmptyPlace(lst);

    if (emptyPlace)
    {
        if (lst->tail != dataIndex) // Вставка элемента не в конец нарушает сортировку списка
            lst->sorted = false;

        if (lst->tail == dataIndex)
            lst->tail = emptyPlace;

        if (dataIndex == 0)
            lst->head = emptyPlace;

        lst->data[emptyPlace] = value;
        
        lst->free = lst->next[emptyPlace];

        lst->next[emptyPlace] = lst->next[dataIndex];

        if (dataIndex != 0)
        {
            lst->next[dataIndex] = emptyPlace;

            if (lst->tail != emptyPlace)
                lst->prev[lst->next[emptyPlace]] = emptyPlace;
        }

        lst->prev[emptyPlace] = dataIndex;

        lst->size++;

        return true;
    }
    else
        puts("Список полон");

    ListDump(lst, stdout);

    return false;
}

bool ListAddElemBefore(List* lst, listType value, size_t dataIndex)
{
    assert(lst);

    ListDump(lst, stdout);

    if (dataIndex == 0)
    {
        puts("Нельзя вставить элемент перед 0, он является зарезервированным.");
        return false;
    }

    size_t emptyPlace = FindEmptyPlace(lst);

    if (emptyPlace)
    {
        lst->sorted = false; // В общем случае любая вставка в начало списка сбивает сортировку

        if (lst->head == dataIndex)
            lst->head = emptyPlace;

        lst->data[emptyPlace] = value;
        
        lst->free = lst->next[emptyPlace];
        
        lst->prev[emptyPlace] = lst->prev[dataIndex];
        
        if (dataIndex != 0)
        {
            lst->prev[dataIndex] = emptyPlace;
            
            if (lst->head != emptyPlace)
                lst->next[lst->prev[emptyPlace]] = emptyPlace;
        }
        
        lst->next[emptyPlace] = dataIndex;
                
        lst->size++;

        return true;
    }
    else
        puts("Список полон.");
    
    ListDump(lst, stdout);

    return false;
}

bool ListRemoveElem(List* lst, size_t dataIndex)
{
    assert(lst);

    ListDump(lst, stdout);

    if (lst->size > 0)
    {
        if (!IsPlaceEmpty(lst, dataIndex) && dataIndex > 0)
        {
            size_t nextIndex = lst->next[dataIndex];
            size_t prevIndex = lst->prev[dataIndex];

            if (lst->sorted && dataIndex != lst->tail && dataIndex != lst->head)
                lst->sorted = false;

            lst->data[dataIndex] = nullVal;

            if (nextIndex != 0)
                lst->prev[nextIndex] = prevIndex;
            if (prevIndex != 0)
                lst->next[prevIndex] = nextIndex;

            if (lst->tail == dataIndex)
                lst->tail = prevIndex;

            if (lst->head == dataIndex)
                lst->head = nextIndex;

            lst->prev[dataIndex] = 0;
            lst->next[dataIndex] = lst->free;
            lst->free = dataIndex;

            lst->size--;

            return true;
        }
        else
            puts("Удаляемый элемент не является частью списка.");
    }
    else
        puts("Список пуст.");
    
    ListDump(lst, stdout);

    return false;
}

listType* ListGetElemAt(List* lst, size_t logicalIndex)
{
    assert(lst);

    if (lst->sorted)
    {
        return lst->data + logicalIndex + lst->head;
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

static inline size_t FindEmptyPlace(List* lst)
{
    assert(lst);

    if (lst->size < lst->capacity)
        return lst->free;
    return 0;
}

static inline bool IsPlaceEmpty(List* lst, size_t dataIndex)
{
    assert(lst);

    if (dataIndex > 0 && dataIndex != lst->head && dataIndex != lst->tail && (lst->prev[dataIndex] == 0))
        return true;
    return false;
}

static void ClearList(List* lst)
{
    assert(lst);

    if (lst)
    {
        lst->head = 0;
        lst->tail = 0;
        lst->capacity = 0;
        lst->size = 0;
        lst->free = 0;
        lst->sorted = 0;

        free(lst->data);
        free(lst->next);
        free(lst->prev);

        lst->data = nullptr;
        lst->next = nullptr;
        lst->prev = nullptr;
    }
}

bool ListSort(List* lst)
{
    assert(lst);

    ListGraphicDump(lst);

    listType* newData = (listType*)calloc(lst->capacity + 1, sizeof(listType));

    if (!newData)
    {
        puts("Нехватает памяти");
        return false;
    }

    memset(newData, nullVal, (lst->capacity + 1) * sizeof(listType));

    size_t cur = lst->head;
    size_t index = 1;

    while (lst->prev[cur] != lst->tail && cur)
    {
        newData[index] = lst->data[cur];
        index++;
        cur = lst->next[cur];
    }
    
    free(lst->data);
    lst->data = newData;

    lst->tail = lst->size;
    lst->head = 1;

    lst->sorted = true;
    for (size_t st = 1; st <= lst->size; st++)
    {
        lst->next[st] = st + 1;
        lst->prev[st] = st - 1;
    }
    lst->next[lst->size] = 0;

    lst->free = lst->size + 1;
    for (size_t st = lst->size + 1; st <= lst->capacity; st++)
    {
        lst->next[st] = st + 1;
        lst->prev[st] = 0;
    }

    lst->next[lst->capacity] = 0;

    ListGraphicDump(lst);

    return true;
}

size_t GetPhysicalIndex(List* lst, size_t logicalIndex)
{
    assert(lst);
    
    if (logicalIndex > lst->size)
        return 0;

    if (lst->sorted)
        return logicalIndex + lst->head;

    size_t cur = lst->head;
    
    for (size_t st = 0; st < logicalIndex; st++)
        cur = lst->next[cur];

    return cur;
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


void ListDump(List* lst, FILE* file)
{
    assert(lst);
    assert(file);

    if (lst == nullptr)
        fputs("List равен null\n", file);
    else
    {
        size_t capacity = lst->capacity;
        size_t size     = lst->size;

        if (lst->head == 0 && lst->size > 0)
            fputs("Голова равна 0\n", file);

        if (lst->tail == 0 && lst->size > 0)
            fputs("Хвост равен 0\n", file);

        if (lst->size == 0 && (lst->tail != 0 && lst->head != 0))
            fputs("Список пуст, а голова и хвост не указывают на 0\n", file);

        if (lst->data[0] != nullVal || lst->next[0] != 0 || lst->prev[0] != 0)
        {
            fputs("Нулевой элемент поломан:\n", file);
            PrintElem(lst, 0, file);
        }

        if (size > capacity)
            fputs("Size is more than capacity\n", file);

        size_t filled = 0;

        for (int st = 1; st <= capacity; st++)
        {
            if (!IsPlaceEmpty(lst, st))
            {
                size_t curPrev = lst->prev[st];
                size_t curNext = lst->next[st];

                size_t nextPrev = lst->next[curPrev];
                size_t prevNext = lst->prev[curNext];

                if ((nextPrev != st && lst->head != st) || (nextPrev != 0 && lst->head == st) ||
                    (prevNext != st && lst->tail != st) || (prevNext != 0 && lst->tail == st))
                {
                    fprintf(file, "Элемент %d поломан:\n", st);
                    PrintElem(lst, st, file);
                    fprintf(file, "Элемент, предыдущий %d:\n", st);
                    PrintElem(lst, lst->prev[st], file);
                    fprintf(file, "Элемент, следующий за %d:\n", st);
                    PrintElem(lst, lst->next[st], file);
                }
                filled++;
            }
        }

        if (filled != size)
            fputs("Число занятых элементов расходится.\n", file);

        size_t freeElems = 0;
        size_t iters = 0;
        size_t index = lst->free;

        while (iters < capacity && index != 0)
        {
            if (IsPlaceEmpty(lst, index))
            {
                if (lst->data[index] != nullVal)
                    fprintf(file, "Значение " listTypeFormate " пустого элемента %zd не очищено.\n", lst->data[index], index);
                if (lst->prev[index] != 0)
                    fprintf(file, "Указатель на предыдущий элемент пустого элемента %zd не равен 0.\n", index);
            }
            else
            {
                fprintf(file, "Элемент %zd в последовательности свободных мест не пуст!\n", index);
                PrintElem(lst, index, file);
            }
            index = lst->next[index];
        }
    }

    fflush(file);
}

static void PrintElem(List* lst, size_t index, FILE* file)
{
    assert (lst);

    fprintf(file, "Статус элемента %zd, %s:\n"
           "   data[%zd] = " listTypeFormate "\n"
           "   next[%zd] = %zd\n"
           "   prev[%zd] = %zd\n\n",
        index, (IsPlaceEmpty(lst, index))? "свободен" : "занят",
        index, lst->data[index], index, lst->next[index], index, lst->prev[index]);
}

void ListGraphicDump(List* lst)
{
    assert(lst);

    if (graphicDumpCallsCount == 1)
        atexit(CreateHtmlGraphicLog);

    char fileInputName[100] = "";
    char fileOutputName[100] = "";

    sprintf(fileOutputName, "D:\\Язык C\\List\\HtmlLog\\img%d.png", graphicDumpCallsCount);
    sprintf(fileInputName, "D:\\Язык C\\List\\HtmlLog\\src%d.gv", graphicDumpCallsCount);

    FILE* file = fopen(fileInputName, "w");

    if (!file)
    {
        puts("Ошибка открытия файла");
        return;
    }

    const char* pattern             = "digraph structs\n"
                                      "{\n"
                                      "graph[splines = ortho]\n"
                                      "{\n"
	                                  "rankdir = LR;\n"
	                                  "rank = same;\n"
	                                  "node [shape = record, style = rounded, margin = \"0.11 0.6\"];\n";

    const char* blueEdgeColor       = "edge [color = blue];\n";
    const char* greenEdgeColor      = "edge [color = green];\n";
    const char* regEdgeColor        = "edge [color = red];\n";
    const char* whiteEdgeColor      = "edge [color = white];\n";
    
    const char* ptrEdgeFormat       = "%s->f%zd;\n";
    const char* nextEdgeFormat      = "f%zd:<l1>->f%zd;\n";
    const char* prevEdgeFormat      = "f%zd:<l2>->f%zd;\n";
    const char* whiteEdgeFormat     = "f%zd->f%zd;\n";

    const char* nodeFormat          = "f%zd [label = \"{"
                                      "{ <l3> index\\n%zd }|"
                                      "{ value\\n" listTypeFormate " }|"
                                      "{ <l1> prev\\n%zd | <l2> next\\n%zd }}\"];\n";

    const char* nodePtrFormat       = "{\n%s [label = \"%s\\n%zd\"];\n}\n";
    const char* nodeStrFormat       = "{\n%s [label = \"%s\\n%s\"];\n}\n";

    fputs(pattern, file);

    for (size_t st = 0; st <= lst->capacity; st++)
        fprintf(file, nodeFormat, st, st, lst->data[st], lst->prev[st], lst->next[st]);

    fputs(whiteEdgeColor, file);
    for (size_t st = 0; st < lst->capacity; st++)
        fprintf(file, whiteEdgeFormat, st, st+1);
        
    fputs("}\n", file);
    
    fprintf(file, nodePtrFormat, "head", "head", lst->head);
    fprintf(file, nodePtrFormat, "tail", "tail", lst->tail);
    fprintf(file, nodePtrFormat, "free", "free", lst->free);
    fprintf(file, nodeStrFormat, "sorted", "sorted", lst->sorted ? "true" : "false");

    fputs(blueEdgeColor, file);
    for (size_t st = 1; st <= lst->capacity; st++)
        if (lst->prev[st] != 0)
            fprintf(file, prevEdgeFormat, st, lst->prev[st]);

    fputs(greenEdgeColor, file);
    for (size_t st = 1; st <= lst->capacity; st++)
        if (lst->next[st] != 0)
            fprintf(file, nextEdgeFormat, st, lst->next[st]);
    
    fputs(regEdgeColor, file);
    fprintf(file, ptrEdgeFormat, "head", lst->head);
    fprintf(file, ptrEdgeFormat, "tail", lst->tail);
    fprintf(file, ptrEdgeFormat, "free", lst->free);

    fputs("}\n", file);

    fclose(file);

    char cmd[200] = "";
    sprintf(cmd, "dot \"%s\" -Tpng > \"%s\"", fileInputName, fileOutputName);
    system(cmd);
    //remove(fileName);

    graphicDumpCallsCount++;
}

static void CreateHtmlGraphicLog()
{
    FILE* file = fopen("D:\\Язык C\\List\\HtmlLog\\Log.html", "w");
    if (!file)
    {
        puts("Ошибка открытия файла");
        return;
    }

    const char* pattern = "<html>\n"
                          "<head><title>Лог бибилотеки StackLibrary.</title><style>font{line - height: 0.8; } body{background - color: #404040; } head{background - color: #404040; }</style></head>\n"
                          "<body>\n"
                          "<h1><font color = \"99B333\">Лог бибилотеки ListLibrary.</font></h1>\n";

    const char* ending  = "</body>\n</html>";

    fputs(pattern, file);

    for (int st = 0; st < graphicDumpCallsCount; st++)
        fprintf(file, "<img src = \"img%d.png\">\n", st);

    fputs(ending, file);

    fclose(file);
}


#endif // LIST_DIAGNOSTIC
