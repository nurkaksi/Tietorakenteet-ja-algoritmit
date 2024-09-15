#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh"

using namespace std;


/**
 * @brief Arrange vector in three subsequent sections:
 *        - those divisible by three (asc order)
 *        - those whose reminder is 1 (asc order)
 *        - those whose reminder is 2 (asc order)
 * @param v vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortMod3(std::vector<int>& v)
{
    try {
        auto dividedByThree = partition(v.begin(), v.end(), [] (int x) { return x % 3 == 0;});
        auto reminderOne = partition(dividedByThree, v.end(), [] (int x) { return x % 3 == 1;});

        sort(v.begin(), dividedByThree);
        sort(dividedByThree, reminderOne);
        sort(reminderOne, v.end());

        return EXIT_SUCCESS;
    }

    catch (const std::exception& e) {

        return EXIT_FAILURE;
    }
}

