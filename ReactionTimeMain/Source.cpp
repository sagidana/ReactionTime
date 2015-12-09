#include <iostream>

#include <NetworkManager.h>

using namespace std;

int main()
{
	NetworkManager* networkManager = new NetworkManager();
	networkManager->PrintNetworkAdapters();
	networkManager->TargetAdapter(3);
	networkManager->SetFilter("http");
	networkManager->StartCapture(false);

	return 1;
}

