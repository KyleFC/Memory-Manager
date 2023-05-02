#include "memory-manager.h"
#include <assert.h>

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
    int allocated_block = allocateMemory(mm, 50, 0);
    assert(mm->free_list->count == 1);
    assert(mm->busy_list->count == 1);
    assert(allocated_block != -1);
    printf("allocateBlock passed!\n");

    // Allocating a few more blocks
    allocated_block = allocateMemory(mm, 50, 0);
    allocated_block = allocateMemory(mm, 30, 0);
    allocated_block = allocateMemory(mm, 50, 0);
    allocated_block = allocateMemory(mm, 10, 0);
    linkedlist *FL = mm->free_list;
    node *head = FL->head;
    printf("allocated more memory...\n");

    // Test dumpMemoryLists function
    dumpMemoryLists(mm); // just to check output manually

    // Test freeMemory function
    int address_to_free = allocated_block;
    //printf("%i\n", address_to_free);
    freeMemory(mm, 100);
    dumpMemoryLists(mm);

    printf("freeMemory passed!\n");
    // Test destroyMemoryManager function
    destroyMemoryManager(mm);
    printf("destroying memory manager for first-fit");
    assert(mm->memory_map == NULL);
    assert(mm->free_list == NULL);
    assert(mm->busy_list == NULL);

    memory_manager *mm2 = createMemoryManager(100);
    allocated_block = allocateMemory(mm, 10, 1);
    allocated_block = allocateMemory(mm, 20, 1);
    allocated_block = allocateMemory(mm, 30, 1);
    allocated_block = allocateMemory(mm, 30, 1);
    
    //FL size 10 address 0, BL s10 a0, s20 a10, s30 a30, s30 a60
    dumpMemoryLists(mm2);
    freeMemory(mm2, 10);
    freeMemory(mm2, 30);
    dumpMemoryLists(mm2);

    int address = allocateMemory(mm2, 10, 1);
    //should be 10 : 90
    printf("%i", address);

    address = allocateMemory(mm2, 30, 1);
    //should be 30 : 10
    printf("%i", address);
    dumpMemoryLists(mm2);
    return 0;
}

