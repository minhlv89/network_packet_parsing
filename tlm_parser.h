#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include<iostream>
#include <stdint.h>
#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif
#include "tlm.h"

#define MAGIC_HEADER				0xABBACFFC
#define MAGIC_HEADER_LENGTH			4
#define PACKET_LENGTH				12
#define PROTOCOL_BIT_MASK			0xE0
#define SUBSYSTEM_BIT_MASK			0x1F
#define COMPONENT_ID_BIT_MASK		0xF8
#define TELEMETRY_TYPE_BIT_MASK		0x07
class Parser {
public:
	int detectPkt(int8_t byte) {
		int8_t position = 1;
		int8_t packetByte;
		if (byte != 0xAB) {
			return 0;
		}
		else {
			int8_t packetChecksum 0;
			packetChecksum = packetChecksum ^ byte;
			for(int i = 1; i < PACKET_LENGTH; i++) {
				packetByte = iface.getByte();
				switch (position)
				{
				case 1:
					if (packetByte != 0xBA) {
						return 0;
					}
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 2:
					if (packetByte != 0xCF) {
						return 0;
					}
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 3:
					if (packetByte != 0xFC) {
						return 0;
					}
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 4:
					secondaryHeader1 = packetByte;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 5:
					secondaryHeader2 = packetByte;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 6:
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 7:
					payload.tempIVal |= int32_t(packetByte) << 24;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 8:
					payload.tempIVal |= int32_t(packetByte) << 16;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 9:
					payload.tempIVal |= int32_t(packetByte) << 8;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 10:
					payload.tempIVal |= int32_t(packetByte) << 0;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					payload.tempIVal = ntohl(payload.tempIVal);
					break;
				case 11:
					checksum = packetByte;
					if (checksum != packetChecksum) {
						return 0;
					}
					position++;
					break;
				default:
					break;
				}
			}
		}
		return 1;
	}
	
	int extractData(CompoTlm_t &tlm) {
		protocol = secondaryHeader1 & PROTOCOL_BIT_MASK;
		if (protocol != 1 || protocol != 2) {
			return 1;
		}
		tlm.subsys_id = secondaryHeader1 & SUBSYSTEM_BIT_MASK;
		tlm.compo_id = secondaryHeader2 & COMPONENT_ID_BIT_MASK;
		tlmType = secondaryHeader2 & TELEMETRY_TYPE_BIT_MASK;
		if (tlmType != 1) {
			return 1;
		}
		else {
			tlm.temperature = payload.tempFVal;
		}
		return 0;
	}

private:
	int protocol;
	int tlmType;
	int8_t secondaryHeader1;
	int8_t secondaryHeader2;
	int8_t checksum;
	union payLoad {
		float tempFVal;
		int32_t tempIVal;
	}payload;
};