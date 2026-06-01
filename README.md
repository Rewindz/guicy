# Guicy
*Guicy* is a small wxWidgets based E-Liquid mixing calculator.
***
# DISCLAIMER
## *NICOTINE IS A HIGHLY ADDICTIVE SUBSTANCE AND IS TOXIC! ALWAYS DOUBLE CHECK YOUR MIX WITH ANOTHER TOOL!*
***
## Dependencies
*Guicy* is dependent on
* nlohmann/json
* wxWidgets
## Requirements
* CMake
* git
* C++23 supported compiler
* *The CMake presets have Ninja set as the build tool ensure you have it or swap it out.*
### Linux
* wxWidgets developments libraries
### Windows
* vcpkg
* Environment variable 'VCPKG_ROOT' set to the *root directory* of your vcpkg install.
* Microsoft Visual Studio (probably)
***
## Building
### Linux
```sh
git clone https://github.com/Rewindz/guicy.git
cd guicy
cmake --preset default
cmake --build build
./build/guicy
```
### Windows
In your *Developer* Powershell
```sh
git clone https://github.com/Rewindz/guicy.git
cd guicy
cmake --preset windows
cmake --build build
.\build\guicy.exe
```
