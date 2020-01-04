cmake_minimum_required(VERSION 3.15)
project(rcon)

set(CMAKE_CXX_STANDARD 14)
include_directories(Rcon_lib)
add_subdirectory(Rcon_lib)


set(SOURCE_FILES cmd.cpp)
add_executable(rconpp ${SOURCE_FILES})


target_link_libraries(rconpp Rcon_lib)


add_subdirectory(Boost_tests)

