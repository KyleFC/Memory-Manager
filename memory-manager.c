//This file will contain the implementation of the functions declared in memory-manager.h
#include "memory-manager.h"

memory_manager *createMemoryManager(int size) {
    // Allocate memory for the memory manager
    memory_manager *mem_manager = malloc(sizeof(memory_manager));

    // Initialize the memory map
    linkedlist *LL = (linkedlist*) malloc(sizeof(linkedlist));
    LL->head = NULL;
    LL->tail = NULL;
    LL->count = 0;
    linkedlist *FL = (linkedlist*) malloc(sizeof(linkedlist));
    FL->head = NULL;
    FL->tail = NULL;
    FL->count = 0;
    linkedlist *BL = (linkedlist*) malloc(sizeof(linkedlist));
    BL->head = NULL;
    BL->tail = NULL;
    BL->count = 0;

    addToStart(LL, size);
    addToStart(FL, size);
    
    mem_manager->memory_map = LL;
    mem_manager->free_list = FL;
    mem_manager->busy_list = BL;

    return mem_manager;
}

int allocateMemory(memory_manager *mm, int size) {
    linkedlist *FL = mm->free_list;
    node *prev = NULL;
    node *curr = FL->head;

    //for all of the nodes "data" is technically the size of the block
    //traverse through the linkedlist of free blocks until it finds one that is big enough to fit "size"
    while (curr != NULL && curr->data < size) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        // Could not find a suitable block in free list
        return -1;
    }

    // Found a suitable block in free list
    if (curr->data > size) {
        // Split the node if it has more space than needed
        // new node is the second half
        node *new_node = malloc(sizeof(node));
        new_node->data = curr->data - size;
        new_node->address = curr->address + size;
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
    int address = curr->address;
    linkedlist *BL = mm->busy_list;
    addToEnd(BL, size, address);

    // Return the address
    return address;
}

void freeMemory(memory_manager *mm, int address) {
    linkedlist *BL = mm->busy_list;
    linkedlist *FL = mm->free_list;
    node *freed_node = unlinkNode(BL, address);

    addToEnd(FL, freed_node->data, freed_node->address);
    selectionSortBlocks(FL);
    coalesceList(FL);
}

void coalesceList(linkedlist *LL) {
    node *curr = LL->head;
    while (curr && curr->next) {
        // Find end of current block
        int curr_end = curr->address + curr->data;
        node *next = curr->next;
        // Find start of next block
        int next_start = next->address;

        if (curr_end == next_start) {
            // Merge the two blocks into a single block
            curr->data += next->data;
            unlinkNode(LL, next->address);
            free(next);
        } else {
            curr = curr->next;
        }
    }
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
    mm->memory_map = NULL;
    mm->free_list = NULL;
    mm->busy_list = NULL;
}