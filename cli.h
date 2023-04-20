//declare functions for the command line interface
//allocate, list, free, view
#include "memory-manager.h"


void printUsage();
void printMemoryMap(memory_manager *mm);
void parseCommand(char *command, memory_manager *mm);
