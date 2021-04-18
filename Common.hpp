#pragma once

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <time.h>


class Common{
public:
    static int check_input(const char* in);
    static char* get_IPV4(u_int32_t address);
};
