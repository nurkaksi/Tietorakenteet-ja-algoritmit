#include "datastructures.hh"
#include "customtypes.hh"

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_bite_count()
{


       return bites_.size();
}

void Datastructures::clear_all()
{

       bites_.clear();
       contours_.clear();
       coordinates_in_use.clear();




       cached_bites_alphabetically_.clear();
       cached_bites_distance_increasing_.clear();

       is_alphabetically_sorted_ = false;
       is_distance_sorted_ = false;
}

std::vector<BiteID> Datastructures::all_bites()
{
    if (bites_.empty()) {
            return {};
    }


    vector<BiteID> bite_ids;

    bite_ids.reserve(bites_.size());

    for ( const auto& bite : bites_ ) {
        bite_ids.push_back(bite.first);
    }

    return bite_ids;
}

bool Datastructures::add_bite(BiteID id, const Name &name, Coord xy)
{


    if (bites_.find(id) != bites_.end()
                || coordinates_in_use.find(xy) != coordinates_in_use.end()) {
            return false;
        }


    bites_.emplace(id, Bite{name, xy});
    coordinates_in_use.emplace(xy, id);


        is_alphabetically_sorted_ = false;
        is_distance_sorted_ = false;

        return true;

}

Name Datastructures::get_bite_name(BiteID id)
{

      if (bites_.find(id) != bites_.end()) {
          return bites_[id].name;
      }

      return NO_NAME;

}

Coord Datastructures::get_bite_coord(BiteID id)
{

  if (bites_.find(id) != bites_.end()) {
      return bites_[id].coord;
  }

  return NO_COORD;
}

std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
    if (bites_.empty()) {
            return {};
        }



        if (is_alphabetically_sorted_) {
            return cached_bites_alphabetically_;
        }



        cached_bites_alphabetically_.clear();

        cached_bites_alphabetically_.reserve(bites_.size());


        for (const auto& [id, info] : bites_) {
            cached_bites_alphabetically_.push_back(id);
        }


        sort(cached_bites_alphabetically_.begin(), cached_bites_alphabetically_.end(),
                      [this](const BiteID& a, const BiteID& b) {
                          if (bites_.at(a).name != bites_.at(b).name) {
                              return bites_.at(a).name < bites_.at(b).name;
                          }
                          return a < b;
                      });


        is_alphabetically_sorted_ = true;
        return cached_bites_alphabetically_;

    }


vector<BiteID> Datastructures::get_bites_distance_increasing()
{
    if (bites_.empty()) {
        return {};
    }

    if (is_distance_sorted_) {
        return cached_bites_distance_increasing_;
    }

    cached_bites_distance_increasing_.clear();
    cached_bites_distance_increasing_.reserve(bites_.size());

    for (const auto& [id, info] : bites_) {
        cached_bites_distance_increasing_.push_back(id);
    }

    sort(cached_bites_distance_increasing_.begin(), cached_bites_distance_increasing_.end(),
                  [this](const BiteID& a, const BiteID& b) {
                      const Coord& coord_a = bites_.at(a).coord;
                      const Coord& coord_b = bites_.at(b).coord;

                      long long manhattan_a = abs(coord_a.x) + abs(coord_a.y);
                      long long manhattan_b = abs(coord_b.x) + abs(coord_b.y);
                      if (manhattan_a != manhattan_b) {
                          return manhattan_a < manhattan_b;
                      }
                      if (coord_a.y != coord_b.y) {
                          return coord_a.y < coord_b.y;
                      }
                      return a < b;
                  });


    is_distance_sorted_ = true;
    return cached_bites_distance_increasing_;
}



BiteID Datastructures::find_bite_with_coord(Coord xy)
{

    auto it = coordinates_in_use.find(xy);



    if (it != coordinates_in_use.end()) {
        return it->second;
    }


    return NO_BITE;
}

bool Datastructures::change_bite_coord(BiteID id, Coord newcoord)
{

    if (bites_.find(id) == bites_.end()) {
        return false;
    }

    if (coordinates_in_use.find(newcoord) != coordinates_in_use.end()) {
        return false;
    }
    const auto& contour_id = bites_[id].bites_contour;

    if ( contour_id != -1 ) {



        if (std::find(contours_[contour_id].coords.begin(),
                      contours_[contour_id].coords.end(),
                      newcoord) != contours_[contour_id].coords.end()) {
            return false;
        }
    }


    coordinates_in_use.erase(bites_[id].coord);
    coordinates_in_use[newcoord] = id;
    bites_[id].coord = newcoord;


    is_alphabetically_sorted_ = false;
    is_distance_sorted_ = false;
    return true;

}

bool Datastructures::add_contour(ContourID id, const Name & name, ContourHeight height,
                                 vector<Coord> coords)
{


    if ( contours_.find(id) != contours_.end() ) {
        return false;
    }


    if ( height > MAX_CONTOUR_HEIGHT or height < -MAX_CONTOUR_HEIGHT) {
        return false;
    }


    if ( coords.empty() ) {
        return false;
    }


    contours_.emplace(id, Contour{name, height, coords, -1, {}});
    return true;
}

vector<ContourID> Datastructures::all_contours()
{

    vector<ContourID> contour_ids;

    contour_ids.reserve(contours_.size());



    for ( const auto& contour : contours_ ) {
        contour_ids.push_back(contour.first);
    }

    return contour_ids;
}

Name Datastructures::get_contour_name(ContourID id)
{


    if ( contours_.find(id) == contours_.end() ) {
        return NO_NAME;
    }
    return contours_[id].name;
}

vector<Coord> Datastructures::get_contour_coords(ContourID id)
{


    if ( contours_.find(id) == contours_.end() ) {
        return {NO_COORD};
    }

    return contours_[id].coords;
}
ContourHeight Datastructures::get_contour_height(ContourID id)
{

    if ( contours_.find(id) == contours_.end() ) {
        return NO_CONTOUR_HEIGHT;
    }

    return contours_[id].height;
}
bool Datastructures::add_subcontour_to_contour(ContourID id,
                                               ContourID parentid)
{


    if (contours_.find(id) == contours_.end() || contours_.find(parentid) == contours_.end()) {
        return false;
    }

    Contour& subcontour = contours_.at(id);
    Contour& parentcontour = contours_.at(parentid);


    if (subcontour.parentid != -1) {
        return false;
    }



    if (std::abs(subcontour.height) <= std::abs(parentcontour.height)) {
        return false;
    }



    if (std::abs(subcontour.height - parentcontour.height) != 1) {
        return false;
    }


    contours_[id].parentid = parentid;

    contours_[parentid].children_ids.push_back(id);

    return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteid, ContourID contourid)
{

    if ( bites_.find(biteid) == bites_.end() ) {
        return false;
    }


    if ( contours_.find(contourid) == contours_.end() ) {
        return false;
    }


    if ( bites_[biteid].incontour == true ) {
        return false;
    }



    if (std::find(contours_[contourid].coords.begin(), contours_[contourid].coords.end(),
                  bites_[biteid].coord) == contours_[contourid].coords.end()) {
        return false;
    }

    bites_[biteid].incontour = true;
    bites_[biteid].bites_contour = contourid;
    return true;

}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID id)
{


    if ( bites_.find(id) == bites_.end() ) {
        return {NO_CONTOUR};
    }



    if ( bites_.find(id) != bites_.end() ) {
        if ( bites_[id].bites_contour == -1 ) {
            return {};
        }
    }

    vector<ContourID> all_contours;



    ContourID current_contour = bites_[id].bites_contour;


    while (current_contour != -1) {
        all_contours.push_back(current_contour);


        current_contour = contours_[current_contour].parentid;
    }

    return all_contours;

}

std::vector<ContourID>
Datastructures::all_subcontours_of_contour(ContourID )
{
  throw NotImplemented("all_subcontours_of_contour");
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID ,
                                                        ContourID )
{

  throw NotImplemented("get_closest_common_ancestor_of_contours");
}

bool Datastructures::remove_bite(BiteID )
{

  throw NotImplemented("remove_bite");
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord )
{

  throw NotImplemented("get_bites_closest_to");
}
