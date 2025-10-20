/**
 * @author Daniel J. Thompson 
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef DMALLOC_H
#define DMALLOC_H
#include <stddef.h>
#include <stdio.h>
#define malloc(a) dmalloc(a)
#define free(a) dfree(a)
#define realloc(a,b) drealloc(a,b)
#define calloc(a,b) dcalloc(a,b)

/*
 * Replacements for libc memory functions
 */

void *dmalloc(size_t size);
void dfree(void *ptr);
void *drealloc(void *ptr, size_t size);
void *dcalloc(size_t amt, size_t size);

/**
 * Prints memory report for
 * @param fp the file descriptor to print the memort report
 */
void printReport(FILE *fp);
#endif 
