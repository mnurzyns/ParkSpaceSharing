# ParkSpaceSharing
Project created by trainees of Bakcyl Programowania in 2022/2023.

# Install Gtest/GMock
```console
[sudo] apt install -y libgmock-dev libgtest-dev
```
# Install Qt5
```console
[sudo] apt install -y qt5-default \
                      qtbase5-dev \
                      qtdeclarative5-dev \
                      qtquickcontrols2-5-dev \
                      qml-module-qtquick-controls
```

# Install Sqlite3
```console
[sudo] apt install -y sqlite3 \
                      libsqlite3-dev 
```

# Run cmake
```console
cmake -Bbuild -H.
cmake --build build -j 4
./build/bin/ParkingSpaceSharing
```