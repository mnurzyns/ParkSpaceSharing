#include <string>
using namespace std;

class ParkingSpace{
public:
    ParkingSpace(int id);

private:
    const unsigned int id_;
};

class Offer{
public:
    Offer(int id);

private:
    const unsigned int id_;
    
};

class User{
public:
    User(int id, string password);

private:
    const unsigned int id_;
    string password_;
};

