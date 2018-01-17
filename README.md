# C++ Library for network communications

# Windows third part library compilation
Boost:
- Download the Boost library (http://www.boost.org/users/download/)
- Here is the compilation command
```
    /yourBoostRootDir> ./boostrap
    /yourBoostRootDir> ./b2.exe --toolset=msvc variant=release link=static threading=multi runtime-link=static address-model=64 stage
```
- To use this library with HoloLens application, you will need the 32 bits version.
```
    /yourBoostRootDir> ./b2.exe --toolset=msvc variant=release link=static threading=multi runtime-link=shared architecture=x86 stage
```
- Define the env variable BOOST_ROOT with the path to your boost directory as value

Protobuf:
- Follow https://github.com/google/protobuf/blob/master/cmake/README.md

# Library compilation

Windows:
- Execute the script in the directory scripts/windows
- This script is meant to be call from  the directory (read the script for more information)

Linux:
- You will find the scripts in the scripts/linux directory
- Execute initRepo.sh
- Execute build.sh
- Libs are in build/lib


# TODO
- Add details on CMake windows compilation (?)
- More unit test
- Add depdencies installation from within CMake