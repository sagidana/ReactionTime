
#include <iostream>


#include <NetworkManager.h>


using namespace std;

void __cdecl packetHandler(u_char* param, const pcap_pkthdr* header, const u_char* data)
{

}

int main()
{
	NetworkManager* networkManager = new NetworkManager();
	networkManager->PrintNetworkAdapters();
	networkManager->TargetAdapter(3);
	networkManager->StartCapture(packetHandler);

	return 1;
}

