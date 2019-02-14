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
#include <sstream>

#define TRUE 1
#define FALSE 0

/*
    200 	(nonstandard success response, see rfc876)
    211 	System status, or system help reply
    214 	Help message
    220 	<domain> Service ready
    221 	<domain> Service closing transmission channel
    250 	Requested mail action okay, completed
    251 	User not local; will forward to <forward-path>
    252 	Cannot VRFY user, but will accept message and attempt delivery
    354 	Start mail input; end with <CRLF>.<CRLF>
    421 	<domain> Service not available, closing transmission channel
    450 	Requested mail action not taken: mailbox unavailable
    451 	Requested action aborted: local error in processing
    452 	Requested action not taken: insufficient system storage
    500 	Syntax error, command unrecognised
    501 	Syntax error in parameters or arguments
    502 	Command not implemented
    503 	Bad sequence of commands
    504 	Command parameter not implemented
    521 	<domain> does not accept mail (see rfc1846)
    530 	Access denied (???a Sendmailism)
    550 	Requested action not taken: mailbox unavailable
    551 	User not local; please try <forward-path>
    552 	Requested mail action aborted: exceeded storage allocation
    553 	Requested action not taken: mailbox name not allowed
    554 	Transaction failed
*/

using namespace std;

void handle(int clientFd, int i){
    ostringstream str;
    str << "hello " << i << "th client";
    string s(str.str());
    send(clientFd, s.c_str(), s.size(), 0);
}

int main(){
    int socketFD, PORT, opt=1;
    char buffer[1024];
    sockaddr_in address;

    PORT = (rand()%9000)+1000;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    bind(socketFD, (sockaddr*)&address, sizeof(address));
    listen(socketFD, 10);
    int addrlen = sizeof(address);

    int i = 1;
    cout << "Server running at port " << PORT << endl;
    while(true){
        int newSockFd = accept(socketFD, (sockaddr*)&address, (socklen_t*)&addrlen);
        char *welcomeMessage = "Welcome from server, connection established";
        send(newSockFd, welcomeMessage, strlen(welcomeMessage), 0);
        // hello
        read(newSockFd, buffer, 1024);
        // mail_from
        read(newSockFd, buffer, 1024);
        // rcpt_to
        read(newSockFd, buffer, 1024);
        // data
        read(newSockFd, buffer, 1024);
        // quit
    }

}
