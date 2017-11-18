// mypacket.h
#ifndef MYPACKET_H_3289307258904789345
#define MYPACKET_H_3289307258904789345

#define MESSAGE_SIZE		140  // max message length
#define PACKET_SIZE 		32*3+1 // pid + crc32 + packet_type + salt
#define PROCESS_ID			923746811 // randomly chose this

#include <stdint.h>

static class Packet {
public:
	static enum {MESSAGE=0, DENY=1, ACK=2, REQUEST=3, RESPONSE=4, CHALLENGE=5}; // read/set in PacketType

	static char* construct_message(uint32_t, char*);
	static char* construct_ack(uint32_t);
	static char* construct_request(uint32_t);
	static char* construct_response(uint32_t);
	static uint32_t construct_challenge(uint32_t); // return server salt
	static char* construct_deny();

private:
	static void addProcessId(char*);
	static void addCRC32(char*);
	static void addPacketType(int, char*);
	static void addSalt(uint32_t, char*);
	static void addPadTo1k(char*);
	static void addMessage(char*, char*);
};

class ClientPacket {
public:
	ClientPacket(); // set salt

	char* construct_request() {
		return Packet.construct_request(salt);
	}

	char* construct_ack() {
		return Packet.construct_ack(salt);
	}

	char* construct_message(char* data) {
		return Packet.construct_message(salt, data);
	}

private:
	uint32_t salt;
};

class ServerPacket {
public:
	ServerPacket(); // set salt

	uint32_t construct_challenge() {
		return Packet.construct_challenge(salt);
	}
	char* construct_deny() {
		return Packet.construct_deny(salt);
	}

	char* construct_ack() {
		return Packet.construct_ack(salt);
	}

	char* construct_message(char* data) {
		return Packet.construct_message(salt, data);
	}

private:
	uint32_t salt;
};

static class PacketReader {
public:
	/* 
		1) check pid and crc32
		2) check packet type
		3) if message or ack, check salt
		4) else, do type-appropriate check and return 1 or -1
		5) if message, copy message to buffer
	*/
	static int read_packet(char* packet, char* message_buffer);

private:
	static int check_pid(char*);
	static int check_crc32(char*);
	static int check_packettype(char*);
	static int check_salt(char*);
	static int read_message(char*);
};

class ClientReader {
public:
	ClientReader();
	ClientReader(ClientPacket*); // use to determine salt
private:
	uint32_t client_salt;
	uint32_t server_salt;
	uint32_t xor_salt;
};

class ServerReader {
public: 
	ServerReader();
	ServerReader(ServerPacket*); // use to determine salt
private:
	uint32_t client_salt;
	uint32_t server_salt;
	uint32_t xor_salt;
};

#endif