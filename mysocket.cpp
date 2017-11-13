/*
https://gafferongames.com/post/sending_and_receiving_packets/
*/

#include "mysocket.h"
#include <stdio.h>

/* Platform Specifics */
#define PLATFORM_WINDOWS	1
#define PLATFORM_MAC		2
#define PLATFORM_UNIX		3

#if defined(_WIN32)
	#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
	#define PLATFORM PLATFORM_MAC
#else
	#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
	#include <winsock2.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	#include <sys/socket.h>
	#include <unistd.h>
	#include <netinet/in.h>
	#include <fcntl.h>
#endif

#if PLATFORM == PLATFORM_WINDOWS
	#pragma comment( lib, "wsock32.lib")
#endif

Socket::Socket() {
	_open = false;
	_handle = -1;
}

Socket::~Socket() {
	if (_open)
		Close();
}

int Socket::initializeSockets() {
	#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup( MAKEWORD(2,2), 
                       &WsaData ) 
        == NO_ERROR;
    #else
    return true;
    #endif
}

void Socket::shutdownSockets() {
	#if PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
	#endif
}

bool Socket::Open(unsigned short port) {
	_handle = socket( AF_INET,
					SOCK_DGRAM,
					IPPROTO_UDP );

	if (_handle <= 0) {
		_open = false;
		return false;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if ( bind( _handle, (const sockaddr*) &address,
				sizeof(sockaddr_in)) < 0) 
	{
		_open = false;
		return false;
	}

	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

        int nonBlocking = 1;
        if ( fcntl( _handle, 
                    F_SETFL, 
                    O_NONBLOCK, 
                    nonBlocking ) == -1 )
        {
            _open = false;
            return false;
        }

    #elif PLATFORM == PLATFORM_WINDOWS

        DWORD nonBlocking = 1;
        if ( ioctlsocket( _handle, 
                          FIONBIO, 
                          &nonBlocking ) != 0 )
        {
            _open = false;
            return false;
        }

    #endif

    _open = true;
    return true;
}

void Socket::Close() {
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	close( _handle );
	#elif PLATFORM == PLATFORM_WINDOWS
	closesocket( _handle );
	#endif

	_open = false;
}

bool Socket::isOpen() const {
	return _open;
}

bool Socket::Send(const Address& destination,
			const void* data,
			int size) {

	// use Address to create the sockaddr
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( destination.getAddress() );
	addr.sin_port = htons( destination.getPort() );

	// actually send to the address
	int sent_bytes =
		sendto( _handle, 
            (const char*)data, 
            size,
            0, 
            (sockaddr*)&addr, 
            sizeof(sockaddr) );

    if ( sent_bytes != size ) {
        return false;
    }
    else {
    	//printf("%s, %d, %d\n", (const char*)data, size, sent_bytes);
    	return true;
    }
}

int Socket::Receive(Address& sender,
			void* data,
			int size) {

	#if PLATFORM == PLATFORM_WINDOWS
	typedef int socklen_t;
	#endif

	// use Address to create the sockaddr
	sockaddr_in from;
	from.sin_family = AF_INET;
	from.sin_addr.s_addr = htonl( sender.getAddress() );
	from.sin_port = htons( sender.getPort() );

	socklen_t fromLength = sizeof( from );

	int bytes = 
		recvfrom( _handle, 
			(char*)data, 
			size,
			0, 
			(sockaddr*)&from, 
			&fromLength );
	return bytes;
}

Address::Address() {
	uint32_t a = 127;
	uint32_t b = 0;
	uint32_t c = 0; 
	uint32_t d = 1;

	address = ( a << 24 ) +
				( b << 16 ) +
				( c << 8 ) +
				d;
	port = 0;
}

Address::Address(uint8_t a,
	uint8_t b,
	uint8_t c,
	uint8_t d,
	unsigned short port) {

	address = ( a << 24 ) +
				( b << 16 ) +
				( c << 8 ) +
				d;
	port = port;
}

Address::Address(unsigned int address,
	unsigned short port) {
	address = address;
	port = port;
}