#include "Precompiled.h"
#include "NetworkAssembler.h"
#include <iostream>
#include <memory>
#include <Windows.h>

using namespace std;

/// Examples

void startCapture()
{
	auto networkManager = make_unique<NetworkAssembler>();

	networkManager->PrintNetworkAdapters();
	networkManager->TargetAdapter(1);
	networkManager->SetFilter("http");
	networkManager->StartCapture(false);
}

void createArpRequestAndSend()
{
	auto networkAssembler = make_unique<NetworkAssembler>();
	networkAssembler->PrintNetworkAdapters();
	networkAssembler->TargetAdapter(1);

	unsigned char senderMAC[6] = { 0x1C, 0x6F, 0x65, 0xD9, 0x2D, 0xB9 }; // local MAC address.
	unsigned char targetMAC[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // Broadcast...

	ethernetHeader* ethernet = networkAssembler->CreateEthernetHeader(senderMAC, targetMAC, 0x0608);
	arpHeader* arp = networkAssembler->CreateArpHeader(0x0100, senderMAC, L"10.0.0.15", targetMAC, L"10.0.0.138");

	networkAssembler->SendPacket(ethernet, arp);
}

void createArpReplyAndSend(NetworkAssembler* networkAssembler, u_char targetMAC[6], PCTSTR targetIP, u_char spoofedMAC[6], PCTSTR spoofedIP)
{
	ethernetHeader* ethernet = networkAssembler->CreateEthernetHeader(spoofedMAC, targetMAC, 0x0608);
	arpHeader* arp = networkAssembler->CreateArpHeader(0x0200, spoofedMAC, spoofedIP, targetMAC, targetIP);

	networkAssembler->SendPacket(ethernet, arp);
}


void createCustomPacketAndSend()
{
	auto networkManager = make_unique<NetworkAssembler>();
	networkManager->PrintNetworkAdapters();
	networkManager->TargetAdapter(1);

	char* payload = (char*)malloc(200);

	if (!payload)
		return;

	memset(payload, 1, 200);

	unsigned char sourceMac[6] = { 0x1C, 0x6F, 0x65, 0xD9, 0x2D, 0xB9 }; // local MAC address.
	unsigned char destinationMac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // Broadcast...

	auto ethernetHeader = networkManager->CreateEthernetHeader(sourceMac, destinationMac);
	auto ipHeader = networkManager->CreateIpHeader(L"9.9.9.9", L"8.8.8.8", 200);

	tcpFlags flags;

	flags.ack = 0;
	flags.syn = 1;
	flags.fin = 0;
	flags.psh = 0;
	flags.rst = 0;
	flags.urg = 0;
	flags.cwr = 0;
	flags.ecn = 0;

	auto tcpHeader = networkManager->CreateTcpHeader(1111, 2222, 0, 0, flags);

	networkManager->SendPacket(ethernetHeader, ipHeader, tcpHeader, payload, 200);
}

auto main() -> int
{
	auto networkAssembler = make_unique<NetworkAssembler>();
	networkAssembler->PrintNetworkAdapters();
	networkAssembler->TargetAdapter(1);

	while (1)
	{
		u_char targetMAC[6] = { 0xA0, 0x39, 0xF7, 0x42, 0x08, 0x27 };
		createArpReplyAndSend(networkAssembler.get(), targetMAC, L"10.0.0.17", targetMAC, L"10.0.0.15");
		Sleep(10);
	}

	return 0;
}



