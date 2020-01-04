//
// Created by fancy on 2019/12/28.
//

#ifndef RCON_SOCK_H
#define RCON_SOCK_H

#include <cstddef>
#include <cstdint>

void net_close(int sd);
int net_connect(const char *host, const char *port);
int net_send(int sd, const uint8_t *buff, size_t size);
bool is_bigendian(void);
int32_t reverse_int32(int32_t n);
#endif //RCON_SOCK_H
