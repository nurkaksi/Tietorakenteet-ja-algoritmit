/**
 * @brief The Datastructures class
 * STUDENTS: Modify the code below to implement the functionality of the class.
 * Also remove comments from the parameter names when you implement an operation
 * (Commenting out parameter name prevents compiler from warning about unused
 * parameters on operations you haven't yet implemented.)
 */

#include "datastructures.hh"
#include "customtypes.hh"

Datastructures::Datastructures()
{
  // Write any initialization you need here
}

Datastructures::~Datastructures()
{
  // Write any cleanup you need here
}


unsigned int Datastructures::getbite_count()
{
    // Returns the amout of bites in the container
    return bites_.size();
}


void Datastructures::clear_all()
{
  // Clears the containers from all bites and contours
  bites_.clear();
  contours_.clear();
}

std::vector<BiteID> Datastructures::all_bites()
{
    // Using vector as a container for bite-ids
    std::vector<BiteID> bite_ids;

    for ( const auto& bite : bites_ ) {
        bite_ids.push_back(bite.first);
    }

    return bite_ids;
}


bool Datastructures::add_bite(BiteID id, Name name, Coord xy)
{
  // Return false if the bite already excists
  if (bites_.find(id) != bites_.end()) {
      return false;
  }
  // Return false if there is already a bite in the coordinates
  for ( const auto& pair : bites_ ) {
      if ( pair.second.coord == xy ) {
          return false;
      }
  }

  // Add new bite to the container and return true
  bites_[id]=Bite{name, xy};
  return true;
}


Name Datastructures::get_bite_name(BiteID id)
{
  // If there is a bite with the given ID, return the name
  if (bites_.find(id) != bites_.end()) {
      return bites_[id].name;
  }
  // If bite was not found, return NO_NAME
  return NO_NAME;
}


Coord Datastructures::get_bite_coord(BiteID id)
// Finds bite's coordinates by its ID
{
  // If there is a bite with given coordinates, return coordinates
  if (bites_.find(id) != bites_.end()) {
      return bites_[id].coord;
  }
  // If the bite was not found, return NO_COORD
  return NO_COORD;
}


std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
  std::vector<BiteID> bite_ids;

  // Reserving enough memory for the vector
  bite_ids.reserve(bites_.size());

  // Push bites into the vector
  for (const auto& [id, bite] : bites_) {
      bite_ids.push_back(id);
  }

  // sort the vector by using a lambda function
  std::sort(bite_ids.begin(), bite_ids.end(), [this](const BiteID& id1, const BiteID& id2) {
      return bites_.at(id1).name < bites_.at(id2).name;
  });

  return bite_ids;
}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bites_distance_increasing");
}

BiteID Datastructures::find_bite_with_coord(Coord xy)
// Finds bite by its coordinates
{
    // goes through bites_ container and compares given
    // coordinates with every bite's coordinates
    for (const auto& [id, bite] : bites_) {
        if (bite.coord == xy) {
            return id;
        }
    }
    // return NO_BITE if bite with given coordinates was not found
    return NO_BITE;
}

bool Datastructures::change_bite_coord(BiteID /*id*/, Coord /*newcoord*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("change_bite_coord");
}

bool Datastructures::add_contour(ContourID /*id*/, const Name & /*name*/, ContourHeight /*height*/,
                                 std::vector<Coord> /*coords*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_contour");
}

std::vector<ContourID> Datastructures::all_contours()
{
  // Replace the line below with your implementation
  throw NotImplemented("all_contours");
}

Name Datastructures::get_contour_name(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_contour_name");
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_contour_coords");
}

ContourHeight Datastructures::get_contour_height(ContourID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_contour_height");
}

bool Datastructures::add_subcontour_to_contour(ContourID /*id*/,
                                               ContourID /*parentid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_subcontour_to_contour");
}

bool Datastructures::add_bite_to_contour(BiteID /*biteid*/, ContourID /*contourid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("add_bite_to_contour");
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bite_in_contours");
}

std::vector<ContourID>
Datastructures::all_subcontours_of_contour(ContourID /*id*/)
{
  throw NotImplemented("all_subcontours_of_contour");
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID /*id1*/,
                                                        ContourID /*id2*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_closest_common_ancestor_of_contours");
}

bool Datastructures::remove_bite(BiteID /*id*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("remove_bite");
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bites_closest_to");
}
