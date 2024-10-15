#include "datastructures.hh"
#include "customtypes.hh"
#include <algorithm>

struct ContourData {
    ContourID id;
    Name name;
    ContourHeight height;
    std::vector<Coord> coords;

    ContourData* parent;
    std::vector<ContourData*> children;
};

struct BiteData {
    BiteID id;
    Name name;
    Coord coords;
    int distance;
    ContourData* parent;
};


using BiteMap = std::unordered_map<BiteID, BiteData>;
BiteMap biteMap = {};


std::unordered_map<Coord, BiteID> biteMapCoords = {};


std::multimap<Name, BiteID> nameMap;


std::multimap<int, BiteID> distMap;


using ContourMap = std::unordered_map<ContourID, ContourData>;
ContourMap contourMap = {};


std::vector<BiteID> alphabeticalBites;
std::vector<BiteID> distanceBites;

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::get_bite_count()
{
    return biteMap.size();
}

void Datastructures::clear_all()
{
    biteMap.clear();
    biteMapCoords.clear();
    nameMap.clear();
    distMap.clear();
    contourMap.clear();
    alphabeticalBites.clear();
    distanceBites.clear();
}

std::vector<BiteID> Datastructures::all_bites()
{
    std::vector<BiteID> biteVector;
    biteVector.reserve(biteMap.size());

    for (const auto& bite : biteMap) {
        biteVector.push_back(bite.first);
    }
    return biteVector;
}

bool Datastructures::add_bite(BiteID id, const Name & name, Coord coords)
{

    if ( biteMapCoords.find(coords) != biteMapCoords.end() || biteMap.find(id) != biteMap.end()) {
        return false;
    }


    biteMap[id] = {id, name, coords, coords.x + coords.y, nullptr};
    nameMap.insert({name, id});
    biteMapCoords.insert({coords, id});
    distMap.insert({coords.x + coords.y, id});


    alphabeticalBites.clear();
    distanceBites.clear();

    return true;
}

Name Datastructures::get_bite_name(BiteID id)
{
    auto it = biteMap.find(id);
    if ( it == biteMap.end() ) {
        return NO_NAME;

    } else {
        return it->second.name;
    }
}

Coord Datastructures::get_bite_coord(BiteID id)
{
    auto it = biteMap.find(id);
    if ( it == biteMap.end() ) {
        return NO_COORD;

    } else {
        return it->second.coords;
    }
}

std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
    if (alphabeticalBites.empty()) {
        for (const auto& bite : nameMap) {
            alphabeticalBites.push_back(bite.second);
        }
    }
    return alphabeticalBites;
}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
    if (distanceBites.empty()) {
        for (const auto& bite : distMap) {
            distanceBites.push_back(bite.second);
        }
    }
    return distanceBites;
}

BiteID Datastructures::find_bite_with_coord(Coord coords)
{
    auto it = biteMapCoords.find(coords);
    if (it == biteMapCoords.end()) {
        return NO_BITE;
    } else {
        return it->second;
    }
}

bool Datastructures::change_bite_coord(BiteID id, Coord newcoord)
{

    auto biteIt = biteMap.find(id);
    if (biteIt == biteMap.end() || biteMapCoords.find(newcoord) != biteMapCoords.end() ) {
        return false;
    }


    if (biteIt->second.parent != nullptr) {
        const auto& contourCoords = biteIt->second.parent->coords;


        if (std::find(contourCoords.begin(), contourCoords.end(), newcoord) == contourCoords.end()) {
            return false;
        }
    }

    biteMapCoords.erase(biteIt->second.coords);
    biteIt->second.coords = newcoord;
    biteMapCoords.insert({newcoord, id});
    return true;
}

bool Datastructures::add_contour(ContourID id, const Name & name, ContourHeight height,
                                 std::vector<Coord> coords)
{
    if (coords.empty()) {
        return false;
    }

    if (height > MAX_CONTOUR_HEIGHT || height < -MAX_CONTOUR_HEIGHT) {
        return false;
    }

    if (contourMap.find(id) != contourMap.end()) {
        return false;
    }

    contourMap[id] = {id, name, height, coords, nullptr, {}};
    return true;
}

std::vector<ContourID> Datastructures::all_contours()
{
    std::vector<BiteID> contourVector;
    contourVector.reserve(contourMap.size());

    for (const auto& contour : contourMap) {
        contourVector.push_back(contour.first);
    }
    return contourVector;
}

Name Datastructures::get_contour_name(ContourID id)
{
    auto it = contourMap.find(id);
    if ( it == contourMap.end() ) {
        return NO_NAME;

    } else {
        return it->second.name;
    }
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID id)
{
    auto it = contourMap.find(id);
    if ( it == contourMap.end() ) {
        std::vector<Coord> noCoordVec = {NO_COORD};
        return noCoordVec;

    } else {
        return it->second.coords;
    }
}

ContourHeight Datastructures::get_contour_height(ContourID id)
{
    auto it = contourMap.find(id);
    if ( it == contourMap.end() ) {
        return NO_CONTOUR_HEIGHT;

    } else {
        return it->second.height;
    }
}

bool Datastructures::add_subcontour_to_contour(ContourID id,
                                               ContourID parentid)
{
    ContourHeight childHeight = get_contour_height(id);
    ContourHeight parentHeight = get_contour_height(parentid);

    auto it = contourMap.find(id);
    if ( it == contourMap.end() || it->second.parent != nullptr ) {
        return false;
    }

    auto parentIt = contourMap.find(parentid);
    if ( parentIt == contourMap.end() ) {
        return false;
    }

    if (std::abs(childHeight - parentHeight) != 1 || std::abs(childHeight) <= std::abs(parentHeight)) {
        return false;
    }

    it->second.parent = &parentIt->second;
    parentIt->second.children.push_back(&it->second);
    return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteid, ContourID contourid)
{
    auto biteIt = biteMap.find(biteid);
    if ( biteIt == biteMap.end() ) {
        return false;
    }

    auto contourIt = contourMap.find(contourid);
    if ( contourIt == contourMap.end() ) {
        return false;
    }

    if ( biteIt->second.parent != nullptr ) {
        return false;
    }


    auto it = std::find_if(contourIt->second.coords.begin(), contourIt->second.coords.end(), [biteIt](Coord coords){return biteIt->second.coords == coords;});
    if ( it == contourIt->second.coords.end() ) {
        return false;
    }

    biteIt->second.parent = &contourIt->second;
    return true;
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID id)
{
    auto biteIt = biteMap.find(id);
    if ( biteIt == biteMap.end() ) {
        return std::vector<ContourID>{NO_CONTOUR};
    }

    if ( biteIt->second.parent == NULL ) {
        return std::vector<ContourID>{};
    }

    std::vector<ContourID> contours;
    ContourData* current = biteIt->second.parent;


    while (current != nullptr) {
        contours.push_back(current->id);
        current = current->parent;
    }

    return contours;
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
