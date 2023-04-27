#include "memory-manager.h"
#include <assert.h>

#include "memory-manager.c"
#include "linked-list.c"

int main() {
    // Test createMemoryManager function
    memory_manager *mm = createMemoryManager(1000);
    //should only be the initial node with a value of 100 for 100 free space
    assert(mm->memory_map->count == 1);
    assert(mm->free_list->count == 1);
    //should not be anything in the busy list
    assert(mm->busy_list->count == 0);
    printf("Memory Map creation passed!\n");

    // Test allocateMemory function
    int allocated_block = allocateMemory(mm, 50);
    assert(mm->free_list->count == 1);
    assert(mm->busy_list->count == 1);
    assert(allocated_block != -1);
    printf("allocateBlock passed!\n");

    // Allocating a few more blocks
    allocated_block = allocateMemory(mm, 50);
    allocated_block = allocateMemory(mm, 30);
    allocated_block = allocateMemory(mm, 50);
    allocated_block = allocateMemory(mm, 10);
    linkedlist *FL = mm->free_list;
    node *head = FL->head;
    printf("allocated more memory...\n");

    // Test dumpMemoryLists function
    dumpMemoryLists(mm); // just to check output manually

    // Test freeMemory function
    int address_to_free = allocated_block;
    printf("%i\n", address_to_free);
    freeMemory(mm, address_to_free);
    dumpMemoryLists(mm);

    printf("freeMemory passed!\n");
    // Test destroyMemoryManager function
    destroyMemoryManager(mm);
    printf("destroying memory manager...");
    assert(mm->memory_map == NULL);
    assert(mm->free_list == NULL);
    assert(mm->busy_list == NULL);

    return 0;
}

