cmake_minimum_required(VERSION 3.15)
project(Rcon_lib)
set(Boost_USE_STATIC_LIBS   ON)
find_package(Boost 1.70 COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(HEADER_FILES
        RconPP.hpp
        SandStormPlayer.hpp
        sock.h
        SandstormRcon.hpp
        )

set(SOURCE_FILES
        RconPP.cpp
        SandstormRcon.cpp
        SandStormPlayer.cpp
        sock.cpp
        )

add_library(Rcon_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#add_executable(rconpp cmd.cpp rcon_lib/RconPP.cpp rcon_lib/sock.cpp rcon_lib/SandstormRcon.cpp rcon_lib/SandStormPlayer.cpp)
target_link_libraries(Rcon_lib Boost::program_options)