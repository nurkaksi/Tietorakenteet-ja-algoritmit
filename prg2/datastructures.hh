// Datastructures.hh
// AI tool used and the version: ChatGPT 4omini

// The purpose of the use: tehokkuuden parantaminen ja lambda-funktioiden tekeminen

// The part(s) where it was utilized: get_bites_alphabetically, get_bites_distance_increasing
// get_closest_common_ancestor_of_contours

// prg2:ssa käytetty bfs algoritmin toteutukseen path_any funktiossa, sekä
// dijkstran algoritmin toteutukseen path_shortest

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include "customtypes.hh"
#include <utility>
#include <vector>

// Add your own STL includes below this comment
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <queue>

using BiteID = long long;
using ContourID = long long;
using Name = std::string;
using Coord = Coord;
using ContourHeight = int;

template <typename Type>
Type random_in_range(Type start, Type end);


class Datastructures
{
public:
  Datastructures();
  ~Datastructures();

  // Estimate of performance: O(1)
  // Short rationale for estimate: stl:n size-funktion käyttö on vakioaikainen operaatio
  unsigned int get_bite_count();

  // Estimate of performance: O(n)
  // Short rationale for estimate: kaikki alkiot käydään kerran läpi
  void clear_all();

  // Estimate of performance: O(n)
  // Short rationale for estimate: kaikki suupalat käydään kerran läpi
  std::vector<BiteID> all_bites();

  // Estimate of performance: O(1)
  // Short rationale for estimate: kaikki tarkistukset ja itse lisäys ovat vakioaikaisia
  bool add_bite(BiteID id, const Name & name, Coord xy);

  // Estimate of performance: O(1)
  // Short rationale for estimate: tarkistus ja nimen palauttaminen ovat vakioaikaisia
  Name get_bite_name(BiteID id);

  // Estimate of performance: O(1)
  // Short rationale for estimate: tarkistus ja sijainnin palautus ovat vakioaikaisia
  Coord get_bite_coord(BiteID id);

  // We recommend you implement the operations below only after implementing the
  // ones above

  // Estimate of performance: O(n*log(n))
  // Short rationale for estimate: lajittelu vie aikaa O(n*log(n))
  std::vector<BiteID> get_bites_alphabetically();

  // Estimate of performance: O(n*log(n))
  // Short rationale for estimate: jälleen joudutaan lajittelemaan joka on O(nlogn)
  std::vector<BiteID> get_bites_distance_increasing();

  // Estimate of performance: O(n)
  // Short rationale for estimate: kaikki suupalat käydään läpi, joten operaatio on lineaarinen
  BiteID find_bite_with_coord(Coord xy);

  // Estimate of performance: O(n)
  // Short rationale for estimate: find-operaatio on lineaarinen, muu vakioaikaista
  bool change_bite_coord(BiteID id, Coord newcoord);

  // Estimate of performance: O(n)
  // Short rationale for estimate: kopioiminen on lineaarinen operaatio, muu vakioaikaista
  bool add_contour(ContourID id, const Name & name, ContourHeight height,
                   std::vector<Coord> coords);

  // Estimate of performance: O(n)
  // Short rationale for estimate: silmukka käy kaiken läpi (lineaarinen),
  // push_back() on vakioaikainen
  std::vector<ContourID> all_contours();

  // Estimate of performance: O(1)
  // Short rationale for estimate: hajautustaulusta haku on vakioaikaista
  Name get_contour_name(ContourID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate: find-operaatio on lineaariaikainen
  std::vector<Coord> get_contour_coords(ContourID id);

  // Estimate of performance: O(1)
  // Short rationale for estimate: hajautustaulusta haku on vakioaikaista
  ContourHeight get_contour_height(ContourID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate: find-operaatio on lineaariaikainen, muu vakioaikaista
  bool add_subcontour_to_contour(ContourID id, ContourID parentid);

  // Estimate of performance: O(n)
  // Short rationale for estimate: find-operaatio on lineaariaikainen, muu vakioaikaista
  bool add_bite_to_contour(BiteID biteid, ContourID parentid);

  // Estimate of performance: O(n)
  // Short rationale for estimate: pahimmassa tapauksessa käydään kaikki korkeuskäyrät läpi,
  // muu vakioaikaista
  std::vector<ContourID> get_bite_in_contours(BiteID id);

  // Non-compulsory operations

  // Estimate of performance: O(n)
  // Short rationale for estimate: pahimmassa tapauksessa silmukka käy jokaisen korkeuskäyrän
  std::vector<ContourID> all_subcontours_of_contour(ContourID id);

  // Estimate of performance: O(n*log(n))
  // Short rationale for estimate: järjestäminen on O(nlogn),
  // muu vakioaikaista
  std::vector<BiteID> get_bites_closest_to(Coord /*xy*/);

  // Estimate of performance: O(1)
  // Short rationale for estimate: molemmat haut ja poistot ovat vakioaikaisia hajautustaulusta
  bool remove_bite(BiteID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate: pahimmassa tapauksessa käydään jokainen käyrä läpi
  ContourID get_closest_common_ancestor_of_contours(ContourID id1,
                                                    ContourID id2);



 /**
   * ============================
   * ============================
   * ========== PRG2 ============
   * ============================
   * ============================
   */

  // Estimate of performance: O(n)
  // Short rationale for estimate: n on koordinaattioen määrä ja ne täytyy käsitellä erikseen. Lisäys vakioaikasita
  bool add_connection(ConnectionID connectionid, BiteID id1, BiteID id2,
                      std::vector<Coord> xy);

  // Estimate of performance: O(n)
  // Short rationale for estimate: n on yhteyksien määrä. unordered mapin tyhjennys on lineaarista
  void clear_connections();

  // Estimate of performance: O(n)
  // Short rationale for estimate: n on suupalan yhteyksien määrä. yksittäisen käsittely O(1)
  std::vector<BiteID> get_next_bites_from(BiteID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate: n yhteyksien määrä, yhteydet käydään läpi kun etsitään id:iden välistä yhteyttä
  std::vector<ConnectionID> get_connections(BiteID id1 = NO_BITE, BiteID id2 = NO_BITE);

  // Estimate of performance: O(n)
  // Short rationale for estimate: n on yhteyden koordinaattien määrä
  std::vector<Coord> get_connection_coords(BiteID biteid,
                                           ConnectionID connectionid);

  // Estimate of performance: O(n^2)
  // Short rationale for estimate: pahimmassa tapauksessa yhteyksiä ja koordinaatteja käsitellään n-kertaa
  // ja jokainen saattaa vaatia O(n) käsittelyn
  std::vector<std::pair<Coord, Distance>> path_any(BiteID fromid, BiteID toid);

  // Non-compulsory operations

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_shortest(BiteID /*fromid*/,
                                                        BiteID /*toid*/);

  // Estimate of performance: O(n^2)
  // Short rationale for estimate: n on suupalojen määrä
  std::vector<std::pair<Coord, Distance>> path_least_bites(BiteID fromid,
                                                           BiteID toid);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_least_uphill(BiteID /*fromid*/,
                                                            BiteID /*toid*/);



private:

  struct Bite {
      Name name;
      Coord coord;
      ContourID bites_contour = -1;
      bool incontour = false;
  };

  struct Contour {
      Name name;
      ContourHeight height;
      std::vector<Coord> coords;
      ContourID parentid = -1;
      std::vector<ContourID> children_ids;
  };
  // säiliö suupaloille
  std::unordered_map<BiteID, Bite> bites_;

  // säiliö korkeuskäyrille
  std::unordered_map<ContourID, Contour> contours_;

  // säiliö suupaloille koordinaattien perusteella
  // (nopeuttaa hakua koordinaatin perusteella ja pitää kirjaa käytetyistä koordinaateista)
  std::unordered_map<Coord, BiteID> coordinates_in_use;

  std::vector<BiteID> cached_bites_alphabetically_;
  std::vector<BiteID> cached_bites_distance_increasing_;
  bool is_alphabetically_sorted_ = false;
  bool is_distance_sorted_ = false;

  /// projekti 2
  ///

  struct Connection {
      BiteID start_bite;
      BiteID end_bite;
      std::vector<Coord> path_coords;
      Distance lenght;

  };

  std::unordered_map<ConnectionID, Connection> connections_;

  // navigointia helpottava rakenne:
  // jokaisesta suupalasta lähtevät yhteydet muihin suupaloihin
  std::unordered_map<BiteID, std::unordered_map<BiteID, ConnectionID>> bite_connections_;
};

#endif // DATASTRUCTURES_HH
