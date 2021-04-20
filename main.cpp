#include "Common.hpp"
#include "Manager.hpp"

std::map<struct IP, int> test_map;

int main()
{
    Common::timer = std::chrono::steady_clock::now();
    Manager manager = Manager();

    /* struct IP ip1, ip2; */
    /* ip1.network = "127.0.0.0"; */
    /* ip1.mask = 31; */
    /* ip1.broadcast = "127.0.0.1"; */

    /* ip2.network = "127.0.0.0"; */
    /* ip2.mask = 31; */
    /* ip2.broadcast = "127.0.0.1"; */

    /* test_map[ip1]=1; */
    /* test_map[ip2]=2; */

    /* for(const auto [key, val] : test_map) */
    /*     printf("%d\n", val); */

  //  printf(Common::get_broadcast(ip1.network, ip1.mask));
    
    /* printf("%d", INET_ADDRSTRLEN); */
    /* while(true) */
    /* { */
    /*     char test_ip[16];// = new char[INET_ADDRSTRLEN]; */
    /*     mask_t mask; */
    /*     scanf("%s%d", &test_ip, &mask); */

    /*     printf("%s <- broadcast\n\n", Common::get_broadcast(test_ip, mask)); */
    /* } */
    
    
    
    if(manager.init())
        manager.run();
    else
        printf("init problem\n");
}
