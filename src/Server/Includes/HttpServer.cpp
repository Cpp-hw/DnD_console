// HttpServer.cpp
//
// http_server
// for 
// SoftServe ITA
//
// Nicholas Tsaruk
// fbt.ksnv@gmail.com
//


#include "stdafx.hpp"
#include "HttpServer.hpp"


/*
* Initializes and starts http server.
*/
HttpServer::HttpServer(int server_port, string path_to_root)
{
	port			= server_port;
	root			= path_to_root;
	cfd				= -1; // set file descriptor for sockets uninitialized
	sfd				= -1; // set file descriptor for sockets uninitialized
	pFile			= NULL;
	response_body	= NULL;
    errno			= 0; // reset errors
	state_error		= 0;


    if (port < 0 || port > 65535) // ensure port is a non-negative short integer
    {
        state_error =  2;
        state_info += "Server's port is out of range 0...65535\n";
		return;
    }


	
	#ifdef _WIN32
		unsigned long root_path_length = 0;
		char  buffer[4096]; buffer[0] = 0; // set null terminating character in case of pull path retrieving fail
		char** lppPart={NULL};
		GetFullPathNameA(root.data(), 4096, buffer, lppPart);
		root = buffer;
	#else
		root = realpath(root.data(), NULL); // full path to server's root on UNIX
	#endif

    if (!root.length()) // endusre path to server's root is specified
    {
        state_error =  3;
        state_info += "Path to server's root is invalid\n";
		return;
    }

    if (access(root.data(), F_OK) == -1) // ensure root exists
    {
        state_error =  4;
        state_info += "Path to server's root does not exist\n";
		return;
    }
	
	#ifdef _WIN32
		if (access(root.data(), W_OK) == -1) // ensure root is writable
		{
			state_error =  5;
			state_info += "You do not have sufficient right on server's root\n";
			return;
		}
	#else
		if (access(root.data(), X_OK) == -1) // ensure root is executable
		{
			state_error =  5;
			state_info += "You do not have sufficient right on server's root\n";
			return;
		}
	#endif

    state_info += "Using "+root+" for server's root\n";
	
	#ifdef _WIN32
		WSADATA wsaData;
		int res = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (res != 0)
		{
			state_error =  6;
			state_info += "WSAStartup failed with error: "+to_string(res);
			return ;
		}
		sfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket
		if (sfd == -1)
		{
			state_error =  7;
			state_info += "Socket creation failed: ";
			state_info += strerror(WSAGetLastError());
			return;
		}
	#else
		sfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket
		if (sfd == -1)
		{
			state_error =  7;
			state_info += "Socket creation failed";
			return;
		}
	#endif


    int optval = 1;
    setsockopt(sfd,
				SOL_SOCKET,
				SO_REUSEADDR, // allow reuse of address (to avoid "Address already in use")
				(char *)&optval,
				sizeof(optval));
	
    // assign name to socket
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		state_error =  7;
		state_info += "Socket bind failed";
		return;
	}
	
    
    if (listen(sfd, SOMAXCONN) == -1) // start port listening
	{
		state_error =  8;
		state_info += "Port listening initiation failed.";
		return;
	}
	
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    if (getsockname(sfd, (struct sockaddr*) &addr, &addrlen) == -1)
	{
		state_error =  9;
		state_info += "Getsockname function failed.";
		return;
	}
	
    state_info += "Listening on port "+to_string(ntohs(addr.sin_port))+"\n";
}

/*
* Stop server, deallocating any resources.
*/
HttpServer::~HttpServer(void)
{
    int errsv = errno; // preserve errno across this function's library calls

    fReset(); // reset server's state
	port = 0;
    root = ""; // free root, which was allocated by realpath
	state_error = -1;

    if (sfd != -1) // close server socket
	{
		#ifdef _WIN32
			closesocket(sfd);
			WSACleanup();
		#else
			close(sfd);
		#endif
	}

    if (errsv) // announce error
        cout<<strerror(errsv)<<endl;
}

/*
* Main server's loop, that waits and serves client connection.
*/
void HttpServer::fRun()
{
	while (!state_error)
    {
		cout<<state_info<<endl;
    	fReset(); // reset server's state
        if (fConnected()) // wait until client is connected
        {
            ssize_t request_size = fParse(); // parse client's HTTP request
            if (request_size < 0)
			{
				state_info += "CLIENT REQUEST FAILED. Error code:"+to_string(request_size)+"\n";
                continue;
			}
			
            int newline_pos = request.find("\r\n");
            if (newline_pos == -1)
            {
            	fError(400); // Bad Request
                continue;
            }
            if (request.length() > LIMIT_REQUEST_LINE)
            {
            	fError(414); // Request-URI Too Long
                continue;
            }
            request_header = request.substr(0, newline_pos); // extract request's request-line header // http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
            // request_header = "GET /favicon.ico HTTP/1.1"; / for test purpose only
            // validate request-header
            int sp_loc = request_header.find(' ');
            int nsp_loc = request_header.find(' ', sp_loc+1);
            int qm_loc = request_header.find('?', nsp_loc+1);
            int version_loc = request_header.find("HTTP/1.1");
			
			string request_method;
			string request_uri;
			string requested_path;
			string requested_file_extension;
			string requested_file_mime_type;
			string request_query;
			string request_version = "HTTP/1.1";

            if (version_loc == -1)
            {
                fError(505); // HTTP Version Not Supported
                continue;
            }

			request_method = request_header.substr(0, sp_loc);
			if (request_method.compare("GET") && request_method.compare("POST") && request_method.compare("PUT") && request_method.compare("DELETE"))
			{
				fError(405); // Method Not Allowed
				continue;
			}

			request_uri = request_header.substr(sp_loc+1, nsp_loc-sp_loc-1);
            if (!request_uri.length())
            {
                fError(400); // Bad Request
                continue;
            }

			if (qm_loc > -1)
			{
				requested_path = request_uri.substr(0, qm_loc);
				request_query = request_uri.substr(qm_loc, nsp_loc);
			}
			else
				requested_path = request_uri.substr(0, nsp_loc);

            requested_file_extension = requested_path.substr(requested_path.find('.') + 1);
            requested_file_mime_type = fLookup(requested_file_extension);
            /*if (!requested_file_mime_type.length())
            {
                fError(501); // Not Implemented
                continue;
            }*/


			state_info += "REQUEST METHOD: "+request_method+"\n";
			state_info += "REQUEST URI: "+request_uri+"\n";
			state_info += "REQUESTED PATH: \""+requested_path+"\"\n";
			state_info += "REQUEST QUERY: "+request_query+"\n";
			state_info += "REQUEST VERSION: "+request_version+"\n";
			state_info += "REQUESTED FILE EXTENSION: "+requested_file_extension+"\n";
			state_info += "REQUESTED FILE MIME TYPE: "+requested_file_mime_type+"\n";
			state_info += "REQUEST VERSION: "+request_version+"\n";

            if (access((root+requested_path).data(), F_OK) == -1 && requested_path.length()>1) // ensure path exists
            {
            	cout<<"trying access to:\""<<(root+requested_path)<<"\""<<endl;
                fError(404); // Not Found
                continue;
            }
            if (access((root+requested_path).data(), R_OK) == -1) // ensure path is readable
            {
                fError(403); // Forbidden
                continue;
            }


            pFile = fopen((root+requested_path).data(), "rb");
			ssize_t length = 0;
            if (pFile == NULL)
            {
				length = 0;
				state_info += "nothing to load\n";

            }
			else
			{
				length = fLoad(); // load file
				if (length == -1)
				{
					fError(500); // Internal Server Error
					continue;
				}
			}
			
			/*int i = newline_pos+2;
			while (i < (int)request.length()-2)
			{
				i = request.find("\r\n", newline_pos);
				string request_line = request.substr(newline_pos, i-newline_pos); // extract request's request-line // http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html
				cout<<"request_line: "<<request_line<<endl;
				newline_pos = i+2;
			}*/
			
            // respond to client
			string response_str = "HTTP/1.1 200 OK\r\n";
			if (send( cfd, response_str.data(), response_str.length(), 0 ) == -1)
				continue;

			response_str = "Connection: close\r\n";
			if (send( cfd, response_str.data(), response_str.length(), 0 ) == -1)
				continue;

			response_str = "Content-Length: "+to_string(length)+"\r\n";
			if (send( cfd, response_str.data(), response_str.length(), 0 ) == -1)
				continue;

			response_str = "Content-Type: "+requested_file_mime_type+"\r\n\r\n";
			if (send( cfd, response_str.data(), response_str.length(), 0 ) == -1)
				continue;

			if (send(cfd, response_body, length, 0) == -1)
				continue;
			response_str = "\r\n\r\n";
			if (send( cfd, response_str.data(), response_str.length(), 0 ) == -1)
			{
				cout<<"noway"<<endl;
				continue;
			}

            // announce OK
            state_info += "HTTP/1.1 200 OK\n";
        }
    }
}

/*
* Resets server's state, deallocating any resources.
*/
void HttpServer::fReset(void)
{
    if (response_body != NULL) // free response's body
    {
        delete [] response_body;
        response_body = NULL;
    }

    if (pFile != NULL) // close file
    {
        fclose(pFile);
        pFile = NULL;
    }

    request = "";
	state_error = 0;
	state_info = "";

    if (cfd != -1) // close client's socket
    {
		#ifdef _WIN32
			closesocket(cfd);
		#else
			close(cfd);
		#endif
        cfd = -1;
    }
}

/*
* Accepts a connection from a client, blocking (i.e., waiting) until one is heard.
* Upon success, returns true; upon failure, returns false.
*/
bool HttpServer::fConnected(void)
{
    struct sockaddr_in cli_addr;
    memset(&cli_addr, 0, sizeof(cli_addr));
    socklen_t cli_len = sizeof(cli_addr);
    cfd = accept(sfd, (struct sockaddr*) &cli_addr, &cli_len);
    if (cfd == -1)
	{
		state_info += "CONNECTION FAILED\n";
        return false;
	}
	state_info += "CONNECTION SUCCESSFUL\n";
    return true;
}


/**
 * Handles client errors (4xx) and server errors (5xx).
 */
bool HttpServer::fError(unsigned short code, int condition)
{
    if (cfd == -1) // ensure client's socket is open
        return false;

    if (code < 400 || code > 599) // ensure code is within range
        return false;

    // determine Status-Line's phrase
    // http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.1
    string phrase;
    switch (code)
    {
        case 400: phrase = "Bad Request"; break;
        case 403: phrase = "Forbidden"; break;
        case 404: phrase = "Not Found"; break;
        case 405: phrase = "Method Not Allowed"; break;
        case 413: phrase = "Request Entity Too Large"; break;
        case 414: phrase = "Request-URI Too Long"; break;
        case 418: phrase = "I'm a teapot"; break;
        case 500: phrase = "Internal Server Error"; break;
        case 501: phrase = "Not Implemented"; break;
        case 505: phrase = "HTTP Version Not Supported"; break;
    }
    if (!phrase.length())
        return false;
	
    // response template
	string error_response = "<html><head><title>"+to_string(code)+" "+phrase+"</title></head><body><h1>"+to_string(code)+" "+phrase+"</h1></body></html>";
	
	state_info += "HTTP/1.1 "+to_string(code)+" "+phrase+"\r\n";
	
	string tmp = "HTTP/1.1 "+to_string(code)+" "+phrase+"\r\n";
	if (send(cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Status-Line
		return false;
	tmp = "Connection: close\r\n";
	if (send(cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Connection header
		return false;
	tmp = "Content-Length: "+to_string(error_response.length())+"\r\n";
	if (send(cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Content-Length header
		return false;
	tmp = "Content-Type: text/html\r\n";
	if (send(cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Content-Type header
		return false;
	tmp = "\r\n";
	if (send(cfd, tmp.data(), tmp.length(), 0) < 0) // respond with CRLF
		return false;
	if (send(cfd, error_response.data(), error_response.length(), 0) == -1) // respond with message-body
		return false;

    // announce Response-Line

    return true;
}

/*
* Parses an HTTP request.
*/
ssize_t HttpServer::fParse(void)
{
    if (cfd == -1) // ensure client's socket is open
        return -2;

    if (request.length()) // ensure request isn't already parsed
        return -3;

    char * request_portion_buffer = new char[OCTETS]; // buffer for octets
    char * full_request = new char[LIMIT_REQUEST_LINE + LIMIT_REQUEST_FIELDS * LIMIT_REQUEST_FIELD_SIZE + 1]; // buffer for octets

    ssize_t request_length = 0;
	request = "";
    while (true) // parse request
    {
        ssize_t bytes_read = recv(cfd, request_portion_buffer, sizeof(char) * OCTETS, 0);

        if (bytes_read == -1) // socket read error
        {
        	fError(500);
            return -4;
        }
        if (bytes_read > 0) // if octets have been read, remember new length
        {
			if (request_length+bytes_read > LIMIT_REQUEST_LINE + LIMIT_REQUEST_FIELDS * LIMIT_REQUEST_FIELD_SIZE) // if buffer's full and we still haven't found CRLF CRLF, then request is too large
			{
        		fError(413);
				delete [] request_portion_buffer;
				delete [] full_request;
				return -5;
			}
			memcpy(full_request+request_length, request_portion_buffer, bytes_read);
            request_length += bytes_read;
			full_request[request_length] = 0;
        }
        else
		{
			delete [] request_portion_buffer;
			delete [] full_request;
			state_info += "CONNECTION IS CLOSED\n";
            return -6;
		}

		request = full_request;
		if (request.find("\r\n\r\n") != string::npos) // search for CRLF CRLF
		{
			request = request.substr(0,request.length()-2); // trim to one CRLF
            if (!request.length())
			{
				delete [] request_portion_buffer;
				delete [] full_request;
                return -7;
			}
            break;
		}

    }

	delete [] request_portion_buffer;
	delete [] full_request;
    return request_length;
}

/*
* Returns MIME type for supported extensions, else NULL.
*/
string HttpServer::fLookup(string extension)
{
	string str_to_return;
    if (!extension.compare("css"))
        return "text/css";

    if (!extension.compare("html"))
        return "text/html";

    if (!extension.compare("gif"))
        return "image/gif";

    if (!extension.compare("ico"))
        return "image/x-icon";

    if (!extension.compare("jpg"))
        return "image/jpeg";

    if (!extension.compare("js"))
        return "text/javascript";

    if (!extension.compare("png"))
        return "image/png";

    return "";
}

/**
 * Loads file into message-body.
 */
ssize_t HttpServer::fLoad(void)
{
    if (!pFile) // ensure file is open
        return -1;

    if (response_body) // ensure body isn't already loaded
        return -1;

	fseek(pFile, 0, SEEK_END);
	size_t file_size = ftell(pFile);
	fseek(pFile, 0, SEEK_SET); // return carret back to the file beginning

	response_body = new char[file_size];//		memset((void *)response_body, 0, sizeof(char)*file_size);
	size_t read_amount = 0;
	while (read_amount < file_size)
	{
		size_t bytes_read = fread(response_body+read_amount, 1, file_size, pFile);
		read_amount += bytes_read;
		if (feof(pFile) && read_amount != file_size)
		{
			state_info += "file read error";
			break;
		}
	}
	return read_amount;
}




