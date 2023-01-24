#pragma once

#include <string>
#include <vector>


/* USER INTERACTION */
void RemoveOffer();
void RentOffer();


struct ParkingSpace{
    const unsigned id;
    const unsigned owner_id;
    std::string location;
};

struct Offer{
    const unsigned id;
    const unsigned user_id;
    std::string description;
};

struct User{
    unsigned id;
    std::string username;
    std::string password;
};



