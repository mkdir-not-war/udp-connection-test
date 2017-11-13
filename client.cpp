#include <cstdlib>
#include <cstring>
#include <iostream>
#include "mysocket.h"

#include <stdio.h>

enum { max_length = 1024 };

int main(int argc, char* argv[]) {
  try {
    // their port
    const int port = 8888;

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
      socket.Send( Address(127, 0, 0, 1, port), request, request_length );
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