#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <cstdint>

template<typename Floating, uint16_t ORDER>
class FIRFilter {
    public:
        /**
         * Constructor
         * @param a Coefficients of the filter as an array with the length ORDER+1
         */
        explicit FIRFilter(Floating a[ORDER+1]):k(0) {
            for(int i=0; i<ORDER+1; i++)
                this->a[i] = a[i];
        }

        /**
         * Filter invokation taking current value and returning filtered current value
         */
        Floating applyFilter(Floating x)
        {
            rightShiftMem();
            this->x[0] = x;
            incr();
            return innerMultiplication();
        }
        
    private:
        // Memory for filter parameter
        Floating a[ORDER+1];
        // Memory for filter history
        Floating x[ORDER+1];
        unsigned int k;
        
        // Shifts values in memory x to the right
        void rightShiftMem()
        {
            for(int ki=ORDER-1; ki>=0; ki--)
                this->x[ki+1] = this->x[ki];
        }
        
        // Inserts current element into the first position of the filter
        void insert(Floating x){ this->x[0]=x; }
        
        // Increments the number of valid values in the memory
        void incr(){ k = (k<(ORDER+1))?++k:ORDER+1; }
        
        // Compute inner dot product 
        Floating innerMultiplication()
        {
            Floating y = 0;
            for(unsigned int i=0; i<k; i++)
                y += a[i]*this->x[i];
            return y;
        }
};


#endif
