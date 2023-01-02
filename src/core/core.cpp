#include "core.hpp"

void getAllData()
{}

ParkingSpace::ParkingSpace(int id):
    id_(id)
{}

Offer::Offer(int id,int ID_of_Offered_Parking_Space):
    id_(id),
    ID_of_Offered_Parking_Space_(ID_of_Offered_Parking_Space)
{}

User::User(int id, string password): 
    id_(id),
    password_(password)
{}