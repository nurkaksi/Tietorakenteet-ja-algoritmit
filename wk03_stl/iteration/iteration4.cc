/**
 * iteration4.cc
 *
 * Print all items of a list in a reverse order
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration4.hh"
using namespace std;


void printReverse(const list<int>& lst)
{
    list<int>::reverse_iterator i;
    for (i = lst.rbegin();i != lst.rend(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
}
