#pragma once

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <asio.hpp>
#include <bind.hpp>

using boost::asio::ip::tcp;

class HttpClient
{
private:
	tcp::resolver _resolver;              // provides the ability to resolve a query to a list of endpoints
	tcp::socket _socket;                  // socket for data exchange between client and server
	std::string _server;                  // server for connect "localhost"
	std::string _port;                    // port for connecting
	std::string _session;                 // user session
	boost::asio::streambuf _request;      // buffer for request
	boost::asio::streambuf _response;     // buffer for response

	void fHandleResolve(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);

	void fHandleConnect(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);

	void fHandleWriteRequest(const boost::system::error_code &err);

	void fHandleReadStatusLine(const boost::system::error_code &err);

	void fHandleReadHeaders(const boost::system::error_code &err);

	void fHandleReadContent(const boost::system::error_code &err);
	
	public:

		HttpClient(boost::asio::io_service& io_service, const std::string &server, const std::string &port);
		
		void PostData(const std::string &path, std::string &data);
		void GetData(std::string &path);
		std::string fGetSession();
		void fSetSession(std::string &session);
};
