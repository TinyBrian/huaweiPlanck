
#include "Workbench.h"
// #include "main.c"

extern SqLinkedList *aquireList[7];
extern SqLinkedList *sellingList[7];

void SqLinkedList_init(SqLinkedList *l)
{
    *l = (SqLinkedList)malloc(sizeof(node));
    (*l)->next = NULL;
    (*l)->data.coordinate[0] = 0;
    (*l)->data.coordinate[1] = 0;
    (*l)->data.lastProduceTime = 0;
    (*l)->data.productSpaceStatus = 0;
    (*l)->data.sourceSpaceStatus = 0;
}

Workbench Workbench_init(int type, int index, double x, double y, int lpt, int sss, int pss)
{
    Workbench w;
    w.type = type;
    w.index = index;
    w.coordinate[0] = x;
    w.coordinate[1] = y;
    w.lastProduceTime = lpt;
    w.productSpaceStatus = sss;
    w.sourceSpaceStatus = pss;
    w.locked = 0;
    w.lure = 0;
    return w;
}

void SqLinkedList_destory(SqLinkedList *l)
{
    node *p;
    while (*l != NULL)
    {
        p = *l;
        *l = (*l)->next;
        free(p);
    }
}

int isEmptyList(SqLinkedList l)
{
    if (l == NULL)
        return -1;
    return l->next == NULL;
}

/**
 * @brief add to the end of list
 *
 * @param l
 * @param e
 * @return int
 */
int SqLinkedList_add(SqLinkedList *l, Workbench e)
{
    if (!(*l)) // 未初始化空表
        return 0;
    node *p, *n;
    n = (node *)malloc(sizeof(node));
    n->data = e;
    n->next = NULL;
    p = (*l);
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = n;
    return 1;
}

int SqLinkedList_add_toHead(SqLinkedList *l, Workbench e)
{
    if (!l) // 未初始化空表
        return 0;
    node *p = *l, *temp;
    temp = (node *)malloc(sizeof(node));
    temp->data = e;
    temp->next = p->next;
    p->next = temp;
    return 1;
}

int clear(SqLinkedList *l)
{
    node *p, *t;
    p = (*l)->next;
    while (p != NULL)
    {
        t = p->next;
        free(p);
        p = t;
    }
    (*l)->next = NULL;
    return 1;
}

int SqLinkedList_length(SqLinkedList l)
{
    int i = 0;
    node *p = l;
    p = p->next;
    while (p != NULL)
    {
        i++;
        p = p->next;
    }
    return i;
}

int SqLinkedList_insert(SqLinkedList *l, Workbench e, int index)
{
    index--;
    int i = 0;
    node *p = (*l);
    node *n;
    int length = SqLinkedList_length(*l);
    if (l == NULL) // l空表判断这里可以直接放到下边的while(l && i<index)
        return -2;
    if (index > length || index < 0)
    {
        printf("insert fail:index range from : 1~ length+1 \n");
        return -1;
    }

    n = (node *)malloc(sizeof(node));
    n->data = e;
    while (i < index)
    {
        i++;
        p = p->next;
    }
    n->next = p->next;
    p->next = n;

    return 1;
}

node *SqLinkedList_get(SqLinkedList l, int index)
{
    node *p = l;
    int i = 0, len = SqLinkedList_length(l);
    if (index < 1 || index > len)
    {
        printf("Get Error: index range from 1 to length");
        return NULL;
    }

    while (i < index)
    {
        i++;
        p = p->next;
    }
    return p;
}

int SqLinkedList_getIndexByContent(SqLinkedList l, Workbench e)
{
    int i = 0;
    Workbench t;
    node *p = l->next;
    while (p != NULL)
    {
        if (p->data.index == e.index)
        {
            break;
        }
        i++;
        p = p->next;
    }
    return i;
}

int SqLinkedList_contains(SqLinkedList l, Workbench w)
{
    if (l == NULL)
    {
        fprintf(stderr, "NULL SQList");
        return -1;
    }
    node *p = (l);
    while (p && p->data.index != w.index)
    {
        p = p->next;
    }
    if (p == NULL)
        return 0;
    else
        return 1;
}

int Workbench_delete(SqLinkedList *l, Workbench w)
{
    int index = SqLinkedList_getIndexByContent(*l, w);
    SqLinkedList_delete(l, index + 1);
    return 1;
}

int SqLinkedList_delete(SqLinkedList *l, int index)
{
    int i = 1;
    node *p = (*l);
    node *t = NULL;
    while (p && i <= index - 1)
    {
        i++;
        p = p->next;
    }
    t = p->next;
    p->next = t->next;
    free(t);
    return 1;
}

int SqLinkedList_update2(SqLinkedList *l, double x, double y, int lastProduceTime, int sourceSpaceStatus, int productSpaceStatus)
{
    // fprintf(stderr, "update:x:%f   y:%f \n", x, y);
    node *p = (*l)->next;
    for (p; p != NULL; p = p->next)
    {
        if (p->data.coordinate[0] == x)
        {
            if (p->data.coordinate[1] == y)
            {
                p->data.lastProduceTime = lastProduceTime;
                p->data.sourceSpaceStatus = sourceSpaceStatus;
                p->data.productSpaceStatus = productSpaceStatus;
            }
        }
    }
}

int SqLinkedList_concat(SqLinkedList *l, SqLinkedList l2)
{
    node *p = (*l)->next;
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = l2->next;
}
