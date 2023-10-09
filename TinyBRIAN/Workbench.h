#ifndef WORKBENCH_H
#define WORKBENCH_H

#include "main.h"

typedef struct w
{
    int index;
    int type;
    double coordinate[2];
    int lastProduceTime;
    int sourceSpaceStatus;
    int productSpaceStatus;
    int locked;  // used to alcomplish occupy ,1-> has been taken;0->not
    double lure; // used to attract robot to accomplish the vacant
} Workbench;

typedef struct node
{
    Workbench data;
    struct node *next;
} node, *SqLinkedList;

SqLinkedList LinkedList();
Workbench Workbench_init(int type, int index, double x, double y, int lpt, int sss, int pss);
int Workbench_delete(SqLinkedList *l, Workbench w);

void SqLinkedList_init(SqLinkedList *l);
void SqLinkedList_destory(SqLinkedList *l);
int isEmptyList(SqLinkedList l);
int SqLinkedList_add(SqLinkedList *l, Workbench e);
int SqLinkedList_add_toHead(SqLinkedList *l, Workbench e);
int SqLinkedList_length(SqLinkedList l);
int SqLinkedList_insert(SqLinkedList *l, Workbench e, int index);
node *SqLinkedList_get(SqLinkedList l, int index);
int SqLinkedList_getIndexByContent(SqLinkedList l, Workbench e);
int SqLinkedList_delete(SqLinkedList *l, int index);
int SqLinkedList_contains(SqLinkedList l, Workbench w);
int SqLinkedList_update2(SqLinkedList *l, double x, double y, int lastProduceTime, int sourceSpaceStatus, int productSpaceStatus);

#endif
