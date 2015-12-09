#include "NetworkManager.h"

pcap_t* NetworkManager::m_ActiveAdapterHandle;

bool NetworkManager::translatePacket(const u_char* packet, ethernetHeader** ethernet, ipHeader** ip, tcpHeader** tcp, char** payload)
{
	u_int ipHeaderLength;
	u_int tcpHeaderLength;

	if (pcap_datalink(m_ActiveAdapterHandle) == DLT_EN10MB) // if datalink is ethernet
	{
		*ethernet = (ethernetHeader*)(packet);
		*ip = (ipHeader*)(packet + ETHERNET_LENGTH); // Found the ip header
		ipHeaderLength = IP_HL(*ip) * 4;

		if (ipHeaderLength < 20) // Bad ip header
			return false;

		*tcp = (tcpHeader*)(packet + ETHERNET_LENGTH + ipHeaderLength); // Found the tcp header
		tcpHeaderLength = TH_OFF(*tcp) * 4;

		if (tcpHeaderLength < 20) // Bad tcp header
			return false;

		*payload = (char *)(packet + ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength);

		return true;
	}

	return false;
}

void __cdecl NetworkManager::genericPacketsHandler(u_char* param, const pcap_pkthdr* header, const u_char* packet)
{
	ethernetHeader* ethernet;
	ipHeader* ip;
	tcpHeader* tcp;
	char* payload;
	
	if (translatePacket(packet, &ethernet, &ip, &tcp, &payload))
	{
		u_int ipHeaderLength = IP_HL(ip) * 4;
		u_int tcpHeaderLength = TH_OFF(tcp) * 4;
		
		if (header->caplen < ETHERNET_LENGTH + ipHeaderLength + tcpHeaderLength) //Length of the data captured was not enough,
			return;

		u_int payloadLength = header->caplen - ETHERNET_LENGTH - ipHeaderLength - tcpHeaderLength;
	
		for (int index = 0; index < payloadLength; index++)
		{
			if (isprint(payload[index]))
				cout << payload[index];

			if ((index % 70 == 0) || index == payloadLength - 1)
				cout << endl;
		}
	}
}

NetworkManager::NetworkManager()
{
	initializeAttachedDevices();
}

NetworkManager::~NetworkManager()
{
	// Free all devices
	pcap_freealldevs(m_NetworkAdapters);
}

pcap_if_t* NetworkManager::getNetworkAdapterByName(char* name)
{
	int nameLength = strnlen_s(name, 100);
	pcap_if_t* ret_NetworkAdapter = m_NetworkAdapters;

	for (ret_NetworkAdapter = m_NetworkAdapters; ret_NetworkAdapter != NULL; ret_NetworkAdapter = ret_NetworkAdapter->next)
		if (strncmp(ret_NetworkAdapter->name, name, nameLength) == 0)
			break;

	return ret_NetworkAdapter;
}

pcap_if_t* NetworkManager::getNetworkAdapterByIndex(int index)
{
	pcap_if_t* ret_NetworkAdapter = m_NetworkAdapters;

	for (int loopIndex = 0; loopIndex < index - 1; loopIndex++)
	{
		if (ret_NetworkAdapter != NULL)
			ret_NetworkAdapter = ret_NetworkAdapter->next;
	}
	
	return ret_NetworkAdapter;
}

void NetworkManager::printNetworkAdapter(pcap_if_t* networkAdapter)
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

char* NetworkManager::convertIpToString(u_long in)
{
	static char output[IPTOSBUFFERS][16];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	_snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

char* NetworkManager::ConvertIpv6ToString(struct sockaddr *socketAddress, char *address, int addressLength)
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

void NetworkManager::printAdapterAddresses(pcap_addr_t* adapterAddresses)
{
	char ip6str[128];

	for (pcap_addr_t* adapterAddress = adapterAddresses; adapterAddress != NULL; adapterAddress = adapterAddress->next)
	{
		switch (adapterAddress->addr->sa_family)
		{
		case AF_INET:
			printf("\tAddress Family Name: AF_INET\n");
			if (adapterAddress->addr)
				cout << "\tAdsress: " << convertIpToString(((struct sockaddr_in *)adapterAddress->addr)->sin_addr.s_addr) << endl;
			if (adapterAddress->netmask)
				cout << "\tNetmask: " << convertIpToString(((struct sockaddr_in *)adapterAddress->netmask)->sin_addr.s_addr) << endl;
			if (adapterAddress->broadaddr)
				cout << "\tBroadcast Address: " << convertIpToString(((struct sockaddr_in *)adapterAddress->broadaddr)->sin_addr.s_addr) << endl;
			if (adapterAddress->dstaddr)
				cout << "\tDestination Address: " << convertIpToString(((struct sockaddr_in *)adapterAddress->dstaddr)->sin_addr.s_addr) << endl;
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

void NetworkManager::initializeAttachedDevices()
{
	// Getting the network adapters information,
	pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL /* auth is not needed*/, &m_NetworkAdapters, ErrorMessage);
}

unsigned int NetworkManager::getNetmask()
{
	if (m_ActiveAdapter->addresses != NULL)
		return ((struct sockaddr_in *)(m_ActiveAdapter->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		return 0xFFFFFF;
}

void NetworkManager::PrintNetworkAdapters()
{
	for (pcap_if_t* networkAdapter = m_NetworkAdapters; networkAdapter != NULL; networkAdapter = networkAdapter->next)
		printNetworkAdapter(networkAdapter);
}

pcap_if_t* NetworkManager::GetNetworkAdapters()
{
	if (m_NetworkAdapters != NULL)
		return m_NetworkAdapters;
	return NULL;
}

bool NetworkManager::TargetAdapter(char* name)
{
	return TargetAdapter(getNetworkAdapterByName(name));
}

bool NetworkManager::TargetAdapter(int index)
{
	return TargetAdapter(getNetworkAdapterByIndex(index));
}

bool NetworkManager::TargetAdapter(pcap_if_t* networkAdapter)
{
	if (networkAdapter == NULL)
		return false;

	m_ActiveAdapter = networkAdapter;
	m_ActiveAdapterHandle = pcap_open(networkAdapter->name, PACKETSIZE, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, ErrorMessage);

	if (m_ActiveAdapterHandle == NULL)
		return false;

	return true;
}

bool NetworkManager::StartCapture(bool isAsync)
{
	if (m_ActiveAdapterHandle == NULL)
		return false;

	if (isAsync)
		pcap_dispatch(m_ActiveAdapterHandle, 0, genericPacketsHandler, NULL);
	else
		pcap_loop(m_ActiveAdapterHandle, 0, genericPacketsHandler, NULL);

	return true;
}

bool NetworkManager::StartCapture(void(*packetHandler)(u_char *param, const struct pcap_pkthdr *header, const u_char *data), bool isAsync)
{	
	if (m_ActiveAdapterHandle == NULL)
		return false;

	if (isAsync)
		pcap_dispatch(m_ActiveAdapterHandle, 0, packetHandler, NULL);
	else
		pcap_loop(m_ActiveAdapterHandle, 0, packetHandler, NULL);

	return true;
}

bool NetworkManager::SetFilter(string filteringExpression)
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

void NetworkManager::SendPacket()
{
	ethernetHeader
}