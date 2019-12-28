cmake_minimum_required(VERSION 3.15)
project(rcon)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)

add_executable(rcon main.c)

add_executable(rconpp cmd.cpp RconPP.cpp)
target_link_libraries(rconpp Boost::program_options)