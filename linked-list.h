#ifndef ll_header
#define ll_header
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node 
{
    int data;
    struct node *next;
    struct node *prev;
} node;

typedef struct linkedlist
{
    node *head;
    node *tail;
    int count;
} linkedlist;

void addToEnd(linkedlist*, int);
void addToStart(linkedlist*, int);
void printList(linkedlist*);
node *removeLast(linkedlist*);
node *removeFirst(linkedlist*);
node *findNode(linkedlist*, int);
void insertBefore(linkedlist*, node**, int);
void insertAfter(linkedlist*, node**, int);
void removeNode(linkedlist*, node**);
node *unlinkNode(linkedlist*, int);
int count(linkedlist*);
void destroyList(linkedlist*);
#endif