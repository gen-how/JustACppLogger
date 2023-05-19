cmake_minimum_required(VERSION 3.5)

project(
  "JustACppLogger"
  VERSION 0.0.1
  LANGUAGES CXX
)
  
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

option(LOGGER_SRC_CODE_INFO "Enable logger print source code information." OFF)
option(LOGGER_LOG_LEVEL "Set logger log level." OFF)

# add_subdirectory(src)
add_subdirectory(app)