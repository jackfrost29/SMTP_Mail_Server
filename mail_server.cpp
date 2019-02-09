//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <vector>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define PORT 8888


int main(){
    int masterSocket, activity, addrlen;
    vector<int> clientSockets;
    sockaddr_in address;
    fd_set readFds;
    char buffer[1025];

    masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(masterSocket, (sockaddr*)&address, sizeof(address));
    listen(masterSocket, 3);

    printf("Waiting for connections...");

    addrlen = sizeof(address);
    while(true){
        FD_ZERO(&readFds);
        FD_SET(masterSocket, readFds);

        int maxSd = masterSocket;  /// maximum value of the socket descriptors

        for(int i=0; i<clientSockets.size(); i++){
            FD_SET(clientSockets[i], &readFds);

            ///highest file descriptor number, need it for the select function
            if(clientSockets[i] > maxSd)
                maxSd = clientSockets;
        }

        activity = select(maxSd+1, &readFds, NULL, NULL, NULL);

        /// it's a request for connection
        if(FD_ISSET(masterSocket, &readFds)){
            int newSocket = accept(masterSocket, (sockaddr*)&address, (socklen_t*)&addrlen);
            /// connection established

            clientSockets.push_back(newSocket);
        }

        for(int i=0; i<clientSockets.size(); i++){
            int sd = clientSockets[i];
            if(FD_ISSET(sd, &readFds)){
                /// check if it was for closing and also read incoming message

                if(read(sd, buffer, 1024) == 0){
                    /// client of with descriptor sd disconnected disconnected
                    close(sd);
                    clientSockets.erase(clientSockets.begin()+i);
                    i--;
                }

                else{
                    /// message came in. send the  appropriate response

                }
            }
        }
    }
}
