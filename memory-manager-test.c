#include "memory-manager.h"
#include <assert.h>

int main() {
    // Test createMemoryManager function
    memory_manager *mm = createMemoryManager(100);
    assert(mm->memory_map->count == 2);
    assert(mm->free_list->count == 0);
    assert(mm->busy_list->count == 0);

    // Test allocateMemory function
    memory_manager *allocated_block = allocateMemory(mm, 50);
    assert(mm->free_list->count == 1);
    assert(mm->busy_list->count == 1);
    assert(allocated_block != NULL);

    // Test allocateMemory function when no suitable block is found
    memory_manager *no_block = allocateMemory(mm, 1000);
    assert(mm->free_list->count == 1);
    assert(mm->busy_list->count == 1);
    assert(no_block == NULL);

    // Test dumpMemoryLists function
    dumpMemoryLists(mm); // just to check output manually

    // Test destroyMemoryManager function
    destroyMemoryManager(mm);
    assert(mm->memory_map == NULL);
    assert(mm->free_list == NULL);
    assert(mm->busy_list == NULL);

    return 0;
}

