#include "NetworkAssembler.h"

using namespace std;

pcap_t* NetworkAssembler::m_ActiveAdapterHandle;

bool NetworkAssembler::AssemblePacket(const u_char* packet, ethernetHeader** ethernet, ipHeader** ip, tcpHeader** tcp, char** payload)
{
	u_int ipHeaderLength;
	u_int tcpHeaderLength;

	if (pcap_datalink(m_ActiveAdapterHandle) == DLT_EN10MB) // if datalink is ethernet
	{
		*ethernet = (ethernetHeader*)(packet);
		*ip = (ipHeader*)(packet + ETHERNET_LENGTH); // Found the ip header
		ipHeaderLength = GetIpLength(*ip);

		if (ipHeaderLength < 20) // Bad ip header
			return false;

		*tcp = (tcpHeader*)(packet + ETHERNET_LENGTH + ipHeaderLength); // Found the tcp header
		tcpHeaderLength = GetTcpLength(*tcp);

		if (tcpHeaderLength < 20) // Bad tcp header
			return false;

		*payload = (char *)(packet + ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength);

		return true;
	}

	return false;
}

string NetworkAssembler::GetDestinationIpAndPort(const u_char* packet, const struct pcap_pkthdr *header)
{
	ethernetHeader * ethernet;
	ipHeader * ip;
	tcpHeader * tcp;
	char * payload;

	if (NetworkAssembler::AssemblePacket(packet, &ethernet, &ip, &tcp, &payload))
	{
		u_int ipHeaderLength = NetworkAssembler::GetIpLength(ip);
		u_int tcpHeaderLength = NetworkAssembler::GetTcpLength(tcp);

		if (header->caplen < ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength) //Length of the data captured was not enough,
			return nullptr;

		string destinationPort = NetworkAssembler::ConvertPortToString(tcp->dest_port);
		string destinationIpAddress = NetworkAssembler::ConvertIpToString(ip->ip_destaddr);

		return destinationIpAddress + ":" + destinationPort;
	}
	return nullptr;
}

string NetworkAssembler::GetSourceIpAndPort(const u_char* packet, const struct pcap_pkthdr *header)
{
	ethernetHeader * ethernet;
	ipHeader * ip;
	tcpHeader * tcp;
	char * payload;

	if (NetworkAssembler::AssemblePacket(packet, &ethernet, &ip, &tcp, &payload))
	{
		u_int ipHeaderLength = NetworkAssembler::GetIpLength(ip);
		u_int tcpHeaderLength = NetworkAssembler::GetTcpLength(tcp);

		if (header->caplen < ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength) //Length of the data captured was not enough,
			return nullptr;

		string sourcePort = NetworkAssembler::ConvertPortToString(tcp->source_port);
		string sourceIpAddress = NetworkAssembler::ConvertIpToString(ip->ip_srcaddr);

		return sourceIpAddress + ":" + sourcePort;
	}
	return nullptr;
}

string NetworkAssembler::ExportHttpPayload(const u_char* packet, const struct pcap_pkthdr *header)
{
	ethernetHeader * ethernet;
	ipHeader * ip;
	tcpHeader * tcp;
	char * payload;

	if (NetworkAssembler::AssemblePacket(packet, &ethernet, &ip, &tcp, &payload))
	{
		u_int ipHeaderLength = NetworkAssembler::GetIpLength(ip);
		u_int tcpHeaderLength = NetworkAssembler::GetTcpLength(tcp);

		if (header->caplen < ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength) //Length of the data captured was not enough,
			return nullptr;

		istringstream httpStream(payload);
		
		return httpStream.str();
	}
}

void __cdecl NetworkAssembler::genericPacketsHandler(u_char* param, const pcap_pkthdr* header, const u_char* packet)
{
	ethernetHeader* ethernet;
	ipHeader* ip;
	tcpHeader* tcp;
	char* payload;
	
	if (AssemblePacket(packet, &ethernet, &ip, &tcp, &payload))
	{
		u_int ipHeaderLength = GetIpLength(ip);
		u_int tcpHeaderLength = GetTcpLength(tcp);
		
		if (header->caplen < ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength) //Length of the data captured was not enough,
			return;

		u_int payloadLength = header->caplen - ETHERNET_LENGTH - ipHeaderLength - tcpHeaderLength;
	
		for (u_int index = 0; index < payloadLength; index++)
		{
			if (isprint(payload[index]))
				cout << payload[index];

			if ((index % 70 == 0) || index == payloadLength - 1)
				cout << endl;
		}
	}
}

NetworkAssembler::NetworkAssembler()
{
	initializeAttachedDevices();
}

NetworkAssembler::~NetworkAssembler()
{
	// Free all devices
	pcap_freealldevs(m_NetworkAdapters);
}

pcap_if_t* NetworkAssembler::getNetworkAdapterByName(char* name)
{
	int nameLength = strnlen_s(name, 100);
	pcap_if_t* ret_NetworkAdapter = m_NetworkAdapters;

	for (ret_NetworkAdapter = m_NetworkAdapters; ret_NetworkAdapter != NULL; ret_NetworkAdapter = ret_NetworkAdapter->next)
		if (strncmp(ret_NetworkAdapter->name, name, nameLength) == 0)
			break;

	return ret_NetworkAdapter;
}

pcap_if_t* NetworkAssembler::getNetworkAdapterByIndex(int index)
{
	pcap_if_t* ret_NetworkAdapter = m_NetworkAdapters;

	for (int loopIndex = 0; loopIndex < index - 1; loopIndex++)
	{
		if (ret_NetworkAdapter != NULL)
			ret_NetworkAdapter = ret_NetworkAdapter->next;
	}
	
	return ret_NetworkAdapter;
}

void NetworkAssembler::printNetworkAdapter(pcap_if_t* networkAdapter)
{
	if (!networkAdapter)
		return;

	cout << networkAdapter->name << ":" << endl;

	if (networkAdapter->description)
		cout << "\tDescription: "<< networkAdapter->description << endl;
	
	bool isLoopback = (networkAdapter->flags & PCAP_IF_LOOPBACK);
	cout << "\tLoopback: " << (isLoopback ? "yes" : "no") << endl;

	printAdapterAddresses(networkAdapter->addresses);
}

string NetworkAssembler::ConvertPortToString(unsigned short port)
{
	short littleEndianPort = ntohs(port);
	ostringstream convert;
	
	convert << littleEndianPort;

	return convert.str();
}

string NetworkAssembler::ConvertIpToString(u_long in)
{
	static char output[IPTOSBUFFERS][16];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	_snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);

	return string(output[which]);
}

unsigned short NetworkAssembler::calculateChecksum(unsigned short* ip, int length)
{
	int i, sum = 0;

	for (i = 0; i < length / 2; i++){
		if (i == 5) continue;
		sum += *ip++;
	}

	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (u_short)~sum;
}

char* NetworkAssembler::ConvertIpv6ToString(struct sockaddr *socketAddress, char *address, int addressLength)
{
	socklen_t sockaddrlen;

#ifdef WIN32
	sockaddrlen = sizeof(struct sockaddr_in6);
#else
	sockaddrlen = sizeof(struct sockaddr_storage);
#endif


	if (getnameinfo(socketAddress,
		sockaddrlen,
		address,
		addressLength,
		NULL,
		0,
		NI_NUMERICHOST) != 0) address = NULL;

	return address;
}

void NetworkAssembler::printAdapterAddresses(pcap_addr_t* adapterAddresses)
{
	char ip6str[128];

	for (pcap_addr_t* adapterAddress = adapterAddresses; adapterAddress != NULL; adapterAddress = adapterAddress->next)
	{
		switch (adapterAddress->addr->sa_family)
		{
		case AF_INET:
			printf("\tAddress Family Name: AF_INET\n");
			if (adapterAddress->addr)
				cout << "\tAdsress: " << ConvertIpToString(((struct sockaddr_in *)adapterAddress->addr)->sin_addr.s_addr).c_str() << endl;
			if (adapterAddress->netmask)
				cout << "\tNetmask: " << ConvertIpToString(((struct sockaddr_in *)adapterAddress->netmask)->sin_addr.s_addr).c_str() << endl;
			if (adapterAddress->broadaddr)
				cout << "\tBroadcast Address: " << ConvertIpToString(((struct sockaddr_in *)adapterAddress->broadaddr)->sin_addr.s_addr).c_str() << endl;
			if (adapterAddress->dstaddr)
				cout << "\tDestination Address: " << ConvertIpToString(((struct sockaddr_in *)adapterAddress->dstaddr)->sin_addr.s_addr).c_str() << endl;
			break;

		case AF_INET6:
			cout << "\tAddress Family Name: AF_INET6" << endl;
			if (adapterAddress->addr)
				cout<< "\tAddress: " << ConvertIpv6ToString(adapterAddress->addr, ip6str, sizeof(ip6str)) << endl;
			break;

		default:
			cout << "\tAddress Family Name: Unknown" << endl;
			break;
		}
	}
}

void NetworkAssembler::initializeAttachedDevices()
{
	// Getting the network adapters information,
	pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed*/, &m_NetworkAdapters, ErrorMessage);
}

unsigned int NetworkAssembler::getNetmask()
{
	if (m_ActiveAdapter->addresses != NULL)
		return ((struct sockaddr_in *)(m_ActiveAdapter->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		return 0xFFFFFF;
}

unsigned int NetworkAssembler::GetTcpLength(tcpHeader* tcp)
{
	return tcp->data_offset * 4;
}

unsigned int NetworkAssembler::GetIpLength(ipHeader* ip)
{
	return ip->ip_header_len * 4;
}

void NetworkAssembler::PrintNetworkAdapters()
{
	for (pcap_if_t* networkAdapter = m_NetworkAdapters; networkAdapter != NULL; networkAdapter = networkAdapter->next)
		printNetworkAdapter(networkAdapter);
}

pcap_if_t* NetworkAssembler::GetNetworkAdapters()
{
	if (m_NetworkAdapters != NULL)
		return m_NetworkAdapters;
	return NULL;
}

bool NetworkAssembler::IsSet()
{
	return m_ActiveAdapterHandle != nullptr;
}

bool NetworkAssembler::TargetAdapter(char* name)
{
	return TargetAdapter(getNetworkAdapterByName(name));
}

bool NetworkAssembler::TargetAdapter(int index)
{
	return TargetAdapter(getNetworkAdapterByIndex(index));
}

bool NetworkAssembler::TargetAdapter(pcap_if_t* networkAdapter)
{
	if (networkAdapter == NULL)
		return false;

	m_ActiveAdapter = networkAdapter;
	m_ActiveAdapterHandle = pcap_open(networkAdapter->name, PACKETSIZE, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, ErrorMessage);

	if (m_ActiveAdapterHandle == NULL)
		return false;

	return true;
}

bool NetworkAssembler::StartCapture(bool isAsync)
{
	if (m_ActiveAdapterHandle == NULL)
		return false;

	if (isAsync)
		pcap_dispatch(m_ActiveAdapterHandle, 0, genericPacketsHandler, NULL);
	else
		pcap_loop(m_ActiveAdapterHandle, 0, genericPacketsHandler, NULL);

	return true;
}

bool NetworkAssembler::StartCapture(void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data), bool isAsync)
{	
	if (m_ActiveAdapterHandle == NULL)
		return false;

	if (isAsync)
		pcap_dispatch(m_ActiveAdapterHandle, 0, packetHandler, NULL);
	else
		pcap_loop(m_ActiveAdapterHandle, 0, packetHandler, NULL);

	return true;
}

bool NetworkAssembler::SetFilter(string filteringExpression)
{
	unsigned int netmask;
	bpf_program compiledFilterCode;

	if ((m_ActiveAdapter == NULL) || (m_ActiveAdapterHandle == NULL))
		return false;

	netmask = getNetmask();

	if (pcap_compile(m_ActiveAdapterHandle, &compiledFilterCode, filteringExpression.c_str(), 1, netmask) < 0)
		return false;

	if (pcap_setfilter(m_ActiveAdapterHandle, &compiledFilterCode) < 0)
		return false;

	return true;
}

tcpHeader* NetworkAssembler::CreateTcpHeader(unsigned short sourcePort, unsigned short destinationPort, unsigned int sequence, unsigned int acknowledge, tcpFlags flags, unsigned short windowsSize)
{
	tcpHeader* ret_TcpHeader = new tcpHeader;

	ret_TcpHeader->source_port = htons(sourcePort);
	ret_TcpHeader->dest_port = htons(destinationPort);
	ret_TcpHeader->sequence = htons(sequence);
	ret_TcpHeader->acknowledge = htons(acknowledge);
	ret_TcpHeader->reserved_part1 = 0;
	ret_TcpHeader->data_offset = 5;
	ret_TcpHeader->fin = flags.fin;
	ret_TcpHeader->syn = flags.syn;
	ret_TcpHeader->rst = flags.rst;
	ret_TcpHeader->psh = flags.psh;
	ret_TcpHeader->ack = flags.ack;
	ret_TcpHeader->urg = flags.urg;
	ret_TcpHeader->ecn = flags.ecn;
	ret_TcpHeader->cwr = flags.cwr;
	ret_TcpHeader->window = htons(windowsSize);
	ret_TcpHeader->checksum = 0;
	ret_TcpHeader->urgent_pointer = 0;

	return ret_TcpHeader;
}

ipHeader* NetworkAssembler::CreateIpHeader(PCTSTR sourceIpAddress, PCTSTR destinationIpAddress, unsigned int payloadLength, unsigned char protocol, unsigned char ttl)
{
	ipHeader* ret_IpHeader = new ipHeader;

	ret_IpHeader->ip_version = 4;
	ret_IpHeader->ip_header_len = 5; //In double words thats 4 bytes
	ret_IpHeader->ip_tos = 0;
	ret_IpHeader->ip_total_length = htons(sizeof(ipHeader) + sizeof(tcpHeader) + payloadLength);
	ret_IpHeader->ip_id = htons(2);
	ret_IpHeader->ip_frag_offset = 0;
	ret_IpHeader->ip_reserved_zero = 0;
	ret_IpHeader->ip_dont_fragment = 1;
	ret_IpHeader->ip_more_fragment = 0;
	ret_IpHeader->ip_frag_offset1 = 0;
	ret_IpHeader->ip_ttl = ttl;
	ret_IpHeader->ip_protocol = protocol;
	InetPton(AF_INET, sourceIpAddress, &ret_IpHeader->ip_srcaddr);
	InetPton(AF_INET, destinationIpAddress, &ret_IpHeader->ip_destaddr);
	ret_IpHeader->ip_checksum = 0;
	ret_IpHeader->ip_checksum = calculateChecksum((unsigned short*)ret_IpHeader, sizeof(ipHeader));

	return ret_IpHeader;
}

ethernetHeader* NetworkAssembler::CreateEthernetHeader(unsigned char sourceMac[6], unsigned char destinationMac[6], unsigned short type)
{
	ethernetHeader* ret_ethernetHeader = new ethernetHeader;
	
	memcpy(ret_ethernetHeader, destinationMac, 6);
	memcpy((char*)ret_ethernetHeader + 6, sourceMac, 6);
	ret_ethernetHeader->ether_type = type;

	return ret_ethernetHeader;
}

arpHeader* NetworkAssembler::CreateArpHeader(u_short opcode, u_char senderMAC[6], PCTSTR senderIP, u_char targetMAC[6], PCTSTR targetIP)
{
	arpHeader* ret_arpHeader = new arpHeader;

	ret_arpHeader->htype = 0x0100; // Ethernet (1).
	ret_arpHeader->ptype = 0x0008; // IP Protocol.
	ret_arpHeader->hlen = 6;
	ret_arpHeader->plen = 4;
	ret_arpHeader->oper = opcode; // Request/Reply.
	
	memcpy(ret_arpHeader->sha, senderMAC, 6);
	InetPton(AF_INET, senderIP, &ret_arpHeader->spa);
	memcpy(ret_arpHeader->tha, targetMAC, 6);
	InetPton(AF_INET, targetIP, &ret_arpHeader->tpa);

	return ret_arpHeader;
}

void NetworkAssembler::SendPacket(ethernetHeader* ethernet, arpHeader* arp)
{
	unsigned int packetLength = ETHERNET_LENGTH + sizeof(arpHeader);
	
	if (packetLength < 60) // minimum packet length.
		packetLength = 60;

	const unsigned char* packet = new unsigned char[packetLength];

	ethernetHeader* ethernetOffset = (ethernetHeader*)packet;
	arpHeader* arpOffset = (arpHeader*)(packet + ETHERNET_LENGTH);

	memcpy(ethernetOffset, ethernet, ETHERNET_LENGTH);
	memcpy(arpOffset, arp, sizeof(arpHeader));

	pcap_sendpacket(m_ActiveAdapterHandle, packet, packetLength);

	delete packet;
}

void NetworkAssembler::SendPacket(ethernetHeader* ethernet, ipHeader* ip, tcpHeader* tcp, char* payload, unsigned int payloadLength)
{
	unsigned int packetLength = payloadLength + ETHERNET_LENGTH + GetIpLength(ip) + GetTcpLength(tcp);
	const unsigned char* packet = new unsigned char[packetLength];

	ethernetHeader* ethernetOffset = (ethernetHeader*)packet;
	ipHeader* ipOffset = (ipHeader*)(packet + ETHERNET_LENGTH);
	tcpHeader* tcpOffset = (tcpHeader*)(packet + ETHERNET_LENGTH + GetIpLength(ip));
	char* payloadOffset = (char*)(packet + ETHERNET_LENGTH + GetIpLength(ip) + GetTcpLength(tcp));

	memcpy(ethernetOffset, ethernet, ETHERNET_LENGTH);
	memcpy(ipOffset, ip, GetIpLength(ip));
	memcpy(tcpOffset, tcp, GetTcpLength(tcp));
	memcpy(payloadOffset, payload, payloadLength);

	pcap_sendpacket(m_ActiveAdapterHandle, packet, packetLength);

	delete packet;
}