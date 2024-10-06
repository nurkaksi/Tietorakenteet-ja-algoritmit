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
    if (bites_.find(id) != bites_.end()
            || coordinates_in_use.find(xy) != coordinates_in_use.end()) {
        return false;
    }

    // lisätään suupala (
    bites_.emplace(id, Bite{name, xy});
    coordinates_in_use.emplace(xy, id);
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

    // Luodaan vektori suupalan id:ille ja nimille
    std::vector<std::pair<BiteID, Name>> bite_pair;
    // varataan sopivasti muistia vektoria varten
    bite_pair.reserve(bites_.size());

    for ( const auto& [id, info] : bites_ ) {
        bite_pair.emplace_back(id, info.name);
    }

    // vertaillaan nimien perusteella
    auto compare_name = [](const std::pair<BiteID, Name>& a,
                           const std::pair<BiteID, Name>& b)
                           -> bool {

        // jos kaksi samaa nimeä, vertaillaan id:itä
        if ( a.second == b.second ) {
            return a.first < b.first;
        }

        return a.second < b.second;

    };

    // sortataan nimen perusteella ja laitetaan vektoriin
    std::sort(bite_pair.begin(), bite_pair.end(), compare_name);

    std::vector<BiteID> sorted_ids_;
    for ( const auto& pair : bite_pair ) {
        sorted_ids_.push_back(pair.first);
    }

    return sorted_ids_;


}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
    // Luodaan vektori suupalan id:itä ja koordinaatteja varten
    std::vector<std::pair<BiteID, Coord>> bite_pair;
    // varataan vektorille sopivasti muistia
    bite_pair.reserve(bites_.size());

    // käydään bites_ säiliö läpi ja laitetaan tiedot luotuun vektoriin
    for (const auto& bite : bites_) {
        bite_pair.push_back({bite.first, bite.second.coord});
    }

    // Vertaillaan alkioita lambda-funktion avulla
    auto compare_coordinates = [](const std::pair<BiteID, Coord>& a,
                                  const std::pair<BiteID, Coord>& b) -> bool {

        const auto& coord_a = a.second;
        const auto& coord_b = b.second;

        // Lasketaan manhattan-etäisyys
        int manhattan_a = std::abs(coord_a.x) + std::abs(coord_a.y);
        int manhattan_b = std::abs(coord_b.x) + std::abs(coord_b.y);

        // Vertaillaan manhattan-etöisyyksiä
        if (manhattan_a != manhattan_b) {
            return manhattan_a < manhattan_b;
        }
        // Vertaillaan y-koordinaatteja
        if (coord_a.y != coord_b.y) {
            return coord_a.y < coord_b.y;
        }
        // Vertaillaan id:itä
        return a.first < b.first;
    };

    // Käytetään ylläolevaa lambda-funktiota vertailuun
    std::sort(bite_pair.begin(), bite_pair.end(), compare_coordinates);

    // Luodaan vektori järjestetyille suupaloille ja laitetaan ne järjestyksessä sinne
    std::vector<BiteID> sorted_ids_;

    for (const auto& pair : bite_pair) {
        sorted_ids_.push_back(pair.first);
    }

    // Return the sorted vector of BiteIDs
    return sorted_ids_;
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
    // Palautetaan false jos annettu korkeus on itseisarvoltaan liian suuri
    if ( height > MAX_CONTOUR_HEIGHT or height < -MAX_CONTOUR_HEIGHT) {
        return false;
    }
    // Palautetaan false jos annettu vektori on tyhjä
    if ( coords.empty() ) {
        return false;
    }

    // Lisätään uusi korkeuskäyrä säiliöön
    contours_.emplace(id, Contour{name, height, coords, -1, {}});
    return true;
}

std::vector<ContourID> Datastructures::all_contours()
{
    // Tallennetaan korkeuskäyrien id:t vektoriin
    std::vector<ContourID> contour_ids;
    // varataan vektorille sopivasti muistia
    contour_ids.reserve(contours_.size());

    // käydään contours_-map läpi ja laitetaan sieltä id:t vektoriin
    for ( const auto& contour : contours_ ) {
        contour_ids.push_back(contour.first);
    }
    // palautetaan vektori jossa on kaikkien korkeuskäyrien id:t
    return contour_ids;
}

Name Datastructures::get_contour_name(ContourID id)
{
    // Jos annetulla id:llä ei löydy korkeuskäyrää, palautetaan NO_NAME
    if ( contours_.find(id) == contours_.end() ) {
        return NO_NAME;
    }
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

    // Lisätään parentid id:n vanhemmaksi
    contours_[id].parentid = parentid;
    // lisätään id parentid:n children_ids vektoriin
    contours_[parentid].children_ids.push_back(id);

    return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteid, ContourID contourid)
{
    // Jos suupalaa ei ole, palautetaan false
    if ( bites_.find(biteid) == bites_.end() ) {
        return false;
    }

    // Jos korkeuskäyrää ei ole, palautetan false
    if ( contours_.find(contourid) == contours_.end() ) {
        return false;
    }

    // Jos suupala on jo jollakin käyrällä, palautetaan false
    if ( bites_[biteid].incontour == true ) {
        return false;
    }

    // Jos suupalan koordinaatit ei ole käyrän alueella, palautetaam false
    if (std::find(contours_[contourid].coords.begin(), contours_[contourid].coords.end(),
                  bites_[biteid].coord) == contours_[contourid].coords.end()) {
        return false;
    }
    // Lisätään korkeuskäyrä suupalan tietoihin
    bites_[biteid].incontour = true;
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
    // palautetaan vektori kaikista suupalan korkeuskäyristä
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
