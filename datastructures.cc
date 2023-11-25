#include "datastructures.hh"

#include <random>

#include <cmath>

#include <iostream>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
}

Datastructures::~Datastructures()
{
}

unsigned int Datastructures::get_affiliation_count()
{
    return AffiliationMap.size();
}

void Datastructures::clear_all()
{
    AffiliationMap.clear();
    PublicationMap.clear();
    AlreadyReferencedSet.clear();
    AffiliationVector.clear();
    AffiliationVectorForCoord.clear();
}

std::vector<AffiliationID> Datastructures::get_all_affiliations()
{
    return AffiliationVector;
}

bool Datastructures::add_affiliation(AffiliationID id, const Name &name, Coord xy)
{
    auto it = AffiliationMap.find(id);
    if (it != AffiliationMap.end()) {
        return false;
    }
    Affiliation new_affiliation;
    new_affiliation.id = id;
    new_affiliation.name = name;
    new_affiliation.coord = xy;
    AffiliationMap.emplace(id, new_affiliation);
    AffiliationVector.push_back(id);
    AffiliationVectorForCoord.push_back(id);
    AreAffiliationsSortedAB = false;
    AreAffiliationsSortedCoord = false;
    return true;
}

Name Datastructures::get_affiliation_name(AffiliationID id)
{
    auto it = AffiliationMap.find(id);
    if (it != AffiliationMap.end()) {
        return it->second.name;
    }
    return NO_NAME;
}

Coord Datastructures::get_affiliation_coord(AffiliationID id)
{
    auto it = AffiliationMap.find(id);
    if (it != AffiliationMap.end()) {
        return it->second.coord;
    }
    return NO_COORD;
}

std::vector<AffiliationID> Datastructures::get_affiliations_alphabetically()
{
    if (AreAffiliationsSortedAB ==  true){
        return AffiliationVector;
    }
    std::sort(AffiliationVector.begin(), AffiliationVector.end(), [this](const auto& a1, const auto& a2) {
        return AffiliationMap.at(a1).name < AffiliationMap.at(a2).name;
    });
    AreAffiliationsSortedAB = true;
    return AffiliationVector;
}

std::vector<AffiliationID> Datastructures::get_affiliations_distance_increasing()
{
    if (AreAffiliationsSortedCoord == true) {
        return AffiliationVectorForCoord;
    }
    std::sort(AffiliationVectorForCoord.begin(), AffiliationVectorForCoord.end(), [this](const auto& a1, const auto& a2) {
        // Calculate distance from origin, then sort by that ascending, if distance is same, sort by y coordinate
        double distance1 = sqrt(pow(AffiliationMap.at(a1).coord.x, 2) + pow(AffiliationMap.at(a1).coord.y, 2));
        double distance2 = sqrt(pow(AffiliationMap.at(a2).coord.x, 2) + pow(AffiliationMap.at(a2).coord.y, 2));
        if (distance1 == distance2) {
            return AffiliationMap.at(a1).coord.y < AffiliationMap.at(a2).coord.y;
        }
        return distance1 < distance2;
    });
    AreAffiliationsSortedCoord = true;
    return AffiliationVectorForCoord;
}

AffiliationID Datastructures::find_affiliation_with_coord(Coord xy)
{   
    for (const auto &affiliation : AffiliationMap) {
        if (affiliation.second.coord == xy) {
            return affiliation.first;
        }
    }
    return NO_AFFILIATION;
}

bool Datastructures::change_affiliation_coord(AffiliationID id, Coord newcoord)
{
    auto it = AffiliationMap.find(id);
    if (it != AffiliationMap.end()) {
        it->second.coord = newcoord;
        AreAffiliationsSortedCoord = false;
        return true;
    }
    return false;
}

bool Datastructures::add_publication(PublicationID id, const Name &name, Year year, const std::vector<AffiliationID> &affiliations)
{
    auto PublicationIterator = PublicationMap.find(id);
    if (PublicationIterator != PublicationMap.end()) {
        return false;
    }
    Publication new_publication;
    new_publication.id = id;
    new_publication.header = name;
    new_publication.year = year;
    PublicationMap.emplace(id, new_publication);
    for (auto affiliation : affiliations) {
        auto AffiliationIterator = AffiliationMap.find(affiliation);
        if (AffiliationIterator != AffiliationMap.end()) {
            AffiliationIterator->second.publications.push_back(id);
            AffiliationIterator->second.publications_struct.push_back(new_publication);
            new_publication.AffiliationsPublishedIn.push_back(affiliation);
        }
    }
    return true;
}

std::vector<PublicationID> Datastructures::all_publications()
{
    std::vector<PublicationID> result;
    for (auto publication : PublicationMap) {
        result.push_back(publication.first);
    }
    return result;
}

Name Datastructures::get_publication_name(PublicationID id)
{
    auto it = PublicationMap.find(id);
    if (it != PublicationMap.end()) {
        return it->second.header;
    }
    return NO_NAME;
}

Year Datastructures::get_publication_year(PublicationID id)
{
    auto it = PublicationMap.find(id);
    if (it != PublicationMap.end()) {
        return it->second.year;
    }
    return NO_YEAR;
}

std::vector<AffiliationID> Datastructures::get_affiliations(PublicationID id)
{
    auto it = PublicationMap.find(id);
    if (it != PublicationMap.end()) {
        return it->second.AffiliationsPublishedIn;
    }
    std::vector<AffiliationID> result;
    result.push_back(NO_AFFILIATION);
    return result;
}

bool Datastructures::add_reference(PublicationID id, PublicationID parentid)
{
    auto it = AlreadyReferencedSet.find(id);
    if (it != AlreadyReferencedSet.end()) {
        return true;
    }

    auto PublicationIterator = PublicationMap.find(id);
    auto ParentIterator = PublicationMap.find(parentid);
    if (PublicationIterator == PublicationMap.end() || ParentIterator == PublicationMap.end()) {
        return false;
    }
    PublicationIterator->second.ReferencedByStruct = &ParentIterator->second;
    ParentIterator->second.ReferencingThesePublications.push_back(id);
    AlreadyReferencedSet.insert(id);
    return true;
}
std::vector<PublicationID> Datastructures::get_direct_references(PublicationID id)
{
    auto it = PublicationMap.find(id);
    if (it != PublicationMap.end()) {
        return it->second.ReferencingThesePublications;
    }
    else{
        std::vector<PublicationID> result;
        result.push_back(NO_PUBLICATION);
        return result;
    }
}

bool Datastructures::add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid)
{
    auto publication = PublicationMap.find(publicationid);
    auto affiliation = AffiliationMap.find(affiliationid);
    if (publication == PublicationMap.end() || affiliation == AffiliationMap.end()) {
        return false;
    }
    publication->second.AffiliationsPublishedIn.push_back(affiliationid);
    affiliation->second.publications.push_back(publicationid);
    affiliation->second.publications_struct.push_back(publication->second);
    return true;
}

std::vector<PublicationID> Datastructures::get_publications(AffiliationID id)
{
    auto it = AffiliationMap.find(id);
    if (it != AffiliationMap.end()) {
        return it->second.publications;
    }
    else{
        std::vector<PublicationID> result;
        result.push_back(NO_PUBLICATION);
        return result;
    }
}

PublicationID Datastructures::get_parent(PublicationID id)
{
    auto it = PublicationMap.find(id);
    if (it != PublicationMap.end()) {
        if (it->second.ReferencedByStruct == nullptr) {
            return NO_PUBLICATION;
        }
        return it->second.ReferencedByStruct->id;
    }
    return NO_PUBLICATION;
}

std::vector<std::pair<Year, PublicationID> > Datastructures::get_publications_after(AffiliationID affiliationid, Year year)
{
    auto it = AffiliationMap.find(affiliationid);
    if (it != AffiliationMap.end()) {
        std::vector<std::pair<Year, PublicationID>> result;
        for (auto publication : it->second.publications_struct) {
            if (publication.year >= year) {
                result.push_back(std::make_pair(publication.year, publication.id));
            }
        }
        return result;
    }
    else{
        std::vector<std::pair<Year, PublicationID>> result;
        result.push_back(std::make_pair(NO_YEAR, NO_PUBLICATION));
        return result;
    }
}

std::vector<PublicationID> Datastructures::get_referenced_by_chain(PublicationID id)
{
    std::vector<PublicationID> result;
    auto it = PublicationMap.find(id);
    if (it != PublicationMap.end()) {
        if (it->second.ReferencedByStruct == nullptr) {
            return result;
        }
        result.push_back(it->second.ReferencedByStruct->id);
        auto parent = it->second.ReferencedByStruct->ReferencedByStruct;
        while (parent != nullptr) {
            result.push_back(parent->id);
            parent = parent->ReferencedByStruct;
        }
        return result;
    }
    else{
        result.push_back(NO_PUBLICATION);
        return result;
    }
}

std::vector<PublicationID> Datastructures::get_all_references(PublicationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_all_references()");
}

std::vector<AffiliationID> Datastructures::get_affiliations_closest_to(Coord /*xy*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_affiliations_closest_to()");
}

bool Datastructures::remove_affiliation(AffiliationID /*id*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("remove_affiliation()");
}

PublicationID Datastructures::get_closest_common_parent(PublicationID /*id1*/, PublicationID /*id2*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("get_closest_common_parent()");
}

bool Datastructures::remove_publication(PublicationID /*publicationid*/)
{
    // Replace the line below with your implementation
    throw NotImplemented("remove_publication()");
}




