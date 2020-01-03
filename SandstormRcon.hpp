//
// Created by fancy on 2020/1/1.
//

#ifndef RCON_SANDSTORMRCON_HPP
#define RCON_SANDSTORMRCON_HPP
#include "RconPP.hpp"

class SandstormRcon: public RconPP {
public:
    SandstormRcon(
                  std::string host, std::string port, std::string password, bool raw_output = false,
                  bool print_colors = true) : RconPP(host, port, password, raw_output, print_colors) {
    }

protected:
    void print(rc_packet &packet, const std::string command);
    void printListPlayers(rc_packet &packet);
};


#endif //RCON_SANDSTORMRCON_HPP
