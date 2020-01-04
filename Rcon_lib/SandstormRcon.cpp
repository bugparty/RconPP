//
// Created by fancy on 2020/1/1.
//

#include <iostream>
#include "SandstormRcon.hpp"
#include "SandStormPlayer.hpp"
void SandstormRcon::print(rc_packet &packet, const std::string command)
{
    if (command == "listplayers"){
        printListPlayers(packet);
    }else {
        RconPP::print(packet, command);
    }

}
void SandstormRcon::printListPlayers(rc_packet &packet){
    int countNL = 0;
    const int barsPerLine = 5;
    /** A typical packet is looking like this below
     * ID	 | Name				 | NetID			 | IP			 | Score		 |
       ===============================================================================
     */
    size_t ssize = 0;
    for(;ssize<packet.size && packet.data[ssize] !='\0';ssize++){}

    for(size_t i=0;i< ssize;i++){
        while(i < ssize){
            putchar(packet.data[i]);
            if(packet.data[i] == '\n'){
                if(++countNL ==2){
                    i++;
                    break;
                }
            }
            i++;
        }
        int verticalBars = 0;
        int begin;
        begin =i;
        std::vector<SandStormPlayer> players;

        while(i < ssize){

            if(packet.data[i++] == '|'){
                if(++verticalBars == barsPerLine){
                    SandStormPlayer temp;
                    verticalBars = 0;

                    if(temp.parse(packet.data + begin, i -begin ) == 0){
                        players.push_back(temp);
                    }
                    begin = i;
                }
            }

        }

    }
}