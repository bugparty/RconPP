//
// Created by fancy on 2019/12/27.
//

#ifndef RCON_RCONPP_H
#define RCON_RCONPP_H
#include <string>
#include <vector>
// a bit too big perhaps?
#define DATA_BUFFSIZE 10240
// rcon packet structure
#define MAX_PACKET_SIZE  (size_t) 1460 // including size member
#define MIN_PACKET_SIZE  (size_t) 10
#define MAX_STRING_SIZE  (size_t) (MAX_PACKET_SIZE - 2 - 3 * sizeof(int32_t))
#define SIZEOF_PACKET(x) (size_t) (x.size + sizeof(int32_t))
#define RCON_EXEC_COMMAND       2
#define RCON_AUTHENTICATE       3
#define RCON_RESPONSEVALUE      0
#define RCON_AUTH_RESPONSE      2
#define RCON_PID                0xBADC0DE
class RconPP {
public:
    RconPP(std::string host, std::string port, std::string password, bool raw_output = false, bool print_colors = true)
            : host(host), port(port),
              password(password), raw_output(raw_output),
              print_colors(print_colors) {}

    // rcon packet structure
    typedef struct _rc_packet {
        int size;
        int id;
        int cmd;
        char data[DATA_BUFFSIZE];
        // ignoring string2 atm.
    } rc_packet;
    struct rcon_packet
    {
        int32_t size;
        int32_t id;
        int32_t cmd;
        uint8_t data[MAX_STRING_SIZE];
    };
    int net_send_packet(int sd, rc_packet *packet);

    rc_packet *net_recv_packet(int sd);

    int net_clean_incoming(int sd, int size);

    // this hacky mess might use some optimizing
    void packet_print(rc_packet *packet);
    rc_packet *packet_build(int id, int cmd, const char *s1);
    uint8_t *packet_build_malloc(size_t *size, int32_t id, int32_t cmd, const char *string);
    void print_color(int color);
    struct rcon_packet packet_build_new(int32_t id, int32_t cmd, char *string);
    int rcon_auth();
    int run_terminal_mode();
    // gets line from stdin and deals with rubbish left in the input buffer
    int get_line(char *buffer, int bsize);
    int run_commands(std::vector<std::string> commands);
    void connect_host();
    void close();
private:
    std::string host;
    std::string port;
    std::string password;
    bool connection_alive;
    bool raw_output;
    bool print_colors;
    bool silent_mode;
    int rsock;

    int rcon_command(int rsock, const char *const command);
};


#endif //RCON_RCONPP_H
