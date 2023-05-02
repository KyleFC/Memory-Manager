#include "memory-manager.h"
#include <assert.h>

int main() {

    //Some linked list testing
    //linkedlist *ll = (*linkedlist) malloc(sizeof(linkedlist));




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
    printf("destroying memory manager for first-fit\n");
    assert(mm->memory_map == NULL);
    assert(mm->free_list == NULL);
    assert(mm->busy_list == NULL);

    memory_manager *mm2 = createMemoryManager(100);
    allocated_block = allocateMemory(mm2, 10, 1);
    allocated_block = allocateMemory(mm2, 20, 1);
    allocated_block = allocateMemory(mm2, 30, 1);
    allocated_block = allocateMemory(mm2, 30, 1);
    printAddresses(mm2->free_list);
    printAddresses(mm2->busy_list);
    
    //BL size 10 address 0, BL s10 a0, s20 a10, s30 a30, s30 a60
    dumpMemoryLists(mm2);
    //printf("tail = %i", mm2->free_list->tail->address);
    freeMemory(mm2, 10);
    dumpMemoryLists(mm2);
    freeMemory(mm2, 30);
    //printf("tail = %i", mm2->free_list->tail->address);
    //FL size 10 address 90, size 50 address 10
    printAddresses(mm2->free_list);
    printAddresses(mm2->busy_list);
    printf("\n");
    dumpMemoryLists(mm2);

    int address = allocateMemory(mm2, 10, 1);
    address = allocateMemory(mm2, 30, 1);
    dumpMemoryLists(mm2);
    return 0;
}

