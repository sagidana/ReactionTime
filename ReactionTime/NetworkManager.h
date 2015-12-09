#ifndef SAGI_PCAP
#include <iostream>

#define SAGI_PCAP
#include "pcap.h"
#pragma comment(lib , "ws2_32.lib")
#pragma comment(lib , "wpcap.lib")
#endif

#ifndef SAGI_STRUCTERS
#define SAGI_STRUCTERS
#include "Structers.h"
#endif

#define PACKETSIZE 65536
#define IPTOSBUFFERS 12

using namespace std;

class __declspec(dllexport) NetworkManager
{
private:
	static void __cdecl genericPacketsHandler(u_char *param, const struct pcap_pkthdr *header, const u_char *data);
	static bool NetworkManager::translatePacket(const u_char* packet, ethernetHeader** ethernet, ipHeader** ip, tcpHeader** tcp, char** payload);
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
	
public:
	char ErrorMessage[PCAP_ERRBUF_SIZE];
	
	NetworkManager();
	~NetworkManager();

	bool TargetAdapter(char* name);
	bool TargetAdapter(int index);
	bool TargetAdapter(pcap_if_t* networkAdapter);
	bool SetFilter(string filteringExpression);
	bool StartCapture(bool isAsync = true);
	bool StartCapture(void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data), bool isAsync = true);
	void PrintNetworkAdapters();
	void SendPacket();
	pcap_if_t* GetNetworkAdapters();
};

