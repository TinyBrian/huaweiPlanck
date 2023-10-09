
#include "Workbench.h"

extern SqLinkedList *aquireList[7];
extern SqLinkedList *sellingList[7];
extern SqLinkedList aquireGood1;
extern SqLinkedList aquireGood2;
extern SqLinkedList aquireGood3;
extern SqLinkedList aquireGood4;
extern SqLinkedList aquireGood5;
extern SqLinkedList aquireGood6;
extern SqLinkedList aquireGood7;
extern SqLinkedList sellingGood1;
extern SqLinkedList sellingGood2;
extern SqLinkedList sellingGood3;
extern SqLinkedList sellingGood4;
extern SqLinkedList sellingGood5;
extern SqLinkedList sellingGood6;
extern SqLinkedList sellingGood7;

void SqLinkedList_init(SqLinkedList *l)
{
    *l = (SqLinkedList)malloc(sizeof(node));
    (*l)->next = NULL;
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
    return w;

    // Workbench b;
    //         b.coordinate[0] = 0.5 * i + 0.25;
    //         b.coordinate[1] = 0.5 * j + 0.25;
    //         b.lastProduceTime = 0;
    //         b.productSpaceStatus = 0;
    //         b.sourceSpaceStatus = 0;
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
    if (!l) // 未初始化空表
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

int SqLinkedList_getByContent(SqLinkedList l, Workbench e)
{
    // Workbench t;
    // node *p = l;
    // while (t !=)
    // {
    //     i++;
    //     p = p->next;
    // }
    // return p->data;
}

int SqLinkedList_contains(SqLinkedList l, Workbench w)
{
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

int SqLinkedList_delete(SqLinkedList *l, Workbench w)
{
    node *p = (*l), *t;
    if (p && p->next)
    {
        while (p->next->data.index != w.index)
        {
            p = p->next;
        }
        t = p->next;
        p->next = t->next;
        free(t);
        return 1;
    }
    return 0;
}
int SqLinkedList_update(Workbench l[], int index, int type,
                        double x, double y, int lastProduceTime,
                        int sourceSpaceStatus, int productSpaceStatus)
{
    Workbench w;
    w.index = index;
    w.type = type;
    w.coordinate[0] = x;
    w.coordinate[1] = y;
    w.lastProduceTime = lastProduceTime;
    w.productSpaceStatus = productSpaceStatus;
    w.sourceSpaceStatus = sourceSpaceStatus;
    l[index] = w;

    if (type == 4)
    {
        if (((sourceSpaceStatus >> 1) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood1, w))
                SqLinkedList_add(&aquireGood1, w);
        }
        else
            SqLinkedList_delete(&aquireGood1, w);

        if (((sourceSpaceStatus >> 2) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood2, w))
                SqLinkedList_add(&aquireGood2, w);
        }
        else
            SqLinkedList_delete(&aquireGood2, w);
    }
    if (type == 5)
    {
        if ((((sourceSpaceStatus >> 1) & 1)) == 0)
        {
            if (!SqLinkedList_contains(aquireGood1, w))
                SqLinkedList_add(&aquireGood1, w);
        }
        else
            SqLinkedList_delete(&aquireGood1, w);

        if (((sourceSpaceStatus >> 3) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood3, w))
                SqLinkedList_add(&aquireGood3, w);
        }
        else
            SqLinkedList_delete(&aquireGood3, w);
    }
    if (type == 6)
    {
        if (((sourceSpaceStatus >> 2) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood2, w))
                SqLinkedList_add(&aquireGood2, w);
        }
        else
            SqLinkedList_delete(&aquireGood2, w);

        if (((sourceSpaceStatus >> 3) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood3, w))
                SqLinkedList_add(&aquireGood3, w);
        }
        else
            SqLinkedList_delete(&aquireGood3, w);
    }
    if (type == 7)
    {
        if (((sourceSpaceStatus >> 4) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood4, w))
                SqLinkedList_add(&aquireGood4, w);
        }
        else
            SqLinkedList_delete(&aquireGood4, w);

        if (((sourceSpaceStatus >> 5) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood5, w))
                SqLinkedList_add(&aquireGood5, w);
        }
        else
            SqLinkedList_delete(&aquireGood5, w);

        if (((sourceSpaceStatus >> 6) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood6, w))
                SqLinkedList_add(&aquireGood6, w);
        }
        else
            SqLinkedList_delete(&aquireGood6, w);
    }
    if (type == 8)
    {
        if (((sourceSpaceStatus >> 7) & 1) == 0)
        {
            if (!SqLinkedList_contains(aquireGood7, w))
                SqLinkedList_add(&aquireGood7, w);
        }
        else
            SqLinkedList_delete(&aquireGood7, w);
    }
    if (type == 9)
    {
        for (int i = 0; i < 7; i++)
        {
            if (((sourceSpaceStatus >> (i + 1)) & 1) == 0)
            {
                if (!SqLinkedList_contains(*aquireList[i], w))
                    SqLinkedList_add(aquireList[i], w);
            }
            else
                SqLinkedList_delete(aquireList[i], w);
        }
    }
    if (type != 8 && type != 9)
    {
        if (productSpaceStatus == 1)
        {
            if (!SqLinkedList_contains(*sellingList[type - 1], w))
                SqLinkedList_add(sellingList[type - 1], w);
        }
        else if (productSpaceStatus == 0)
            SqLinkedList_delete(sellingList[type - 1], w);
    }
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
