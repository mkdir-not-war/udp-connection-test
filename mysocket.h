// mysocket.h
#ifndef MYSOCKET_H_364703426758904315
#define MYSOCKET_H_364703426758904315

#include <stdint.h>

class Address {
public:
	Address();

	Address(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

	Address(unsigned int, uint16_t);

	unsigned int getAddress() const;

	uint8_t getA() const {return (address >> 24) & 0xFF;}
	uint8_t getB() const {return (address >> 16) & 0xFF;}
	uint8_t getC() const {return (address >> 8) & 0xFF;}
	uint8_t getD() const {return address & 0xFF;}

	unsigned short getPort() const;

	bool operator==(const Address& other) const {
		return address == other.address &&
			port == other.port;
	}

private:
	uint32_t address;
	uint16_t port;
};

class Socket {
public:
	Socket();
	~Socket();

	int initializeSockets();
	void shutdownSockets();

	bool Open(uint16_t);

	void Close();

	bool isOpen() const;

	bool Send(const Address&,
				const void*,
				int);

	int Receive(Address&,
				void*,
				int);

private:
	bool _open;
	int _handle;
};

#endif