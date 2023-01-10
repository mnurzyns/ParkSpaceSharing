#include "core.hpp"

ParkingSpace::ParkingSpace(int id, int owner_id, std::string location):
    id_(id),
    owner_id_(owner_id),
    location_(location)
{}

Offer::Offer(int id,int id_parking_space, std::string description):
    id_(id),
    parking_space_id(id_parking_space),
    description_(description)
{}

User::User(int id, std::string username, std::string password):
    id_(id),
    username_(username),
    password_(password)
{}

void RemoveOffer() {}
void RentOffer()   {}
