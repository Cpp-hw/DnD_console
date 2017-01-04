// GameServer.cpp
//
// server
// for 
// SoftServe ITA
//
// Nicholas Tsaruk
// fbt.ksnv@gmail.com
//

#include "Includes/stdafx.hpp"
#include "Includes/HttpServer.hpp"
#include "Includes/IniParser.hpp"

using namespace std;

void fHandler(int signal);
void * pGlobalServing = nullptr;



int main(int argc, char* argv[])
{
	// read data from config.in file
		IniParser* pIni_parser = new IniParser("config.ini");
		auto params = pIni_parser->fGetParams();



	// start HTTP server with correct termination
		//HttpServer * pHttp_server = new HttpServer(stoi(params["server.port"]), params["server.root"]);
		HttpServer * pHttp_server = new HttpServer(33000, "Root/");
		pGlobalServing = (void*)pHttp_server; // fHandler used the pointer for correct program termination
		signal(SIGINT, fHandler); // listen for SIGINT (aka control-c), if it comes call function named fHandler


		pHttp_server->fRun(); // server's loop waiting for user connections

    // clear memory // sclose socets (in destructors, etc)
		delete pHttp_server;
		delete pIni_parser;
}




/**
 * Handles user, OS signals.
 */
void fHandler(int signal)
{
    if (signal == SIGINT) // signal == Ctrl+C
    {
		delete pGlobalServing;
		exit(-1);
    }
}
