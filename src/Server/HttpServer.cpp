// HttpServer.cpp
//
// http_server
// for 
// SoftServe ITA
//
// Nicholas Tsaruk
// fbt.ksnv@gmail.com
//


#include "Includes/stdafx.hpp"
#include "Includes/HttpServer.hpp"


/*
* Initializes and starts http server.
*/
HttpServer::HttpServer(int server_port, string &path_to_root)
{
    _port                        = server_port;
    _root                        = path_to_root;
    _cfd                         = -1; // set file descriptor for sockets uninitialized
    _sfd                         = -1; // set file descriptor for sockets uninitialized
    _pFile                       = nullptr;
    _response_body               = nullptr;
    _response_body_length        = 0;
    _response_body_content_type  = "";
    state_error                 = 0;
    errno                       = 0; // reset errors
    fGenerateResponse           = nullptr;

    _path_exceptions.push_back("api/");

    if (_port < 0 || _port > 65535) // ensure port is a non-negative short integer
    {
        state_error =  2;
        state_info += "Server's port is out of range 0...65535\n";
        return;
    }



    #ifdef _WIN32
        unsigned long root_path_length = 0;
        char  buffer[4096]; buffer[0] = 0; // set null terminating character in case of pull path retrieving fail
        char** lppPart={nullptr};
        GetFullPathNameA(_root.data(), 4096, buffer, lppPart);
        _root = buffer;
    #else
        _root = realpath(_root.data(), nullptr); // full path to server's root on UNIX
    #endif

    if (!_root.length()) // endusre path to server's root is specified
    {
        state_error =  3;
        state_info += "Path to server's root is invalid\n";
        return;
    }

    if (access(_root.data(), F_OK) == -1) // ensure root exists
    {
        state_error =  4;
        state_info += "Path to server's root does not exist\n";
        return;
    }

    #ifdef _WIN32
        if (access(_root.data(), W_OK) == -1) // ensure root is writable
        {
            state_error =  5;
            state_info += "You do not have sufficient right on server's root\n";
            return;
        }
    #else
        if (access(_root.data(), X_OK) == -1) // ensure root is executable
        {
            state_error =  5;
            state_info += "You do not have sufficient right on server's root\n";
            return;
        }
    #endif

    state_info += "Using "+_root+" for server's root\n";

    #ifdef _WIN32
        WSADATA wsaData;
        int res = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (res != 0)
        {
            state_error =  6;
            state_info += "WSAStartup failed with error: "+to_string(res);
            return ;
        }
        _sfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket
        if (_sfd == -1)
        {
            state_error =  7;
            state_info += "Socket creation failed: ";
            state_info += strerror(WSAGetLastError());
            return;
        }
    #else
        _sfd = socket(AF_INET, SOCK_STREAM, 0); // create a socket
        if (_sfd == -1)
        {
            state_error =  7;
            state_info += "Socket creation failed";
            return;
        }
    #endif


    int optval = 1;
    setsockopt(_sfd,
                SOL_SOCKET,
                SO_REUSEADDR, // allow reuse of address (to avoid "Address already in use")
                (char *)&optval,
                sizeof(optval));

    // assign name to socket
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    {
        state_error =  7;
        state_info += "Socket bind failed";
        return;
    }

    
    if (listen(_sfd, SOMAXCONN) == -1) // start port listening
    {
        state_error =  8;
        state_info += "Port listening initiation failed.";
        return;
    }

    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    if (getsockname(_sfd, (struct sockaddr*) &addr, &addrlen) == -1)
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
    _port = 0;
    _root = ""; // free root, which was allocated by realpath
    state_error = -1;
    _request_headers.clear();

    if (_sfd != -1) // close server socket
    {
        #ifdef _WIN32
            closesocket(_sfd);
            WSACleanup();
        #else
            close(_sfd);
        #endif
    }

    if (errsv) // announce error
        cout<<strerror(errsv)<<endl;
}

/*
* Main server's loop, that waits and serves client connection.
*/
void HttpServer::fRun(void)
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

            int newline_pos = _request.find("\r\n");
            if (newline_pos == -1)
            {
                fError(400); // Bad Request
                continue;
            }
            if (_request.length() > LIMIT_REQUEST_LINE)
            {
                fError(414); // Request-URI Too Long
                continue;
            }

            _request_line = _request.substr(0, newline_pos); // extract request's request-line header // http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html

            // validate request-header
            int sp_loc = _request_line.find(' ');
            int nsp_loc = _request_line.find(' ', sp_loc+1);
            int qm_loc = _request_line.find('?', nsp_loc+1);
            int version_loc = _request_line.find("HTTP/1.1");

            string request_method;
            string request_uri;
            string requested_path;
            string requested_file_extension;
            string request_query;
            string request_version = "HTTP/1.1";

            if (version_loc == -1)
            {
                fError(505); // HTTP Version Not Supported
                continue;
            }

            request_method = _request_line.substr(0, sp_loc);
            if (request_method.compare("GET") && request_method.compare("POST") && request_method.compare("PUT") && request_method.compare("DELETE"))
            {
                fError(405); // Method Not Allowed
                continue;
            }

            request_uri = _request_line.substr(sp_loc+1, nsp_loc-sp_loc-1);
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
            _response_body_content_type = fLookup(requested_file_extension);
            /*if (!response_body_content_type.length())
            {
                fError(501); // Not Implemented
                continue;
            }*/



            state_info += "REQUEST METHOD: "+request_method+"\n";
            state_info += "REQUEST URI: "+request_uri+"\n";
            state_info += "REQUESTED PATH: "+requested_path+"\n";
            state_info += "REQUEST QUERY: "+request_query+"\n";
            state_info += "REQUEST VERSION: "+request_version+"\n";
            state_info += "REQUESTED FILE EXTENSION: "+requested_file_extension+"\n";
            state_info += "REQUESTED FILE MIME TYPE: "+_response_body_content_type+"\n";
            state_info += "REQUEST VERSION: "+request_version+"\n";

            newline_pos += 2;
            int i = newline_pos;
            while (i < (int)_request.length()-2)
            {
                i = _request.find("\r\n", newline_pos);
                string request_header = _request.substr(newline_pos, i-newline_pos);
                int dp_pos = request_header.find(':');
                if (dp_pos < 1)
                    state_info += "incorrect request header was met: \""+request_header+"\"\n";
                else
                    _request_headers.insert ( pair<std::string, std::string>(fTrimString(request_header.substr(0, dp_pos)), fTrimString(request_header.substr(dp_pos+1))) );
                newline_pos = i+2;
            }
            state_info += "REQUEST LINES:\n";
            for (auto it = _request_headers.begin(); it != _request_headers.end(); ++it)
            {
                state_info += "\t"+(*it).first + ":" + (*it).second + "\n";
            }
            state_info += "\n";

            i = 0;
            int l = _path_exceptions.size();
            int exception_case = 0;
            while (i < l)
            {
                if (requested_path.find(_path_exceptions[i++]) == 1)
                {
                    ++exception_case;
                    fGenerateResponse(requested_path, _request_headers);
                    break;
                }
            }
            if (!exception_case)
            {
                if (access((_root+requested_path).data(), F_OK) == -1 && requested_path.length()>1) // ensure path exists
                {
                    state_info += "trying access to:\""+(_root+requested_path)+"\n";
                    fError(404); // Not Found
                    continue;
                }
                if (access((_root+requested_path).data(), R_OK) == -1) // ensure path is readable
                {
                    fError(403); // Forbidden
                    continue;
                }


                int path_is_file = 1;
                #ifdef _WIN32
                    // define if it is file or directory
                #else
                    DIR* directory = opendir((_root+requested_path).data());
                    if(directory != nullptr)
                    {
                        closedir(directory);
                        path_is_file = 0;
                    }
                    else if(errno == ENOTDIR)
                    {
                        path_is_file = 1;
                    }
                #endif
                if (!path_is_file)
                {
                    state_info += "ERROR: 422\n";
                    fError(422); // Unprocessable Entity
                    continue;
                }


                _pFile = fopen((_root+requested_path).data(), "rb");
                if (_pFile == nullptr)
                    state_info += "nothing to load\n";
                else
                {
                    if (!fLoad()) // load requested file
                    {
                        fError(500); // Internal Server Error
                        continue;
                    }
                }
            }
            if (!fRespond())
                continue;

            // announce OK
            state_info += "HTTP/1.1 200 OK\n";
        }
    }
}

/*
* Sends response to client.
*/
bool HttpServer::fRespond(void)
{
    string response_str = "HTTP/1.1 200 OK\r\n";
    if (send(_cfd, response_str.data(), response_str.length(), 0 ) == -1)
        return 0;

    response_str = "Connection: close\r\n";
    if (send(_cfd, response_str.data(), response_str.length(), 0 ) == -1)
        return 0;

    response_str = "Content-Length: "+to_string(_response_body_length)+"\r\n";
    if (send(_cfd, response_str.data(), response_str.length(), 0 ) == -1)
        return 0;

    response_str = "Content-Type: "+_response_body_content_type+"\r\n\r\n";
    if (send(_cfd, response_str.data(), response_str.length(), 0 ) == -1)
        return 0;

    if (send(_cfd, _response_body, _response_body_length, 0) == -1)
        return 0;
    response_str = "\r\n\r\n";
    if (send(_cfd, response_str.data(), response_str.length(), 0 ) == -1)
    {
        state_info += "Failed to close keep-alive connection\n";
        return 0;
    }
    return 1;
}

/*
* Resets server's state, deallocating any resources.
*/
void HttpServer::fReset(void)
{
    _request_headers.clear();

    if (_response_body != nullptr) // free response's body
    {
        delete [] _response_body;
        _response_body = nullptr;
    }
    _response_body_length = 0;
    _response_body_content_type = "";

    if (_pFile != nullptr) // close file
    {
        fclose(_pFile);
        _pFile = nullptr;
    }

    _request = "";
    state_error = 0;
    state_info = "";

    if (_cfd != -1) // close client's socket
    {
        #ifdef _WIN32
            closesocket(_cfd);
        #else
            close(_cfd);
        #endif
        _cfd = -1;
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
    _cfd = accept(_sfd, (struct sockaddr*) &cli_addr, &cli_len);
    if (_cfd == -1)
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
    if (_cfd == -1) // ensure client's socket is open
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
    if (send(_cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Status-Line
        return false;
    tmp = "Connection: close\r\n";
    if (send(_cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Connection header
        return false;
    tmp = "Content-Length: "+to_string(error_response.length())+"\r\n";
    if (send(_cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Content-Length header
        return false;
    tmp = "Content-Type: text/html\r\n";
    if (send(_cfd, tmp.data(), tmp.length(), 0) < 0) // respond with Content-Type header
        return false;
    tmp = "\r\n";
    if (send(_cfd, tmp.data(), tmp.length(), 0) < 0) // respond with CRLF
        return false;
    if (send(_cfd, error_response.data(), error_response.length(), 0) == -1) // respond with message-body
        return false;

    // announce Response-Line

    return true;
}

/*
* Parses an HTTP request.
*/
ssize_t HttpServer::fParse(void)
{
    if (_cfd == -1) // ensure client's socket is open
        return -2;

    if (_request.length()) // ensure request isn't already parsed
        return -3;

    char * request_portion_buffer = new char[OCTETS]; // buffer for octets
    char * full_request = new char[LIMIT_REQUEST_LINE + LIMIT_REQUEST_FIELDS * LIMIT_REQUEST_FIELD_SIZE + 1]; // buffer for octets

    ssize_t request_length = 0;
    _request = "";
    while (true) // parse request
    {
        ssize_t bytes_read = recv(_cfd, request_portion_buffer, sizeof(char) * OCTETS, 0);

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

        _request = full_request;
        if (_request.find("\r\n\r\n") != string::npos) // search for CRLF CRLF
        {
            _request = _request.substr(0,_request.length()-2); // trim to one CRLF
            if (!_request.length())
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
* Returns MIME type for supported extensions, else empty string.
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
bool HttpServer::fLoad(void)
{
    if (!_pFile) // ensure file is open
        return 0;

    if (_response_body) // ensure body isn't already loaded
        return 0;

    fseek(_pFile, 0, SEEK_END);
    size_t file_size = ftell(_pFile);
    fseek(_pFile, 0, SEEK_SET); // return carret back to the file beginning

    _response_body = new char[file_size];//        memset((void *)response_body, 0, sizeof(char)*file_size);
    _response_body_length = 0;
    while (_response_body_length < file_size)
    {
        size_t bytes_read = fread(_response_body + _response_body_length, 1, file_size, _pFile);
        _response_body_length += bytes_read;
        if (feof(_pFile) && _response_body_length != file_size)
        {
            state_info += "file read error";
            break;
        }
    }
    return 1;
}


/**
 * Loads [game]server's responce into message-body.
 */
bool HttpServer::fSetResponse(const char* body, const unsigned int length, const std::string &content_type)
{
    if (length < 0)
        return 0;

    _response_body_length = length;
    _response_body_content_type = content_type;
    if (length)
    {
        _response_body = new char[length];
        memcpy(_response_body, body, length);
    }
    return 1;
}


