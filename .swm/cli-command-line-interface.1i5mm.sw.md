---
id: 1i5mm
title: CLI (command line interface)
file_version: 1.1.2
app_version: 1.7.2
---

<div align="center"><img src="https://firebasestorage.googleapis.com/v0/b/swimmio-content/o/repositories%2FZ2l0aHViJTNBJTNBTWVtb3J5LU1hbmFnZXIlM0ElM0FLeWxlRkM%3D%2F1906b416-8076-4b0a-85eb-1359bc334666.png?alt=media&token=cf4eebfb-0a6e-4a3e-a924-e025c55b8fcb" style="width:'50%'"/></div>

<br/>

## This file contains all the functions for the command line interface of the project.

<br/>

### Functions/Methods (click drop down to see code snippet of function)

<br/>

### Main

<br/>

1.  This is a C program that creates a memory manager with a given size using the createMemoryManager function.

2.  It then enters a loop where it prompts the user for commands, which it reads in from stdin using the fgets function. It removes the trailing newline character from the command and passes the command to the parseCommand function along with the memory manager.

3.  The loop continues until the user enters an end-of-file character or the program is terminated.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 cli.c
<!-- collapsed -->

```c
9      int main(int argc, char *argv[]) {
10         if (argc < 2) {
11             printUsage();
12             return 1;
13         }
14     
15         int size = atoi(argv[1]);
16         if (size <= 0) {
17             printf("Invalid size argument\n");
18             return 1;
19         }
20     
21         memory_manager *mm = createMemoryManager(size);
22         char command[BUFFER_SIZE];
23     
24         while (1) {
25             printf("> ");
26             if (fgets(command, BUFFER_SIZE, stdin) == NULL) {
27                 printf("\n");
28                 break;
29             }
30     
31             // Remove trailing newline character
32             int len = strlen(command);
33             if (command[len - 1] == '\n') {
34                 command[len - 1] = '\0';
35             }
36     
37             parseCommand(command, mm);
38         }
39     
40         destroyMemoryManager(mm);
41         return 0;
42     }
43     
```

<br/>

### PrintUsage and PrintMemoryMap

<br/>

_PrintUsage:_

printUsage() prints out a usage message for a memory manager command-line interface. It takes no parameters and simply calls printf() to display a message informing the user of the correct command-line syntax for the program.

_PrintMemoryMap:_

printMemoryMap() prints out a memory map for a given memory manager. It takes a pointer to the memory manager as its only parameter and uses it to access the linked list of memory blocks in the manager. It then iterates through the linked list and prints out each memory block's starting address, ending address, and size. The function prints the information in the format "start\_address - end\_address : size bytes".
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 cli.c
<!-- collapsed -->

```c
44     void printUsage() {
45         printf("Usage: memory-manager-cli [size]\n");
46     }
47     
48     void printMemoryMap(memory_manager *mm) {
49         printf("Memory Map:\n");
50         linkedlist *map = mm->memory_map;
51         node *curr = map->head;
52     
53         while (curr != NULL) {
54             printf("%p - %p : %d bytes\n", curr, (void*)((char*)curr + curr->data - 1), curr->data);
55             curr = curr->next;
56         }
57     }
```

<br/>

### Parse Command

<br/>

parseCommand() reads and interprets user input for a memory manager command-line interface. It checks the input for keywords like "allocate", "free", "list" or "view" and performs corresponding memory management operations. It also calls the printMemoryMap() function to display the current memory map. The function provides a simple way to allocate and free memory blocks, as well as view the current memory state.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 cli.c
```c
59     void parseCommand(char *command, memory_manager *mm) {
60         char *token = strtok(command, " ");
61     
62         if (token == NULL) {
63             return;
64         }
65     
66         if (strcmp(token, "allocate") == 0) {
67             token = strtok(NULL, " ");
68             if (token == NULL) {
69                 printf("allocate command requires size argument\n");
70                 return;
71             }
72     
73             int size = atoi(token);
74             if (size <= 0) {
75                 printf("Invalid size argument\n");
76                 return;
77             }
78     
79             int address = allocateMemory(mm, size);
80             if (address == -1) {
81                 printf("Failed to allocate %d bytes\n", size);
82             } else {
83                 //%p for hex so it looks like real memory addresses.
84                 printf("Allocated %d bytes at address %i\n", size, address);
85             }
86         } else if (strcmp(token, "free") == 0) {
87             token = strtok(NULL, " ");
88             if (token == NULL) {
89                 printf("free command requires address argument\n");
90                 return;
91             }
92     
93             //void *address = (void*) strtoull(token, NULL, 16);
94             //freeMemory(mm, address);
95             //printf("Freed memory at address %p\n", address);
96         } else if (strcmp(token, "list") == 0 || strcmp(token, "view") == 0) {
97             printMemoryMap(mm);
98         } else {
99             printf("Unknown command: %s\n", token);
100        }
101    }
```

<br/>

This file was generated by Swimm. [Click here to view it in the app](https://app.swimm.io/repos/Z2l0aHViJTNBJTNBTWVtb3J5LU1hbmFnZXIlM0ElM0FLeWxlRkM=/docs/1i5mm).
