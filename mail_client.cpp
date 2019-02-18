#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
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
#include <chrono>
#include <ctime>
#include <fstream>

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

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    result.erase(result.size()-1, result.size()-1);
    return result;
}


int main(int argc, char const *argv[]){

    sockaddr_in address, servAddress;
    int client, PORT;
    char buffer[1024];
    string argument(argv[1]), host, user, input, output, mailFromOut, mailToOut;

    user = toUser(argv[1]);
    host = toHost(argv[1]);
    cout << "Enter the port number: ";
    cin >> PORT;

    client = socket(AF_INET, SOCK_STREAM, 0);
    servAddress.sin_family = AF_INET;
    servAddress.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &servAddress.sin_addr); // for running in local host

    cout << "Waiting for connection with server" << endl;
    connect(client, (sockaddr*)&servAddress, sizeof(servAddress));
    cout << "connected with server" << endl;
    read(client, buffer, 1024);
    printf("From server: %s\n", buffer);

    // hello
    output = "HELO SERVER";
    send(client, output.c_str(), output.size(), 0);
    read(client, buffer, 1024);
    input = string(buffer);
    if(input == __500){
        cout << buffer << endl;
        close(client);
        return 0;
    }
    // mail_from
    host = gethostname(buffer, 1024);
    user = exec("whoami");
    mailFromOut = "MAIL FROM " + user + "@" + host;
    send(client, mailFromOut.c_str(), mailFromOut.size(), 0);
    read(client, buffer, 1024);
    if(strcmp(buffer, __500) == 0){
        cout << buffer << endl;
        // error, close the socket
        close(client);
        return 0;
    }

    // rcpt_to
    mailToOut = "RCPT TO " + string(argv[1]);
    mailToOut.erase(mailToOut.size()-5, mailToOut.size()-1);
    send(client, mailToOut.c_str(), mailToOut.size(), 0);
    read(client, buffer, 1024);
    if(strcmp(buffer, __250) != 0){
        // error, close
        cout << buffer << endl;
        close(client);
        return 0;
    }
    // data

    send(client, "DATA", 4, 0);
    read(client, buffer, 1024);
    if(strcmp(buffer, __354) != 0){
        // error close
        cout << buffer << endl;
        close(client);
        return 0;
    }

    mailFromOut.erase(0, 9);
    mailToOut.erase(0, 7);
    time_t time = std::chrono::system_clock::to_time_t(chrono::system_clock::now());
    string date = ctime(&time);
    output = "FROM: " + mailFromOut + "\nTO: " + mailToOut + "\nSUBJECT: " + argv[2] + "\nDATE: " + date + "\n\n";
    send(client, output.c_str(), output.size(), 0);
    ifstream file(argv[3]);
    while(file.peek() != EOF){
        getline(file, output);
        send(client, output.c_str(), output.size(), 0);
    }
    send(client, ".", 1, 0);
    file.close();
    
    // quit
    send(client, "QUIT", 4, 0);
    read(client, buffer, 1024);
    if(strcmp(buffer, __221)!= 0){
        //error, close
        cout << buffer << endl;
        close(client);
        return 0;
    }

    cout << "MAIL TRANSMISSION SUCCESSFULL" << endl;
    close(client);
    return 0;

    close(client);

    
}
