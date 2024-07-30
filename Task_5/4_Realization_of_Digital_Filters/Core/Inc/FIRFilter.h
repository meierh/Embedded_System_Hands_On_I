#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <cstdint>

template<typename Floating, uint16_t ORDER>
class FIRFilter {
    public:
        /**
         * Constructor
         * @param a
         */
        explicit FIRFilter(Floating a[ORDER+1]):k(0) {
            for(int i=0; i<ORDER+1; i++)
                this->a[i] = a[i];
        }


        Floating applyFilter(Floating x)
        {
            rightShiftMem();
            this->x[0] = x;
            incr();
            return innerMultiplication();
        }
        
    private:
        // store filter parameter
        Floating a[ORDER+1];
        Floating x[ORDER+1];
        unsigned int k;
        
        void rightShiftMem()
        {
            for(int ki=ORDER-1; ki>=0; ki--)
                this->x[ki+1] = this->x[ki];
        }
        
        void insert(Floating x){ this->x[0]=x; }
        
        void incr(){ k = (k<(ORDER+1))?++k:ORDER+1; }
        
        Floating innerMultiplication()
        {
            Floating y = 0;
            for(unsigned int i=0; i<k; i++)
                y += a[i]*this->x[i];
            return y;
        }
};


#endif
