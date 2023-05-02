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

node  *findBestBlock(linkedlist *LL, int size) {
    node *best_fit = NULL;
    int min = __INT_MAX__;
    node *curr = LL->head;
    node *prev = NULL;

    while (curr != NULL) {
        if (curr->data >= size) {
            best_fit = curr;
            min = curr->data - size;
        }
        prev = curr;
        curr = curr->next;
    } 
    return best_fit;
}

int allocateMemory(memory_manager *mm, int size, int mode) {
    linkedlist *FL = mm->free_list;
    node *prev = NULL;
    node *curr = FL->head;

    //for all of the nodes "data" is technically the size of the block
    //traverse through the linkedlist of free blocks until it finds one that is big enough to fit "size"
    if (mode == 0) {
        while (curr != NULL && curr->data < size) {
            prev = curr;
            curr = curr->next;
        }
    }
    else {
        node *curr = findBestBlock(FL, size);
    }

    if (curr == NULL) {
        // Could not find a suitable block in free list
        return -1;
    }

    // Found a suitable block in free list
    if (curr->data >= size) {
        // Split the node if it has more space than needed
        // new node is the second half

        insertAfter(FL, curr, curr->data - size, curr->address + size);
        /*
        node *new_node = malloc(sizeof(node));
        new_node->data = curr->data - size;
        new_node->address = curr->address + size;
        new_node->next = curr->next;
        if (new_node->next != NULL) {
            new_node->next->prev = new_node;
        }
        new_node->prev = curr;
        curr->next = new_node;
        */
        curr->data = size;
    }
    // Remove the allocated block from the free list
    int address = curr->address;
    unlinkNode(FL, curr->address);


    // Add the allocated block to the busy list
    linkedlist *BL = mm->busy_list;
    addToEnd(BL, size, address);

    // Return the address
    return address;
}

void freeMemory(memory_manager *mm, int address) {
    linkedlist *BL = mm->busy_list;
    linkedlist *FL = mm->free_list;
    node *freed_node = unlinkNode(BL, address);
    printList(FL);
    addToEnd(FL, freed_node->data, freed_node->address);
    printList(FL);
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
        //printf("%d, %d", curr_end, next_start);
        if (curr_end == next_start) {
            // Merge the two blocks into a single block
            //printf("Found match");
            curr->data += next->data;
            unlinkNode(LL, next->address);
        }
        else {
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