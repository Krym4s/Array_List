#include <iostream>
#include "cmake-build-debug/IsE_Array_List.h"

int main() {
    struct List list = {};
    char* graph_logs_name = "graph_picture.dot";
    FILE* logs = fopen ("logs.txt", "w");
    FILE* graph_logs = fopen ("graph_picture.dot", "w");
    ListConstruct (2, &list, logs, graph_logs, graph_logs_name);
    int oldpos = 3;
    int pos = InsertAfterIndex (&list, 12, 0);
    pos = InsertAfterIndex (&list, 13, pos);
    InsertAfterIndex (&list, 14, pos-1);
    InsertAfterIndex (&list, 124, 0);

    DeleteCurrentValue (&list, oldpos);

    InsertAfterIndex (&list, 12345, 0);

    //list.data[4].next = 7;
    //list.next[7] = 5;

    ListVerify (&list);

    //NormalizeListOrder (&list);

    TextDump (&list, "no_reason", __LINE__);
    PictureDump (&list, "no_reason", __LINE__);

    ListDestruct (&list);
    //system ("dot -Tpng graph_picture.dot -o myGraph.png");
    //system ("start myGraph.png");

    return 0;
}
