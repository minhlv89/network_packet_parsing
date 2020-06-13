#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <queue>

#include "tlm_parser.h"

using namespace std;

vector<uint8_t> packetValid{0xAB, 0xBA, 0xCF, 0xFC, 0x25, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x7B};	//pro 1, subsys 5, comp 20, tlm 1
vector<uint8_t> packetSpurious1{0xAB, 0xAB, 0xAB, 0xBA, 0xCF, 0xFC, 0x25, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x7B};
vector<uint8_t> packetSpurious2{0xAB, 0xAB, 0xAB, 0xBA, 0xCF, 0xFC, 0x25, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x7B, 0xAA, 0xBB};
vector<uint8_t> packetMagicInvalid{0xAB, 0xCD, 0xCF, 0xFC, 0x25, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x7B};
vector<uint8_t> packetChecksumInvalid{0xAB, 0xBA, 0xCF, 0xFC, 0x25, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0xBB};
vector<uint8_t> packetProtocolInvalid{0xAB, 0xBA, 0xCF, 0xFC, 0xA5, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0xFB};	//pro 5
vector<uint8_t> packetSubsystemInvalid{0xAB, 0xBA, 0xCF, 0xFC, 0x2A, 0xA1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x74};	//subsys 10
vector<uint8_t> packetComponentInvalid{0xAB, 0xBA, 0xCF, 0xFC, 0x25, 0xF1, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x2B};	//subsys 5, comp 30
vector<uint8_t> packetTelemetryInvalid{0xAB, 0xBA, 0xCF, 0xFC, 0x25, 0xA3, 0xFF, 0x43, 0x83, 0xC0, 0x00, 0x79};	//tlm 3

int setPacketData(vector<uint8_t> &data, queue<uint8_t> &dataQ){
	for(int i = 0; i < data.size(); i++){
		dataQ.push(data[i]);
	}
	return 0;
}

int unit_test(vector<uint8_t>&testPacket, int detect, uint8_t subsys, uint8_t compo_id, float temperature) {
	PacketInterface iface;
	setPacketData(testPacket, packetDataQ);
	Parser parser;

	//cout << "Now start to parse the packet" << endl;
	CompoTlm_t tlm = {};
	int isDetect = 1;

	while (iface.existRcvData()) {
		uint8_t byte = iface.getByte();
		// Parse incoming bytes and detect packet
		if (parser.detectPkt(byte)) {
			// One, correct packet has been detected
			//cout << "Packet detected. Now extract!!!" << endl;
			int err = parser.extractData(tlm);
			if (err == 0) {
			//printf("Extract result: subsys=%d, comp=%d, temp=%.1f\n", tlm.subsys_id, tlm.compo_id, tlm.temperature);
			}
			else {
				// Error handling, etc.
			}
		}
		else{
			isDetect = 0;
		}
	}
	if(detect == isDetect && subsys == tlm.subsys_id && compo_id == tlm.compo_id && temperature == tlm.temperature){
		return 1;
	}
	return 0;
}

int main(void)
{
	printf("Test case 1 is :%s\n", (unit_test(packetValid, 1, 5, 20, 263.5)) ? "Pass" : "False");
	printf("Test case 2 is :%s\n", (unit_test(packetSpurious1, 1, 5, 20, 263.5)) ? "Pass" : "False");
	printf("Test case 3 is :%s\n", (unit_test(packetSpurious2, 1, 5, 20, 263.5)) ? "Pass" : "False");
	printf("Test case 4 is :%s\n", (unit_test(packetMagicInvalid, 0, 0, 0, 0)) ? "Pass" : "False");
	printf("Test case 5 is :%s\n", (unit_test(packetChecksumInvalid, 0, 0, 0, 0)) ? "Pass" : "False");
	printf("Test case 6 is :%s\n", (unit_test(packetProtocolInvalid, 1, 0, 0, 0)) ? "Pass" : "False");
	printf("Test case 7 is :%s\n", (unit_test(packetSubsystemInvalid, 1, 0, 0, 0)) ? "Pass" : "False");
	printf("Test case 8 is :%s\n", (unit_test(packetComponentInvalid, 1, 0, 0, 0)) ? "Pass" : "False");
	printf("Test case 9 is :%s\n", (unit_test(packetTelemetryInvalid, 1, 0, 0, 0)) ? "Pass" : "False");
	return 0;
}
