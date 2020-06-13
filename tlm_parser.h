#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include<iostream>
#include <stdint.h>
#ifdef OS_WINDOWS
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif
#include "tlm.h"
#include "rx_interface.h"

#define MAGIC_HEADER				0xABBACFFC
#define MAGIC_HEADER_LENGTH			4
#define PACKET_LENGTH				12
#define PROTOCOL_BIT_MASK			0xE0	//1110 0000
#define SUBSYSTEM_BIT_MASK			0x1F	//0001 1111
#define COMPONENT_ID_BIT_MASK			0xF8	//1111 1000
#define TELEMETRY_TYPE_BIT_MASK			0x07	//0000 0111

#define SUBSYSTEM_AOCS		1
#define SUBSYSTEM_CDH		3
#define SUBSYSTEM_COM		5

#define COMPONENT_CDH		0
#define COMPONENT_COM_1		1
#define COMPONENT_COM_2		2
#define COMPONENT_COM_3		10
#define COMPONENT_COM_4		20
class Parser {
public:
	int detectPkt(uint8_t byte) {
		//printf("Parser::detectPkt byte = %2x\n", byte);
		uint8_t position = 4;
		uint8_t packetByte;
		/*
		if (byte != 0xAB) {
			return 0;
		}
		*/
		if(0 == detectMagicHeader(byte)){
			return 0;
		}
		else {
			uint8_t packetChecksum = 0;
			payload.tempIVal = 0;
			for(int i = 1; i < PACKET_LENGTH; i++) {
				//packetByte = getByte();

				packetByte = PacketInterface::getByte();
				switch (position)
				{
				/*
				case 1:
					if (packetByte != 0xBA) {
						printf("Parser::detectPkt Error magic 1\n");
						return 0;
					}
					position++;
					break;
				case 2:
					if (packetByte != 0xCF) {
						printf("Parser::detectPkt Error magic 2\n");
						return 0;
					}
					position++;
					break;
				case 3:
					if (packetByte != 0xFC) {
						printf("Parser::detectPkt Error magic 3\n");
						return 0;
					}
					position++;
					break;
				*/
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
					payload.tempIVal |= uint32_t(packetByte) << 24;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 8:
					payload.tempIVal |= uint32_t(packetByte) << 16;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 9:
					payload.tempIVal |= uint32_t(packetByte) << 8;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					break;
				case 10:
					payload.tempIVal |= uint32_t(packetByte) << 0;
					packetChecksum = packetChecksum ^ packetByte;
					position++;
					//printf("Parser::detectPkt tempIval:0x%x\n", payload.tempIVal);
					break;
				case 11:
					checksum = packetByte;
					if (checksum != packetChecksum) {
						//printf("Parser::detectPkt Error checksum %2x, %2x\n", checksum, packetChecksum);
						return 0;
					}
					position++;
					break;
				default:
					break;
				}
			}
		}
		//printf("Parser::detectPkt Finish to parse packet\n");

		return 1;
	}
	
	int extractData(CompoTlm_t &tlm) {
		protocol = (secondaryHeader1 & PROTOCOL_BIT_MASK) >>5;

		if (protocol != 1 && protocol != 2) {
			return 1;
		}
		subsystemID = secondaryHeader1 & SUBSYSTEM_BIT_MASK;
		componentID = (secondaryHeader2 & COMPONENT_ID_BIT_MASK) >>3;

		if(subsystemID == SUBSYSTEM_AOCS){
			if(0 == validateAocsComponentID(subsystemID)){
				return 1;
			}
		}
		else if(subsystemID == SUBSYSTEM_CDH){
			if(componentID != COMPONENT_CDH){
				return 1;
			}
		}
		else if(subsystemID == SUBSYSTEM_COM){
			if(componentID != COMPONENT_COM_1 && componentID != COMPONENT_COM_2 &&
				componentID != COMPONENT_COM_3 && componentID != COMPONENT_COM_4){
				return 1;
			}
		}
		else{
			return 1;
		}
		tlm.subsys_id = subsystemID;
		tlm.compo_id = componentID;
		tlmType = secondaryHeader2 & TELEMETRY_TYPE_BIT_MASK;


		if (tlmType != 1) {
			printf("Parser::extractData invalid Telemetry:%d\n",protocol);
			return 1;
		}
		else {
			tlm.temperature = payload.tempFVal;
		}
		//printf("Parser::extractData: tlmType:%d, subsys:%d, compo:%d, temp=%.1f\n"
		//	,tlmType, tlm.subsys_id, tlm.compo_id, tlm.temperature);
		return 0;
	}
private:
	int detectMagicHeader(uint8_t byte){
		if(byte != 0xAB){
			return 0;
		}
		else{
			while(PacketInterface::existRcvData()){
				uint8_t packetByte = PacketInterface::getByte();
				if(packetByte != 0xBA){
					if(packetByte = 0xAB){
						continue;
					}
					else{
						return 0;
					}
				}
				packetByte = PacketInterface::getByte();
				if(packetByte != 0xCF){
					if(packetByte = 0xAB){
						continue;
					}
					else{
						return 0;
					}
				}
				packetByte = PacketInterface::getByte();
				if(packetByte != 0xFC){
					if(packetByte = 0xAB){
						continue;
					}
					else{
						return 0;
					}
				}
				else{
					return 1;
				}
			}
		}
		return 0;
	}

	int validateAocsComponentID(int subsystemID){
		const int validSubsys[6] = {20, 21, 22, 23, 30, 31};
		for(int i = 0; i < 6; i++){
			if(subsystemID == validSubsys[i]){
				return 1;
			}
		}
		return 0;
	}

private:
	int protocol;
	int tlmType;
	int subsystemID;
	int componentID;
	uint8_t secondaryHeader1;
	uint8_t secondaryHeader2;
	uint8_t checksum;
	union payLoad {
		float tempFVal;
		uint32_t tempIVal;
	}payload;
};
