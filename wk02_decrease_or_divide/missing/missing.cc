#include <cstdlib>
#include <iostream>
#include "test.hh"

int searchSmallestMissingIteration(int* arr, int size){
    int i = 0;
    int j = arr[0];
    for ( i ; i<size; ++i){
        if (j != arr[i]){
            return j;
        }
        j++;
    }
    return NO_VALUE_MISSING;
}
