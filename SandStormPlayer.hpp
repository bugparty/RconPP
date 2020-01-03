//
// Created by fancy on 2020/1/2.
//

#ifndef RCON_SANDSTORMPLAYER_HPP
#define RCON_SANDSTORMPLAYER_HPP

#include <string>

struct SandStormPlayer {
    int id;
    std::string name;
    std::string net_id;
    std::string ip;
    int score;
    int parse(const char* str, size_t strSize);

    void skipTableAndSeperator(const char *str, size_t &i) const;
    size_t getTokenLength(const char *str) const;


};


#endif //RCON_SANDSTORMPLAYER_HPP
