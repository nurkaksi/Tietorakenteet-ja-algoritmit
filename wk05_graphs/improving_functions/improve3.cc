#include <iterator>
#include <vector>
#include <map>
#include <algorithm>


/**
 * @brief Sums up the values of the vector cumulatively, storing cumulative sum
 *        of the vector in a map, where the keys are the unique vector items,
 *        and the value is the last cumulative sum when that value was last
 *        encountered. (example: {4, 5, 4, 6} => {{4, 13}, {5, 9}, {6, 19}}).
 *
 * @param vec vector which is used for the sum operation.
 * @return std::map<int, int> sums - the sums of each cumulation stored in
 *         a map as values, vector values used as keys.
 */
std::map<int, int> cumulativeSums(const std::vector<int> v) {
    // tuonne parametreihin laitettu const niin vektoria ei
    // tarvitse kopioida montaa kertaa

    std::map<int,int> sums;
    // luodaan apumuuttuja joka ylläpitää summaa niin sitä ei
    // tarvitse laskea jokaisella for loopin kierroksella
    int cumulativesum = v[0];

    // lisätään ensimmäinen alkio semmoisenaan, ei tarvitse
    // joka välissä tarkistaa onko sums tyhjä
    sums[v[0]] = v[0];
    for (unsigned int i=1; i<v.size(); ++i) {

       // poistettu operaatio .at, koska se on turha.
       // aloitetaan täällä toisesta elementistä
       cumulativesum += v[i];
       sums[v[i]] = cumulativesum;
    }
    return sums;
}
