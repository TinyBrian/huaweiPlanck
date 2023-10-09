
#include "Workbench.h"

Workbench *workbenchList;
SqLinkedList aquireGood1;
SqLinkedList aquireGood2;
SqLinkedList aquireGood3;
SqLinkedList aquireGood4;
SqLinkedList aquireGood5;
SqLinkedList aquireGood6;
SqLinkedList aquireGood7;
SqLinkedList sellingGood1;
SqLinkedList sellingGood2;
SqLinkedList sellingGood3;
SqLinkedList sellingGood4;
SqLinkedList sellingGood5;
SqLinkedList sellingGood6;
SqLinkedList sellingGood7;

SqLinkedList *aquireList[7] = {&aquireGood1, &aquireGood2, &aquireGood3,
                               &aquireGood4, &aquireGood5, &aquireGood6,
                               &aquireGood7};

SqLinkedList *sellingList[7] = {&sellingGood1, &sellingGood2, &sellingGood3,
                                &sellingGood4, &sellingGood5, &sellingGood6,
                                &sellingGood7};

void inputProcess(char *l, double *t)
{
    int i = 1;
    char *p = strtok(l, " ");
    if (p)
    {
        t[0] = atof(p);
    }
    while (p = strtok(NULL, " "))
    {
        t[i++] = atof(p);
    }
}

int main()
{

    workbenchList = (Workbench *)malloc(sizeof(Workbench) * 50);
    char line[1024];
    double temp[10];
    int a = 0;
    for (int i = 0; i < 9; i++)
    {
        SqLinkedList_init(aquireList[i]);
    }
    for (int i = 0; i < 7; i++)
    {
        SqLinkedList_init(sellingList[i]);
    }
    FILE *fp = fopen("C:\\Users\\Cosbly\\Desktop\\C\\unit-test\\input.txt", "r");
    while (fgets(line, sizeof line, fp))
    {
        a++;
        inputProcess(line, temp);
        SqLinkedList_update(workbenchList, a, temp[0], temp[1], temp[2], (int)temp[3], (int)temp[4], (int)temp[5]);
    }
    Workbench w;
    w.index = 4;
    SqLinkedList_delete(&aquireGood1, w);

    fclose(fp);
}
