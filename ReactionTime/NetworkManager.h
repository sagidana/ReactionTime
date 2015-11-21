#ifndef SAGI_PCAP
#define SAGI_PCAP
#include "pcap.h"
#pragma comment(lib , "ws2_32.lib")
#pragma comment(lib , "wpcap.lib")
#endif
#pragma once
class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
};

