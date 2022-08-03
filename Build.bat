@echo off
mkdir Build                     %create build folder%
%cmake clean Build%             %clean cache in build folder%
set /P OPT=<cmake_option.txt    %read cmake_option.txt into OPT%
cmake -S %~dp0 %OPT% -B Build   %build vs sln%
pause