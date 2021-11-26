@echo off
rd /s /q %~dp0Build
mkdir Build
cmake -S %~dp0 %OPT% -B Build
pause