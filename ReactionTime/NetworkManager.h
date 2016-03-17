#pragma once

#include <iostream>
#include "pcap.h"

#pragma comment(lib , "ws2_32.lib")
#pragma comment(lib , "wpcap.lib")

#include "Structs.h"

#define PACKETSIZE 65536
#define IPTOSBUFFERS 12

class __declspec(dllexport) NetworkManager
{
private:
	static void __cdecl genericPacketsHandler(u_char *param, const struct pcap_pkthdr *header, const u_char *data);
	static bool translatePacket(const u_char* packet, ethernetHeader** ethernet, ipHeader** ip, tcpHeader** tcp, char** payload);
	static unsigned int getTcpLength(tcpHeader* tcp);
	static unsigned int getIpLength(ipHeader* ip);
	static pcap_t* m_ActiveAdapterHandle;
	
	pcap_if_t* m_NetworkAdapters;
	pcap_if_t* m_ActiveAdapter;
	
	void initializeAttachedDevices();
	void printNetworkAdapter(pcap_if_t* networkAdapter);
	void printAdapterAddresses(pcap_addr_t * adapterAddresses);
	char* convertIpToString(unsigned long in);
	char* ConvertIpv6ToString(struct sockaddr *socketAddress, char *address, int addressLength);
	pcap_if_t* getNetworkAdapterByIndex(int index);
	pcap_if_t* getNetworkAdapterByName(char* name);
	unsigned int getNetmask();
	unsigned short calculateChecksum(unsigned short* ip, int length);
	
public:
	char ErrorMessage[PCAP_ERRBUF_SIZE];
	
	NetworkManager();
	~NetworkManager();

	bool TargetAdapter(char* name);
	bool TargetAdapter(int index);
	bool TargetAdapter(pcap_if_t* networkAdapter);
	bool SetFilter(std::string filteringExpression);
	bool StartCapture(bool isAsync = true);
	bool StartCapture(void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data), bool isAsync = true);
	void PrintNetworkAdapters();
	void SendPacket(ethernetHeader* ethernet, ipHeader* ip, tcpHeader* tcp, char* payload, unsigned int payloadLength);
	ipHeader* CreateIpHeader(PCTSTR sourceIpAddress, PCTSTR destinationIpAddress, unsigned int payloadLength, unsigned char protocol = IPPROTO_TCP, unsigned char ttl = 3);
	tcpHeader* CreateTcpHeader(unsigned short sourcePort, unsigned short destinationPort, unsigned int sequence, unsigned int acknowledge, unsigned short windowsSize = 64240);
	ethernetHeader* CreateEthernetHeader(unsigned char sourceMac[6], unsigned char destinationMac[6], unsigned short type = 0x0008);
	pcap_if_t* GetNetworkAdapters();
};

