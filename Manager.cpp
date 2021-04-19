#include"Manager.hpp"

Manager::Manager()
{
    this->timer = clock();
}


void Manager::init()
{
    int n;
    std::cin>>n;
    for(int i=0; i<n; i++)
    {
        std::string ip_with_mask, garbage;
        u_int32_t distance;

        std::cin>>ip_with_mask>>garbage>>distance;
        struct IP ip_input;
        Common::set_ip(ip_with_mask.c_str(), &ip_input);

        #ifdef DEBUG
        std::cout<<ip_input.mask<<" mask\n";
        #endif

        u_int32_t ip_uint32 = Common::ip_to_uint32(ip_input.broadcast);
        this->routing_table[std::pair<u_int32_t, int8_t>(ip_uint32, ip_input.mask)] = 
            std::pair<u_int32_t, u_int32_t>(distance, 0);
    }
}

void Manager::run()
{
    display_current_table();

    std::string dupa;

    while(true)
    {
        std::cin>>dupa;
        std::cout<<dupa<<" "<<dupa;

        if(dupa == "exit")
            return;
    }
}

void Manager::send_current_table()
{

    return;
}

void Manager::display_current_table()
{

    for(auto const& [key, val] : this->routing_table)
    {
        std::cout<<Common::ip_to_char(key.first)<<"/"<<key.second<<" "<<val.first<<" ";
        if(val.second == 0)
            std::cout<<"connected directly\n";
        else
            std::cout<<val.second<<"\n";
    }


    return;
}

int Manager::check_turn()
{
    double passed = (clock() - this->timer)/CLOCKS_PER_SEC;
    if(passed >= Common::turn_time)
    {
        this->timer = clock();
        return 1;
    }
    else
    {
        return 0;
    }
}
