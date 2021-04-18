#include "Common.hpp"

int Common::check_input(const char* in)
{
    char c = in[0];
    int it = 0;
    int dots = 0;

    while(c != '\0')
    {
        if(!isdigit(c))
        {
            if(c != '.')
                return 0;
            else
                dots += 1;
        }
        it+=1;
        c = in[it];
    }

    if(it > 15)
        return 0;
    if(dots != 3)
        return 0;

    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, in, &(sa.sin_addr));
    if(result != 0)
        return 1;
    else
        return 0;
}

char* Common::get_IPV4(u_int32_t address)
{
    unsigned char b1 = (address & 0xFF000000)>>24;
    unsigned char b2 = (address & 0xFF0000)>>16;
    unsigned char b3 = (address & 0xFF00)>>8;
    unsigned char b4 = address & 0xFF;

    char* res = (char*)malloc(4*3 + 3 + 1);
    sprintf(res, "%u.%u.%u.%u", b4, b3, b2, b1);
    return res;
}

