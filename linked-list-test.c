#include "memory-manager.h"
#include "linked-list.h"
#include <assert.h>


int main() {

    linkedlist *LL = (linkedlist*) malloc(sizeof(linkedlist));
    LL->head = NULL;
    LL->tail = NULL;
    LL->count = 0;


    assert(LL->count == 0);
    assert(LL->head == NULL);
    assert(LL->tail == NULL);


    addToEnd(LL, 10, 0);
    assert(LL->head->data == 10);
    assert(LL->head->address == 0);
    assert(LL->tail->data == 10);
    assert(LL->tail->address == 0);


    addToEnd(LL, 10, 10);
    assert(LL->head->data == 10);
    assert(LL->head->address == 0);
    assert(LL->tail->data == 10);
    assert(LL->tail->address == 10);
    assert(LL->head->next == LL->tail);
    assert(LL->tail->prev == LL->head);


    addToEnd(LL, 10, 60);
    addToEnd(LL, 10, 30);
    addToEnd(LL, 10, 40);
    addToEnd(LL, 10, 20);
    addToEnd(LL, 10, 50);
    addToEnd(LL, 10, 90);
    addToEnd(LL, 10, 70);
    addToEnd(LL, 10, 100);
    addToEnd(LL, 10, 80);


    selectionSortBlocks(LL);
    assert(LL->head->address == 0);
    assert(LL->head->next->address == 10);
    assert(LL->head->next->next->address == 20);
    assert(LL->head->next->next->next->address == 30);
    assert(LL->head->next->next->next->next->address == 40);
    assert(LL->head->next->next->next->next->next->address == 50);
    assert(LL->head->next->next->next->next->next->next->address == 60);
    assert(LL->head->next->next->next->next->next->next->next->address == 70);
    assert(LL->head->next->next->next->next->next->next->next->next->address == 80);
    assert(LL->head->next->next->next->next->next->next->next->next->next->address == 90);
    assert(LL->head->next->next->next->next->next->next->next->next->next->next->address == 100);
    assert(LL->tail->address == 100);
    assert(LL->tail->prev->address == 90);
    assert(LL->tail->prev->prev->address == 80);
    assert(LL->tail->prev->prev->prev->address == 70);
    assert(LL->tail->prev->prev->prev->prev->address == 60);
    assert(LL->tail->prev->prev->prev->prev->prev->address == 50);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->address == 40);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->address == 30);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->address == 20);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->prev->address == 10);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->prev->prev->address == 0);


    unlinkNode(LL, 70);
    assert(LL->head->address == 0);
    assert(LL->head->next->address == 10);
    assert(LL->head->next->next->address == 20);
    assert(LL->head->next->next->next->address == 30);
    assert(LL->head->next->next->next->next->address == 40);
    assert(LL->head->next->next->next->next->next->address == 50);
    assert(LL->head->next->next->next->next->next->next->address == 60);
    assert(LL->head->next->next->next->next->next->next->next->address == 80);
    assert(LL->head->next->next->next->next->next->next->next->next->address == 90);
    assert(LL->head->next->next->next->next->next->next->next->next->next->address == 100);
    assert(LL->tail->address == 100);
    assert(LL->tail->prev->address == 90);
    assert(LL->tail->prev->prev->address == 80);
    assert(LL->tail->prev->prev->prev->address == 60);
    assert(LL->tail->prev->prev->prev->prev->address == 50);
    assert(LL->tail->prev->prev->prev->prev->prev->address == 40);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->address == 30);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->address == 20);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->address == 10);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->prev->address == 0);



    addToEnd(LL, 10, 70);
    selectionSortBlocks(LL);
    assert(LL->head->address == 0);
    assert(LL->head->next->address == 10);
    assert(LL->head->next->next->address == 20);
    assert(LL->head->next->next->next->address == 30);
    assert(LL->head->next->next->next->next->address == 40);
    assert(LL->head->next->next->next->next->next->address == 50);
    assert(LL->head->next->next->next->next->next->next->address == 60);
    assert(LL->head->next->next->next->next->next->next->next->address == 70);
    assert(LL->head->next->next->next->next->next->next->next->next->address == 80);
    assert(LL->head->next->next->next->next->next->next->next->next->next->address == 90);
    assert(LL->head->next->next->next->next->next->next->next->next->next->next->address == 100);
    assert(LL->tail->address == 100);
    assert(LL->tail->prev->address == 90);
    assert(LL->tail->prev->prev->address == 80);
    assert(LL->tail->prev->prev->prev->address == 70);
    assert(LL->tail->prev->prev->prev->prev->address == 60);
    assert(LL->tail->prev->prev->prev->prev->prev->address == 50);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->address == 40);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->address == 30);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->address == 20);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->prev->address == 10);
    assert(LL->tail->prev->prev->prev->prev->prev->prev->prev->prev->prev->prev->address == 0);



    free(LL);



    LL = (linkedlist*) malloc(sizeof(linkedlist));
    LL->head = NULL;
    LL->tail = NULL;
    LL->count = 0;


    assert(LL->count == 0);
    assert(LL->head == NULL);
    assert(LL->tail == NULL);


    addToStart(LL, 10);
    assert(LL->head->data == 10);
    assert(LL->head->address == 0);
    assert(LL->tail->data == 10);
    assert(LL->tail->address == 0);


    addToEnd(LL, 10, 10);
    assert(LL->head->data == 10);
    assert(LL->head->address == 0);
    assert(LL->tail->data == 10);
    assert(LL->tail->address == 10);
    assert(LL->head->next == LL->tail);
    assert(LL->tail->prev == LL->head);



    printf("\nUnit test complete!\n");
}