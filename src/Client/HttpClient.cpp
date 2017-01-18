// HttpClient.cpp

#include "Includes\HttpClient.hpp"



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
	_resolver.async_resolve(query, boost::bind(&HttpClient::fHandleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
}


void HttpClient :: fHandleResolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		// Attempt a connection to the first endpoint in the list. Each endpoint
		// will be tried until we successfully establish a connection.
		tcp::endpoint endpoint = *endpoint_iterator;
		_socket.async_connect(endpoint, boost::bind(&HttpClient::fHandleConnect, this, boost::asio::placeholders::error, ++endpoint_iterator));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

/*
    Method for connecting to server.
*/
void HttpClient :: fHandleConnect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
	if (!err)
	{
		// The connection was successful. Send the request.
		boost::asio::async_write(_socket, _request, boost::bind(&HttpClient::fHandleWriteRequest, this, boost::asio::placeholders::error));
	}
	else if (endpoint_iterator != tcp::resolver::iterator())
	{
		// The connection failed. Try the next endpoint in the list.
		_socket.close();
		tcp::endpoint endpoint = *endpoint_iterator;
		_socket.async_connect(endpoint, boost::bind(&HttpClient::fHandleConnect, this, boost::asio::placeholders::error, ++endpoint_iterator));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}


/*
    Method for sending request to server.
*/

void HttpClient:: fHandleWriteRequest(const boost::system::error_code& err)
{
	if (!err)
	{
		// Read the response status line.
		boost::asio::async_read_until(_socket, _response, "\r\n", boost::bind(&HttpClient::fHandleReadStatusLine, this, boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err.message() << "\n";
	}
}

/*
    Method for getting connection status.
*/
void HttpClient:: fHandleReadStatusLine(const boost::system::error_code& err)
{
	if (!err)
	{
		// Check that response is OK.
		std::istream response_stream(&_response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			std::cout << "Response returned with status code ";
			std::cout << status_code << "\n";
			return;
		}

		// Read the response headers, which are terminated by a blank line.
		boost::asio::async_read_until(_socket, _response, "\r\n\r\n", boost::bind(&HttpClient::fHandleReadHeaders, this, boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}


/*
    Method for reading headers.
*/
void HttpClient:: fHandleReadHeaders(const boost::system::error_code& err)
{
	if (!err)
	{
		// Process the response headers.
		std::istream response_stream(&_response);
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
			std::cout << header << "\n";
		std::cout << "\n";

		// Write whatever content we already have to output.
		
		if (_response.size() > 0)
		{
			std::istream istream(&_response);
			std::getline(istream, _session);
			std::cout << &_response;
			
		}
		
		// Start reading remaining data until EOF.
		boost::asio::async_read(_socket, _response, boost::asio::transfer_at_least(1), boost::bind(&HttpClient::fHandleReadContent, this, boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "Error: " << err << "\n";
	}
}

/*
    Method for getting server response.
*/
void HttpClient:: fHandleReadContent(const boost::system::error_code& err)
{
	if (!err)
	{
		// Write all of the data that has been read so far.
		std::cout << &_response;
		// Continue reading remaining data until EOF.
		boost::asio::async_read(_socket, _response, boost::asio::transfer_at_least(1), boost::bind(&HttpClient::fHandleReadContent, this, boost::asio::placeholders::error));
	}
	else if (err != boost::asio::error::eof)
	{
		std::cout << "Error: " << err << "\n";
	}
}


/*
    Method for getting data from server.
*/
void HttpClient::GetData(std::string &path)
{
	// Form the request.
	std::ostream request_stream(&_request);
	request_stream << "GET " << path << " HTTP/1.1\r\n";    // Request type and path
	request_stream << "Host: " << _server << "\r\n";		    // Host "localhost" for example
	request_stream << "Accept: *///*//\r\n";   
	request_stream << "Connection: close\r\n\r\n";
}

/*
	Method for postring data to server.
*/
void HttpClient::PostData(const std::string &path, std::string &data)
{
	//Form the request
	std::ostream request_stream(&_request);
	request_stream << "POST " << path << " HTTP/1.1\r\n";	                // Request type and path
	request_stream << "Host: " << _server << " \r\n";		                // Host "localhost" for example
	request_stream << "Accept: */*\r\n";
	request_stream << "Content-Type: " << "application/json" << " \r\n";    
	request_stream << "Content: "<< data << " \r\n";                          // JSON data
	request_stream << "Content-Length: " << data.length() << " \r\n";
	request_stream << "Connection: close\r\n\r\n";

}

/*
	Method for getting user session.
*/
std::string HttpClient::fGetSession()
{
	return this->_session;
}

/*
	Method for reset user session.
*/
void HttpClient::fSetSession(std::string &session)
{
	this->_session = session;
}