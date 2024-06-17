#include <stdlib.h>
#include "FIRFilter.h"

void initialize
(
    FIRFilter* filter,
    unsigned int N,
    const float* a
)
{   
    filter->N = N;
    filter->k = 0;

    filter->a = (float*)malloc(sizeof(float)*(N+1));
    for(uint i=0; i<(N+1); i++)
        (filter->a)[i] = a[i];

    filter->x = (float*)malloc(sizeof(float)*(N+1));
}

void close(FIRFilter* filter)
{
    free(filter->a);
    free(filter->x);
}

float filterValue
(
    FIRFilter* filter,
    float x
)
{
    float* a = filter->a;
    float* x_mem = filter->x;
    unsigned int N = filter->N;
    unsigned int k = filter->k;
    
    for(int ki=N-1; ki>=0; ki--)
    {
        x_mem[ki+1] = x_mem[ki];
    }
    x_mem[0] = x;
    
    filter->k = (k<(N+1))?++k:N+1;
    float y = 0;
    for(unsigned int i=0; i<k; i++)
    {
        y += a[i]*x_mem[i];
    }
    return y;
}

