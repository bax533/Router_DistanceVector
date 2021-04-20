#include"Common.hpp"
#include<map>
#include<iostream>
#include<string.h>
#include<chrono>
#include<vector>
#include<climits>

class Manager
{
public:
    
    Manager();
 
    int init();   
    void run();
private:

    void send_current_table();
    void display_current_table();
    void get_response();
    char* prepare_message(const struct IP &ip_addr, dist_t distance, char* &msg);
    void interpret_message(int8_t* msg, char* sender);
    void update_table();

    int check_turn();//bool
        
    //ip (broadcast), mask -> distance, ip
    std::map<struct IP, std::pair<dist_t, ip_t> > routing_table;
    int sockfd;
    std::vector<Response*> responses;
    std::vector<char*> neighbours;
};
