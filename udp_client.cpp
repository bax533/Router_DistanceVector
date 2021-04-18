#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "Common.hpp"

int main(int argc, char** argv)
{
    char* ip_send;
    if(argc == 2)
    {
        ip_send = argv[1];
        if(!Common::check_input(ip_send))
        {
            printf("wrong input\n");
            return 0;
        }
    }
    else
    {
        printf("podaj ip\n");
        return 0;
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
            fprintf(stderr, "socket error: %s\n", strerror(errno)); 
            return EXIT_FAILURE;
    }

    int broadcastPermission = 1;
    setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST,
            (void *)&broadcastPermission,
            sizeof(broadcastPermission));

    struct sockaddr_in server_address;
    bzero (&server_address, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(54321);
    inet_pton(AF_INET, ip_send, &server_address.sin_addr);

    char* message = "Hello server!lsdkfjlsdkjflsdkfjldskfjlsdkfjldskfjlsdkjflsdkfjlsdkfjlsdkjflsdkfjlsdkjflksdjflksdjflkjdslkfjsldkfjslkd";
    ssize_t message_len = strlen(message);
    if (sendto(sockfd, message, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len) {
            fprintf(stderr, "sendto error: %s\n", strerror(errno)); 
            return EXIT_FAILURE;		
    }

    close (sockfd);
    return EXIT_SUCCESS;
}
	
