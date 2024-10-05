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


unsigned int Datastructures::get_bite_count()
{
    // Returns the amout of bites in the container
    return bites_.size();
}


void Datastructures::clear_all()
{
  // Clears the containers from all bites and contours
  bites_.clear();
  contours_.clear();
  coordinates_in_use.clear();
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


bool Datastructures::add_bite(BiteID id, const Name &name, Coord xy)
{
    // Jos suupala on jo olemassa tai koordinaatti on jo käytössä, palautetaan false
    if (bites_.find(id) != bites_.end() || coordinates_in_use.find(xy) != coordinates_in_use.end()) {
        return false;
    }

    // lisätään suupala (
    bites_[id] = Bite{name, xy};
    coordinates_in_use[xy] = id;
    return true;
}


Name Datastructures::get_bite_name(BiteID id)
{
  // Etsitään suupala säiliöstä ja palautetaan sen nimi
  if (bites_.find(id) != bites_.end()) {
      return bites_[id].name;
  }
  // jos suupalaa ei löydy, palautetaan NO_NAME
  return NO_NAME;
}


Coord Datastructures::get_bite_coord(BiteID id)
// Etsii suupalan koordinaatit
{
  // Etsitään suupala ja palautetaan sen koordinaatit
  if (bites_.find(id) != bites_.end()) {
      return bites_[id].coord;
  }
  // Jos suupalaa ei ole, palautetaan NO_COORD
  return NO_COORD;
}


std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
  std::vector<BiteID> bite_ids;

  // Varataan tilaa vektorille
  bite_ids.reserve(bites_.size());

  // Lisätään suupalaut vektoriin
  for (const auto& [id, bite] : bites_) {
      bite_ids.push_back(id);
  }

  // Sortataan vektori aakkosjärjestykseen, jos nimet ovat samat, sortataan id:llä
  std::sort(bite_ids.begin(), bite_ids.end(), [this](const BiteID& id1, const BiteID& id2) {
      const auto& bite1 = bites_.at(id1);
      const auto& bite2 = bites_.at(id2);

      // Ensin vertaillaan nimiä
      if (bite1.name != bite2.name) {
          return bite1.name < bite2.name;
      }

      // Jos nimet ovat samat, vertaillaan id:tä
      return id1 < id2;
  });

  return bite_ids;
}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
    std::vector<BiteID> bite_ids;
    bite_ids.reserve(bites_.size());

    // Kopioidaan kaikki suupalau vektoriin
    for (const auto& pair : bites_) {
        bite_ids.push_back(pair.first);
    }

    // Sortataan manhattan etäisyyden perusteella, käytämällä lambda-funktiota
    std::sort(bite_ids.begin(), bite_ids.end(), [this](const BiteID& id1, const BiteID& id2) {
        const auto& coord1 = bites_.at(id1).coord;
        const auto& coord2 = bites_.at(id2).coord;

        int distance1 = std::abs(coord1.x) + std::abs(coord1.y);
        int distance2 = std::abs(coord2.x) + std::abs(coord2.y);

        // Ensisijaisesti vertaillaan Manhattan-etäisyyden mukaan
        if (distance1 != distance2) {
            return distance1 < distance2;
        }
        // Jos manhattan, etäisyydet ovat samat, sortataan y:n perusteella
        else if (coord1.y != coord2.y) {
            return coord1.y < coord2.y;
        }
        // Jos y:t samat, sortataan id:n perusteella
        else {
            return id1 < id2;
        }

    });

    return bite_ids;
}


BiteID Datastructures::find_bite_with_coord(Coord xy)
// Etsitään suupala sen koordinaattien perusteella
{
    // Etsitään koordinaatti kartasta
    auto it = coordinates_in_use.find(xy);

    // Jos koordinaatti löytyy
    if (it != coordinates_in_use.end()) {
        return it->second;  // Palautetaan löydetty BiteID
    }

    // Jos koordinaattia ei löytynyt, palautetaan NO_BITE
    return NO_BITE;
}

bool Datastructures::change_bite_coord(BiteID id, Coord newcoord)
{
    // jos suupalaa ei löydy, palautetaan false
    if (bites_.find(id) == bites_.end()) {
        return false;
    }
    // jos koordinaatissa on jo suupala, palautetaan false
    if (coordinates_in_use.find(newcoord) != coordinates_in_use.end()) {
        return false;
    }
    const auto& contour_id = bites_[id].bites_contour;
    // Tarkistetaan onko suupala jo lisätty korkeuskäyrään
    if ( contour_id != -1 ) {

        // Jos uusi koordinaatti ei ole korkeuskäyrän alueella, palautetaan false
        if (std::find(contours_[contour_id].coords.begin(),
                      contours_[contour_id].coords.end(),
                      newcoord) != contours_[contour_id].coords.end()) {
            return false;
        }
    }

    // Vaihdetaan uusi koordinaatti tietoihin
    coordinates_in_use.erase(bites_[id].coord);
    coordinates_in_use[newcoord] = id;
    bites_[id].coord = newcoord;

    return true;

}

bool Datastructures::add_contour(ContourID id, const Name & name, ContourHeight height,
                                 std::vector<Coord> coords)
{
    // Palautetaan false jos annettu id on jo olemassa
    if ( contours_.find(id) != contours_.end() ) {
        return false;
    }
    // Palautetaan false jos annettu korkeus on liian suuri
    if ( height > MAX_CONTOUR_HEIGHT or height < -MAX_CONTOUR_HEIGHT) {
        return false;
    }
    // Palautetaan false jos annettu vektori on tyhjä
    if ( coords.empty() ) {
        return false;
    }

    // Lisätään uusi korkeuskäyrä säiliöön
    contours_[id] = Contour{name, height, coords, -1, {}, {}};

    return true;
}

std::vector<ContourID> Datastructures::all_contours()
{
    // Tallennetaan korkeuskäyrien id:t vektoriin
    std::vector<ContourID> contour_ids;

    for ( const auto& contour : contours_ ) {
        contour_ids.push_back(contour.first);
    }

    return contour_ids;
}

Name Datastructures::get_contour_name(ContourID id)
{
    // Jos annetulla id:llä ei löydy korkeuskäyrää, palautetaan NO_NAME
    if ( contours_.find(id) == contours_.end() ) {
        return NO_NAME;
    }

    // Return contours height
    return contours_[id].name;
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID id)
{
    // Jos annetulla id:llä ei löydy korkeuskäyrää, palautetaan NO_COORD
    if ( contours_.find(id) == contours_.end() ) {
        return {NO_COORD};
    }
    // palautetaan korkeuskäyrän koodinaattivektori
    return contours_[id].coords;
}


ContourHeight Datastructures::get_contour_height(ContourID id)
{
    // Tarkistetaan löytyykö annetulla id:llä korkeuskäyrää
    if ( contours_.find(id) == contours_.end() ) {
        return NO_CONTOUR_HEIGHT;
    }
    // palautetaan korkeus
    return contours_[id].height;
}

bool Datastructures::add_subcontour_to_contour(ContourID id,
                                               ContourID parentid)
{
    // Tarkistetaan, että molemmat annetut korkeuskäyrät löytyvät säiliöstä
    // Jos ei löydy, palautetaan false
    if (contours_.find(id) == contours_.end() || contours_.find(parentid) == contours_.end()) {
        return false;
    }


    Contour& subcontour = contours_.at(id);
    Contour& parentcontour = contours_.at(parentid);

    // Jos alikäyrällä on jo vanhempi, palautetaan false
    if (subcontour.parentid != -1) {
        return false;
    }

    // Tarkistetaan, että alikäyrän korkeus on
    // itseisarvoltaan suurempi kuin vanhemman
    if (std::abs(subcontour.height) <= std::abs(parentcontour.height)) {
        return false;
    }

    // Palautetaan false jos korkeusero ei ole yksi
    if (std::abs(subcontour.height - parentcontour.height) != 1) {
        return false;
    }

    // Jos kaikki ehdot täyttyvät, asetetaan alikäyrän parent_id ja lisätään alikäyrä vanhemman lapsiksi
    subcontour.parentid = parentid;
    parentcontour.children_ids.push_back(id);

    return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteid, ContourID contourid)
{
    // jos suupalaa tai korkeuskäyrää ei löydy, palautetaan false
    if (bites_.find(biteid) == bites_.end() || contours_.find(contourid) == contours_.end()) {
           return false;
       }

    // jos suupala on jo lisätty johonkin korkeuskäyrään, palautetaan false
    for ( const auto& [id, info] : contours_ ) {
        if ( std::find(info.bites_in_contour.begin(), info.bites_in_contour.end(),
                      biteid) != info.bites_in_contour.end() ) {
            return false;
        }
    }

    for ( const auto& [id, info] : contours_ ) {
        if (std::find(info.coords.begin(), info.coords.end(),
                      bites_[biteid].coord) == info.coords.end()) {
            return false;
        }

    }

    // Lisätään suupala korkeuskäyrän tietoihin ja korkeuskäyrä suupalan tietoihin
    contours_[contourid].bites_in_contour.push_back(biteid);
    bites_[biteid].bites_contour = contourid;
    return true;
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID id)
{
    // palautetaan NO_CONTOUR jos suupalaa ei löydy
    if ( bites_.find(id) == bites_.end() ) {
        return {NO_CONTOUR};
    }

    // Palautetaan tyhjä vektori jos suupala ei kuulu mihinkään korkeuskäyrään
    if ( bites_.find(id) != bites_.end() ) {
        if ( bites_[id].bites_contour == -1 ) {
            return {};
        }
    }

    // luodaan vektori johon tallennetaan kaikki korkeuskäyrät joihin
    // suupala kuuluu suorasti tai epäsuorasti
    std::vector<ContourID> all_contours;

    // etsitään mille korkeuskäyrälle suupala on tallennettu ja lähdetään etenemään siitä
    ContourID current_contour = bites_[id].bites_contour;

    // Silmukka etsii suupalan korkeuskäyrän kaikki vanhemmat
    while (current_contour != -1) {
        all_contours.push_back(current_contour);

        // siirrytään ylempään vanhempaan
        current_contour = contours_[current_contour].parentid;
    }

    return all_contours;

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
