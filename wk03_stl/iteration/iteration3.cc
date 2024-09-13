/**
 * iteration3.cc
 *
 * Print beginning half of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration3.hh"
using namespace std;


void printHalf(const list<int>& lst)
{
    list<int>::const_iterator left = lst.begin();
    list<int>::const_iterator right = left;
    size_t size = lst.size();
    advance(right, size/2);

    for (; left != right; ++left ) {
        cout << *left << " ";
    }
    cout << endl;
}
