cmake -Bbuild -H.
cmake --build build -j 4
./build/bin/CoreTest
./build/bin/DatabaseTest
./build/bin/Server
./build/bin/ParkingSpaceSharing