//
// Created by fancy on 2020/1/2.
//

#ifndef RCON_SANDSTORMPLAYER_HPP
#define RCON_SANDSTORMPLAYER_HPP

#include <string>

struct SandStormPlayer {
    int id = 0;
    std::string name;
    std::string net_id;
    std::string ip;
    int score = 0;
    int parse(const char* str, size_t strSize);

    void skipTableAndSeparator(const char *str, size_t &i, size_t limit) const;
    size_t getTokenLength(const char *str) const;


};


#endif //RCON_SANDSTORMPLAYER_HPP
