@echo off

set BUILD_TYPE=%1
if "%BUILD_TYPE%" == "" (
    echo No "BUILD_TYPE" specified, using default value: "Release".
    set BUILD_TYPE="Release"
)

cmake -A x64 -S . -B "build" ^
  -D CMAKE_EXPORT_COMPILE_COMMANDS=ON ^
  -D CMAKE_BUILD_TYPE=%BUILD_TYPE% ^
  -D LOGGER_SRC_CODE_INFO=ON ^
  -D LOGGER_LOG_LEVEL=INFO

if %ERRORLEVEL% == 0 (
    cmake --build "build" --config %BUILD_TYPE%
)