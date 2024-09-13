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
    bool print = true;
        for ( list<int>::const_iterator it = lst.begin() ; it != lst.end() ; ++it ) {
            if ( print ) {
                cout << *it << " ";
            }
            print = !print;
        }
        cout << endl;
}
