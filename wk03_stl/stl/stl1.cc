#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh"

using namespace std;


/**
 * @brief sort a given vector to an ascending order
 *
 * @param v vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortAsc(vector<int>& v)
{

    try {
        sort(v.begin(), v.end());

        return EXIT_SUCCESS;
    }

    catch(const exception& e) {

        return EXIT_FAILURE;
    }

}
