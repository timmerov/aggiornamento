@echo off

if not exist build-win64 ( mkdir build-win64 )
pushd build-win64
echo Platform is windows 64.
"C:\Program Files (x86)\CMake\bin\cmake.exe" -G "Visual Studio 12 Win64" -DPLATFORM_FILE:STRING=win64 ..
popd
