//
// Created by fancy on 2019/12/27.
//

#include "RconPP.hpp"
#include "sock.h"
#include <sys/socket.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <memory>
#include <iostream>

void RconPP::connect_host(){
    // open socket
    rsock = net_connect(host.c_str(), port.c_str());
}
void RconPP::close() {
    net_close(rsock);
    rsock = -1;
}
RconPP::~RconPP(){
    if(rsock!= -1){
        close();
    }
}
int RconPP::net_send_packet(int sd, rc_packet *packet)
{
    int len;
    int total = 0;	// bytes we've sent
    int bytesleft;	// bytes left to send
    int ret = -1;

    bytesleft = len = packet->size + sizeof(int);

    while(total < len)
    {
        ret = send(sd, (char *) packet + total, bytesleft, 0);
        if(ret == -1) { break; }
        total += ret;
        bytesleft -= ret;
    }

    return ret == -1 ? -1 : 1;
}
int RconPP::net_clean_incoming(int sd, int size)
{
    char tmp[size];

    int ret = recv(sd, tmp, size, 0);

    if(ret == 0)
    {
        fprintf(stderr, "Connection lost.\n");
        connection_alive = 0;
    }

    return ret;
}
int RconPP::recv_response(rc_packet &packet)
{
    int psize;

    // packet.size = packet.id = packet.cmd = 0;

    int ret = recv(rsock, (char *) &psize, sizeof(int), 0);

    if (ret == 0)
    {
        fprintf(stderr, "Connection lost.\n");
        connection_alive = 0;
        return -1;
    }

    if (ret != sizeof(int))
    {
        fprintf(stderr, "Error: recv() failed. Invalid packet size (%d).\n", ret);
        connection_alive = 0;
        return -1;
    }

    if (psize < 10 || psize > DataBufferSize)
    {
        fprintf(stderr, "Warning: invalid packet size (%d). Must over 10 and less than %d.\n", psize, DataBufferSize);

        if(psize > DataBufferSize || psize < 0) psize = DataBufferSize;
        net_clean_incoming(rsock, psize);

        return -1;
    }

    packet.size = psize;

    ret = recv(rsock, (char *) &packet + sizeof(int), psize, 0);
    if (ret == 0)
    {
        fprintf(stderr, "Connection lost.\n");
        connection_alive = 0;
        return -1;
    }

    if(ret != psize)
    {
        fprintf(stderr, "Warning: recv() return value (%d) does not match expected packet size (%d).\n", ret, psize);
        net_clean_incoming(rsock, DataBufferSize); /* Should be enough. Needs some checking */
        return -1;
    }

    return 0;
}
RconPP::rc_packet * RconPP::net_recv_packet(int sd)
{
    int psize;
    static rc_packet packet = {0, 0, 0, { 0x00 }};

    // packet.size = packet.id = packet.cmd = 0;

    int ret = recv(sd, (char *) &psize, sizeof(int), 0);

    if (ret == 0)
    {
        fprintf(stderr, "Connection lost.\n");
        connection_alive = 0;
        return NULL;
    }

    if (ret != sizeof(int))
    {
        fprintf(stderr, "Error: recv() failed. Invalid packet size (%d).\n", ret);
        connection_alive = 0;
        return NULL;
    }

    if (psize < 10 || psize > DataBufferSize)
    {
        fprintf(stderr, "Warning: invalid packet size (%d). Must over 10 and less than %d.\n", psize, DataBufferSize);

        if(psize > DataBufferSize || psize < 0) psize = DataBufferSize;
        net_clean_incoming(sd, psize);

        return NULL;
    }

    packet.size = psize;

    ret = recv(sd, (char *) &packet + sizeof(int), psize, 0);
    if (ret == 0)
    {
        fprintf(stderr, "Connection lost.\n");
        connection_alive = 0;
        return NULL;
    }

    if(ret != psize)
    {
        fprintf(stderr, "Warning: recv() return value (%d) does not match expected packet size (%d).\n", ret, psize);
        net_clean_incoming(sd, DataBufferSize); /* Should be enough. Needs some checking */
        return NULL;
    }

    return &packet;
}
void RconPP::print_color(int color)
{
    // sh compatible color array
#ifndef _WIN32
    const char * const colors[] =
            {
                    "\033[0;30m",   /* 00 BLACK     0x30 */
                    "\033[0;34m",   /* 01 BLUE      0x31 */
                    "\033[0;32m",   /* 02 GREEN     0x32 */
                    "\033[0;36m",   /* 03 CYAN      0x33 */
                    "\033[0;31m",   /* 04 RED       0x34 */
                    "\033[0;35m",   /* 05 PURPLE    0x35 */
                    "\033[0;33m",   /* 06 GOLD      0x36 */
                    "\033[0;37m",   /* 07 GREY      0x37 */
                    "\033[0;1;30m", /* 08 DGREY     0x38 */
                    "\033[0;1;34m", /* 09 LBLUE     0x39 */
                    "\033[0;1;32m", /* 10 LGREEN    0x61 */
                    "\033[0:1;36m", /* 11 LCYAN     0x62 */
                    "\033[0;1;31m", /* 12 LRED      0x63 */
                    "\033[0;1;35m", /* 13 LPURPLE   0x64 */
                    "\033[0;1;33m", /* 14 YELLOW    0x65 */
                    "\033[0;1;37m", /* 15 WHITE     0x66 */
                    "\033[4m"       /* 16 UNDERLINE 0x6e */
            };

    if(color == 0 || color == 0x72) /* 0x72: 'r' */
    {
        fputs("\033[0m", stdout); /* CANCEL COLOR */
    }
    else
#endif
    {
        if(color >= 0x61 && color <= 0x66) color -= 0x57;
        else if(color >= 0x30 && color <= 0x39) color -= 0x30;
        else if(color == 0x6e) color=16; /* 0x6e: 'n' */
        else return;

#ifndef _WIN32
        fputs(colors[color], stdout);
#else
        SetConsoleTextAttribute(console_handle, color);
#endif
    }
}
void RconPP::print_raw(rc_packet &packet){

        for (int i = 0; packet.data[i] != 0; ++i) putchar(packet.data[i]);
        return;

}
void RconPP::print(rc_packet &packet, const std::string command)
{
    int i;
    for(i=0;i<packet.size && packet.data[i]!='\0';i++){
        putchar(packet.data[i]);
    }

    if (packet.data[i-1] != '\n' && packet.data[i-1] != '\r')
        puts("\r\n");
}
// this hacky mess might use some optimizing
void RconPP::packet_print(rc_packet *packet)
{
    if (raw_output == 1)
    {
        for (int i = 0; packet->data[i] != 0; ++i) putchar(packet->data[i]);
        return;
    }

    int i;
    int def_color = 0;

#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO console_info;

	if (GetConsoleScreenBufferInfo(console_handle, &console_info) != 0)
		def_color = console_info.wAttributes + 0x30;
	else
		def_color = 0x37;
#endif

    // colors enabled so try to handle the bukkit colors for terminal
    if (print_colors == 1)
    {
        for (i = 0; (unsigned char) packet->data[i] != 0; ++i)
        {
            if (packet->data[i] == 0x0A) print_color(def_color);
            else if((unsigned char) packet->data[i] == 0xc2 && (unsigned char) packet->data[i+1] == 0xa7){
                print_color(packet->data[i+=2]);
                continue;
            }
            putchar(packet->data[i]);
        }
        print_color(def_color); // cancel coloring
    }
        // strip colors
    else
    {
        for (i = 0; (unsigned char) packet->data[i] != 0; ++i)
        {
            if ((unsigned char) packet->data[i] == 0xc2 && (unsigned char) packet->data[i+1] == 0xa7){
                i+=2;
                continue;
            }
            putchar(packet->data[i]);
        }
    }

    // print newline if string has no newline
    if (packet->data[i-1] != 10 && packet->data[i-1] != 13)
        putchar('\n');
}

RconPP::rc_packet *RconPP::packet_build(int id, int cmd, const char *s1)
{
    static rc_packet packet = {0, 0, 0, { 0x00 }};

    // size + id + cmd + s1 + s2 NULL terminator
    int s1_len = strlen(s1);
    if (s1_len > DataBufferSize)
    {
        fprintf(stderr, "Warning: Command string too long (%d). Maximum allowed: %d.\n", s1_len, DataBufferSize);
        return NULL;
    }

    packet.size = sizeof(int) * 2 + s1_len + 2;
    packet.id = id;
    packet.cmd = cmd;
    strncpy(packet.data, s1, DataBufferSize);

    return &packet;
}

// TODO(Tiiffi): String length limit?
uint8_t *RconPP::packet_build_malloc(size_t &size, int32_t id, int32_t cmd, const char *string)
{
    size_t string_length = strlen(string);

    size = 3 * sizeof(int32_t) + string_length + 2;
    uint8_t * packet = (uint8_t *)malloc(size);
    if (packet == NULL) return NULL;

    int32_t *p = (int32_t *) packet;
    p[0] = (int32_t) size - sizeof(int32_t);
    p[1] = id;
    p[2] = cmd;

    memcpy(&p[3], string, string_length);

    packet[12 + string_length] = 0;
    packet[13 + string_length] = 0;

    return packet;
}

struct RconPP::rcon_packet RconPP::packet_build_new(int32_t id, int32_t cmd, char *string)
{
    struct RconPP::rcon_packet packet;
    size_t string_length = strlen(string);

    if (string_length > MaxStringSize)
    {
        string_length = MaxStringSize;
        fprintf(stderr,
                "Warning: command string is too long. Truncating to "
                "%u characters.\n", (unsigned) MaxStringSize
        );
    }

    packet.size = 2 * sizeof(int32_t) + string_length + 2;
    packet.id = id;
    packet.cmd = cmd;
    memcpy(packet.data, string, string_length);
    packet.data[string_length] = 0;
    packet.data[string_length + 1] = 0;

    return packet;
}

int RconPP::rcon_auth()
{
    int ret;

    rc_packet *packet = packet_build(RconPid, RconAuthenticate, password.c_str());
    if (packet == NULL)
        return 0;

    ret = net_send_packet(rsock, packet);
    if (!ret)
        return 0; // send failed

    packet = net_recv_packet(rsock);
    if (packet == NULL)
        return 0;

    // return 1 if authentication OK
    return packet->id == -1 ? 0 : 1;
}

int RconPP::rcon_command(int rsock, const std::string command)
{
    size_t size;
    std::unique_ptr<unsigned char> p (packet_build_malloc(size, RconPid, RconExecuteCmd, command.c_str()));

    if (p.get() == NULL)
    {
        fprintf(stderr, "%s", "packet build failed");
        connection_alive = 0;
        return 0;
    }
    net_send(rsock, p.get(), size);

    //ret = net_send_packet(rsock, packet);
    //if(!ret) return 0; /* send failed */
    rc_packet packet;
    int status = recv_response(packet);
    if ( status != 0)
        return 0;

    if (packet.id != RconPid)
        return 0;

    if (!silent_mode)
    {
        /*
        if(packet->size == 10) {
            printf("Unknown command \"%s\". Type \"help\" or \"?\" for help.\n", command);
        }
        else
        */
        if (packet.size > 10)
            if(raw_output){
                print_raw(packet);
            }else{
                print(packet, command.c_str());
            }

    }

    return 1;
}

int RconPP::run_commands(std::vector<std::string> commands)
{
    int ok = 1;
    int ret = 0;
    for (auto &command: commands){
        ok = rcon_command(rsock, command.c_str());
        ret += ok;
    }

    return ret;
}

// interactive terminal mode
int RconPP::run_terminal_mode()
{
    int ret = 0;
    char command[DataBufferSize] = {0x00};

    puts("Logged in. Type \"Q\" to quit!");

    while (connection_alive)
    {
        int len = get_line(command, DataBufferSize);
        if(command[0] == 'Q' && command[1] == 0)
            break;

        if(len > 0 && connection_alive)
            ret += rcon_command(rsock, command);

        command[0] = len = 0;
    }

    return ret;
}

// gets line from stdin and deals with rubbish left in the input buffer
int RconPP::get_line(char *buffer, int bsize)
{
    int ch, len;

    fputs(">", stdout);
    (void) fgets(buffer, bsize, stdin);

    if (buffer[0] == 0)
        connection_alive = 0;

    // remove unwanted characters from the buffer
    buffer[strcspn(buffer, "\r\n")] = '\0';

    len = strlen(buffer);

    // clean input buffer if needed
    if (len == bsize - 1)
        while ((ch = getchar()) != '\n' && ch != EOF);

    return len;
}


