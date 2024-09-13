/**
 * iteration2.cc
 *
 * Print every second item of a list starting from the first item
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration2.hh"
using namespace std;


void printEverySecond(const list<int>& lst)
{
    list<int>::const_iterator i;
    for (i = lst.begin();i != lst.end(); ++i) {
        cout << *i << " ";
        // hypätään kaksi kerrallaan käyttämällä valmista funktiota
        advance(i,2);
    }
    cout << endl;
}
