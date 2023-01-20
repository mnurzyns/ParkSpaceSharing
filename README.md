# ParkSpaceSharing
Project created by trainees of Bakcyl Programowania in 2022/2023.

## Build from source

### Prerequisites
 - `C++20` Compiler e.g. `GCC`
 - `CMake`
 - `CMake` supported generator e.g. `Ninja`
 - [`Conan`](https://conan.io/) or [install dependencies manually](#install-conan-dependencies-manually)

Debian/Ubuntu
```bash
sudo apt-get update && sudo apt-get install -y gcc g++ cmake ninja-build python3-pip && python3 -m pip install --user --upgrade conan
```

Fedora
```bash
sudo dnf install gcc g++ cmake ninja-build python3-pip && python3 -m pip install --user --upgrade conan
```

Arch
```bash
sudo pacman -S gcc cmake ninja python-pip && python3 -m pip install --user --upgrade conan
```

### Build
```bash
cmake -S. -Bbuild -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release -DCONAN:BOOL=ON
cmake --build build
```

### Run
```bash
./build/bin/ParkingSpaceSharing
```

### Install conan dependencies manually
Required dependencies:
 - `GTest`
 - `Qt5`
 - `SQLite3`
 - `Nlohmann JSON`

Debian/Ubuntu
```bash
sudo apt-get install -y libgtest-dev qtbase5-dev qtdeclarative5-dev qtquickcontrols2-5-dev qml-module-qtquick-controls sqlite3 libsqlite3-dev nlohmann-json3-dev
```
