# ParkingSpaceSharing
Project created by trainees of Bakcyl Programowania in 2022/2023.

## Run server using docker-compose

```bash
docker-compose build
docker-compose up
```

## Build from source

### Prerequisites
 - `C++20` Compiler e.g. `GCC`
 - `CMake`
 - `CMake` supported generator e.g. `Ninja`
 - [`Conan`](https://conan.io/) **< 2.0** or [install dependencies manually](#install-conan-dependencies-manually)

#### Debian / Ubuntu
```bash
sudo apt-get update && sudo apt-get install -y gcc g++ cmake ninja-build python3-pip && python3 -m pip install --user --upgrade conan==1.59
```

#### CentOS / RHEL / Fedora
```bash
sudo yum -y install gcc g++ cmake ninja-build python3-pip && python3 -m pip install --user --upgrade conan==1.59
```

#### Alpine
```bash
apk add build-base gcc g++ cmake samurai py3-pip && python3 -m pip install --user --upgrade conan==1.59
```

### CMake configure
```bash
cmake -S. -Bbuild -G Ninja -DCMAKE_BUILD_TYPE:STRING=Release
```

### Build
```bash
cmake --build build
```

### Run
```bash
# server
./build/bin/pss-server

# client
./build/bin/client
```

### Install required conan dependencies manually

| Dependency                                                | Tested version |
|-----------------------------------------------------------|----------------|
| [`oatpp`](https://github.com/oatpp/oatpp)                 | **1.3.0**      |
| [`oatpp-swagger`](https://github.com/oatpp/oatpp-swagger) | **1.3.0**      |
| [`oatpp-sqlite`](https://github.com/oatpp/oatpp-sqlite)   | **1.3.0**      |
| [`oatpp-openssl`](https://github.com/oatpp/oatpp-openssl) | **1.3.0**      |
| [`openssl`](https://github.com/openssl/openssl)           | **3.0.8**      |
| [`jwt-cpp`](https://github.com/Thalhammer/jwt-cpp)        | **0.6.0**      |
| [`nlohmann_json`](https://github.com/nlohmann/json)       | **3.11.2**     |
| [`toml11`](https://github.com/ToruNiina/toml11)           | **3.7.1**      |
| [`Botan`](https://github.com/randombit/botan)             | **2.19.3**     |
| [`re2`](https://github.com/google/re2)                    | **2023-03-01** |
