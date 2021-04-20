#pragma once

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <cstring>

#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

#define DEBUG

typedef u_int32_t ip_t;
typedef u_int32_t dist_t;
typedef int8_t mask_t;


struct IP
{
    char* network;
    char* broadcast;
    int8_t mask;
    
    int operator==(const IP &o) const{
        return mask==o.mask && !strcmp(network, o.network);
    }
    bool operator<(const IP &o) const{
        if(mask != o.mask)
            mask < o.mask;
        else if(strcmp(network, o.network))
            return network<o.network;
        else return false;
    }
};


struct Response
{
    char* network;
    mask_t mask;
    dist_t distance;
    char* via;
};

class Common{
public:
    static int check_input(const char* in);
    static char* get_IPV4(u_int32_t address);
    static char* set_ip(const char* address, struct IP* src);
    static u_int32_t ip_to_uint32(const char* address);
    static char* ip_to_char(const u_int32_t address);
    static char* int32_to_char(const dist_t address);
    static char* get_broadcast(const char* address, mask_t mask);
    static void dbg_ip(struct IP addr);

    static std::chrono::time_point<std::chrono::steady_clock> timer;
    static int turn_time;
    static u_int32_t INF;    
};
