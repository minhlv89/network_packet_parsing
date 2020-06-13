#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;

queue<uint8_t> packetDataQ;

class PacketInterface{
public:

	static int existRcvData(){
		return !packetDataQ.empty();
	}


	static uint8_t getByte(){
		uint8_t readByte;
		if(0 == packetDataQ.empty()){
			readByte = packetDataQ.front();
			packetDataQ.pop();
		}
		//printf("PacketInterface::GetByte readByte = %2x\n", readByte);
		return readByte;
	}
};
