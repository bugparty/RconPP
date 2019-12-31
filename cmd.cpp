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
    bool terminalMode;
    po::options_description desc( "Usage: rcon [OPTIONS]... [COMMANDS]...\n\n"
                                                  "Sends rcon commands to game server.\n\n"
                                                  "Option:");
    desc.add_options()
            ("help,h", "Print usage")
            ("host,H", po::value<std::string>()->required(), "Server address")
            ("port,P", po::value<std::string>()->default_value("27102"), "Port (default is 27102)")
            ("password,p", po::value<std::string>(), "Rcon password")
            ("interactive,t", "Interactive terminal mode")
            ("silent,s", "Silent mode (do not print received packets)")
            ("nocolor,c", "Disable colors")
            ( "raw,r", "Output raw packets (debugging and custom handling)")
            ("command", po::value<std::vector<std::string>>()->multitoken(),"commands")
            (  "version,v", "Output version information");
    po::positional_options_description pod;
    pod.add("command", 999);
    po::variables_map vm;
    try{
        po::store(po::parse_environment(desc, env_name_mapper), vm);
        auto pr = po::command_line_parser (argc, argv).options(desc).positional(pod).run();
        po::store(pr, vm);
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
        cout << vm["port"].as<string>() << endl;
    }
    if (vm.count("password")){
        cout << vm["password"].as<string>() <<endl;
    }
    if (vm.count("command")){
        vector<string> cmds = vm["command"].as<vector<string>>();
        for(auto &s: cmds){
            cout << s << ",";
        }
        cout <<endl;
    }
    if (vm.count("interactive")){
        terminalMode = true;
    }
    RconPP client(vm["host"].as<string>(), vm["port"].as<string>(), vm["password"].as<string>());
    client.connect_host();
    // auth & commands
    if (client.rcon_auth())
    {
        if (terminalMode)
            client.run_terminal_mode();
        else
            client.run_commands(vm["command"].as<vector<string>>());
    }
    else // auth failed
        fprintf(stdout, "Authentication failed!\n");

    client.close();

    return EXIT_SUCCESS;
    return 0;
}