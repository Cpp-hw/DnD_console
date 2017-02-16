#pragma once

#include "HttpClient.hpp"
#include "UserActions.hpp"


class GameClient
{
private:
	HttpClient *_http_client;    // http-client object
	std::string _game_session;   // game session
public:
	GameClient(const std::string &host, const std::string &port);

	void fMenu();
	~GameClient();
private:
	char fGetInput();
	void fDisplayDmMenu(const std::string &user_session);
	void fDisplayPlayerMenu(const std::string &user_session);
	void fDisplayNPCMenu(const std::string &user_session);
  void fDisplayTerrainMenu(const std::string &user_session);
  void fDisplayBoardMenu(const std::string &user_session);
	void fSwitchMode(const std::string &user_session);
};
