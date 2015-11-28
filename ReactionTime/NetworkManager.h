#ifndef SAGI_PCAP
#include <iostream>

#define SAGI_PCAP
#include "pcap.h"
#pragma comment(lib , "ws2_32.lib")
#pragma comment(lib , "wpcap.lib")
#endif

#define PACKETSIZE 65536
#define IPTOSBUFFERS 12

using namespace std;

class NetworkManager
{
private:
	pcap_if_t* m_NetworkAdapters;
	pcap_if_t* m_ActiveAdapter;
	pcap_t* m_ActiveAdapterHandle;

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

	bool SetFilter(string filteringExpression);
	bool StartCapture(char* name, void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data));
	bool StartCapture(int index, void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data));
	bool StartCapture(pcap_if_t* networkAdapter, void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data));
	void PrintNetworkAdapters();
	pcap_if_t* GetNetworkAdapters();
};

