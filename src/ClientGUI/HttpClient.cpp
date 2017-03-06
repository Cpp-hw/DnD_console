// HttpClient.cpp

#include "Includes/HttpClient.hpp"



/*
Initializes and starts http server.
*/
HttpClient::HttpClient(boost::asio::io_service& io_service, const std::string &server, const std::string &port) : _resolver(io_service), _socket(io_service)
{
	this->_server  =  server;
	this->_port    =  port;
	// Start an asynchronous resolve to translate the server and service names
	// into a list of endpoints.
	tcp::resolver::query query(server, port);
	_endpoint_iterator = _resolver.resolve(query);
}

/*
    Method for getting data from server.
*/
void HttpClient::fGet(const std::string &path)
{
	// Form the request.
	std::ostream request_stream(&_request);
	request_stream << "GET " << path << " HTTP/1.1\r\n";    // Request type and path
	request_stream << "Host: " << _server << "\r\n";		    // Host "localhost" for example
	request_stream << "Accept: *///*//\r\n";   
	request_stream << "Connection: close\r\n\r\n";
	
	boost::asio::write(_socket, _request);
}

/*
	Method for postring data to server.
*/
void HttpClient::fPost(const std::string &path, const std::string &data)
{
	//Form the request

	//boost::asio::connect(_socket, _endpoint_iterator);
	std::ostream request_stream(&_request);
	request_stream << "POST " << path << " HTTP/1.1\r\n";	                // Request type and path
	request_stream << "Host: " << _server << " \r\n";		                // Host "localhost" for example
	request_stream << "Accept: */*\r\n";
	request_stream << "Content-Type: " << "application/json" << " \r\n";    
	request_stream << "Content-Length: " << data.length() << " \r\n";
	request_stream << "Connection: close\r\n\r\n";
	request_stream << data;                                                 // JSON data
	
	boost::asio::write(_socket, _request);
}

void HttpClient::fPut(const std::string &path, const std::string &data)
{
	std::ostream request_stream(&_request);
	request_stream << "PUT " << path << " HTTP/1.1\r\n";	                // Request type and path
	request_stream << "Host: " << _server << " \r\n";		                // Host "localhost" for example
	request_stream << "Accept: */*\r\n";
	request_stream << "Content-Type: " << "application/json" << " \r\n";
	request_stream << "Content-Length: " << data.length() << " \r\n";
	request_stream << "Connection: close\r\n\r\n";
	request_stream << data;                                                 // JSON data

	boost::asio::write(_socket, _request);
}

void HttpClient::fDelete(const std::string &path, const std::string &data)
{
	std::ostream request_stream(&_request);
	request_stream << "DELETE " << path << " HTTP/1.1\r\n";	                // Request type and path
	request_stream << "Host: " << _server << " \r\n";		                // Host "localhost" for example
	request_stream << "Accept: */*\r\n";
	request_stream << "Content-Type: " << "application/json" << " \r\n";
	request_stream << "Content-Length: " << data.length() << " \r\n";
	request_stream << "Connection: close\r\n\r\n";
	request_stream << data; 
	// JSON data
	boost::asio::write(_socket, _request);
}

void HttpClient::fSendRequest(const Methods &method,const std::string &path, const std::string &data=" ")
{
	switch (method)
	{
	case HttpClient::Methods::_GET:
	{
		fConnect();
		fGet(path);
	}
	break;
	case HttpClient::Methods::_POST:
	{
		fConnect();
		fPost(path, data);
	}
	break;
	case HttpClient::Methods::_PUT:
	{
		fConnect();
		fPut(path,data);
	}
	break;
	case HttpClient::Methods::_DELETE:
	{
		fConnect();
		fDelete(path, data);
	}
	}
}

void HttpClient::fGetResponse(std::string &response_)
{
	boost::asio::streambuf response;
	boost::asio::read_until(_socket, response, "\r\n");

	// Check that response is OK.
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	std::string status_message;
	std::getline(response_stream, status_message);
	if (!response_stream || http_version.substr(0, 5) != "HTTP/")
	{
		std::cout << "Invalid response\n";
	}
	if (status_code != 200)
	{
		std::cout << "Response returned with status code " << status_code << "\n";
	}

	// Read the response headers, which are terminated by a blank line.
	boost::asio::read_until(_socket, response, "\r\n\r\n");

	// Process the response headers.
	std::string header;
	while (std::getline(response_stream, header) && header != "\r")
		std::cout << header << "\n";
	std::cout << "\n";

	// Write whatever content we already have to output.
	std::ostringstream oss;

	if (response.size() > 0)
	{
		oss << &response;
	}
	// Read until EOF, writing data to output as we go.
	boost::system::error_code error;
	while (boost::asio::read(_socket, response,
		boost::asio::transfer_at_least(1), error))
		oss << &response;
	if (error != boost::asio::error::eof)
		throw boost::system::system_error(error);
	response_ = oss.str();
}

void HttpClient::fConnect()
{
	boost::asio::connect(_socket, _endpoint_iterator);
}

/*
	Method for getting response from server.
*/
//std::string HttpClient::fGetResponse()
//{
//	//fReadResponse();
//	return this->_response_string;
//	_socket.close();
//}

