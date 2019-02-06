#include<iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#define PORT 8080 

using namespace std;

void _send(int socketFd){
    string s;
    while(true){
        getline(cin, s, '\n');
        send(socketFd, s.c_str(), s.length(), 0);
    }
    
}

int main(){

    // reply will be sent from mail thread

    string read, write;

    sockaddr_in address;
    int serFd, newSerFd, valread;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    // no need to understand, just memorize
    serFd = socket(AF_INET, SOCK_STREAM, 0);
    bind(serFd, (sockaddr *)&address, sizeof(address));
    listen(serFd, 3);
    // no need to understand, just memorize

    // accepts a connection FROM SERVER
    newSerFd = accept(serFd, (sockaddr *)&address, (socklen_t *)(sizeof(address)));

        

}