@echo off

if not exist build-win64 ( mkdir build-win64 )
pushd build-win64
echo Platform is windows 64.
"C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 14 2015 Win64" -DPLATFORM_FILE:STRING=win64 ..
popd
