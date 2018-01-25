#pragma once
#include <cpprest/http_client.h>


class NetherConnector
{
public:
	NetherConnector();
	~NetherConnector();

	pplx::task<void> RequestTask();
};

