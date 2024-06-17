#include <stdio.h>
#include "FIRFilter.h"

int main()
{
    FIRFilter filter;
    
    printf("-----------Test 1-----------\n");
    float a1[5] = {1,0,0,0,0};
    initialize(&filter,4,a1);
    float x1[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i=0; i<10; i++)
    {
        float y = filterValue(&filter,x1[i]);
        printf("%2d -> %2d\n",(int)x1[i],(int)y);
    }
    close(&filter);
    
    printf("-----------Test 2-----------\n");
    float a2[5] = {1,0,1,0,1};
    initialize(&filter,4,a2);
    float x2[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i=0; i<10; i++)
    {
        float y = filterValue(&filter,x2[i]);
        printf("%2d -> %2d\n",(int)x2[i],(int)y);
    }
    close(&filter);
    
    return 0;
}
