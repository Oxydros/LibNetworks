cd ..\..

copy scripts\linux\ProtoCMakeLists.txt lib_src\proto\CMakeLists.txt


cmake -G "Visual Studio 15"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
devenv LibNetworks.sln /build Release

