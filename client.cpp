#include <cstdlib>
#include <cstring>
#include <iostream>
#include "mysocket.h"

#include <stdio.h>

enum { max_length = 1024, port = 8888 };

void parseIP(char* ipnumbers[], int* serverip) {
  for (int i=0; i<4; i++) {
    serverip[i] = std::stoi(ipnumbers[i+1]);
  }
}

int main(int argc, char* argv[]) {
  try {

    // parse the server IP address
    int serverip[4];
    if (argc != 5) {
      printf("./client <server ip> <ip> <ip> <ip> \n");
      return 1;
    }
    else {
      parseIP(argv, serverip);
    }

    Address server = Address(serverip[0], 
        serverip[1], 
        serverip[2], 
        serverip[3],
        port);

    // create socket
    Socket socket;
    socket.initializeSockets();

    if (!socket.Open(0)) {
      printf("failed to create socket!\n");
      return 1;
    }

    // create buffers
    char request[max_length];
    char reply[max_length];

    // input loop
    while (true) {
      // get message and pack into buffer
      std::cout << "Enter message: ";
      std::cin.getline(request, max_length);
      size_t request_length = std::strlen(request);

      // send the message
      //Address local = Address(127, 0, 0, 1, port);
      socket.Send( server, request, request_length );
    }

    socket.shutdownSockets();
  }
  catch (std::exception& e) {
    Socket socket;
    socket.shutdownSockets();
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}