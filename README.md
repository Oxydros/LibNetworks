# C++ Library for network communications

**DISCLAIMER: This repository is not yet functional. It will be soon.**

## Introduction

This library is part of our end of studies project called CubZ.  

# Update submodules

Before compiling the library using the respective build.sh or GenerateVSSolution.bat script, you need  
to update the submodules.  
If you are on Linux:  
```
    git submodule sync --recursive
    git submodule update --init --remote --merge
```  
  
You can use the same parameters using TortoiseGit on Windows.  

# Windows third part library compilation
__Please follow https://gitlab.com/cubz/LibNetworks/wikis/Compilation__

# Library compilation

Windows:
- Execute the script in the directory scripts/windows
- This script is meant to be call from  the directory (read the script for more information)
- You will find the proto and cubznetworks lib in the lib directory

Linux:
- You will find the scripts in the scripts/linux directory
- Execute initRepo.sh
- Execute build.sh
- Libs are in build/lib

# Windows third part library compilation (DEPRECATED)

__The instructions bellow are DEPRECATED, and keep for maintenance purposes:__  
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

## Contact

You can contact me on Twitter @oxydros, or via e-mail oxydros at gmail.