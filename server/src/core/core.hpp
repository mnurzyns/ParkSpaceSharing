#pragma once

#include <string>
#include <vector>


/* USER INTERACTION */
void RemoveOffer();
void AddRentOffer();


struct ParkingSpace{
    unsigned id;
    unsigned owner_id;
    std::string location;
};

struct Offer{
    unsigned id;
    unsigned user_id;
    std::string description;
};

struct User{
    unsigned id;
    std::string username;
    std::string password;
};

