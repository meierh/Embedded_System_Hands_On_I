//
// Created by nico on 13.07.24.
//

#ifndef INC_3_INTER_INTEGRATED_CIRCUIT_ARRAY_H
#define INC_3_INTER_INTEGRATED_CIRCUIT_ARRAY_H
#include "main.h"


template<uint16_t sizeTemp>
class array {
public:

    uint8_t operator[](int idx) const {
        return *(dataPtr+idx);
    }

    uint8_t& operator[](int idx) {
        return *(dataPtr+idx);
    }

    uint8_t* data(){
        return dataPtr;
    }


    uint16_t size(){
        return arraySize;
    }

private:
    uint8_t dataPtr[sizeTemp]{};
    uint16_t arraySize = sizeTemp;
};

#endif //INC_3_INTER_INTEGRATED_CIRCUIT_ARRAY_H
