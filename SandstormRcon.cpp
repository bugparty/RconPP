//
// Created by fancy on 2020/1/1.
//

#include "SandstormRcon.hpp"
void SandstormRcon::print(rc_packet &packet, const std::string command)
{
    if (command == "listplayers"){
        std::string lines(packet.data);
        auto it = lines.begin();
        int countNL = 0;
        const int barsPerLine = 5;
        /** A typical packet is looking like this below
         * ID	 | Name				 | NetID			 | IP			 | Score		 |
           ===============================================================================
         */
        for(int i=0;i< lines.size();i++){
            while(i < lines.size()){
                putchar(lines[i]);
                if(*it == '\n'){
                    if(++countNL ==2){
                        i++;
                        break;
                    }
                }
                i++;
            }
            int verticalBars = 0;
            while(i < lines.size()){
                if(lines[i] == '|'){
                    if(++verticalBars == barsPerLine){
                        verticalBars = 0;
                        puts("\r\n");
                    }
                }
                putchar(lines[i]);
            }

        }

    }else {
        RconPP::print(packet, command);
    }

}