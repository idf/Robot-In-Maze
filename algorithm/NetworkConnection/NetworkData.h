#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

    INIT_CONNECTION = 0,
	
    ACTION_EVENT = 1,

	TEST = 2,
};

struct Packet {

    unsigned int packet_type;
    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};