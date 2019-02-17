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

#define __200 "200 (nonstandard success response, see rfc876)";
#define __211 "211 System status, or system help reply";
#define __214 "214 Help message";
#define __220 "220 <domain> Service ready";
#define __221 "221 <domain> Service closing transmission channel";
#define __250 "250 Requested mail action okay, completed";
#define __251 "251 User not local; will forward to <forward-path>";
#define __252 "252 Cannot VRFY user, but will accept message and attempt delivery";
#define __354 "354 Start mail input; end with <CRLF>.<CRLF>";
#define __421 "421 <domain> Service not available, closing transmission channel";
#define __450 "450 Requested mail action not taken: mailbox unavailable";
#define __200 "451 Requested action aborted: local error in processing";
#define __452 "452 Requested action not taken: insufficient system storage";
#define __500 "500 Syntax error, command unrecognised";
#define __501 "501 Syntax error in parameters or arguments";
#define __502 "502 Command not implemented";
#define __503 "503 Bad sequence of commands";
#define __504 "504 Command parameter not implemented";
#define __521 "521 <domain> does not accept mail (see rfc1846)";
#define __530 "530 Access denied (???a Sendmailism)";
#define __550 "550 Requested action not taken: mailbox unavailable";
#define __551 "551 User not local; please try <forward-path>";
#define __552 "552 Requested mail action aborted: exceeded storage allocation";
#define __553 "553 Requested action not taken: mailbox name not allowed";
#define __554 "554 Transaction failed";

using namespace std;

string toUser(string s){
    // argument was user@host:PORT
    // extracts the user out of argument

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


int main(int argc, char const *argv[]){

    sockaddr_in address, servAddress;
    int client, PORT;
    char buffer[1024];
    string argument(argv[1]), host, user;
    cout << argument << endl;

    cout << toUser(argument) << endl;
    cout << toHost(argument) << endl;
/*
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

    */
}
