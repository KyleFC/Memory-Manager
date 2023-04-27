//This file will contain all the function prototypes and data structures required for the memory manager API.
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include "linked-list.h"

// Define a structure to represent the memory manager
typedef struct memory_manager {
    linkedlist *memory_map;   // The memory map
    linkedlist *free_list;        // The free list
    linkedlist *busy_list;        // The busy list
} memory_manager;

memory_manager *createMemoryManager(int);
int allocateMemory(memory_manager*, int);
void freeMemory(memory_manager*, int);
void dumpMemoryLists(memory_manager*);
void destroyMemoryManager(memory_manager*);
void freeMemory(memory_manager*, int);
void coalesceList(linkedlist *);
#endif