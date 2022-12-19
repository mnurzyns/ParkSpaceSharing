#include <string>
using namespace std;

class ParkingSpace{

};

class User{
public:
    User(int id, string password);

private:
    const unsigned int id_;
    string password_;
};