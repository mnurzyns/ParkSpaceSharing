#include "core.hpp"

ParkingSpace::ParkingSpace(int id):
    id_(id)
{}

Offer::Offer(int id):
    id_(id)
{}

User::User(int id, string password): 
    id_(id),
    password_(password)
{}