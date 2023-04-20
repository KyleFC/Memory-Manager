//implement cli (command line interface) functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    int size = atoi(argv[1]);
    if (size <= 0) {
        printf("Invalid size argument\n");
        return 1;
    }

    memory_manager *mm = createMemoryManager(size);
    char command[BUFFER_SIZE];

    while (1) {
        printf("> ");
        if (fgets(command, BUFFER_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        // Remove trailing newline character
        int len = strlen(command);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        parseCommand(command, mm);
    }

    destroyMemoryManager(mm);
    return 0;
}

void printUsage() {
    printf("Usage: memory-manager-cli [size]\n");
}

void printMemoryMap(memory_manager *mm) {
    printf("Memory Map:\n");
    linkedlist *map = mm->memory_map;
    node *curr = map->head;

    while (curr != NULL) {
        printf("%p - %p : %d bytes\n", curr, (void*)((char*)curr + curr->data - 1), curr->data);
        curr = curr->next;
    }
}

void parseCommand(char *command, memory_manager *mm) {
    char *token = strtok(command, " ");

    if (token == NULL) {
        return;
    }

    if (strcmp(token, "allocate") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("allocate command requires size argument\n");
            return;
        }

        int size = atoi(token);
        if (size <= 0) {
            printf("Invalid size argument\n");
            return;
        }

        void *address = allocateMemory(mm, size);
        if (address == NULL) {
            printf("Failed to allocate %d bytes\n", size);
        } else {
            printf("Allocated %d bytes at address %p\n", size, address);
        }
    } else if (strcmp(token, "free") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("free command requires address argument\n");
            return;
        }

        void *address = (void*) strtoull(token, NULL, 16);
        freeMemory(mm, address);
        printf("Freed memory at address %p\n", address);
    } else if (strcmp(token, "list") == 0 || strcmp(token, "view") == 0) {
        printMemoryMap(mm);
    } else {
        printf("Unknown command: %s\n", token);
    }
}