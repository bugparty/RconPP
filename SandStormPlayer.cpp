//
// Created by fancy on 2020/1/2.
//

#include "SandStormPlayer.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>

int SandStormPlayer::parse(const char *str, size_t strSize) {
    size_t offset = 0;
    const char *begin=str;
    try {
        skipTableAndSeperator(str, offset);
        begin = str + offset;
        size_t  size = getTokenLength(begin);
        id = boost::lexical_cast<int>(begin, size);
        offset+=size;
        skipTableAndSeperator(str, offset);
        begin = str + offset;
        size = getTokenLength(begin);
        name  = boost::lexical_cast<std::string> (begin, size);
        offset+=size;
        skipTableAndSeperator(str, offset);
        begin = str + offset;
        size = getTokenLength(begin);
        net_id = boost::lexical_cast<std::string> (begin, size);
        offset+=size;
        skipTableAndSeperator(str, offset);
        begin = str + offset;
        size = getTokenLength(begin);
        ip = boost::lexical_cast<std::string> (begin, size);
        offset+=size;
        skipTableAndSeperator(str, offset);
        begin = str + offset;
        size = getTokenLength(begin);
        score = boost::lexical_cast<int> (begin, size);

    } catch (boost::bad_lexical_cast &e) {
        std::cout << e.what();
        return -1;
    }
    return 0;

}

void SandStormPlayer::skipTableAndSeperator(const char *str, size_t &i) const {
    while (*(str + i) == '\t' || *(str + i) == '|' || *(str + i) == ' ')i++;
}

size_t SandStormPlayer::getTokenLength(const char *str) const {
    size_t i=0;
    while (*(str + i) != '\t' && *(str + i) != '|')i++;
    return i;
}

