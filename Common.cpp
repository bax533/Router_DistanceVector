#include "Common.hpp"

int Common::turn_time = 5;
u_int32_t Common::INF = 4294967295; 
std::chrono::time_point<std::chrono::steady_clock> Common::timer = std::chrono::steady_clock::now();

void Common::dbg_ip(struct IP addr)
{
    printf("%s <- network\n%s <- broadcast\n%u <- mask\n", addr.network, addr.broadcast, addr.mask);
}

char* Common::get_IPV4(u_int32_t address)
{
    unsigned char b1 = (address & 0xFF000000)>>24;
    unsigned char b2 = (address & 0xFF0000)>>16;
    unsigned char b3 = (address & 0xFF00)>>8;
    unsigned char b4 = address & 0xFF;

    char* res = (char*)malloc(4 + 3 + 1);
    sprintf(res, "%u.%u.%u.%u", b1, b2, b3, b4);
    return res;
}

//return ip without mask from address
char* Common::set_ip(const char* address, struct IP* src)
{
    std::string tmp = "";
    int it = 0;
    while(address[it] != '/')
    {
        tmp += address[it];
        it++;
    }

    it++;
    int32_t mask = 0;

    while(address[it] != '\0')
    {
        int32_t add = address[it] - '0';
        mask = mask*10 + add;
        it++;
    }



    #ifdef DEBUG
    std::cout<<mask<<" <- int32_t mask in Common::set_ip\n";
    std::cout<<(mask_t)mask<<" <- mask parsed to int8 same function\n";
    #endif

    u_int32_t mask_bit = 0;
    for(int i=0; i<32-(mask); i++)
        mask_bit = (mask_bit << 1) | 1;

    struct sockaddr_in sa;
    char* str = new char[INET_ADDRSTRLEN];

    inet_pton(AF_INET, tmp.c_str(), &(sa.sin_addr));
    u_int32_t ip = (u_int32_t)sa.sin_addr.s_addr;
    
    ip = SWAP_UINT32(ip);
    ip = ip | mask_bit;

    inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
    

    src->broadcast = Common::get_IPV4(ip);
    src->mask = (mask_t)mask;
    src->network = Common::get_IPV4(ip & (~mask_bit));

    return str;
}

char* Common::int32_to_char(const dist_t address)
{
    char* ret = new char[5];
    for (u_int32_t i = 0; i < 4; i++)
        ret[3 - i] = (address >> (i * 8));
    
    
    return ret;
}

u_int32_t Common::ip_to_uint32(const char* address)
{
    struct sockaddr_in sa;
    inet_pton(AF_INET, address, &(sa.sin_addr));
    u_int32_t ip = (u_int32_t)sa.sin_addr.s_addr;

    return ip;
}

char* Common::ip_to_char(const u_int32_t address)
{
    char* str = new char[INET_ADDRSTRLEN];
    long tmp_address = (long)address;
    inet_ntop(AF_INET, &tmp_address, str, INET_ADDRSTRLEN);
    return str;
}

char* Common::get_broadcast(const char* address, mask_t mask)
{
    ip_t ip_int = Common::ip_to_uint32(address);
    ip_int = SWAP_UINT32(ip_int);
    u_int32_t mask_bit = 0;
    for(int i=0; i<32-(mask); i++)
        mask_bit = (mask_bit << 1) | 1;
    ip_int = ip_int | mask_bit;
    ip_int = SWAP_UINT32(ip_int);
    return Common::ip_to_char(ip_int);
}
