#ifndef LIST_H_
#define LIST_H_


/// @brief ��� ��������� ������
/// �������� �� ��� ���������, ������� ����� ������������ ������
typedef int listType;
/// ������� ������� ������
const   listType nullVal = -1;

/// �������� ListValidate, ListDump
#define LIST_DIAGNOSTIC

/*#define LIST_SCALE_COEF 2
#define LIST_DECREASE_COEF 0.1
#define LIST_MIN_SIZE   256*/

/// ��������� ������
struct List
{
    /// ������ ������.
    listType* data;
    /// ������, �������� ������� ���������� �������� ������.
    size_t* next;
    /// ������,  �������� ������� ����������� �������� ������.
    size_t* prev;
    /// ��������� �� ��������� ��������� ������� ������.
    size_t free;
    /// ������ ������. ������� ���������� ������� ���������.
    size_t size;
    /// ����������� ������. ������������ ���������� ����� �������� ���������.
    size_t capacity;
    /// ������������ �� ������. ����� �������� ���������, �������� ������� SortList(). ���� �����, �� ������ � ��������� ������ ����� ����������� �������.
    bool   sorted;   
    /// ������ ����������� �������� ������.
    size_t head;
    /// ������ ��������� �������� ������.
    size_t tail;
};

/// ���� ������ ������
enum ListError
{
    NO_ERRORS          = 0,      /// ������ ���
    NULL_DATA_PTR      = 1 << 2, /// ��������� �� ������ ������ ����� nullptr
    NULL_LIST_PTR      = 1 << 3, /// ��������� �� ������ ����� nullptr
    NEXT_CORRUPTED     = 1 << 4, /// � ������� next ���� ���������� ������������ �������.
    PREV_CORRUPTED     = 1 << 5, /// � ������� prev ���� ���������� ������������ �������.
    DATA_CORRUPTED     = 1 << 6, /// � ������� data ���� ���������� ������������ ������.
    RESERVED_FAULT     = 1 << 7, /// 
    HEAD_FAULT         = 1 << 8, /// 
    TAIL_FAULT         = 1 << 9, /// 
};

/// ����������� ������
void ListConstructor(List* lst);

/// ���������� ������
void ListDestructor(List* lst);

/// ������ ������. ���������� ������������ ���������
inline size_t GetListSize(List* lst);

/// �������� ������� � ������ ����� �������� data[dataIndex]
bool AddElemAfter(List* lst, listType value, size_t dataIndex);

/// �������� ������� � ������ ����� ��������� data[dataIndex]
bool AddElemBefore(List* lst, listType value, size_t dataIndex);

#ifdef LIST_DIAGNOSTIC

int ListDump(List* lst, FILE* file);

int ValidateList(List* lst);

#endif // LIST_DIAGNOSTIC


#endif