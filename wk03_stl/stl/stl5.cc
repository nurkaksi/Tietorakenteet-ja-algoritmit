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
        auto divisibleBy3 = std::stable_partition(v.begin(), v.end(),
            [](int x) { return x % 3 == 0; });

        // Ne, joiden jakojäännös on 1
        auto remainder1 = std::stable_partition(divisibleBy3, v.end(),
            [](int x) { return x % 3 == 1; });

        // Nyt järjestetään jokainen osa nousevaan järjestykseen
        sort(v.begin(), divisibleBy3); // Jaolliset kolmella
        sort(divisibleBy3, remainder1);  // Jakojäännös 1
        sort(remainder1, v.end());     // Jakojäännös 2

        return EXIT_SUCCESS;
    }

    catch (const std::exception& e) {

        return EXIT_FAILURE;
    }
}

