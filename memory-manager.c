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
    addToEnd(BL, size);

    // Return a pointer to the allocated block
    return address;
}

void freeMemory(memory_manager *mm, int address) {
    // currently implements its own findNode function. 
    // Could be worth creating a findNode function specifically for addresses later.
    linkedlist *FL = mm->free_list;
    linkedlist *BL = mm->busy_list;

    // Find the node in the busy list with the given address
    node *prev = NULL;
    node *curr = BL->head;
    while (curr != NULL && curr->address != address) {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL) {
        // Could not find the block in the busy list
        return;
    }

    // Remove the node from the busy list
    if (prev == NULL) {
        BL->head = curr->next;
    } else {
        prev->next = curr->next;
    }
    // so at this point the node has been removed from the busy list
    // now we use the address of this node (curr) and find where it's supposed to be in the free list
    // the node should also be coalesced with adjacent free blocks
    address = curr->address;
    curr = FL->head;
    while (curr != NULL && curr->address < address) {
        prev = curr;
        curr = curr->next;
    }

    // Coalesce with adjacent free blocks if possible
    // this way will coalesce as we add the node in, but it's more confusing to look at
    // it could be worth it to make a less efficient coalescing function that would go over the entire list and coalesce.
    if (prev != NULL && prev->address + prev->data == address) {
        prev->data += curr->data;
        if (curr->next != NULL && address + curr->data == curr->next->address) {
            prev->data += curr->next->data;
            prev->next = curr->next->next;
            free(curr->next);
        } else {
            free(curr);
        }
    } else if (curr->next != NULL && address + curr->data == curr->next->address) {
        curr->data += curr->next->data;
        curr->address = address;
        curr->next = curr->next->next;
        free(curr->next);
        if (prev == NULL) {
            FL->head = curr;
        } else {
            prev->next = curr;
        }
    } else {
        curr->address = address;
        if (prev == NULL) {
            FL->head = curr;
        } else {
            prev->next = curr;
        }
        curr->prev = prev;
    }
}

void dumpMemoryLists(memory_manager *mm) {
    linkedlist *bl = mm->busy_list;
    linkedlist *fl = mm->free_list;
    printf("Busy List ");
    printList(bl);
    printf("Free List ");
    printList(fl);
    printf("\n");
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