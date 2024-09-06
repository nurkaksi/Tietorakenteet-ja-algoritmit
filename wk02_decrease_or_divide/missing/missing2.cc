#include <cstdlib>
#include <iostream>
#include "test.hh"

int searchSmallestMissing(int* A, int left, int right){
    /// triviaalitapaus, eli vasen ja oikea osoittavat samaan soluun
    if (left == right) {
        /// katsotaan onko arvo indeksin mukaan oikeassa paikassa
        if (A[left] == A[0]+left) {
            /// Mitään ei puutu jos indeksi on oikein
            return NO_VALUE_MISSING;

        /// Jos taas left ja right osoittivat samaan paikkaan, mutta
        /// indeksin arvo ja indeksi olivat vituillaan, tämä on puuttuva arvo
        } else {
            return A[0]+left;
        }

    }
    else {
       /// tarkastetaan onko keskikohta oikeassa paikassa
       int mid = left + (left+right)/2;

       /// jos keskikohta on väärässä paikassa, etsittävä arvo
       /// on vasemmassa puolikkaassa
       if (A[mid] != A[0]+mid) {
           return searchSmallestMissing(A, left, mid);
       }
       else {
           /// jos keskikohta on oikein, etsitään oikeasta puolikkaasta
           return searchSmallestMissing(A, mid+1, right);
       }
    }

}
