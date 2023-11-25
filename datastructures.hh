#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <set>
#include <unordered_set>
#include <unordered_map>

// Types for IDs
using AffiliationID = std::string;
using PublicationID = unsigned long long int;
using Name = std::string;
using Year = unsigned short int;
using Weight = int;
using Distance = int;

// Return values for cases where required thing was not found
AffiliationID const NO_AFFILIATION = "---";
PublicationID const NO_PUBLICATION = -1;
Name const NO_NAME = "!NO_NAME!";
Year const NO_YEAR = -1;
Weight const NO_WEIGHT = -1;

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};


// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: That's what cppreference says about size()
    unsigned int get_affiliation_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: clear() is linear on the number of elements
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Looping through the whole map once
    std::vector<AffiliationID> get_all_affiliations();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Find operation is constant on average, so is the emplace operation
    // they both do have worst case of linear time, but that's not the average case.
    bool add_affiliation(AffiliationID id, Name const& name, Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: It's constant on average, but worst case linear
    Name get_affiliation_name(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: It's O(1) on average, but worst case linear
    Coord get_affiliation_coord(AffiliationID id);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: reserving space for vector is linear, std::sort is n*log(n) and for loop is n
    std::vector<AffiliationID> get_affiliations_alphabetically();

    // Estimate of performance: O(n * log(n))
    // Short rationale for estimate: sorting is again n*log(n), for loop is n
    std::vector<AffiliationID> get_affiliations_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Loops through n elements until finds correct one or not found
    AffiliationID find_affiliation_with_coord(Coord xy);

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(1) on average but worst case linear
    bool change_affiliation_coord(AffiliationID id, Coord newcoord);


    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: This one is a bit tricky imo, but I think it's O(n) n being the amount of affiliations given as parameter
    // if the given parameter amount of affiliations is small then this is almost constant time, but if it's big then it's linear
    bool add_publication(PublicationID id, Name const& name, Year year, const std::vector<AffiliationID> & affiliations);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Loops through all publications
    std::vector<PublicationID> all_publications();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Find is constant on average
    Name get_publication_name(PublicationID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Find is constant on average
    Year get_publication_year(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: I guess it's the copying of the vector thats linear since grader
    // doesnt accept O(1)
    std::vector<AffiliationID> get_affiliations(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(1) on average, but worst case linear
    bool add_reference(PublicationID id, PublicationID parentid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: O(1) on average, but worst case linear
    std::vector<PublicationID> get_direct_references(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is constant on average, but worst case linear
    bool add_affiliation_to_publication(AffiliationID affiliationid, PublicationID publicationid);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is constant on average, but worst case linear
    std::vector<PublicationID> get_publications(AffiliationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Find is constant on average, but worst case linear
    PublicationID get_parent(PublicationID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: for loop is n, everything else is constant
    std::vector<std::pair<Year, PublicationID>> get_publications_after(AffiliationID affiliationid, Year year);

    // Estimate of performance: O(n)
    // Short rationale for estimate: While loop is n depending on the depth of relations, everything else is constant
    std::vector<PublicationID> get_referenced_by_chain(PublicationID id);


    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<PublicationID> get_all_references(PublicationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<AffiliationID> get_affiliations_closest_to(Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_affiliation(AffiliationID id);

    // Estimate of performance:
    // Short rationale for estimate:
    PublicationID get_closest_common_parent(PublicationID id1, PublicationID id2);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_publication(PublicationID publicationid);


private:
    std::unordered_set<PublicationID> AlreadyReferencedSet;

    struct Publication {
        PublicationID id;
        Name header;
        Year year;
        std::vector<AffiliationID> AffiliationsPublishedIn = {};
        std::vector<PublicationID> ReferencingThesePublications = {};
        Publication* ReferencedByStruct = nullptr;
    };

    struct Affiliation {
        AffiliationID id;
        Name name;
        Coord coord;
        std::vector<PublicationID> publications = {};
        std::vector<Publication> publications_struct = {};
    };
    std::unordered_map<AffiliationID, Affiliation> AffiliationMap;
    std::unordered_map<PublicationID, Publication> PublicationMap;
    bool AreAffiliationsSortedAB = false;
    bool AreAffiliationsSortedCoord = false;
    std::vector<AffiliationID> AffiliationVector;
    std::vector<AffiliationID> AffiliationVectorForCoord;
};

#endif // DATASTRUCTURES_HH
