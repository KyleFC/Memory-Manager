//implement cli (command line interface) functions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printUsage();
        return 1;
    }

    FILE *config_file = fopen(argv[1], "r");
    if (config_file == NULL) {
        perror("Unable to open config file\n");
        exit(1);
    }

    int count = 0;
    char* fit_mode;
    char* allocate_size;
    char line[128];

    while (fgets(line, sizeof line, config_file) != NULL) {
        if (count == 0) {
            fit_mode = line;
        }
        else if (count == 1) {
            allocate_size = line;
        }
        else {
            break;
        }
        count++;
    }

    int fit;
    if (strcmp(fit_mode, "first-fit")) {
        fit = 0;
    }
    else if (strcmp(fit_mode, "best-fit")) {
        fit = 1;
    }
    else {
        printf("Invalid fit mode\n");
        return 1;
    }

    int size = atoi(allocate_size);
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

        parseCommand(command, fit, mm);
    }

    destroyMemoryManager(mm);
    return 0;
}



void printUsage() {
    printf("Usage: [config file path]\n");
}

void printMemoryMap(memory_manager *mm) {
    dumpMemoryLists(mm);
    return;

    printf("Memory Map:\n");
    linkedlist *map = mm->memory_map;
    node *curr = map->head;

    while (curr != NULL) {
        printf("%p - %p : %d bytes\n", curr, (void*)((char*)curr + curr->data - 1), curr->data);
        curr = curr->next;
    }
}

void parseCommand(char *command, int fit, memory_manager *mm) {
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

        int address = allocateMemory(mm, size, fit);
        if (address == -1) {
            printf("Failed to allocate %d bytes\n", size);
        } else {
            //%p for hex so it looks like real memory addresses.
            printf("Allocated %d bytes at address %i\n", size, address);
        }
    } else if (strcmp(token, "free") == 0) {
        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("free command requires address argument\n");
            return;
        }

        int address = atoi(token);
        if (address <= 0) {
            printf("Invalid address argument\n");
        }

        freeMemory(mm, address);
        printf("Freed memory at address %d\n", address);
    } else if (strcmp(token, "list") == 0 || strcmp(token, "view") == 0) {
        printMemoryMap(mm);
    } else {
        printf("Unknown command: %s\n", token);
    }
}