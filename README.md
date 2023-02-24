# ParkSpaceSharing
Project created by trainees of Bakcyl Programowania in 2022/2023.

## Build from source

### Prerequisites
 - `C++20` Compiler e.g. `GCC`
 - `CMake`
 - `CMake` supported generator e.g. `Ninja`
 - [`Conan`](https://conan.io/) < 2.0 or [install dependencies manually](#install-conan-dependencies-manually)

#### Debian / Ubuntu
```bash
sudo apt-get update && sudo apt-get install -y gcc g++ cmake ninja-build python3-pip qtbase5-dev qtdeclarative5-dev qtquickcontrols2-5-dev && python3 -m pip install --user --upgrade conan==1.59
```

#### CentOS / RHEL / Fedora
```bash
sudo yum -y install gcc g++ cmake ninja-build python3-pip qt5-qtbase-devel qt5-qtdeclarative-devel qt5-qtquickcontrols2-devel && python3 -m pip install --user --upgrade conan==1.59
```

#### Alpine
```bash
apk add build-base gcc g++ cmake samurai py3-pip qt5-qtbase-dev qt5-qtdeclarative-dev qt5-qtquickcontrols2-dev && python3 -m pip install --user --upgrade conan==1.59
```

### CMake configure
```bash
cmake -S. -Bbuild -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release -DCONAN:BOOL=ON
```

### Build
```bash
cmake --build build
```

### Run
```bash
./build/bin/ParkingSpaceSharing
```

### Install conan dependencies manually
Required dependencies:
 - `GTest`
 - `SQLite3`
 - `Nlohmann JSON`

#### Debian / Ubuntu
```bash
sudo apt-get install -y libgtest-dev sqlite3 libsqlite3-dev nlohmann-json3-dev
```

#### Alpine
```bash
apk add gtest-dev sqlite sqlite-dev nlohmann-json
```
`nlohmann-json` is only available on edge branch, but can be [built from source](https://github.com/nlohmann/json).
