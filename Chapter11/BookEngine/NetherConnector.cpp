#include "NetherConnector.h"



NetherConnector::NetherConnector()
{
}


NetherConnector::~NetherConnector()
{
}

pplx::task<void> NetherConnector::RequestTask()
{

	// Create http_client to send the request.
	web::http::client::http_client client(U("http://netherwebdpv5t6ercaslq.azurewebsites.net"));

	// Build request URI and start the request.
	web::uri_builder builder(U("/api/leaderboard/"));
	//builder.append_query(U("q"), U("cpprestsdk github"));
	return client.request(web::http::methods::GET, builder.to_string()).then([=](web::http::http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());

		// Write response body into the file.
		//return response.body().read_to_end(fileStream->streambuf());
	});
}
