#include "memory-manager.h"
#include <assert.h>

int main() {

    printf("\nRunning unit tests ...\n");

    // Test createMemoryManager function
    memory_manager *mm = createMemoryManager(1000);
    //should only be the initial node with a value of 100 for 100 free space
    assert(mm->memory_map->count == 1);
    assert(mm->free_list->count == 1);
    //should not be anything in the busy list
    assert(mm->busy_list->count == 0);

    // Test allocateMemory function
    int allocated_block = allocateMemory(mm, 50, 0);
    assert(mm->free_list->count == 1);
    assert(mm->busy_list->count == 1);
    assert(allocated_block != -1);

    // Allocating a few more blocks
    allocated_block = allocateMemory(mm, 50, 0);
    allocated_block = allocateMemory(mm, 30, 0);
    allocated_block = allocateMemory(mm, 50, 0);
    allocated_block = allocateMemory(mm, 10, 0);
    linkedlist *FL = mm->free_list;
    node *head = FL->head;

    // Test freeMemory function
    freeMemory(mm, 100);

    // Test destroyMemoryManager function
    destroyMemoryManager(mm);
    assert(mm->memory_map == NULL);
    assert(mm->free_list == NULL);
    assert(mm->busy_list == NULL);

    mm = createMemoryManager(100);
    allocated_block = allocateMemory(mm, 10, 1);
    allocated_block = allocateMemory(mm, 20, 1);
    allocated_block = allocateMemory(mm, 30, 1);
    allocated_block = allocateMemory(mm, 30, 1);

    // Test lists manually
    assert(mm->busy_list->count == 4);
    assert(mm->busy_list->head->address == 0);
    assert(mm->busy_list->head->data == 10);
    assert(mm->busy_list->head->next->address == 10);
    assert(mm->busy_list->head->next->data == 20);
    assert(mm->busy_list->head->next->next->address == 30);
    assert(mm->busy_list->head->next->next->data == 30);
    assert(mm->busy_list->head->next->next->next->address == 60);
    assert(mm->busy_list->head->next->next->next->data == 30);
    assert(mm->busy_list->tail->address == 60);
    assert(mm->busy_list->tail->data == 30);

    assert(mm->free_list->count == 1);
    assert(mm->free_list->head->address == 90);
    assert(mm->free_list->head->data == 10);
    assert(mm->free_list->tail->address == 90);
    assert(mm->free_list->tail->data == 10);
    
    // Test lists manually
    freeMemory(mm, 10);
    freeMemory(mm, 30);

    assert(mm->busy_list->count == 2);
    assert(mm->busy_list->head->address == 0);
    assert(mm->busy_list->head->data == 10);
    assert(mm->busy_list->head->next->address == 60);
    assert(mm->busy_list->head->next->data == 30);
    assert(mm->busy_list->tail->address == 60);
    assert(mm->busy_list->tail->data == 30);

    assert(mm->free_list->count == 2);
    assert(mm->free_list->head->address == 10);
    assert(mm->free_list->head->data == 50);
    assert(mm->free_list->head->next->address == 90);
    assert(mm->free_list->head->next->data == 10);
    assert(mm->free_list->tail->address == 90);
    assert(mm->free_list->tail->data == 10);

    // Test lists manually
    allocateMemory(mm, 10, 1);
    allocateMemory(mm, 30, 1);

    assert(mm->busy_list->count == 4);
    assert(mm->busy_list->head->address == 0);
    assert(mm->busy_list->head->data == 10);
    assert(mm->busy_list->head->next->address == 60);
    assert(mm->busy_list->head->next->data == 30);
    assert(mm->busy_list->head->next->next->address == 90);
    assert(mm->busy_list->head->next->next->data == 10);
    assert(mm->busy_list->head->next->next->next->address == 10);
    assert(mm->busy_list->head->next->next->next->data == 30);
    assert(mm->busy_list->tail->address == 10);
    assert(mm->busy_list->tail->data == 30);

    assert(mm->free_list->count == 1);
    assert(mm->free_list->head->address == 40);
    assert(mm->free_list->head->data == 20);
    assert(mm->free_list->tail->address == 40);
    assert(mm->free_list->tail->data == 20);

    freeMemory(mm, 0);
    freeMemory(mm, 60);

    assert(mm->busy_list->count == 2);
    assert(mm->busy_list->head->address == 90);
    assert(mm->busy_list->head->data == 10);
    assert(mm->busy_list->head->next->address == 10);
    assert(mm->busy_list->head->next->data == 30);
    assert(mm->busy_list->tail->address == 10);
    assert(mm->busy_list->tail->data == 30);

    assert(mm->free_list->count == 2);
    assert(mm->free_list->head->address == 0);
    assert(mm->free_list->head->data == 10);
    assert(mm->free_list->head->next->address == 40);
    assert(mm->free_list->head->next->data == 50);
    assert(mm->free_list->tail->address == 40);
    assert(mm->free_list->tail->data == 50);

    freeMemory(mm, 90);
    freeMemory(mm, 10);

    assert(mm->busy_list->count == 0);
    assert(mm->busy_list->head == NULL);
    assert(mm->busy_list->tail == NULL);

    assert(mm->free_list->count == 1);
    assert(mm->free_list->head->address == 0);
    assert(mm->free_list->head->data == 100);
    assert(mm->free_list->tail->address == 0);
    assert(mm->free_list->tail->data == 100);

    allocateMemory(mm, 50, 0);
    allocateMemory(mm, 30, 0);
    allocateMemory(mm, 20, 0);

    assert(mm->busy_list->count == 3);
    assert(mm->busy_list->head->address == 0);
    assert(mm->busy_list->head->data == 50);
    assert(mm->busy_list->head->next->address == 50);
    assert(mm->busy_list->head->next->data == 30);
    assert(mm->busy_list->head->next->next->address == 80);
    assert(mm->busy_list->head->next->next->data == 20);
    assert(mm->busy_list->tail->address == 80);
    assert(mm->busy_list->tail->data == 20);

    assert(mm->free_list->count == 0);
    assert(mm->free_list->head == NULL);
    assert(mm->free_list->tail == NULL);

    printf("\nUnit test complete!\n");
    return 0;
}

