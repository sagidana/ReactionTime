#include "NetworkReact.h"

using namespace std;

NetworkReact::NetworkReact(unique_ptr<NetworkAssembler> assembler)
{
	if (assembler->IsSet())
		_assembler = move(assembler);
	else
		throw bad_exception("invalid NetworkAssembler argument");
}

void NetworkReact::BeginProxy(string filter)
{
	if (filter == "")
		throw logic_error("Filter cannot be empty.");

	_assembler->SetFilter(filter);

	_assembler->StartCapture([] (u_char *param, const struct pcap_pkthdr *header, const u_char *packet) {

		string source = NetworkAssembler::GetSourceIpAndPort(packet, header);
		string destination = NetworkAssembler::GetDestinationIpAndPort(packet, header);
		string httpPayload = NetworkAssembler::ExportHttpPayload(packet, header);

		int a = 0;

	}, false);
}

void NetworkReact::SendArpRequest(u_char targetMAC[6], PCTSTR targetIP, u_char spoofedMAC[6], PCTSTR spoofedIP)
{
	ethernetHeader* ethernet = _assembler->CreateEthernetHeader(spoofedMAC, targetMAC, 0x0608 /*Arp protocol*/ );
	arpHeader* arp = _assembler->CreateArpHeader(0x0100, spoofedMAC, spoofedIP, targetMAC, targetIP);

	_assembler->SendPacket(ethernet, arp);
}

void NetworkReact::SendArpReply(u_char targetMAC[6], PCTSTR targetIP, u_char spoofedMAC[6], PCTSTR spoofedIP)
{
	ethernetHeader* ethernet = _assembler->CreateEthernetHeader(spoofedMAC, targetMAC, 0x0608 /*Arp protocol*/);
	arpHeader* arp = _assembler->CreateArpHeader(0x0200, spoofedMAC, spoofedIP, targetMAC, targetIP);

	_assembler->SendPacket(ethernet, arp);
}

NetworkReact::~NetworkReact() { }