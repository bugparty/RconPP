cmake_minimum_required(VERSION 3.15)
project(rcon)
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_STANDARD 14)
add_executable(rcon main.c)

add_executable(rconpp cmd.cpp RconPP.cpp sock.cpp SandstormRcon.cpp)
target_link_libraries(rconpp Boost::program_options)