//
// Created by egolg on 02.11.2020.
//
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <cmath>

#ifndef LIST_ARRAY_ISE_ARRAY_LIST_H
#define LIST_ARRAY_ISE_ARRAY_LIST_H

struct List
{
    double* values;
    int* next;
    int* prev;
    int* status;

    int headOfValue;
    int tailOfValue;
    int headOfFree;
    int tailOfFree;

    unsigned int sz;
    unsigned int capacity;

    bool normalized;

    FILE* list_logs;
    FILE* graph_logs;

    char* graph_logs_name;

    int errCode;
};

enum List_errors
{
    NO_ERRORS = 0,
    DOUBLE_INSERT = -1,
    DELETE_NOTHING = -2,
    LIST_INTERRUPT = -3,
    FREE_MEMORY_INTERRUPTION = -4,
    UNKNOWN_ERROR = -5,
    NO_LIST = -6,
    NO_FREE_MEMORY = -7
};

enum memberStatus
{
    FREE = 0,
    BLOCKED = 1,
    ERROR = 2,
    HOLY = 3
};

const double EPS = 1e-10;

const double POISON = NAN;

int ListConstruct (unsigned int bufferSize, struct List* thou, FILE* list_logs, FILE* graph_logs, char* graph_logs_name);

int ListInit (struct List* thou);

int InsertAfterIndex (struct List* thou, double value, int position);

int InsertBeforeIndex (struct List* thou, double value, int position);

int InsertInTheMiddleAfterIndex (struct List* thou, double value, int position);

int InsertInTheBegin (struct List* thou, double value, int position);

int InsertInTheEnd (struct List* thou, double value, int position);

int DeleteCurrentValue (struct List* thou, int position);

int DeleteFromEnd (struct List* thou);

int DeleteFromBegin (struct List* thou);

int DeleteFromMiddle (struct List* thou, int position);

void TextDump (struct List* thou, char* reason, int line);

void PictureDump (struct List* thou, char* reason, int line);

int ListVerificate (struct List* thou);

void SetSameRank (struct List* thou, int status);

int EnlargeList (struct List* thou);

void PrepareFreeMemory (struct List* thou);

int NormalizeListOrder (struct List* thou);

int FindValue (struct List* thou, double value);

//int textDump (struct List* )
#endif //LIST_ARRAY_ISE_ARRAY_LIST_H
