#include <string>
#include <vector>


/* USER INTERACTION */
void MakeOffer()   {}
void RemoveOffer() {}
void RentOffer()   {}
void BuyOffer()    {}


class ParkingSpace{
public:
    ParkingSpace(int id);

private:
    const unsigned int id_;
};

class Offer{
public:
    Offer(int id,int id_parking_space);

protected:
    const unsigned int id_;
    const unsigned int parking_space_id;

};

class User{
public:
    User(int id, std::string password);

private:
    const unsigned int id_;
    std::string password_;

    std::vector<const unsigned int> id_owned_parking_spaces;
    std::vector<const unsigned int> id_posted_offers;

};

