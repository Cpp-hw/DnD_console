#pragma once

#include "HttpClient.hpp"
#include "UserActions.hpp"


class GameClient
{
private:
	//HttpClient *_http_client;    // http-client object
	std::string _game_session;   // game session
public:
	GameClient();

	void fMenu(std::string &host, std::string &port);
	~GameClient();
private:
	char fGetInput();
	void fDisplayDmMenu(std::string &host, std::string &port,const std::string &user_session);
	void fDisplayPlayerMenu(std::string &host, std::string &port,const std::string &user_session);
	void fDisplayNPCMenu(std::string &host, std::string &port, const std::string &user_session);
    void fDisplayTerrainMenu(std::string &host, std::string &port, const std::string &user_session);
    void fDisplayBoardMenu(std::string &host, std::string &port, const std::string &user_session);
	void fSwitchMode(std::string &host, std::string &port, const std::string &user_session);
};
