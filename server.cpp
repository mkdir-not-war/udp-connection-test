#include <cstdlib>
#include <iostream>
#include "mysocket.h"

enum { max_length = 1024, max_clients = 64 };

class UDPConnectServer
{
  private:
    int m_maxClients;
    int m_numConnectedClients;
    bool m_clientConnected[max_clients];
    Address m_clientAddress[max_clients];

    int findFreeClientIndex() const // const at the end means "read-only"
    {
      for (int i = 0; i<m_maxClients; i++)
      {
        if (!m_clientConnected[i])
          return i;
      }
      return -1;
    }

    int findExistingClientIndex(const Address& address) const
    {
      for (int i=0; i<m_maxClients; i++)
      {
        if (m_clientConnected[i] && m_clientAddress[i] == address)
          return i;
      }
      return -1;
    }

    bool isClientConnected(int clientIndex) const
    {
      return m_clientConnected[clientIndex];
    }

    const Address& getClientAddress(int clientIndex) const
    {
      return m_clientAddress[clientIndex];
    }


  public:
    UDPConnectServer() 
    {
      m_maxClients = max_clients;
      m_numConnectedClients = 0;

      for (int i=0; i<max_clients; i++)
      {
        m_clientConnected[i] = false;
      }
    }

    int run() 
    {
      try {
        // create socket
        Socket socket;
        socket.initializeSockets();

        if (!socket.Open(8888)) {
          printf("failed to create socket!\n");
          return 1;
        }

        // create buffers
        char buffer[max_length];

        std::cout<<"entering loop..."<<std::endl;

        // loop
        while (true) {
          // receive message back, unpack and print
          int bytes_read;

          while (true) {
            Address sender;
            bytes_read = 
              socket.Receive( sender, buffer, sizeof(buffer) ); // receive here
            
            if (bytes_read == 0)
              break;
            else if (bytes_read > 0)
              printf("bytes read!\n");
          }
          
          // print message
          std::cout << "Message received: ";
          std::cout << bytes_read;
          std::cout.write(buffer, bytes_read);
          std::cout << "\n";
          //
        }
      }
      catch (std::exception& e) {
        Socket socket;
        socket.shutdownSockets();
        std::cerr << "Exception: " << e.what() << "\n";
      }

      return 0;
    }
};



int main(int argc, char* argv[])
{
  UDPConnectServer myserver;
  return myserver.run();
}
