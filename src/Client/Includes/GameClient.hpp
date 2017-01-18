#pragma once

#include "HttpClient.hpp"
#include "UserActions.hpp"


class GameClient
{
private:
	HttpClient *_http_client;    // http-client object
	std::string _game_session;   // game session
public:
	GameClient();

	void fMenu();

	~GameClient();

};
