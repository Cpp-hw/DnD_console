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
	std::string _response_string;         // response string from response buffer
	boost::asio::streambuf _request;      // buffer for request
	boost::asio::streambuf _response;     // buffer for response

	void fHandleResolve(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);

	void fHandleConnect(const boost::system::error_code &err, tcp::resolver::iterator endpoint_iterator);

	void fHandleWriteRequest(const boost::system::error_code &err);

	void fHandleReadStatusLine(const boost::system::error_code &err);

	void fHandleReadHeaders(const boost::system::error_code &err);

	void fHandleReadContent(const boost::system::error_code &err);
	

	void fPostData(const std::string &path, const std::string &data);

	void fGetData(const std::string &path);

	void fPutData(const std::string &path, const std::string &data);

	void fDeleteData(const std::string &path, const std::string &data);

	public:

		enum Methods {
			_GET,
			_POST,
			_PUT,
			_DELETE
		};

		HttpClient(boost::asio::io_service& io_service, const std::string &server, const std::string &port);
		
		void fRequest(const Methods &method,const std::string &path, const std::string &data);

		std::string fGetResponse();
};
