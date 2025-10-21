/**
 * @author Daniel J. Thompson 
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include <cstddef>
#include <unordered_map>
#include <cstdlib>
#include <cstdio>
#include <iostream>
bool memoryLeak = false;
size_t amountAllocated = 0;
size_t amountFreed = 0;

std::unordered_map<void *, size_t> allocMap;
std::unordered_map<void *, size_t> freeList;
extern "C" {
    void removeFromFreeList(void *ptr) {
        if (freeList.find(ptr) != freeList.end()) {
	    freeList.erase(ptr);
	}
    } 
    void *dmalloc(size_t size) {
        void *tmp = malloc(size);
        allocMap.insert({tmp, size});
	removeFromFreeList(tmp);
        return tmp;
    }
    void dfree(void *ptr) {
        // Not in free list
        if (freeList.find(ptr) == freeList.end()) {
            size_t size = allocMap[ptr];
            freeList.insert({ptr, size});
            free(ptr);
        } else {
            // Send warning to stderr and cause double free
            fprintf(stderr, "Pointer %p was aleady freed\n", ptr);
            free(ptr); 
        }
        //printf("Freed\n");
    }
    void *drealloc(void *ptr, size_t size) {
        // Ptr present
        void *tmp = realloc(ptr, size);
	removeFromFreeList(tmp);
        if (tmp == ptr) {
            if (allocMap.find(ptr) != allocMap.end()) {
                allocMap[ptr] = size;
                return ptr;
            }
        } else {
            allocMap.erase(ptr);
            allocMap.insert({tmp, size});
            return tmp;
        }
    }
    void *dcalloc(size_t amt, size_t size) {
        void *tmp = calloc(amt, size);
        size_t actualSize = amt * size;
        allocMap.insert({tmp, actualSize});
        removeFromFreeList(tmp);
	return tmp;
    }
    /**
     * @brief Creates a report of all memory allocations and frees
     * @param fp The file pointer to write the report to
     */
    void printReport(FILE *fp) {
        // Check to see if the pointer has been freed 
        for (auto it = allocMap.begin(); it != allocMap.end(); ++it) {
            if (freeList.find(it->first) != freeList.end()) {
                amountFreed += freeList.at(it->first);
            } else {
                memoryLeak = true;
                fprintf(fp, "Pointer %p | Size: %zu\n", it->first, it->second);
		//std::cout << it->first << " | " << it->second << std::endl;
            }
            amountAllocated += it->second;
        }
        // Print report
        if (memoryLeak) {
            fprintf(fp, "Memory leak present!!!\n");
        }
        fprintf(fp, "Amount allocated %zu\n", amountAllocated);
        fprintf(fp, "Amount Freed %zu\n", amountFreed);
        
    }
}
