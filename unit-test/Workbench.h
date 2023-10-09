#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct w
{
    int index;
    int type;
    double coordinate[2];
    int lastProduceTime;
    int sourceSpaceStatus;
    int productSpaceStatus;
} Workbench;

typedef struct node
{
    Workbench data;
    struct node *next;
} node, *SqLinkedList;

SqLinkedList LinkedList();
Workbench Workbench_init(int type, int index, double x, double y, int lpt, int sss, int pss);

void SqLinkedList_init(SqLinkedList *l);
void SqLinkedList_destory(SqLinkedList *l);
int isEmptyList(SqLinkedList l);
int SqLinkedList_add(SqLinkedList *l, Workbench e);
int SqLinkedList_add_toHead(SqLinkedList *l, Workbench e);
int SqLinkedList_length(SqLinkedList l);
int SqLinkedList_insert(SqLinkedList *l, Workbench e, int index);
node *SqLinkedList_get(SqLinkedList l, int index);
int SqLinkedList_getByContent(SqLinkedList l, Workbench e);
int SqLinkedList_delete(SqLinkedList *l, Workbench w);
int SqLinkedList_contains(SqLinkedList l, Workbench w);
int SqLinkedList_update(Workbench l[], int index, int type, double x, double y, int lastProduceTime, int sourceSpaceStatus, int productSpaceStatus);
int SqLinkedList_update2(SqLinkedList *l, double x, double y, int lastProduceTime, int sourceSpaceStatus, int productSpaceStatus);

#endif
