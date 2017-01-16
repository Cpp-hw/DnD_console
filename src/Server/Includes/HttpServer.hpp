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
#define LIMIT_REQUEST_FIELDS		  50
#define LIMIT_REQUEST_FIELD_SIZE	4094
#define LIMIT_REQUEST_LINE			8190
#define OCTETS 512 // number of octets for buffered reads
#define byte char

class HttpServer
{
private:
	int		port;			// port for listening
	string	root;			// path to server's root
	int		cfd;			// file descriptor for sockets // connection file descriptor
	int		sfd;			// file descriptor for sockets // socket file descriptor
	string	request;		// buffer for request
	string	request_header;	// buffer for request_header
	byte*	response_body;	// buffer for response-body
	FILE*	pFile;			// FILE pointer for requested file
	int		state_error;	// server's state. 0 - running; !0 - some errors
	string	state_info;		// informs on state condition

public:
	void	fRun(void);
private:
	void	fReset(void);
	bool	fConnected(void);
	bool	fError(unsigned short code, int condition = -1);
	ssize_t	fLoad(void);
	string	fLookup(string extension);
	ssize_t	fParse(void);
public:
	HttpServer(int server_port, string path_to_root);
	~HttpServer(void);
};