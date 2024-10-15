#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH 

#include "customtypes.hh"
#include <utility>
#include <vector>

#include <unordered_map>
#include <string>
#include <iostream>
#include <algorithm>


using namespace std;
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




  unsigned int get_bite_count();




  void clear_all();




  std::vector<BiteID> all_bites();




  bool add_bite(BiteID id, const Name & name, Coord xy);




  Name get_bite_name(BiteID id);




  Coord get_bite_coord(BiteID id);







  std::vector<BiteID> get_bites_alphabetically();




  std::vector<BiteID> get_bites_distance_increasing();



  BiteID find_bite_with_coord(Coord xy);




  bool change_bite_coord(BiteID id, Coord newcoord);




  bool add_contour(ContourID id, const Name & name, ContourHeight height,
                   std::vector<Coord> coords);




  std::vector<ContourID> all_contours();




  Name get_contour_name(ContourID id);



  std::vector<Coord> get_contour_coords(ContourID id);




  ContourHeight get_contour_height(ContourID id);



  bool add_subcontour_to_contour(ContourID id, ContourID parentid);



  bool add_bite_to_contour(BiteID biteid, ContourID parentid);




  std::vector<ContourID> get_bite_in_contours(BiteID id);





  std::vector<ContourID> all_subcontours_of_contour(ContourID );



  std::vector<BiteID> get_bites_closest_to(Coord );



  bool remove_bite(BiteID );



  ContourID get_closest_common_ancestor_of_contours(ContourID ,
                                                    ContourID );

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
      vector<Coord> coords;
      ContourID parentid = -1;
      vector<ContourID> children_ids;
  };

  unordered_map<BiteID, Bite> bites_;

  unordered_map<ContourID, Contour> contours_;

  unordered_map<Coord, BiteID> coordinates_in_use;

    vector<BiteID> cached_bites_alphabetically_;
    vector<BiteID> cached_bites_distance_increasing_;
    bool is_alphabetically_sorted_ = false;
    bool is_distance_sorted_ = false;
};

#endif
