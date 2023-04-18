//This file will contain the implementation of the functions declared in memory-manager.h
#include "memory-manager.h"

memory_manager *createMemoryManager(int size) {
    // Allocate memory for the memory manager
    memory_manager *mem_manager = malloc(sizeof(memory_manager));

    // Initialize the memory map
    linkedlist *LL = (linkedlist*) malloc(sizeof(linkedlist));
    LL->head = NULL;
    LL->count = 0;
    linkedlist *FL = (linkedlist*) malloc(sizeof(linkedlist));
    FL->head = NULL;
    FL->count = 0;
    linkedlist *BL = (linkedlist*) malloc(sizeof(linkedlist));
    BL->head = NULL;
    BL->count = 0;

    addToStart(LL, size);
    addToStart(FL, size);
    
    mem_manager->memory_map = LL;
    mem_manager->free_list = FL;
    mem_manager->busy_list = BL;

    return mem_manager;
}

memory_manager *allocateMemory(memory_manager *mm, int size) {
    linkedlist *FL = mm->free_list;
    node *prev = NULL;
    node *curr = FL->head;

    while (curr != NULL && curr->data < size) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        // Could not find a suitable block in free list
        return NULL;
    }

    // Found a suitable block in free list
    if (curr->data > size) {
        // Split the node if it has more space than needed
        node *new_node = malloc(sizeof(node));
        new_node->data = curr->data - size;
        new_node->next = NULL;
        new_node->prev = curr;
        curr->data = size;
        curr->next = new_node;
    }

    // Remove the allocated block from the free list
    if (prev == NULL) {
        FL->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    // Add the allocated block to the busy list
    linkedlist *BL = mm->busy_list;
    addToStart(BL, curr->data);

    // Return a pointer to the allocated block
    return (memory_manager *) curr;
}

void dumpMemoryLists(memory_manager *mm) {
    linkedlist *bl = mm->busy_list;
    linkedlist *fl = mm->free_list;
    printf("Busy List ");
    printList(bl);
    printf("Free List ");
    printList(fl);
}

void destroyMemoryManager(memory_manager *mm) {
    linkedlist *map  = mm->memory_map;
    linkedlist *fl  = mm->free_list;
    linkedlist *bl  = mm->busy_list;
    destroyList(map);
    destroyList(fl);
    destroyList(bl);
}