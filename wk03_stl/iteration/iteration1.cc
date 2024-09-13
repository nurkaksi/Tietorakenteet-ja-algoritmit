/**
 * iteration1.cc
 *
 * Print all items of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration1.hh"
using namespace std;


void printAllItems(const list<int>& lst)
{
    list<int>::const_iterator i;
    for (i = lst.begin();i != lst.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
}
