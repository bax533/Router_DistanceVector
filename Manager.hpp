#include"Common.hpp"
#include<map>
#include<iostream>

class Manager
{
public:
    
    Manager();
 
    void init();   
    void run();
private:

    void send_current_table();
    void display_current_table();
    int check_turn();//bool

    //ip (broadcast), mask -> distance, ip
    std::map<std::pair<ip_t, mask_t>, std::pair<dist_t, ip_t> > routing_table;
    clock_t timer;
};
