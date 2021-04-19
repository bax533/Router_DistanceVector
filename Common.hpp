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


#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

#define DEBUG

typedef u_int32_t ip_t;
typedef u_int32_t dist_t;
typedef int8_t mask_t;


struct IP
{
    char* broadcast;
    char* ip;
    int8_t mask;
};



class Common{
public:
    static int check_input(const char* in);
    static char* get_IPV4(u_int32_t address);
    static void set_ip(const char* address, struct IP* src);
    static u_int32_t ip_to_uint32(const char* address);
    static char* ip_to_char(const u_int32_t address);
    

    static int turn_time;
    
};
