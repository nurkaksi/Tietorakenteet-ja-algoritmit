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

  connections_.clear();
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
Datastructures::get_closest_common_ancestor_of_contours(ContourID id1,
                                                        ContourID id2)
{
    // jos käyrää ei olemassa, palautetaan NO_CONTOUR
    if (contours_.find(id1) == contours_.end() || contours_.find(id2) == contours_.end()) {
        return NO_CONTOUR;
    }

    // luodaan vektori käyrä 1:n esivanhemmille
    std::unordered_set<ContourID> ancestors1;
    // lisätään käyrä 1: vanhempi vektoriin ja aloitetaan käsittely siitä
    ContourID current_id = contours_[id1].parentid;
    // silmukassa edetään ylemmäs kunnes esivanhempia ei enää ole
    while (current_id != -1) {
        ancestors1.insert(current_id);
        current_id = contours_[current_id].parentid;
    }

    // käydään silmukalla läpi käyrä 2:n esivanhempia vanhemmasta aloittaen
    // ja vertaillaan niitä ancestors1 vektorin alkioihin
    current_id = contours_[id2].parentid;
    while (current_id != -1) {
        if (ancestors1.find(current_id) != ancestors1.end()) {
            return current_id;
        }
        current_id = contours_[current_id].parentid;
    }

    // jos yhteistä esivanhempaa ei löydy, palautetaan NO_CONTOUR
    return NO_CONTOUR;
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

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord xy)
{
    std::vector<std::pair<BiteID, int>> distances;

    // Lasketaan Manhattan-etäisyys jokaiselle suupalalle
    for (const auto& bite : bites_) {
        const Coord& coord = bite.second.coord;
        int distance = std::abs(coord.x - xy.x) + std::abs(coord.y - xy.y);
        distances.push_back({bite.first, distance});
    }

    // Järjestetään suupalat etäisyyden mukaan
    std::sort(distances.begin(), distances.end(), [this](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second < b.second;
        const Coord& coord_a = bites_.at(a.first).coord;
        const Coord& coord_b = bites_.at(b.first).coord;
        if (coord_a.y != coord_b.y) return coord_a.y < coord_b.y;
        return a.first < b.first;
    });

    // Palautetaan enintään kolme lähintä suupalaa
    std::vector<BiteID> result;
    for (size_t i = 0; i < std::min(distances.size(), size_t(3)); ++i) {
        result.push_back(distances[i].first);
    }

    return result;
}


/**
 * ============================
 * ============================
 * ========== PRG2 ============
 * ============================
 * ============================
 */

bool Datastructures::add_connection(ConnectionID connectionid, BiteID id1, BiteID id2,
                                    std::vector<Coord> xy)
{
    // tarkistetaan löytyykö suupalat ja onko yhteys jo olemassa
    if (connections_.find(connectionid) != connections_.end() ||
        bites_.find(id1) == bites_.end() || bites_.find(id2) == bites_.end()) {
        return false;
    }

    // jos yhteyden koordinaattien vektori on tyhjä, tarkistetaan onko koordinaatit vinottain
    if (!xy.empty()) {
        for (size_t i = 1; i < xy.size(); ++i) {
            int x_diff = std::abs(xy[i].x - xy[i-1].x);
            int y_diff = std::abs(xy[i].y - xy[i-1].y);
            if (x_diff > 0 && y_diff > 0) {
                return false;
            }
        }
    }

    // lasketaan kokonaispituus yhteydelle
    Distance total_distance = 0;

    // lisätään alkusuupalan ja ensimmäisen koordinaatin välinen etäisyys
    Coord previous_coord = bites_[id1].coord;
    for (const Coord& coord : xy) {
        total_distance += std::abs(coord.x - previous_coord.x) + std::abs(coord.y - previous_coord.y);
        previous_coord = coord;
    }

    // lisätään viimeisen koordinaatin ja loppusuupalan välinen etäisyys
    total_distance += std::abs(bites_[id2].coord.x - previous_coord.x) +
                      std::abs(bites_[id2].coord.y - previous_coord.y);


    connections_.emplace(connectionid, Connection{id1, id2, xy, total_distance});
    // tallennetaan yhteys molempiin suuntiin
    bite_connections_[id1][id2] = connectionid;
    bite_connections_[id2][id1] = connectionid;

    return true;
}


void Datastructures::clear_connections()
{
  connections_.clear();
  bite_connections_.clear();
}


std::vector<BiteID> Datastructures::get_next_bites_from(BiteID id)
{
    // jos suupalaa ei löydy, palautetaan NO_BITE
    if (bites_.find(id) == bites_.end()) {
        return {NO_BITE};
    }

    // käytetään unordered_setiä uniikkien suupalojen keräämiseen
    std::unordered_set<BiteID> unique_bites;

    // käydään läpi kaikki yhteydet connections_ säiliössä
    for (const auto& [connection_id, connection] : connections_) {
        // jos suupala on yhteyden alku, lisätään sen loppupiste
        if (connection.start_bite == id) {
            unique_bites.insert(connection.end_bite);
        }
        // jos suupala on yhteyden loppupiste, lisätään sen alkupiste
        else if (connection.end_bite == id) {
            unique_bites.insert(connection.start_bite);
        }
    }

    // muutetaan setti vektoriksi
    std::vector<BiteID> connected_bites(unique_bites.begin(), unique_bites.end());

    return connected_bites;
}


std::vector<ConnectionID> Datastructures::get_connections(BiteID id1, BiteID id2)
{
    // jos parametreja ei ole annettu, palautetaan kaikki yhteydet
    if (id1 == NO_BITE && id2 == NO_BITE) {
        std::vector<ConnectionID> all_connections;
        all_connections.reserve(connections_.size());

        for (const auto& [id, info] : connections_) {
            all_connections.push_back(id);
        }
        return all_connections;
    }

    // jos annettu vain yksi bite id, palautetaan sen yhteydet
    if (id1 != NO_BITE && id2 == NO_BITE) {
        std::vector<ConnectionID> bites_connections;
        bites_connections.reserve(connections_.size());

        for (const auto& [id, info] : connections_) {
            if (info.start_bite == id1 || info.end_bite == id1) {
                bites_connections.push_back(id);
            }
        }

        if (bites_connections.empty()) {
            return {NO_CONNECTION};
        }

        return bites_connections;
    }

    // jos annettu 2 bite id:tä, mutta nillä ei ole yhteyttä, palautetaan NO_CONNECTION
    if (bites_.find(id1) == bites_.end() || bites_.find(id2) == bites_.end()) {
        return {NO_CONNECTION};
    }

    // jos annettu 2 bite id:tä käydään läpi niiden väliset yhteydet
    std::vector<ConnectionID> connections_between_bites;
    connections_between_bites.reserve(connections_.size());

    for (const auto& [id, info] : connections_) {
        // Tarkistetaan, ovatko annetut id1 ja id2 kytkettyjä yhteydessä
        if ((info.start_bite == id1 && info.end_bite == id2) ||
            (info.start_bite == id2 && info.end_bite == id1)) {
            connections_between_bites.push_back(id);
        }
    }

    if (connections_between_bites.empty()) {
        return {};
    }

    return connections_between_bites;
}




std::vector<Coord> Datastructures::get_connection_coords(BiteID biteid,
                                                         ConnectionID connectionid)
{
    // jos suupalaa tai yhteyttä ei ole, palautetaan NO_COORD
    if (bites_.find(biteid) == bites_.end() ||
            connections_.find(connectionid)==connections_.end()){
        return {NO_COORD};
    }
    const Connection& connection = connections_[connectionid];

    // jos suupala ei ole yhteyden alku eikä loppu, palautetaan NO_COORD
    if (connection.start_bite != biteid && connection.end_bite != biteid) {
        return {NO_COORD};
    }
    std::vector<Coord> connectioncoords;
    Coord start_coord = bites_[connection.start_bite].coord;
    Coord end_coord = bites_[connection.end_bite].coord;
    // jos suupala on yhteyden loppukoordinaatti, palautetaan
    // vektori päinvastaisessa järjestyksessä
    if (connection.end_bite == biteid && connection.start_bite != connection.end_bite) {
        // lisätään vektoriin myös alku- ja loppusuupalojen koordinaatit
        connectioncoords.push_back(end_coord);
        connectioncoords.insert(connectioncoords.end(),
                                connection.path_coords.rbegin(),
                                connection.path_coords.rend());
        connectioncoords.push_back(start_coord);
        return connectioncoords;
    }
    // jos suupala on yhteyden alkupiste, palautetaan vektori oikeassa järjestyksessä
    // lisätään vektoriin myös alku- ja loppusuupalojen koordinaatit
    connectioncoords.push_back(start_coord);
    connectioncoords.insert(connectioncoords.end(),
                            connection.path_coords.begin(),
                            connection.path_coords.end());
    connectioncoords.push_back(end_coord);
    return connectioncoords;
}


std::vector<std::pair<Coord, Distance>> Datastructures::path_any(BiteID fromid, BiteID toid)
{
    // jos suupaloja ei ole olemassa, palautetaan {{NO_COORD, NO_DISTANCE}}
    if (bites_.find(fromid) == bites_.end() || bites_.find(toid) == bites_.end()) {
        return {{NO_COORD, NO_DISTANCE}};
    }
    if ( fromid == toid ) {
        return {};
    }
    // käytetään BFS eli syvyys ensin-hakua, koska se löytää kaikki polut
    // BFS-muuttujat: edeltävät solmut ja BFS-jono
    std::unordered_map<BiteID, BiteID> came_from;
    std::queue<BiteID> queue;
    queue.push(fromid);
    came_from[fromid] = NO_BITE;

    // BFS-silmukka
    while (!queue.empty()) {
        BiteID current = queue.front();
        queue.pop();

        // käydään läpi kaikki nykyisestä suupalasta lähtevät yhteydet
        for (const auto& [neighbor, connection_id] : bite_connections_[current]) {
            // Jos naapuri ei ole vielä käyty läpi
            if (came_from.find(neighbor) == came_from.end()) {
                came_from[neighbor] = current;

                // jos naapuri on kohde, rakennetaan polku
                if (neighbor == toid) {
                    std::vector<BiteID> path;
                    BiteID step = toid;
                    while (step != NO_BITE) {
                        path.push_back(step);
                        step = came_from[step];
                    }
                    std::reverse(path.begin(), path.end());

                    // rakennetaan lopullinen koordinaatti- ja etäisyysvektori
                    std::vector<std::pair<Coord, Distance>> result;
                    Distance total_distance = 0;
                    Coord previous_coord = bites_[path[0]].coord;
                    result.push_back({previous_coord, total_distance});

                    // käydään läpi polun suupalat ja lisätään väliyhteyksien koordinaatit
                    for (size_t i = 1; i < path.size(); ++i) {
                        std::vector<Coord> connection_coords = get_connection_coords(path[i - 1], bite_connections_[path[i - 1]][path[i]]);

                        // lisätään väliyhteyksien koordinaatit, mutta jätetään ensimmäinen koordinaatti pois
                        for (size_t j = 1; j < connection_coords.size(); ++j) {
                            Distance distance = std::abs(connection_coords[j].x - previous_coord.x) + std::abs(connection_coords[j].y - previous_coord.y);
                            total_distance += distance;
                            result.push_back({connection_coords[j], total_distance});
                            previous_coord = connection_coords[j];
                        }
                    }
                    return result;
                }

                queue.push(neighbor);
            }
        }
    }

    // jos polkua ei löytynyt, palautetaan tyhjä vektori
    return {};
}



// Non-compulsory operations
std::vector<std::pair<Coord, Distance>> Datastructures::path_shortest(BiteID /*fromid*/,
                                                                      BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_shortest");
}
std::vector<std::pair<Coord, Distance>> Datastructures::path_least_bites(BiteID fromid, BiteID toid)
{
    // Jos suupaloja ei ole olemassa, palautetaan {{NO_COORD, NO_DISTANCE}}
    if (bites_.find(fromid) == bites_.end() || bites_.find(toid) == bites_.end()) {
        return {{NO_COORD, NO_DISTANCE}};
    }
    if (fromid == toid) {
        return {};
    }

    // Käytetään BFS hakua, koska se löytää vähiten yhteyksiä sisältävän reitin
    std::unordered_map<BiteID, BiteID> came_from;
    std::queue<BiteID> queue;
    queue.push(fromid);
    came_from[fromid] = NO_BITE;

    // BFS-silmukka
    while (!queue.empty()) {
        BiteID current = queue.front();
        queue.pop();

        // Käydään läpi kaikki yhteydet nykyisestä suupalasta
        for (const auto& [neighbor, connection_id] : bite_connections_[current]) {
            // Jos naapuri ei ole vielä käyty läpi
            if (came_from.find(neighbor) == came_from.end()) {
                came_from[neighbor] = current;

                // Jos naapuri on kohde, rakennetaan polku
                if (neighbor == toid) {
                    std::vector<BiteID> path;
                    BiteID step = toid;
                    while (step != NO_BITE) {
                        path.push_back(step);
                        step = came_from[step];
                    }
                    std::reverse(path.begin(), path.end());

                    // Rakennetaan lopullinen koordinaatti- ja etäisyysvektori
                    std::vector<std::pair<Coord, Distance>> result;
                    Distance total_distance = 0;
                    Coord previous_coord = bites_[path[0]].coord;
                    result.push_back({previous_coord, total_distance});

                    // Käydään läpi polun suupalat ja lisätään väliyhteyksien koordinaatit
                    for (size_t i = 1; i < path.size(); ++i) {
                        std::vector<Coord> connection_coords = get_connection_coords(path[i - 1], bite_connections_[path[i - 1]][path[i]]);

                        // Lisätään väliyhteyksien koordinaatit mutta jätetään ensimmäinen pois (vältetään päällekkäisyys)
                        for (size_t j = 1; j < connection_coords.size(); ++j) {
                            Distance distance = std::abs(connection_coords[j].x - previous_coord.x) + std::abs(connection_coords[j].y - previous_coord.y);
                            total_distance += distance;
                            result.push_back({connection_coords[j], total_distance});
                            previous_coord = connection_coords[j];
                        }
                    }
                    return result;
                }

                queue.push(neighbor);
            }
        }
    }

    // Jos polkua ei löytynyt, palautetaan tyhjä vektori
    return {};
}


std::vector<std::pair<Coord, Distance>> Datastructures::path_least_uphill(BiteID /*fromid*/,
                                                                          BiteID /*toid*/)
{
  // Replace the line below with your implementation
  throw NotImplemented("path_least_uphill");
}
