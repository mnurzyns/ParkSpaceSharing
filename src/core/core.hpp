#include <string>
#include <vector>
using namespace std;


std::vector<ParkingSpace> ParkingSpaces;
std::vector<Offer> Offers;
std::vector<User> Users;


/* TAKES ALL DATA FROM DATABASE */
void getAllData();


class ParkingSpace{
public:
    ParkingSpace(int id);

private:
    const unsigned int id_;
};

class Offer{
public:
    Offer(int id,int ID_of_Offered_Parking_Space);

protected:
    const unsigned int id_;
    const unsigned int ID_of_Offered_Parking_Space_;

};

class User{
public:
    User(int id, string password);

    /* USER INTERACTION */
    void MakeOffer()   {};
    void RemoveOffer() {};
    void RentOffer()   {};
    void BuyOffer()    {};

private:
    const unsigned int id_;
    string password_;

    std::vector<const unsigned int> ID_of_Owned_Parking_Spaces;
    std::vector<const unsigned int> ID_of_Posted_Offers;

};

