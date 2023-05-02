---
id: fzgcc
title: Memory Manager Library
file_version: 1.1.2
app_version: 1.7.2
---

## Implements the main portion of memory manager

<br/>

### Memory Manager

<br/>

createMemoryManager() creates a memory manager with a specified size. It allocates memory for the manager, as well as three linked lists that will be used to manage memory blocks. It initializes the linked lists, adding the initial block of memory to both the memory map and free list. Finally, it returns the created memory manager. This function sets up the initial memory management state for the program.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
4      memory_manager *createMemoryManager(int size) {
5          // Allocate memory for the memory manager
6          memory_manager *mem_manager = malloc(sizeof(memory_manager));
7      
8          // Initialize the memory map
9          linkedlist *LL = (linkedlist*) malloc(sizeof(linkedlist));
10         LL->head = NULL;
11         LL->tail = NULL;
12         LL->count = 0;
13         linkedlist *FL = (linkedlist*) malloc(sizeof(linkedlist));
14         FL->head = NULL;
15         FL->tail = NULL;
16         FL->count = 0;
17         linkedlist *BL = (linkedlist*) malloc(sizeof(linkedlist));
18         BL->head = NULL;
19         BL->tail = NULL;
20         BL->count = 0;
21     
22         addToStart(LL, size);
23         addToStart(FL, size);
24         
25         mem_manager->memory_map = LL;
26         mem_manager->free_list = FL;
27         mem_manager->busy_list = BL;
28     
29         return mem_manager;
30     }
```

<br/>

### Find Best Block

<br/>

The findBestBlock function searches through a linked list of memory blocks to find the best-fit block for the given size. It initializes a best\_fit pointer to NULL and a min variable to the maximum integer value. It then iterates through each node of the linked list, updating the best\_fit pointer if the current block is at least the required size and if the difference between the size of the current block and the required size is less than the current min. Finally, it returns the best\_fit pointer.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
32     node  *findBestBlock(linkedlist *LL, int size) {
33         node *best_fit = NULL;
34         int min = __INT_MAX__;
35         node *curr = LL->head;
36         node *prev = NULL;
37     
38         while (curr != NULL) {
39             if (curr->data >= size) {
40                 best_fit = curr;
41                 min = curr->data - size;
42             }
43             prev = curr;
44             curr = curr->next;
45         } 
46         return best_fit;
47     }
```

<br/>

### Allocate Memory

<br/>

This function takes a memory\_manager struct and an integer size and returns the starting address of a block of memory of the given size. It first checks the linkedlist of free blocks for a suitable block that can fit the requested size, using either first-fit or best-fit allocation mode based on the value of the "mode" parameter. If a block is found, it is removed from the free list and added to the busy list, and its starting address is returned. If no suitable block is found, -1 is returned to indicate a failure to allocate memory. If a block is found but has more space than needed, it is split into two blocks, with the second block being added to the free list.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
49     int allocateMemory(memory_manager *mm, int size, int mode) {
50         linkedlist *FL = mm->free_list;
51         node *prev = NULL;
52         node *curr = FL->head;
53     
54         //for all of the nodes "data" is technically the size of the block
55         //traverse through the linkedlist of free blocks until it finds one that is big enough to fit "size"
56         if (mode == 0) {
57             while (curr != NULL && curr->data < size) {
58                 prev = curr;
59                 curr = curr->next;
60             }
61         }
62         else {
63             node *curr = findBestBlock(FL, size);
64         }
65     
66         if (curr == NULL) {
67             // Could not find a suitable block in free list
68             return -1;
69         }
70     
71         // Found a suitable block in free list
72         if (curr->data >= size) {
73             // Split the node if it has more space than needed
74             // new node is the second half
75             node *new_node = malloc(sizeof(node));
76             new_node->data = curr->data - size;
77             new_node->address = curr->address + size;
78             new_node->next = NULL;
79             new_node->prev = curr;
80             curr->data = size;
81             curr->next = new_node;
82         }
83         // Remove the allocated block from the free list
84         if (prev == NULL) {
85             FL->head = curr->next;
86         } else {
87             prev->next = curr->next;
88         }
89     
90         // Add the allocated block to the busy list
91         int address = curr->address;
92         linkedlist *BL = mm->busy_list;
93         addToEnd(BL, size, address);
94     
95         // Return the address
96         return address;
97     }
```

<br/>

### Free Memory

<br/>

This function takes an allocated memory address and frees the corresponding memory block by moving it from the busy list to the free list. The function also performs block coalescing and sorting of the free list after the new block is added. Block coalescing combines adjacent free blocks into a larger block to prevent fragmentation, while sorting ensures that the free list is always in ascending order by address.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
99     void freeMemory(memory_manager *mm, int address) {
100        linkedlist *BL = mm->busy_list;
101        linkedlist *FL = mm->free_list;
102        node *freed_node = unlinkNode(BL, address);
103        //printf("%i", freed_node->address);
104        addToEnd(FL, freed_node->data, freed_node->address);
105        selectionSortBlocks(FL);
106        coalesceList(FL);
107    }
```

<br/>

### Coalesce List

<br/>

The \`coalesceList()\` function takes a linked list of memory blocks as input and merges adjacent blocks that are free. It uses a \`while\` loop to iterate through the linked list and check if the end of the current block matches the start of the next block. If a match is found, the two blocks are merged into a single block. If there is no match, the function moves on to the next block. The function runs until there are no more adjacent blocks to merge.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
109    void coalesceList(linkedlist *LL) {
110        node *curr = LL->head;
111        while (curr && curr->next) {
112            // Find end of current block
113            int curr_end = curr->address + curr->data;
114            node *next = curr->next;
115            // Find start of next block
116            int next_start = next->address;
117            //printf("%d, %d", curr_end, next_start);
118            if (curr_end == next_start) {
119                // Merge the two blocks into a single block
120                //printf("Found match");
121                curr->data += next->data;
122                unlinkNode(LL, next->address);
123            } 
124            else if (LL->tail == curr) {
125                curr->next = NULL;
126            }
127            else {
128                curr = curr->next;
129            }
130        }
131    }
```

<br/>

### Dump Memory List

<br/>

The \`dumpMemoryLists\` function takes a memory manager \`mm\` as input and prints the current state of the busy and free lists to the console. It first retrieves the busy and free lists from the memory manager, and then calls the \`printList\` function to print the details of each list. The output includes information such as the address and size of each block in the lists.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
133    void dumpMemoryLists(memory_manager *mm) {
134        linkedlist *bl = mm->busy_list;
135        linkedlist *fl = mm->free_list;
136        printf("Busy List ");
137        printList(bl);
138        printf("Free List ");
139        printList(fl);
140    }
```

<br/>

### Destroy Memory Manager

<br/>

This is a function that destroys the memory manager and frees all the memory associated with its lists. It takes a pointer to a memory\_manager struct as its argument. The function calls the destroyList() function on the memory\_map, free\_list, and busy\_list linked lists to free the memory allocated for the nodes in those lists. Then it sets those pointers to NULL to avoid any possible dangling pointers.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
142    void destroyMemoryManager(memory_manager *mm) {
143        linkedlist *map  = mm->memory_map;
144        linkedlist *fl  = mm->free_list;
145        linkedlist *bl  = mm->busy_list;
146        destroyList(map);
147        destroyList(fl);
148        destroyList(bl);
149        mm->memory_map = NULL;
150        mm->free_list = NULL;
151        mm->busy_list = NULL;
152    }
```

<br/>

This file was generated by Swimm. [Click here to view it in the app](https://app.swimm.io/repos/Z2l0aHViJTNBJTNBTWVtb3J5LU1hbmFnZXIlM0ElM0FLeWxlRkM=/docs/fzgcc).
