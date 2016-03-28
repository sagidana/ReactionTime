#pragma once

#include "NetworkAssembler.h"
#include <memory>
#include <exception>

class __declspec(dllexport) NetworkReact
{
private:
	std::unique_ptr<NetworkAssembler> _assembler;
public:
	NetworkReact(std::unique_ptr<NetworkAssembler> assembler);
	void SendArpRequest(u_char targetMAC[6], PCTSTR targetIP, u_char spoofedMAC[6], PCTSTR spoofedIP);
	void SendArpReply(u_char targetMAC[6], PCTSTR targetIP, u_char spoofedMAC[6], PCTSTR spoofedIP);
	~NetworkReact();
};

