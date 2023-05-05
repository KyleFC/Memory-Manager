---
id: fzgcc
title: Memory Manager Library
file_version: 1.1.2
app_version: 1.7.2
---

## Implements the main portion of memory manager

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
22         addToEnd(LL, size, 0);
23         addToEnd(FL, size, 0);
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
39             if (curr->data >= size && curr->data < min) {
40                 best_fit = curr;
41                 min = curr->data;
42             }
43             prev = curr;
44             curr = curr->next;
45         } 
46     
47         return best_fit;
48     }
```

<br/>

### Allocate Memory

<br/>

This function takes a memory\_manager struct and an integer size and returns the starting address of a block of memory of the given size. It first checks the linkedlist of free blocks for a suitable block that can fit the requested size, using either first-fit or best-fit allocation mode based on the value of the "mode" parameter. If a block is found, it is removed from the free list and added to the busy list, and its starting address is returned. If no suitable block is found, -1 is returned to indicate a failure to allocate memory. If a block is found but has more space than needed, it is split into two blocks, with the second block being added to the free list.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
50     int allocateMemory(memory_manager *mm, int size, int mode) {
51         linkedlist *FL = mm->free_list;
52         node *prev = NULL;
53         node *curr = FL->head;
54     
55         //for all of the nodes "data" is technically the size of the block
56         //traverse through the linkedlist of free blocks until it finds one that is big enough to fit "size"
57         if (mode == 0) {
58             while (curr != NULL && curr->data < size) {
59                 prev = curr;
60                 curr = curr->next;
61             }
62         }
63         else {
64             curr = findBestBlock(FL, size);
65         }
66     
67         if (curr == NULL) {
68             // Could not find a suitable block in free list
69             return -1;
70         }
71     
72         // Found a suitable block in free list
73         if (curr->data > size) {
74             // Split the node if it has more space than needed
75             // new node is the second half
76     
77             insertAfter(FL, curr, curr->data - size, curr->address + size);
78     
79             curr->data = size;
80         }
81         // Remove the allocated block from the free list
82         int address = curr->address;
83         unlinkNode(FL, curr->address);
84     
85     
86         // Add the allocated block to the busy list
87         linkedlist *BL = mm->busy_list;
88         addToEnd(BL, size, address);
89     
90         // Return the address
91         return address;
92     }
93     
94     void freeMemory(memory_manager *mm, int address) {
95         linkedlist *BL = mm->busy_list;
96         linkedlist *FL = mm->free_list;
97         node *freed_node = unlinkNode(BL, address);
98         addToEnd(FL, freed_node->data, freed_node->address);
99         selectionSortBlocks(FL);
100        coalesceList(FL);
101    }
102    
103    void coalesceList(linkedlist *LL) {
104        node *curr = LL->head;
105        while (curr != NULL && curr->next != NULL) {
106            // Find end of current block
107            int curr_end = curr->address + curr->data;
108            node *next = curr->next;
109            int size = next->data;
110            // Find start of next block
111            int next_start = next->address;
112            if (curr_end == next_start) {
113                // Merge the two blocks into a single block
114    
115                curr->data += next->data;
116                unlinkNode(LL, next->address);
117            }
118            else {
119                curr = curr->next;
120            }
121        }
122    }
123    
124    void printAddresses(linkedlist *LL) {
125        node *curr = LL->head;
126        printf("Addresses: ");
127        while (curr != NULL) {
128            printf("%i ", curr->address);
129            curr = curr->next;
130        }
131        printf("\n");
132    }
133    
134    void dumpMemoryLists(memory_manager *mm) {
135        linkedlist *bl = mm->busy_list;
136        linkedlist *fl = mm->free_list;
137        printf("Busy List ");
138        printList(bl);
139        printf("Free List ");
140        printList(fl);
141        printf("\n");
142    }
143    
144    
145    
146    void destroyMemoryManager(memory_manager *mm) {
147        linkedlist *map  = mm->memory_map;
148        linkedlist *fl  = mm->free_list;
149        linkedlist *bl  = mm->busy_list;
150        destroyList(map);
151        destroyList(fl);
152        destroyList(bl);
153        mm->memory_map = NULL;
154        mm->free_list = NULL;
155        mm->busy_list = NULL;
156    }
```

<br/>

### Free Memory

<br/>

This function takes an allocated memory address and frees the corresponding memory block by moving it from the busy list to the free list. The function also performs block coalescing and sorting of the free list after the new block is added. Block coalescing combines adjacent free blocks into a larger block to prevent fragmentation, while sorting ensures that the free list is always in ascending order by address.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
94     void freeMemory(memory_manager *mm, int address) {
95         linkedlist *BL = mm->busy_list;
96         linkedlist *FL = mm->free_list;
97         node *freed_node = unlinkNode(BL, address);
98         addToEnd(FL, freed_node->data, freed_node->address);
99         selectionSortBlocks(FL);
100        coalesceList(FL);
101    }
```

<br/>

### Coalesce List

<br/>

The \`coalesceList()\` function takes a linked list of memory blocks as input and merges adjacent blocks that are free. It uses a \`while\` loop to iterate through the linked list and check if the end of the current block matches the start of the next block. If a match is found, the two blocks are merged into a single block. If there is no match, the function moves on to the next block. The function runs until there are no more adjacent blocks to merge.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
103    void coalesceList(linkedlist *LL) {
104        node *curr = LL->head;
105        while (curr != NULL && curr->next != NULL) {
106            // Find end of current block
107            int curr_end = curr->address + curr->data;
108            node *next = curr->next;
109            int size = next->data;
110            // Find start of next block
111            int next_start = next->address;
112            if (curr_end == next_start) {
113                // Merge the two blocks into a single block
114    
115                curr->data += next->data;
116                unlinkNode(LL, next->address);
117            }
118            else {
119                curr = curr->next;
120            }
121        }
122    }
123    
124    void printAddresses(linkedlist *LL) {
125        node *curr = LL->head;
126        printf("Addresses: ");
127        while (curr != NULL) {
128            printf("%i ", curr->address);
129            curr = curr->next;
130        }
131        printf("\n");
132    }
133    
134    void dumpMemoryLists(memory_manager *mm) {
135        linkedlist *bl = mm->busy_list;
136        linkedlist *fl = mm->free_list;
137        printf("Busy List ");
138        printList(bl);
139        printf("Free List ");
140        printList(fl);
141        printf("\n");
142    }
```

<br/>

### Dump Memory List

<br/>

The \`dumpMemoryLists\` function takes a memory manager \`mm\` as input and prints the current state of the busy and free lists to the console. It first retrieves the busy and free lists from the memory manager, and then calls the \`printList\` function to print the details of each list. The output includes information such as the address and size of each block in the lists.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
134    void dumpMemoryLists(memory_manager *mm) {
135        linkedlist *bl = mm->busy_list;
136        linkedlist *fl = mm->free_list;
137        printf("Busy List ");
138        printList(bl);
139        printf("Free List ");
140        printList(fl);
141        printf("\n");
142    }
```

<br/>

### Destroy Memory Manager

<br/>

This is a function that destroys the memory manager and frees all the memory associated with its lists. It takes a pointer to a memory\_manager struct as its argument. The function calls the destroyList() function on the memory\_map, free\_list, and busy\_list linked lists to free the memory allocated for the nodes in those lists. Then it sets those pointers to NULL to avoid any possible dangling pointers.
<!-- NOTE-swimm-snippet: the lines below link your snippet to Swimm -->
### 📄 memory-manager.c
<!-- collapsed -->

```c
146    void destroyMemoryManager(memory_manager *mm) {
147        linkedlist *map  = mm->memory_map;
148        linkedlist *fl  = mm->free_list;
149        linkedlist *bl  = mm->busy_list;
150        destroyList(map);
151        destroyList(fl);
152        destroyList(bl);
153        mm->memory_map = NULL;
154        mm->free_list = NULL;
155        mm->busy_list = NULL;
156    }
```

<br/>

This file was generated by Swimm. [Click here to view it in the app](https://app.swimm.io/repos/Z2l0aHViJTNBJTNBTWVtb3J5LU1hbmFnZXIlM0ElM0FLeWxlRkM=/docs/fzgcc).
