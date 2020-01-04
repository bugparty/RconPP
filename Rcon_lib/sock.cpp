//
// Created by fancy on 2019/12/28.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#ifdef _WIN32
// for name resolving on windows
    // enable this if you get compiler whine about getaddrinfo on windows
    //#define _WIN32_WINNT 0x0501

    #include <ws2tcpip.h>
    #include <winsock2.h>
    #include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

// socket close and cleanup
void net_close(int sd)
{
#ifdef _WIN32
    closesocket(sd);
	WSACleanup();
#else
    close(sd);
#endif
}

// Opens and connects socket
// http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
// https://bugs.chromium.org/p/chromium/issues/detail?id=44489
int net_connect(const char *host, const char *port)
{
    int sd;

    struct addrinfo hints;
    struct addrinfo *server_info, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

#ifdef _WIN32
    net_init_WSA();
#endif

    int ret = getaddrinfo(host, port, &hints, &server_info);
    if (ret != 0)
    {
        fprintf(stderr, "Name resolution failed.\n");
#ifdef _WIN32
        fprintf(stderr, "Error %d: %s", ret, gai_strerror(ret));
#else
        fprintf(stderr, "Error %d: %s\n", ret, gai_strerror(ret));
#endif
        exit(EXIT_FAILURE);
    }

    // Go through the hosts and try to connect
    for (p = server_info; p != NULL; p = p->ai_next)
    {
        sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sd == -1)
            continue;

        ret = connect(sd, p->ai_addr, p->ai_addrlen);
        if (ret == -1)
        {
            net_close(sd);
            continue;
        }
        // Get out of the loop when connect is successful
        break;
    }

    if (p == NULL)
    {
        /* TODO (Tiiffi): Check why windows does not report errors */
        fprintf(stderr, "Connection failed.\n");
#ifndef _WIN32
        fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
#endif
        freeaddrinfo(server_info);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(server_info);

    return sd;
}
int net_send(int sd, const uint8_t *buff, size_t size)
{
    size_t sent = 0;
    size_t left = size;

    while (sent < size)
    {
        int result = send(sd, (const char *) buff + sent, left, 0);

        if (result == -1) return -1;

        sent += result;
        left -= sent;
    }

    return 0;
}
// http://www.ibm.com/developerworks/aix/library/au-endianc/
bool is_bigendian(void)
{
    const int32_t n = 1;
    if (*(uint8_t *) &n == 0 ) return true;
    return false;
}

int32_t reverse_int32(int32_t n)
{
    int32_t tmp;
    uint8_t *t = (uint8_t *) &tmp;
    uint8_t *p = (uint8_t *) &n;

    t[0] = p[3];
    t[1] = p[2];
    t[2] = p[1];
    t[3] = p[0];

    return tmp;
}

