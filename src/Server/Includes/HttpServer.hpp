// HttpServer.hpp
//
// http_server
// for 
// SoftServe ITA
//
// Nicholas Tsaruk
// fbt.ksnv@gmail.com
//
#pragma once

// limits on an HTTP request's size, based on Apache's
// http://httpd.apache.org/docs/2.2/mod/core.html
#define LIMIT_REQUEST_FIELDS          50
#define LIMIT_REQUEST_FIELD_SIZE    4094
#define LIMIT_REQUEST_LINE            8190
#define OCTETS 512 // number of octets for buffered reads
#define byte char

class HttpServer
{
private:
    int                                 _port;                       // port for listening
    string                              _root;                       // path to server's root
    int                                 _cfd;                        // file descriptor for sockets // connection file descriptor
    int                                 _sfd;                        // file descriptor for sockets // socket file descriptor
    vector <string>                     _path_exceptions;            // e.g. request "/api/userregister" will not search for userregister file in Root/api/ directory, instead, the response will be filled by game server.
    string                              _request;                    // buffer for client request
    string                              _request_line;               // buffer for first line of user request
    std::map <std::string, std::string> _request_headers;
    byte*                               _response_body;              // buffer for response-body
    unsigned int                        _response_body_length;       // size of response-body
    string                              _response_body_content_type; // response-body content type
    FILE*                               _pFile;                      // FILE pointer for requested file
public:
    int                                 state_error;                // server's state. 0 - running; !0 - some errors
    string                              state_info;                 // informs on state condition

private: /* see cpp file for the below functions descriptions*/
    void    fReset(void);
    bool    fRespond(void);
    bool    fConnected(void);
    bool    fError(unsigned short code, int condition = -1);
    bool    fLoad(void);
    string  fLookup(string extension);
    ssize_t fParse(void);
public:
    void    fRun(void);
    bool    fSetResponse(const char* body, const unsigned int length, const string &content_type);
    void    (*fGenerateResponse)(std::string&, std::map <std::string, std::string> &);    // pointer to user function that is called in case of path exception
    HttpServer(int server_port, string &path_to_root);
    ~HttpServer(void);
};


