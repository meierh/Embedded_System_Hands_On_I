#include <stdio.h>
#include "FIRFilter.h"

int main()
{    
    printf("-----------Test 1-----------\n");
    float a1[5] = {1,0,0,0,0};
    FIRFilter<float,4> filterO4_1(a1);
    float x1[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i=0; i<10; i++)
    {
        float y = filterO4_1(x1[i]);
        printf("%2d -> %2d\n",(int)x1[i],(int)y);
    }
    
    printf("-----------Test 2-----------\n");
    float a2[5] = {1,0,1,0,1};
    FIRFilter<float,4>filterO4_2(a2);
    float x2[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i=0; i<10; i++)
    {
        float y = filterO4_2(x2[i]);
        printf("%2d -> %2d\n",(int)x2[i],(int)y);
    }
    
    return 0;
}
