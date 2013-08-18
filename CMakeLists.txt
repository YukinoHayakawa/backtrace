cmake_minimum_required(VERSION 2.6)

project(backtrace-test)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=c++11 -O3 -g -pthread")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

find_Package(SDL2 REQUIRED)

# Workaround for the non-working REQUIRED flag
if(NOT SDL2_FOUND)
   message(FATAL_ERROR "SDL2 not found!")
endif(NOT SDL2_FOUND)

link_libraries(
    ${SDL2_LIBRARY}
    SDL2main # Sadly not included in SDL_LIBRARY variable
)

add_executable(
    backtrace-test
    main.cpp
)
