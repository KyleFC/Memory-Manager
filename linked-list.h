#ifndef ll_header
#define ll_header
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node 
{
    int data;
    int address;
    struct node *next;
    struct node *prev;
} node;

typedef struct linkedlist
{
    node *head;
    node *tail;
    int count;
} linkedlist;

void addToEnd(linkedlist*, int, int);
void addToStart(linkedlist*, int);
void printList(linkedlist*);
node *removeLast(linkedlist*);
node *removeFirst(linkedlist*);
node *findNode(linkedlist*, int);
node *findBlock(linkedlist*, int);
void insertBefore(linkedlist*, node**, int);
void insertAfter(linkedlist*, node**, int);
void removeNode(linkedlist*, node**);
node *unlinkNode(linkedlist*, int);
int count(linkedlist*);
void destroyList(linkedlist*);
void selectionSort(linkedlist*);
void selectionSortBlocks(linkedlist*);
#endif