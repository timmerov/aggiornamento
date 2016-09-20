@echo off

if not defined CMAKE_EXE ( set CMAKE_EXE="C:\Program Files (x86)\CMake\bin\cmake.exe" )

set BUILD_DIR_64=build-win64
set GEN_CMD_64=gen-win64.bat

echo Building sdk...
call %GEN_CMD_64%
pushd %BUILD_DIR_64%
%CMAKE_EXE% --build . --config Debug
popd

call make-win64-release.bat
