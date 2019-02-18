#include <fstream>
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

const char * __200 = "200 (nonstandard success response, see rfc876)";
const char * __211 = "211 System status, or system help reply";
const char * __214 = "214 Help message";
const char * __220 = "220 <domain> Service ready";
const char * __221 = "221 <domain> Service closing transmission channel";
const char * __250 = "250 Requested mail action okay, completed";
const char * __251 = "251 User not local; will forward to <forward-path>";
const char * __252 = "252 Cannot VRFY user, but will accept message and attempt delivery";
const char * __354 = "354 Start mail input; end with <CRLF>.<CRLF>";
const char * __421 = "421 <domain> Service not available, closing transmission channel";
const char * __450 = "450 Requested mail action not taken: mailbox unavailable";
const char * __451 = "Requested action aborted: local error in processing";
const char * __452 = "452 Requested action not taken: insufficient system storage";
const char * __500 = "500 Syntax error, command unrecognised";
const char * __501 = "501 Syntax error in parameters or arguments";
const char * __502 = "502 Command not implemented";
const char * __503 = "503 Bad sequence of commands";
const char * __504 = "504 Command parameter not implemented";
const char * __521 = "521 <domain> does not accept mail (see rfc1846)";
const char * __530 = "530 Access denied (???a Sendmailism)";
const char * __550 = "550 Requested action not taken: mailbox unavailable";
const char * __551 = "551 User not local; please try <forward-path>";
const char * __552 = "552 Requested mail action aborted: exceeded storage allocation";
const char * __553 = "553 Requested action not taken: mailbox name not allowed";
const char * __554 = "554 Transaction failed";

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

string toUser(string s){    // not yet implemented
    // argument was RCPT TO user@host
    // extracts the user out of argument

    s.erase(0, 8);

    ostringstream os;
    for(int i=0; s[i] != '@'; i++)
        os << s[i];

    return os.str();
}

string toHost(string s){
    // argument was user@host:PORT
    // extracts the host out of argument

    ostringstream os;
    bool flag = false;
    for(int i=0; s[i] != ':'; i++){
        if(flag==true)
            os << s[i];
        if(s[i] == '@')
            flag = true;
    }

    return os.str();
}

int main(){
    int socketFD, PORT, opt=1;
    char buffer[1024];
    string input, output, user;
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
        char welcomeMessage[] = "Welcome from server, connection established";
        send(newSockFd, welcomeMessage, strlen(welcomeMessage), 0);
        // hello: if hello req then ok(200), or systax error(500)
        read(newSockFd, buffer, 1024);
        input = string(buffer);
        if(input.substr(0, 4) == "HELO"){
            output = __200;
            send(newSockFd, output.c_str(), output.size(), 0);
        }
        else{
            output = __500;
            send(newSockFd, output.c_str(), output.size(), 0);
            continue;
        }

        // mail_from
        read(newSockFd, buffer, 1024);
        input = string(buffer);
        if(input.substr(0, 9) == "MAIL FROM"){
            output = __200;
            send(newSockFd, output.c_str(), output.size(), 0);
        }
        else{
            output = __500;
            send(newSockFd, output.c_str(), output.size(), 0);
            continue;
        }
        // rcpt_to
        read(newSockFd, buffer, 1024);
        input = string(buffer);
        if(input.substr(0, 7) != "RCPT TO"){
            output = __500;
            send(newSockFd, output.c_str(), output.size(), 0);
            continue;
        }
        else
        {
            user = toUser(input);
            user = user + ".txt";
            ifstream file(user.c_str());
            if(!file.good()){   // user doesnot exixts
                output = __450;
                send(newSockFd, output.c_str(), output.size(), 0);
                continue;
            }
            else
            {
                // user exists, you may proceed
                output = __250;
                send(newSockFd, output.c_str(), output.size(), 0);
            }
            
        }
        
        // data
        read(newSockFd, buffer, 1024);
        input = string(buffer);
        if(input.substr(0, 4) == "DATA"){
            output = __354;
            send(newSockFd, output.c_str(), output.size(), 0);
        }
        else{
            output = __500;
            send(newSockFd, output.c_str(), output.size(), 0);
            continue;
        }

        ofstream file(user.c_str(), ios_base::app);
        while(true){
            read(newSockFd, buffer, 1024);
            if(strcmp(buffer, ".") == 0)    // <.> recieved: end of message
                break;
            file << string(buffer) << endl;
        }
        file << endl;
        file.close();
        

        // quit
        read(newSockFd, buffer, 1024);
        input = string(buffer);
        if(input.substr(0, 4) == "QUIT"){
            output = __221;
            send(newSockFd, output.c_str(), output.size(), 0);
        }
        else{
            output = __500;
            send(newSockFd, output.c_str(), output.size(), 0);
        }
        close(newSockFd);

    }

}
