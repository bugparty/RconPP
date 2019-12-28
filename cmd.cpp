//
// Created by claymore on 2019/12/27.
//

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "RconPP.h"
namespace po = boost::program_options;
using  boost::lexical_cast;
using namespace std;

string env_name_mapper(const string& env_name){
    static map<string, string> nm = {
            {"SDRCON_HOST", "host"},
            {"SDRCON_PORT", "port"},
            {"SDRCON_PASSWORD", "password"}
    };
    return nm[env_name];
}

int main(int argc, const char* argv[]){
    po::options_description desc( "Usage: rcon [OPTIONS]... [COMMANDS]...\n\n"
                                                  "Sends rcon commands to game server.\n\n"
                                                  "Option:");
    desc.add_options()
            ("help,h", "Print usage")
            ("host,H", po::value<std::string>()->required(), "Server address")
            ("port,P", po::value<int>()->default_value(27102), "Port (default is 27102)")
            ("password,p", po::value<std::string>(), "Rcon password")
            ("interactive,t", "Interactive terminal mode")
            ("silent,s", "Silent mode (do not print received packets)")
            ("nocolor,c", "Disable colors")
            ( "raw,r", "Output raw packets (debugging and custom handling)")
            (  "version,v", "Output version information");

    po::variables_map vm;
    try{
        po::store(po::parse_environment(desc, env_name_mapper), vm);
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }catch (boost::wrapexcept<boost::program_options::required_option> & e){
        cout << e.what() << endl;
        cout << desc << endl;
        return -1;
    }



    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("host")){
        cout << vm["host"].as<string>()<< endl;
    }
    if (vm.count("port")){
        cout << vm["port"].as<int>() << endl;
    }
    if (vm.count("password")){
        cout << vm["password"].as<string>() <<endl;
    }


    return 0;
}