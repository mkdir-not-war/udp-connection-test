#include "mypacket.h"

static char* Packet::construct_message(uint32_t salt, char* data) {
	char buffer[PACKET_SIZE + MESSAGE_SIZE];
	Packet.addProcessId(buffer);
	Packet.addCRC32(buffer);
	Packet.addPacketType(MESSAGE, buffer);
	Packet.addSalt(salt, buffer);
	Packet.addMessage(data, buffer);
	return buffer;
}

static char* Packet::construct_response(uint32_t salt) {
	char buffer[1000];
	Packet.addProcessId(buffer);
	Packet.addCRC32(buffer);
	Packet.addPacketType(RESPONSE, buffer);
	Packet.addSalt(salt, buffer);
	return buffer;
}

static char* Packet::construct_ack(uint32_t salt) {
	char buffer[PACKET_SIZE];
	Packet.addProcessId(buffer);
	Packet.addCRC32(buffer);
	Packet.addPacketType(ACK, buffer);
	Packet.addSalt(salt, buffer);
	return buffer;
}

static char* Packet::construct_request(uint32_t salt) {
	char buffer[1000];
	Packet.addProcessId(buffer);
	Packet.addCRC32(buffer);
	Packet.addPacketType(REQUEST, buffer);
	Packet.addSalt(salt, buffer);
	Packet.addPadto1k(buffer);
	return buffer;
}

static char* Packet::construct_challenge(uint32_t salt) {
	char buffer[PACKET_SIZE];
	Packet.addProcessId(buffer);
	Packet.addCRC32(buffer);
	Packet.addPacketType(CHALLENGE, buffer);
	Packet.addSalt(salt, buffer);
	return buffer;
}

static char* Packet::construct_deny(uint32_t salt) {
	char buffer[PACKET_SIZE];
	Packet.addProcessId(buffer);
	Packet.addCRC32(buffer);
	Packet.addPacketType(DENY, buffer);
	Packet.addSalt(salt, buffer);
	return buffer;
}