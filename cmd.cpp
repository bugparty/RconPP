//
// Created by fancy on 2019/12/27.
//

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
namespace po = boost::program_options;
using  boost::lexical_cast;
using namespace std;
int main(int argc, const char* argv[]){
    po::options_description desc( "Usage: rcon [OPTIONS]... [COMMANDS]...\n\n"
                                                  "Sends rcon commands to game server.\n\n"
                                                  "Option:");
    desc.add_options()
            ("help,h", "Print usage")
            ("host,H", po::value<std::string>(), "Server address")
            ("port,P", po::value<int>(), "Port (default is 27102)")
            ("password,p", po::value<std::string>(), "Rcon password")
            ("interactive,t", "Interactive terminal mode")
            ("silent,s", "Silent mode (do not print received packets)")
            ("nocolor,c", "Disable colors")
            ( "raw,r", "Output raw packets (debugging and custom handling)")
            (  "version,v", "Output version information");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);


    char *env_host = getenv("SDRCON_HOST");
    char *env_pass = getenv("SDRCON_PASS");
    char *env_port = getenv("SDRCON_PORT");
    string host;
    string password;
    int port = -1;
    if(env_host){
        host.assign(env_host);
    }
    if(env_pass){
        password.assign(env_pass);
    }
    if(env_port){
        try{
            port = lexical_cast<int>(env_port);
        }catch(boost::bad_lexical_cast){

        }
    }

    if (vm.count("help") || vm.size()  < 2 ) {
        cout << desc << "\n";
        return 1;
    }

    return 0;
}