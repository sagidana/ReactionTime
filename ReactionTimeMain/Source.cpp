#include "Precompiled.h"

#include <iostream>
#include <memory>

using namespace std;


int main()
{

	
	return 0;
}


// Example 1.

//NetworkManager* networkManager = new NetworkManager();
//networkManager->PrintNetworkAdapters();
//networkManager->TargetAdapter(1);
//networkManager->SetFilter("http");
//networkManager->StartCapture(false);

// Example 2.

//NetworkManager* networkManager = new NetworkManager();
//networkManager->PrintNetworkAdapters();
//networkManager->TargetAdapter(1);

//char* payload = (char*) malloc(200);

//if (!payload)
//	return 1;

//memset(payload, 1, 200);

//unsigned char sourceMac[6] = { 0x1C, 0x6F, 0x65, 0xD9, 0x2D, 0xB9 };
//unsigned char destinationMac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//auto ethernetHeader = networkManager->CreateEthernetHeader(sourceMac, destinationMac);
//auto ipHeader = networkManager->CreateIpHeader(L"9.9.9.9", L"8.8.8.8", 200);
//auto tcpHeader = networkManager->CreateTcpHeader(1111,2222,0,0);

//networkManager->SendPacket(ethernetHeader, ipHeader, tcpHeader, payload, 200);