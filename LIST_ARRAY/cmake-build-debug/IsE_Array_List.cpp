//
// Created by egolg on 02.11.2020.
//


#include "IsE_Array_List.h"

#define ASSERTED !ListVerify (thou) || PictureDump (thou, ErrorName (thou->errCode), __LINE__);


int ListConstruct (unsigned int bufferSize, struct List* thou, FILE* list_logs, FILE* graph_logs, char* graph_logs_name)
{
    assert (thou);

    thou->capacity = bufferSize;
    thou->values = (double *) calloc (sizeof(*thou->values), bufferSize + 1);
    thou->headOfValue = 0;
    thou->tailOfValue = 0;

    for (int i = 0; i <= bufferSize; i++)
        thou->values[i] = POISON;


    thou->next = (int*) calloc (sizeof(*thou->next), bufferSize + 1);
    for (int i = 1; i < bufferSize; i++)
        *(thou->next + i) = (i + 1);

    *(thou->next + bufferSize) = 0;
    thou->prev = (int*) calloc (sizeof(*thou->prev), bufferSize + 1);
    for (int i = 1; i <= bufferSize; i++)
        *(thou->prev + i) = -1;

    thou->headOfFree = 1;
    thou->tailOfFree = bufferSize;
    thou->sz = 0;
    thou->list_logs = list_logs;
    //thou->graph_logs = graph_logs;
    thou->graph_logs_name = graph_logs_name;
    thou->errCode = 0;
    thou->normalized = true;

    thou->status = (int*) calloc (sizeof(*thou->next), bufferSize + 1);
    thou->status[0] = HOLY;

    return NO_ERRORS;
}

int ListInit (struct List* thou)
{
    assert (thou);
    int bufferSize = 10;

    thou->capacity = bufferSize;
    thou->values = (double *) calloc (sizeof(*thou->values), bufferSize + 1);
    thou->headOfValue = 0;
    thou->tailOfValue = 0;

    for (int i = 0; i <= bufferSize; i++)
        thou->values[i] = NAN;

    *(thou->next) = 0;
    thou->next = (int*) calloc (sizeof(*thou->next), bufferSize + 1);

    for (int i = 1; i < bufferSize; i++)
        *(thou->next + i) = (i + 1);

    *(thou->next + bufferSize) = 0;
    thou->prev = (int*) calloc (sizeof(*thou->prev), bufferSize + 1);
    *(thou->prev) = 0;
    for (int i = 1; i <= bufferSize; i++)
        *(thou->prev + i) = -1;

    thou->headOfFree = 1;
    thou->tailOfFree = bufferSize;
    thou->sz = 0;
    thou->list_logs = fopen ("list_logs.txt", "w");
    //thou->graph_logs = fopen ("graph_picture.dot", "w");
    thou->normalized = true;
    thou->graph_logs_name = "graph_picture.dot";
    thou->errCode = 0;

    thou->status = (int*) calloc (sizeof(*thou->next), bufferSize + 1);
    thou->status[0] = HOLY;

    return NO_ERRORS;
}

int InsertAfterIndex (struct List* thou, double value, int position)
{
    assert (thou);

    ASSERTED

    if (thou->next[position] != 0 && position !=0)
        return InsertInTheMiddleAfterIndex (thou, value, position);

    if (position == 0)
        return InsertInTheBegin (thou, value, position);

    if (position == thou->tailOfValue)
        return InsertInTheEnd (thou, value, position);

    ASSERTED

    return UNKNOWN_ERROR;
}

int InsertBeforeIndex (struct List* thou, double value, int position)
{
    assert (thou);
    ASSERTED
    return InsertAfterIndex (thou, value, thou->prev[position]);
}

int DeleteCurrentValue (struct List* thou, int position)
{
    assert (thou);
    ASSERTED
    if (position == thou->headOfValue)
        return  DeleteFromBegin (thou);
    if (position == thou->tailOfValue)
        return DeleteFromEnd (thou);
    ASSERTED

    return DeleteFromMiddle (thou, position);
}

int ListVerify (struct List* thou)
{
    if (!thou)
        return NO_LIST;

    if (thou->sz > thou->capacity)
        return BAD_SIZE;

    if (thou->headOfValue > thou->capacity)
        return BAD_HEAD_OF_VALUE;

    if (thou->tailOfValue > thou->capacity)
        return BAD_TAIL_OF_VALUE;

    if (thou->headOfFree > thou->capacity)
        return BAD_HEAD_OF_FREE;

    if (thou->tailOfFree > thou->capacity)
        return BAD_TAIL_OF_FREE;

    int tempsz = thou->sz;

    int tempPointer = thou->headOfValue;

    while (tempsz > 1)
    {
        if (thou->prev[thou->next[tempPointer]] != tempPointer)
        {
            thou->status[tempPointer] = ERROR;
            return LIST_INTERRUPT;
        }

        tempsz--;
        tempPointer = thou->next[tempPointer];
    }

    tempsz = thou->sz;
    tempPointer = thou->tailOfValue;
    while (tempsz > 1) {
        if (thou->next[thou->prev[tempPointer]] != tempPointer)
        {
            thou->status[tempPointer] = ERROR;
            return LIST_INTERRUPT;
        }
        tempsz--;
        tempPointer = thou->prev[tempPointer];
    }

    tempsz = thou->sz;
    tempPointer = thou->headOfValue;
    while (tempsz > 0)
    {
        if (tempPointer == 0)
            return LIST_INTERRUPT;
        tempsz--;
        tempPointer = thou->next[tempPointer];
    }
    if (tempPointer != 0)
        return LIST_INTERRUPT;

    tempsz = thou->sz;
    tempPointer = thou->tailOfValue;
    while (tempsz > 0)
    {
        if (tempPointer == 0)
            return LIST_INTERRUPT;
        tempsz--;
        tempPointer = thou->prev[tempPointer];
    }
    if (tempPointer != 0)
        return LIST_INTERRUPT;

    tempsz =thou->capacity - thou->sz;
    tempPointer = thou->headOfFree;
    while (tempsz > 0)
    {
        if (tempPointer == 0 || thou->prev[tempPointer] != -1)
        {
            printf ("aaa");
            thou->status[tempPointer] = ERROR;

            while (tempsz > 0)
            {
                thou->status[tempPointer] = ERROR;
                tempsz--;
                tempPointer = thou->next[tempPointer];
            }
            return FREE_MEMORY_INTERRUPTION;
        }
        tempsz--;
        tempPointer = thou->next[tempPointer];
    }

    if (tempPointer != 0)
    {
        printf ("aaa");
        thou->status[tempPointer] = ERROR;
        tempsz =thou->capacity - thou->sz;
        tempPointer = thou->headOfFree;
        while (tempsz > 0)
        {
            thou->status[tempPointer] = ERROR;
            tempsz--;
            tempPointer = thou->next[tempPointer];
        }
        return FREE_MEMORY_INTERRUPTION;
    }

    return NO_ERRORS;
}

int InsertInTheMiddleAfterIndex (struct List* thou, double value, int position)
{
    assert (thou);
    thou->normalized = false;

    ASSERTED

    int oldHeadOfFree = thou->headOfFree;
    thou->status[oldHeadOfFree] = BLOCKED;
    int newHeadOfFree = thou->next[thou->headOfFree];

    thou->values[thou->headOfFree] = value;
    thou->sz++;

    thou->next[thou->headOfFree] = thou->next[position];
    thou->prev[thou->headOfFree] = thou->prev[thou->next[position]];

    thou->prev[thou->next[position]] = thou->headOfFree;
    thou->next[position] = thou->headOfFree;

    thou->headOfFree = newHeadOfFree;

    if (thou->sz == thou->capacity - 1)
        if (EnlargeList (thou))
            return NO_FREE_MEMORY;

    ASSERTED

    return oldHeadOfFree;
}

int InsertInTheBegin (struct List* thou, double value, int position)
{
    assert (thou);
    thou->normalized = false;

    ASSERTED

    int oldHeadOfFree = thou->headOfFree;
    thou->status[oldHeadOfFree] = BLOCKED;
    int newHeadOfFree = thou->next[thou->headOfFree];
    thou->values[thou->headOfFree] = value;

    if (thou->sz == 0)
    {
        thou->sz++;
        thou->tailOfValue = thou->headOfFree;
        thou->headOfValue = thou->headOfFree;
        thou->next[thou->headOfFree] = 0;
        thou->prev[thou->headOfFree] = 0;
        thou->headOfFree = newHeadOfFree;

        if (thou->sz == thou->capacity - 1)
            if (EnlargeList (thou))
                return NO_FREE_MEMORY;

        ASSERTED

        return oldHeadOfFree;
    }
    else
    {
        thou->sz++;
        thou->next[thou->headOfFree] = thou->headOfValue;
        thou->prev[thou->headOfValue] = thou->headOfFree;
        thou->prev[thou->headOfFree] = 0;
        thou->headOfValue = thou->headOfFree;
        thou->headOfFree = newHeadOfFree;

        if (thou->sz == thou->capacity - 1)
            if (EnlargeList (thou))
                return NO_FREE_MEMORY;

        ASSERTED

        return  oldHeadOfFree;
    }

}

int InsertInTheEnd (struct List* thou, double value, int position)
{
    assert (thou);

    ASSERTED

    int oldHeadOfFree = thou->headOfFree;
    thou->status[oldHeadOfFree] = BLOCKED;
    int newHeadOfFree = thou->next[thou->headOfFree];
    thou->values[thou->headOfFree] = value;
    thou->sz++;
    thou->next[position] = thou->headOfFree;
    thou->next[thou->headOfFree] = 0;
    thou->prev[thou->headOfFree] = position;
    thou->tailOfValue = oldHeadOfFree;
    thou->headOfFree = newHeadOfFree;

    if (thou->sz == thou->capacity - 1)
        if (EnlargeList (thou))
            return NO_FREE_MEMORY;

    ASSERTED

    return oldHeadOfFree;
}

int DeleteFromEnd (struct List* thou)
{
    assert (thou);

    ASSERTED

    int position = thou->tailOfValue;
    thou->status[position] = FREE;
    thou->sz--;
    thou->next[thou->prev[position]] = 0;
    thou->tailOfValue = thou->prev[position];
    thou->prev[position] = -1;
    thou->next[position] = 0;
    thou->values[position] = NAN;
    thou->next[thou->tailOfFree] = position;
    thou->tailOfFree = position;

    ASSERTED

    return NO_ERRORS;
}

int DeleteFromBegin (struct List* thou)
{
    assert (thou);
    thou->normalized = false;

    ASSERTED

    int position = thou->headOfValue;
    thou->status[position] = FREE;
    thou->sz--;
    thou->prev[thou->next[position]] = 0;
    thou->headOfValue = thou->next[thou->headOfValue];
    thou->prev[position] = -1;
    thou->next[position] = 0;
    thou->values[position] = NAN;
    thou->next[thou->tailOfFree] = position;
    thou->tailOfFree = position;

    ASSERTED

    return NO_ERRORS;
}

int DeleteFromMiddle (struct List* thou, int position)
{
    assert (thou);
    thou->normalized = false;

    ASSERTED

    thou->status[position] = FREE;
    thou->sz--;
    thou->next[thou->prev[position]] = thou->next[position];
    thou->prev[thou->next[position]] = thou->prev[position];
    thou->prev[position] = -1;
    thou->next[position] = 0;
    thou->values[position] = POISON;
    thou->next[thou->tailOfFree] = position;
    thou->tailOfFree = position;

    ASSERTED

    return NO_ERRORS;
}

int TextDump (struct List* thou, char* reason, int line)
{
    if (!thou)
        printf ("there is no List. Executed on line %d", line);

    fprintf (thou->list_logs, "Executed on line %d\n", line);
    fprintf (thou->list_logs, "{\n");
    fprintf (thou->list_logs, "\t headOfValue = %d\n", thou->headOfValue);
    fprintf (thou->list_logs, "\t tailOfValue = %d\n", thou->tailOfValue);
    fprintf (thou->list_logs, "\t headOfFree  = %d\n", thou->headOfFree);
    fprintf (thou->list_logs, "\t tailOfFree  = %d\n", thou->tailOfFree);
    fprintf (thou->list_logs, "\t sz          = %d\n", thou->sz);
    fprintf (thou->list_logs, "\t capacity    = %d\n", thou->capacity);
    fprintf (thou->list_logs, "\t errcode     = %d\n", thou->errCode);
    for (int i = 0; i <= thou->capacity; i++)
    {
        fprintf (thou->list_logs, "\n\t{\n");
        fprintf (thou->list_logs, "\t\t physical number = %d\n", i);
        fprintf (thou->list_logs, "\t\t value           = %lg\n", thou->values[i]);
        fprintf (thou->list_logs, "\t\t next            = %d\n", thou->next[i]);
        fprintf (thou->list_logs, "\t\t prev            = %d\n", thou->prev[i]);
        fprintf (thou->list_logs, "\t\t status          = %d\n", thou->status[i]);
        fprintf (thou->list_logs, "\t}\n");
    }
    fprintf (thou->list_logs, "}\n");

    return NO_ERRORS;
}

int PictureDump (struct List* thou, char* reason, int line)
{
    if (!thou)
        printf ("there is no List. Executed on line %d", line);

    thou->graph_logs = fopen (thou->graph_logs_name, "w");

    fprintf (thou->graph_logs, "digraph LIST{\n");
    fprintf (thou->graph_logs, "\t" "rankdir = TB;\n");

    for (int i = 0; i <= thou->capacity; i++)
    {
        fprintf (thou->graph_logs, "%d [shape=record,color=",i);
        switch (thou->status[i])
        {
            case FREE: fprintf (thou->graph_logs, "\"green\""); break;
            case HOLY: fprintf (thou->graph_logs, "\"yellow\""); break;
            case BLOCKED: fprintf (thou->graph_logs, "\"blue\""); break;
            case ERROR: fprintf (thou->graph_logs, "\"red\""); break;
        }
        fprintf (thou->graph_logs,",label=\" { %lg |  <prev> %d | <current> %d | <next> %d } \"];\n",thou->values[i], thou->prev[i], i, thou->next[i]);
    }

    fprintf (thou->graph_logs, "\n");

    SetSameRank (thou, BLOCKED);
    SetSameRank (thou, HOLY);
    SetSameRank (thou, FREE);

    fprintf (thou->graph_logs, "\n");

    for (int i = 0; i <= thou->capacity; i++)
    {

        if (thou->prev[i] != -1)
            fprintf (thou->graph_logs,"\t" "%d:prev -> %d;\n", i, thou->prev[i]);

        if (thou->next[i] != -1)
            fprintf (thou->graph_logs,"\t" "%d:next -> %d;\n", i, thou->next[i]);
    }

    fprintf (thou->graph_logs,"}\n");

    fclose (thou->graph_logs);
    system ("dot -Tpng graph_picture.dot -o myGraph.png");
    system ("start myGraph.png");

    return NO_ERRORS;
}

void SetSameRank (struct List* thou, int status)
{
    fprintf (thou->graph_logs, "\n");

    fprintf (thou->graph_logs, "{rank = same;");
    for (int i = 0; i <= thou->capacity; i++)
        if (thou->status[i] == status)
            fprintf (thou->graph_logs, "%d;", i);

    fprintf (thou->graph_logs, "}");
}

int EnlargeList (struct List* thou)
{
    assert (thou);

    int* tempIntPointer = (int*) realloc (thou->next, sizeof (*tempIntPointer) * (2 *  thou->capacity + 1));
    if (!tempIntPointer)
        return NO_FREE_MEMORY;
    thou->next = tempIntPointer;

    tempIntPointer = (int*) realloc (thou->prev, sizeof (*tempIntPointer) * (2 *  thou->capacity + 1));
    if (!tempIntPointer)
        return NO_FREE_MEMORY;
    thou->prev = tempIntPointer;

    tempIntPointer = (int*) realloc (thou->status, sizeof (*tempIntPointer) * (2 *  thou->capacity + 1));
    if (!tempIntPointer)
        return NO_FREE_MEMORY;
    thou->status = tempIntPointer;

    double* tempDoublePointer = (double *) realloc (thou->values, sizeof (*tempDoublePointer) * (2 *  thou->capacity + 1));
    if (!tempDoublePointer)
        return NO_FREE_MEMORY;
    thou->values = tempDoublePointer;

    thou->next[thou->tailOfFree] = thou->capacity + 1;

    PrepareFreeMemory (thou);

    return NO_ERRORS;
}

void PrepareFreeMemory (struct List* thou)
{
    thou->next[thou->tailOfFree] = thou->capacity + 1;
    thou->tailOfFree = thou->capacity * 2;

    for (int i = thou->capacity + 1; i <= thou->capacity*2; ++i)
    {
        thou->values[i] = POISON;
        thou->prev[i] = -1;
        thou->status[i] = FREE;
        thou->next[i] = (i + 1) % (thou->capacity*2 + 1);
    }

    thou->capacity *= 2;
}

int NormalizeListOrder (struct List* thou)
{
    if (ListVerify (thou))
        return thou->errCode;

    double* normalOrder = (double*) calloc (sizeof(*normalOrder), thou->capacity + 1);
    if (!normalOrder)
        return NO_FREE_MEMORY;

    normalOrder[0] = POISON;

    int index = thou->headOfValue;

    for (int i = 1; i <= thou->sz; ++i)
    {
        normalOrder[i] = thou->values[index];
        index = thou->next[index];
    }

    for (int i = thou->sz + 1; i <= thou->capacity; ++i)
        normalOrder[i] = POISON;

    for (int i = 1; i <= thou->sz; ++i)
    {
        thou->next[i] = (i + 1) % (thou->sz + 1);
        thou->prev[i] = i - 1;
        thou->status[i] = BLOCKED;
    }

    for (int i = thou->sz + 1; i <= thou->capacity; ++i)
    {
        thou->next[i] = (i + 1) % (thou->capacity + 1);
        thou->prev[i] = -1;
        thou->status[i] = FREE;
    }
    free (thou->values);
    thou->values = normalOrder;
    thou->normalized = true;
    thou->headOfValue = 1;
    thou->tailOfValue = thou->sz;
    thou->headOfFree = thou->sz + 1;
    thou->tailOfFree = thou->capacity;

    return NO_ERRORS;
}

int FindValue (struct List* thou, double value)
{
    int index = thou->headOfValue;
    while (index != 0)
    {
        if (fabs(thou->values[index] - value) < 1e-10)
            return index;

        index = thou->next[index];
    }

    return 0;
}

#define ARGNAME(x) #x

char* ErrorName (int error)
{
    switch (error)
    {
        case NO_ERRORS: return ARGNAME (NO_ERRORS);
        case DOUBLE_INSERT: return ARGNAME (DOUBLE_INSERT);
        case DELETE_NOTHING: return  ARGNAME (DELETE_NOTHING);
        case LIST_INTERRUPT: return  ARGNAME (LIST_INTERRUPT);
        case FREE_MEMORY_INTERRUPTION: return  ARGNAME (FREE_MEMORY_INTERRUPTION);
        case UNKNOWN_ERROR: return ARGNAME (UNKNOWN_ERROR);
        case NO_LIST: return ARGNAME (NO_LIST);
        case NO_FREE_MEMORY: return ARGNAME (NO_FREE_MEMORY);
        default: return ARGNAME (UNKNOWN_ERROR);
    }
}

#undef ASSERTED
#undef ARGNAME
