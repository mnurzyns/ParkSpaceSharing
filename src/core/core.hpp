#include <string>
#include <vector>


/* USER INTERACTION */
static void RemoveOffer() {}
static void RentOffer()   {}


class ParkingSpace{
public:
    ParkingSpace(int id, int owner_id, std::string location);

private:
    const unsigned int id_;
    const unsigned int owner_id_;
    std::string location_;

};

class Offer{
public:
    Offer(int id,int id_parking_space, std::string description);

protected:
    const unsigned int id_;
    const unsigned int parking_space_id;
    std::string description_;

};

class User{
public:
    User(int id, std::string username, std::string password);

private:
    const unsigned int id_;
    std::string username_;
    std::string password_;

};

