#include<iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#define PORT 8080 

using namespace std;

int main(){
    int sockFd;
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    if(connect(sockFd, (sockaddr *)&address, sizeof(address) < 0) ){
        printf("\nConnection Failed \n"); 
        return -1;
    }

    // next step: chat system



}