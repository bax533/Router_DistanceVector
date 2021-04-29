#include"Manager.hpp"

Manager::Manager()
{
}


int Manager::init()
{

    Common::timer = std::chrono::steady_clock::now();

    int n;
    std::cin>>n;
    for(int i=0; i<n; i++)
    {
        std::string ip_with_mask, garbage;
        u_int32_t distance;

        std::cin>>ip_with_mask>>garbage>>distance;
        struct IP ip_input;
        Common::set_ip(ip_with_mask.c_str(), &ip_input);
        this->neighbours.push_back(ip_input.broadcast);

        #ifdef DEBUG
        std::cout<<ip_input.mask<<" mask\n"<<ip_input.network<<" network\n";
        #endif

        //u_int32_t ip_uint32 = Common::ip_to_uint32(ip_input.broadcast);
        this->routing_table[ip_input] = 
            std::pair<u_int32_t, u_int32_t>(distance, 0);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
            fprintf(stderr, "socket error: %s\n", strerror(errno));
            return 0;
    }

    struct sockaddr_in server_address;
    bzero (&server_address, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(54321);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind (sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        return 0;
    }
    return 1;
}

void Manager::run()
{
    display_current_table();

    while(true)
    {
        get_response();

        if(check_turn())
        {
            send_current_table();
        }
    }
}

void Manager::get_response()
{
    struct sockaddr_in 	sender;	
    socklen_t   	sender_len = sizeof(sender);
    int8_t 		buffer[IP_MAXPACKET+1];

    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sockfd, &descriptors);
    struct timeval tv; tv.tv_sec = 1; tv.tv_usec = 0;

    
    int cur = select(sockfd+1, &descriptors, NULL, NULL, &tv);

    if(cur <= 0)
        return;

    ssize_t datagram_len = recvfrom (sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
    if (datagram_len < 0) {
            fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
            return;
    }

    char sender_ip_str[20]; 
    inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
    
    //printf ("Received UDP packet from IP address: %s, port: %d\n", sender_ip_str, ntohs(sender.sin_port));

    buffer[datagram_len] = 0;
        
    interpret_message(buffer, sender_ip_str);
    //printf ("%ld-byte message: +%s+\n", datagram_len, buffer);

    char* reply = "Thank you!";
    ssize_t reply_len = strlen(reply);
    if (sendto(sockfd, reply, reply_len, 0, (struct sockaddr*)&sender, sender_len) != reply_len) {
            //fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
            return;		
    }
    fflush(stdout);

}

void Manager::send_current_table()
{
    int sockfd_client = socket(AF_INET, SOCK_DGRAM, 0);
     
    if (sockfd_client < 0) {
            fprintf(stderr, "socket error: %s\n", strerror(errno)); 
            return;
    }

    int broadcastPermission = 1;
    setsockopt (sockfd_client, SOL_SOCKET, SO_BROADCAST,
            (void *)&broadcastPermission,
            sizeof(broadcastPermission));
    for(const char* ip_send : this->neighbours)
    {
        for(auto const& [key, val] : this->routing_table)
        {
            dist_t distance = val.first;

            struct sockaddr_in server_address;
            bzero (&server_address, sizeof(server_address));
            server_address.sin_family      = AF_INET;
            server_address.sin_port        = htons(54321);
            inet_pton(AF_INET, ip_send, &server_address.sin_addr);

            
            char* msg = (char*)malloc(9*sizeof(char));
            this->prepare_message(key, distance, msg);
            ssize_t message_len = strlen(msg);

            if (sendto(sockfd_client, msg, 9, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len
                    && !strcmp("Success", strerror(errno))) {
                    //fprintf(stderr, "sendto error: %s\n", strerror(errno));		
            }
        }
    }

    close (sockfd_client);
    //printf("CONSIDER IT SENT\n");
    return;
}

void Manager::interpret_message(int8_t* msg, char* sender)
{
    unsigned char ip_b1 = msg[0];
    unsigned char ip_b2 = msg[1];
    unsigned char ip_b3 = msg[2];
    unsigned char ip_b4 = msg[3];

    mask_t mask = msg[4];

    char dist_b1 = msg[5];
    char dist_b2 = msg[6];
    char dist_b3 = msg[7];
    char dist_b4 = msg[8];

    ip_t ip = ip_b1<<24 | ip_b2<<16 | ip_b3<<8 | ip_b4;
    
    dist_t dist = dist_b1<<24 | dist_b2<<16 | dist_b3<<8 | dist_b4;

 
    //printf("\n ----INTERPRETTING:\n%s <- ip\n%d <- mask\n%d <- dist\n", Common::ip_to_char(ip), mask, dist);
    
    struct Response* response = (struct Response*)malloc(sizeof(struct Response));
    response->network = Common::ip_to_char(ip);
    response->mask = mask;
    response->via = sender;
    response->distance = dist;

    this->responses.push_back(response);
}

char* Manager::prepare_message(const struct IP &ip_addr, dist_t distance, char* &msg)
{
    //printf("before: %s\n", ip_addr.network);
    ip_t ip_network_int = Common::ip_to_uint32(ip_addr.network);

    mask_t mask = ip_addr.mask;

    const unsigned char ip_b1 = (ip_network_int & 0xFF000000)>>24;
    const unsigned char ip_b2 = (ip_network_int & 0xFF0000)>>16;
    const unsigned char ip_b3 = (ip_network_int & 0xFF00)>>8;
    const unsigned char ip_b4 = ip_network_int & 0xFF;

    const char dist_b1 = (distance & 0xFF000000)>>24;
    const char dist_b2 = (distance & 0xFF0000)>>16;
    const char dist_b3 = (distance & 0xFF00)>>8;
    const char dist_b4 = distance & 0xFF;

    ip_t ip = ip_b1<<24 | ip_b2<<16 | ip_b3<<8 | ip_b4;
    dist_t dist = dist_b1<<24 | dist_b2<<16 | dist_b3<<8 | dist_b4;
    
    //printf("\n ----PREPARING TO SEND:\n%s <- ip\n%d <- mask\n%d <- dist\n", Common::ip_to_char(ip), mask, dist);

    sprintf(msg, "%c%c%c%c%c%c%c%c%c", ip_b1, ip_b2, ip_b3, ip_b4, mask, dist_b1, dist_b2, dist_b3, dist_b4);

    return msg; 
}

void Manager::display_current_table()
{
    printf("Routing Table:\n");
    
    for(auto const& [key, val] : this->routing_table)
    {
        //Common::dbg_ip(key);

        printf("%s/%d ",key.network,key.mask);

        if(val.first == Common::INF)
            printf("unreachable ");
        else
            printf("%d ", val.first);

        if(val.second == 0)
            printf("connected directly\n\n");
        else
            printf("via %s\n\n",Common::ip_to_char(val.second));
    }


    return;
}

void Manager::update_table()
{
    for(const Response* resp : this->responses)
    {
        struct IP ip;
        ip.mask = resp->mask;
        ip.network = resp->network;
        ip.broadcast = Common::get_broadcast(resp->network, resp->mask);
        
        if(this->routing_table.find(ip) != this->routing_table.end())
        {
            auto val = this->routing_table[ip];
            if(resp->distance < val.first)
            {
                this->routing_table[ip] = std::pair<dist_t, ip_t>(
                    resp->distance,
                    Common::ip_to_uint32(resp->via)
                );
            }
        }
        else{
            this->routing_table[ip] = std::pair<dist_t, ip_t>(
                resp->distance,
                Common::ip_to_uint32(resp->via)
            );
        }
    }
    
    std::vector<struct IP> unreachable;
    for(auto const& [key, val] : this->routing_table)
    {
        int in_response = 0;
        for(const Response* resp : this->responses)
        {
            if(!strcmp(resp->network, key.network) && resp->mask==key.mask)
            {
                in_response = 1;
                break; 
            }    
        }
        if(!in_response)
            unreachable.push_back(key);
        else
            this->unreachable_count[key] = 0;
    }
    for(auto const key : unreachable)
    {
        if(this->unreachable_count.find(key) != this->unreachable_count.end())
        {
            this->unreachable_count[key] += 1;
            if(this->unreachable_count[key] >= 3)
                this->routing_table[key].first = Common::INF;
        }
        else
            this->unreachable_count[key] = 1; 
    }
    this->responses.clear();
}

int Manager::check_turn()
{
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end-Common::timer;
    double passed = elapsed.count();

    if(passed >= Common::turn_time)
    {
        Common::timer = std::chrono::steady_clock::now();
        update_table();
        display_current_table();
        return 1;
    }
    else
    {
        return 0;
    }
}
