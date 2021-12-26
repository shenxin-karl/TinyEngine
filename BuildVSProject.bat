@echo off
// rd /s /q %~dp0Build
mkdir Build
cmake.exe clean Build
cmake.exe -S %~dp0 %OPT% -B Build
pause