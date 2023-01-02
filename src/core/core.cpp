#include "core.hpp"

ParkingSpace::ParkingSpace(int id):
    id_(id)
{}

Offer::Offer(int id,int id_offered_parking_space):
    id_(id),
    parking_space_id(id_offered_parking_space)
{}

User::User(int id, std::string password): 
    id_(id),
    password_(password)
{}