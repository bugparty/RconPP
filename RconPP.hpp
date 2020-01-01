//
// Created by fancy on 2019/12/27.
//

#ifndef RCON_RCONPP_HPP
#define RCON_RCONPP_HPP
#include <string>
#include <vector>


class RconPP {
public:

    RconPP(std::string host, std::string port, std::string password, bool raw_output = false, bool print_colors = true)
            : host(host), port(port),
              password(password), raw_output(raw_output),
              print_colors(print_colors) {}

    static const size_t MaxPacketSize  = 1460; // including size member
    static const size_t MinPacketSize  = 10;
    static const size_t MaxStringSize   =  (MaxPacketSize - 2 - 3 * sizeof(int32_t));
    // a bit too big perhaps?
    static const int  DataBufferSize    =     10240;
    static const int RconExecuteCmd     =    2;
    static const int RconAuthenticate     =    3;
    static const int RconResponseValue    =    0;
    static const int RconPid              =    0xBADC0DE;
    static const int RconAuthResponse    =    2;
    // rcon packet structure
    typedef struct _rc_packet {
        int size;
        int id;
        int cmd;
        char data[DataBufferSize];
        // ignoring string2 atm.
    } rc_packet;
    struct rcon_packet
    {
        int32_t size;
        int32_t id;
        int32_t cmd;
        uint8_t data[MaxStringSize];
    };

    int net_clean_incoming(int sd, int size);
    // this hacky mess might use some optimizing
    void packet_print(rc_packet *packet);
    void print_color(int color);
    int rcon_auth();
    int run_terminal_mode();
    int run_commands(std::vector<std::string> commands);
    void connect_host();
    void close();
    virtual ~RconPP();

protected:
    std::string host;
    std::string port;
    std::string password;
    bool connection_alive = true;
    bool raw_output;
    bool print_colors;
    bool silent_mode;
    int rsock;
    int net_send_packet(int sd, rc_packet *packet);

    rc_packet *net_recv_packet(int sd);
    // gets line from stdin and deals with rubbish left in the input buffer
    int get_line(char *buffer, int bsize);
    int rcon_command(int rsock, const std::string command);
    void print_raw(rc_packet &packet);
    int recv_response(rc_packet &packet);
    virtual void print(rc_packet &packet, const std::string command);
    rc_packet *packet_build(int id, int cmd, const char *s1);
    uint8_t *packet_build_malloc(size_t *size, int32_t id, int32_t cmd, const char *string);
    struct rcon_packet packet_build_new(int32_t id, int32_t cmd, char *string);
};


#endif //RCON_RCONPP_HPP
