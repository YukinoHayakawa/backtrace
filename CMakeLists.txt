cmake_minimum_required(VERSION 2.6)

project(backtrace-test)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -std=c++11 -O3 -g -pthread")
endif()

find_Package(SDL REQUIRED)

# Workaround for the non-working REQUIRED flag
if(NOT SDL_FOUND)
   message(FATAL_ERROR "SDL not found!")
endif(NOT SDL_FOUND)

link_libraries(
    ${SDL_LIBRARY}
    SDLmain # Sadly not included in SDL_LIBRARY variable
)

add_executable(
    backtrace-test
    main.cpp
)
