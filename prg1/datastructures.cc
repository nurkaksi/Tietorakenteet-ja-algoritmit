/**
 * @brief The Datastructures class
 * Tässä tiedostossa on funktioiden toteutukset
 *
 * Name : Sara Nurminen
 * Student ID : 50224946
 * User name : vhsanu
 * E-mail : sara.nurminen@tuni.fi
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
    // Palauttaa tiedon bites_ säiliön alkioiden määrästä
    return bites_.size();
}


void Datastructures::clear_all()
{
  // Tyhjennetään kaikki säiliöt
  bites_.clear();
  contours_.clear();
  coordinates_in_use.clear();

  // tyhjennetään välimuistit ja päivitettään tiedot järjestyksestä

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
    // luodaan vektori suupalojen id:itä varten
    std::vector<BiteID> bite_ids;
    // varataan vektorille sopivasti muistia
    bite_ids.reserve(bites_.size());

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

    // päivitetään tieto lajittelusta
    is_alphabetically_sorted_ = false;
    is_distance_sorted_ = false;
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
    if (bites_.empty()) {
        return {};
    }

    // jos muutoksia ei ole tapahtunut, palautetaan muistissa oleva vektori
    if (is_alphabetically_sorted_) {
        return cached_bites_alphabetically_;
    }

    // jos järjestys ei enää pidä paikkansa, tyhjennetään se
    cached_bites_alphabetically_.clear();
    // varataan sopivasti vektorilla sopivasti tilaa
    cached_bites_alphabetically_.reserve(bites_.size());

    // laitetaan suupalat vektoriin
    for (const auto& [id, info] : bites_) {
        cached_bites_alphabetically_.push_back(id);
    }

    // sortataan aakkosjärjestykseen
    std::sort(cached_bites_alphabetically_.begin(), cached_bites_alphabetically_.end(),
                  [this](const BiteID& a, const BiteID& b) {
                      if (bites_.at(a).name != bites_.at(b).name) {
                          return bites_.at(a).name < bites_.at(b).name;
                      }
                      return a < b;
                  });

    // merkitään että järjestys pitää paikkansa
    is_alphabetically_sorted_ = true;
    return cached_bites_alphabetically_;

}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
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

    std::sort(cached_bites_distance_increasing_.begin(), cached_bites_distance_increasing_.end(),
                  [this](const BiteID& a, const BiteID& b) {
                      const Coord& coord_a = bites_.at(a).coord;
                      const Coord& coord_b = bites_.at(b).coord;
                      // Manhattan-etäisyyden laskeminen
                      long long manhattan_a = std::abs(coord_a.x) + std::abs(coord_a.y);
                      long long manhattan_b = std::abs(coord_b.x) + std::abs(coord_b.y);
                      if (manhattan_a != manhattan_b) {
                          return manhattan_a < manhattan_b;
                      }
                      if (coord_a.y != coord_b.y) {
                          return coord_a.y < coord_b.y;
                      }
                      return a < b;
                  });


    is_distance_sorted_ = true;  // Merkitään välimuisti järjestetyksi
    return cached_bites_distance_increasing_;
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

    // päivitetään tieto lajittelusta
    is_alphabetically_sorted_ = false;
    is_distance_sorted_ = false;
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
Datastructures::all_subcontours_of_contour(ContourID id)
{
    // Tarkistetaan, onko korkeuskäyrä olemassa
    if (contours_.find(id) == contours_.end()) {
        return {NO_CONTOUR};
    }

    // Luodaan vektori johon tallennetaan kaikki alikäyrät
    std::vector<ContourID> all_subcontours;

    // Luodaan "pino" johon lisätään löydetyt käyrät, jotta voidaan tutkia
    // löytyykö niille vielä alikäyriä
    std::vector<ContourID> stack;
    // lisätään annettu korkeuskäyrä ensimmäiseksi pinoon
    stack.push_back(id);

    // silmukka käy pinoa läpi
    while (!stack.empty()) {
        ContourID current_contour = stack.back();
        stack.pop_back();  // Poistetaan nykyinen käsittelyssä oleva käyrä pinosta

        // Käydään läpi kaikki nykyisen käyrän suorat alikäyrät
        for (const auto& subcontour_id : contours_[current_contour].children_ids) {
            all_subcontours.push_back(subcontour_id);  // Lisätään alikäyrä tulosvektoriin
            stack.push_back(subcontour_id);            // Lisätään alikäyrä tutkittavaksi
        }
    }

    return all_subcontours;
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID /*id1*/,
                                                        ContourID /*id2*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_closest_common_ancestor_of_contours");
}

bool Datastructures::remove_bite(BiteID id)
{
    // tarkistetaan onko suupala olemassa
    if (bites_.find(id) == bites_.end()) {
        return false;
    }

    // poistetaan suupala kummastakin säiliöstä
    coordinates_in_use.erase(bites_[id].coord);
    bites_.erase(id);

    // päivitetään tieto lajittelusta
    is_alphabetically_sorted_ = false;
    is_distance_sorted_ = false;

    return true;
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord /*xy*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("get_bites_closest_to");
}
