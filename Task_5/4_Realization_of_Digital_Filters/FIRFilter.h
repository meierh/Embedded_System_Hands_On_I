#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <stdio.h>

typedef struct
{
    unsigned int N;
    unsigned int k;
    float* a;
    float* x;
} FIRFilter;

// a must be a memory of size N+1, a is read only, allocates memory
void initialize(FIRFilter* filter, unsigned int N, const float* a);

// frees memory if allocated
void close(FIRFilter* filter);

// input value and output filtered value, filter must be initialized
float filterValue(FIRFilter* filter, float x);

#endif
