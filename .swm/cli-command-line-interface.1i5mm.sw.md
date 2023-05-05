---
id: 1i5mm
title: CLI (command line interface)
file_version: 1.1.2
app_version: 1.7.2
---

<div align="center"><img src="https://firebasestorage.googleapis.com/v0/b/swimmio-content/o/repositories%2FZ2l0aHViJTNBJTNBTWVtb3J5LU1hbmFnZXIlM0ElM0FLeWxlRkM%3D%2F1906b416-8076-4b0a-85eb-1359bc334666.png?alt=media&token=cf4eebfb-0a6e-4a3e-a924-e025c55b8fcb" style="width:'50%'"/></div>

<br/>

## This file contains all the functions for the command line interface of the project.

### Functions/Methods (click drop down to see code snippet of function)

### Main

<br/>

1.  This is a C program that creates a memory manager with a given size using the createMemoryManager function.

2.  It then enters a loop where it prompts the user for commands, which it reads in from stdin using the fgets function. It removes the trailing newline character from the command and passes the command to the parseCommand function along with the memory manager.

3.  The loop continues until the user enters an end-of-file character or the program is terminated.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 cli.c
```c
9      int main(int argc, char *argv[]) {
10         if (argc != 2) {
11             printUsage();
12             return 1;
13         }
14     
15         FILE *config_file = fopen(argv[1], "r");
16         if (config_file == NULL) {
17             perror("Unable to open config file\n");
18             exit(1);
19         }
20     
21         int count = 0;
22         char* fit_mode;
23         char* allocate_size;
24         char line[128];
25     
26         while (fgets(line, sizeof line, config_file) != NULL) {
27             if (count == 0) {
28                 fit_mode = line;
29             }
30             else if (count == 1) {
31                 allocate_size = line;
32             }
33             else {
34                 break;
35             }
36             count++;
37         }
38     
39         int fit;
40         if (strcmp(fit_mode, "first-fit")) {
41             fit = 0;
42         }
43         else if (strcmp(fit_mode, "best-fit")) {
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
82     void printUsage() {
83         printf("Usage: [config file path]\n");
84     }
85     
86     void printMemoryMap(memory_manager *mm) {
87         dumpMemoryLists(mm);
88         return;
89     
90         printf("Memory Map:\n");
91         linkedlist *map = mm->memory_map;
92         node *curr = map->head;
93     
94         while (curr != NULL) {
95             printf("%p - %p : %d bytes\n", curr, (void*)((char*)curr + curr->data - 1), curr->data);
96             curr = curr->next;
97         }
98     }
```

<br/>

### Parse Command

<br/>

parseCommand() reads and interprets user input for a memory manager command-line interface. It checks the input for keywords like "allocate", "free", "list" or "view" and performs corresponding memory management operations. It also calls the printMemoryMap() function to display the current memory map. The function provides a simple way to allocate and free memory blocks, as well as view the current memory state.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 cli.c
<!-- collapsed -->

```c
100    void parseCommand(char *command, int fit, memory_manager *mm) {
101        char *token = strtok(command, " ");
102    
103        if (token == NULL) {
104            return;
105        }
106    
107        if (strcmp(token, "allocate") == 0) {
108            token = strtok(NULL, " ");
109            if (token == NULL) {
110                printf("allocate command requires size argument\n");
111                return;
112            }
113    
114            int size = atoi(token);
115            if (size <= 0) {
116                printf("Invalid size argument\n");
117                return;
118            }
119    
120            int address = allocateMemory(mm, size, fit);
121            if (address == -1) {
122                printf("Failed to allocate %d bytes\n", size);
123            } else {
124                //%p for hex so it looks like real memory addresses.
125                printf("Allocated %d bytes at address %i\n", size, address);
126            }
127        } else if (strcmp(token, "free") == 0) {
128            token = strtok(NULL, " ");
129            if (token == NULL) {
130                printf("free command requires address argument\n");
131                return;
132            }
133    
134            int address = atoi(token);
135            if (address <= 0) {
136                printf("Invalid address argument\n");
137            }
138    
139            freeMemory(mm, address);
140            printf("Freed memory at address %d\n", address);
141        } else if (strcmp(token, "list") == 0 || strcmp(token, "view") == 0) {
142            printMemoryMap(mm);
143        } else {
144            printf("Unknown command: %s\n", token);
145        }
146    }
```

<br/>

This file was generated by Swimm. [Click here to view it in the app](https://app.swimm.io/repos/Z2l0aHViJTNBJTNBTWVtb3J5LU1hbmFnZXIlM0ElM0FLeWxlRkM=/docs/1i5mm).
