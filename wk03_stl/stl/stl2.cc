#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh"

using namespace std;


/**
 * @brief sort a given vector to an descending order
 *
 * @param v a vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortDesc(vector<int>& v)
{

    try {
        sort(v.rbegin(), v.rend());

        return EXIT_SUCCESS;
    }

    catch(const std::exception& e) {

        return EXIT_FAILURE;
    }

}

