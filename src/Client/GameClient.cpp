#include "Includes/GameClient.hpp"



boost::asio::io_service io_service;  // io_service represents your program's link to the operating system's I/O services.




GameClient::GameClient(const std::string &host, const std::string &port)
{
	_http_client=new HttpClient(io_service, host, port);

}
char GameClient::fGetInput()
{
	char choice;
	std::cin >> choice;
	return choice;
}


void GameClient::fDisplayDmMenu(const std::string &user_session)
{
	int choice = -1;

	do
	{
		std::cout << "********** DM Mode **********" << std::endl;
		std::cout << "1. NPC menu" << std::endl;
        std::cout << "2. Terrain menu" << std::endl;
		std::cout << "0. Back to previous  menu" << std::endl;
		std::cout << "Enter choice: ";  // user enter option 

		choice = fGetInput()-48;

		switch (choice)
		{
		case 1:
		{

			fDisplayNPCMenu(user_session);
		}
		break;
        case 2:
        {
			fDisplayTerrainMenu(user_session);
            
        }
            break;
		case 0:
			break;
		default:
			std::cout << "Wrong menu option !!!" << std::endl;
			break;
		}
	} while (choice != 0);
}

void GameClient::fDisplayPlayerMenu(const std::string &user_session)
{
	int choice = -1;
	
	do
	{
		std::cout << "********** Player Mode **********" << std::endl;
		std::cout << "1. Create character" << std::endl;
		std::cout << "2. Edit character" << std::endl;
		std::cout << "3. Load character by name" << std::endl;
		std::cout << "4. Full list of my characters" << std::endl;
		std::cout << "5. Delete character" << std::endl;
		std::cout << "0.  Back to previous  menu" << std::endl;
		std::cout << "Enter choice:" << std::endl;

		choice= fGetInput() - 48;
		switch (choice)
		{
		case 1:
		{
			std::string request = UserActions::fCreateCharacter(user_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/addcharacter", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 2:
		{
			std::cout << "Which Character do you want to edit? ";
			auto temp_request = UserActions::fLoadCharacterByName(_game_session).dump();
			std::string temp_response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loaddefinedcharacter", temp_request);
			_http_client->fGetResponse(temp_response);
			json response_json = json::parse(temp_response.c_str());
			response_json.erase("status");
			response_json.erase("characters_quantity");
			auto request = UserActions::fEditCharacter(response_json).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/editcharacter", temp_request);
			_http_client->fGetResponse(response);
			std::cout << temp_response << std::endl;
		}
		break;
		case 3:
		{
			std::string request = UserActions::fLoadCharacterByName(user_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loaddefinedcharacter", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;

		}
		break;
		case 4:
		{
			auto request = UserActions::fLoadMyCharacters(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loadmycharacterslist", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 5:
		{
			auto request = UserActions::fDeleteCharacter(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/deletecharacter", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		}
	} while (choice != 0);
}

void GameClient::fSwitchMode(const std::string &user_session)
{
	int mode = -1;

	do
	{
		std::cout << "********** Game Modes **********" << std::endl;
		std::cout << "1. Player Mode" << std::endl;
		std::cout << "2. DM Mode" << std::endl;
		std::cout << "0. Back to main menu" << std::endl;

		std::cout << "Choose mode: ";

		mode = fGetInput()-48;

		switch (mode)
		{
		case 1:
		{
			fDisplayPlayerMenu(user_session);
		}
		break;
		case 2:
		{
			fDisplayDmMenu(user_session);
		}
		break;
		case 0:
		break;
		default:
			std::cout << "Wrong menu option !!!" << std::endl;
			break;
		}

	} while (mode != 0);
}

void GameClient::fDisplayNPCMenu(const std::string &user_session)
{
	int choice = -1;

	do
	{
		std::cout << "********** NPC Menu **********" << std::endl;
		std::cout << "1. Create NPC" << std::endl;
		std::cout << "2. Show my NPCs" << std::endl;
		std::cout << "3. Load NPC by its id" << std::endl;
		std::cout << "4. Edit my NPC by its id" << std::endl;
		std::cout << "5. Delete my NPC by its id" << std::endl;
		std::cout << "0. Back to previous  menu" << std::endl;
		std::cout << "Enter choice: ";  // user enter option 

		choice = fGetInput() - 48;

		switch (choice)
		{
		case 1:
		{

			std::string request = UserActions::fCreateNpc(user_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/addnpc", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 2:
		{
			auto request = UserActions::fLoadMyNpcs(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loadmynpcslist", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;

		}
		break;
		case 3:
		{
			auto request = UserActions::fLoadNpc(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loadnpc", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 4:
		{
			std::cout << "Which NPC do you want to edit? ";
			auto temp_request = UserActions::fLoadNpc(_game_session).dump();
			std::string temp_response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loadmynpc", temp_request);
			_http_client->fGetResponse(temp_response);
			json response_json = json::parse(temp_response.c_str());
            
            if (response_json["status"] == "success")
            {
                response_json.erase("status");
                auto request = UserActions::fEditNpc(response_json).dump();
                std::string response;
                _http_client->fSendRequest(HttpClient::_POST, "/api/editnpc", temp_request);
                _http_client->fGetResponse(response);
                std::cout << response << std::endl;
            }
            else
                cout << response_json << endl;
		}
		break;
		case 5:
		{
			auto request = UserActions::fDeleteNpc(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/deletenpc", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 0:
			break;
		default:
			std::cout << "Wrong menu option !!!" << std::endl;
			break;
		}
	} while (choice != 0);
}

void GameClient::fDisplayTerrainMenu(const std::string &user_session)
{
	int choice = -1;

	do
	{
		std::cout << "********** Terrain Menu **********" << std::endl;
		std::cout << "1. Create Terrain" << std::endl;
		std::cout << "2. Full list of terrains" << std::endl;
		std::cout << "3. Show my terrains" << std::endl;
		std::cout << "4. Load terrain by id" << std::endl;
		std::cout << "0. Back to previous  menu" << std::endl;
		std::cout << "Enter choice: ";  // user enter option 

		choice = fGetInput() - 48;

		switch (choice)
		{
		case 1:
		{
			std::string request = UserActions::fCreateTerrain(user_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/addterrain", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 2:
		{
			std::string request = UserActions::fShowFullListOfTerrains(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loaddefinedterrains", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;
		}
		break;
		case 3:
		{
			std::string request = UserActions::fLoadMyTerrains(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loadmyterrainslist", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;

		}
		break;
		case 4:
		{
			std::string request = UserActions::fLoadTerrain(_game_session).dump();
			std::string response;
			_http_client->fSendRequest(HttpClient::_POST, "/api/loadterrain", request);
			_http_client->fGetResponse(response);
			std::cout << response << std::endl;

		}
		break;
		case 0:
			break;
		default:
			std::cout << "Wrong menu option !!!" << std::endl;
			break;
		}
	} while (choice != 0);
}

void GameClient::fDisplayBoardMenu(const std::string &user_session)
{
    int choice = -1;
    
    do
    {
        std::cout << "********** Board Menu **********" << std::endl;
        //std::cout << "1. Create board" << std::endl;
        std::cout << "2. Show my boards" << std::endl;
        std::cout << "3. Load board by its id" << std::endl;
        std::cout << "0. Back to previous  menu" << std::endl;
        std::cout << "Enter choice: ";  // user enter option
        
        choice = fGetInput() - 48;
        
        switch (choice)
        {
            case 1:
            {
            }
                break;
            case 2:
            {
                auto request = UserActions::fLoadMyBoards(_game_session).dump();
                std::string response;
				_http_client->fSendRequest(HttpClient::_POST, "/api/loadmyboardslist", request);
				_http_client->fGetResponse(response);
				std::cout << response << std::endl;
                
            }
                break;
            case 3:
            {
                auto request = UserActions::fLoadBoard(_game_session).dump();
                std::string response;
				_http_client->fSendRequest(HttpClient::_POST, "/api/loadboard", request);
				_http_client->fGetResponse(response);
				std::cout << response << std::endl;
            }
                break;
            case 0:
                break;
            default:
                std::cout << "Wrong menu option !!!" << std::endl;
                break;
        }
    } while (choice != 0);
}

void GameClient::fMenu()
{
	int choice = -1;
	
	try
	{
		do
		{
			std::cout << "********** Main Menu **********" << std::endl;
			std::cout << "1. User registration" << std::endl;
			std::cout << "2. User login" << std::endl;
			std::cout << "3. User logout" << std::endl;
			std::cout << "0. Exit" << std::endl;
			std::cout << "Enter choice: ";									// user enter option 

			choice = fGetInput()-48;

			switch (choice)
			{
			case 1:
			{

				std::string request = UserActions::fRegistration().dump();
				std::string response;
				
				_http_client->fSendRequest(HttpClient::_POST, "/api/userregister", request);
				_http_client->fGetResponse(response);
				std::cout << response << std::endl;
			}
			break;
			case 2:
			{
				std::string request = UserActions::fLogin().dump();
				std::string response;
				_http_client->fSendRequest(HttpClient::_POST, "/api/userlogin", request);
				_http_client->fGetResponse(response);
				
				nlohmann::json json_response = json::parse(response);
				string status = json_response["status"];

				if (status == "success" || status == "already logged in")
				{
					_game_session = json_response["session_id"];
					system("cls");

					fSwitchMode(_game_session);

				}
				else
				{
					string error_message = json_response["message"];
					cout << "login failed with message:" << error_message << endl;
				}
				


			}
			break;
			case 3:
			{
				std::string response;
				_http_client->fSendRequest(HttpClient::_POST, "/api/userlogout", _game_session);
				_http_client->fGetResponse(response);
				std::cout << response << std::endl;
				_game_session = UserActions::fLogout(_game_session);
			}
			break;
			case 0:
				break;
			default:
				std::cout << "Wrong menu option !!!" << std::endl;
				break;
			}

		} while (choice != 0);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}

}

GameClient::~GameClient()
{
	delete _http_client;
}
