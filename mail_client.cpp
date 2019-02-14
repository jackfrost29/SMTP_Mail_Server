#include <vector>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>

using namespace std;


int main(int argc, char const *argv[]){
    sockaddr_in address, servAddress;
    int client, PORT;
    char buffer[1024];
    cout << "Type the server port number: ";
    cin >> PORT;

    client = socket(AF_INET, SOCK_STREAM, 0);
    servAddress.sin_family = AF_INET;
    servAddress.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &servAddress.sin_addr);

    connect(client, (sockaddr*)&servAddress, sizeof(servAddress));
    read(client, buffer, 1024);
    printf("From server: %s\n", buffer);

    // hello
    // mail_from
    // rcpt_to
    // data
    // quit

    close(client);
}
