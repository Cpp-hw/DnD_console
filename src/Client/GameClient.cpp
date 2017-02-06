#include "Includes/GameClient.hpp"

//boost::asio::io_service io_service;  // io_service represents your program's link to the operating system's I/O services.




GameClient::GameClient()
{
	
}
char GameClient::fGetInput()
{
	char choice;
	std::cin >> choice;
	return choice;
}

void SendRequest(const HttpClient::Methods &method, std::string &host, std::string &port,const std::string &url, std::string &response, std::string &data)
{
	boost::asio::io_service *io_service = new boost::asio::io_service;

	HttpClient *http_client = new HttpClient(*io_service, host, port);
	
	http_client->fRequest(method, url, data);
	io_service->run();
	response = http_client->fGetResponse();
	delete http_client;
	delete io_service;
}

void GameClient::fDisplayDmMenu(std::string &host, std::string &port, const std::string &user_session)
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

			fDisplayNPCMenu(host,port,user_session);
		}
		break;
        case 2:
        {
			fDisplayTerrainMenu(host, port, user_session);
            
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

void GameClient::fDisplayPlayerMenu(std::string &host, std::string &port,const std::string &user_session)
{
	int choice = -1;
	
	do
	{
		std::cout << "********** Player Mode **********" << std::endl;
		std::cout << "1. Create character" << std::endl;
		//std::cout << "2. Load character by name" << std::endl;
		//std::cout << "3. Full list of my characters" << std::endl;
		std::cout << "0.  Back to previous  menu" << std::endl;
		std::cout << "Enter choice:" << std::endl;

		choice= fGetInput() - 48;
		switch (choice)
		{
		case 1:
		{
			std::string request = UserActions::fCreateCharacter(user_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/addcharacter", response, request);
			std::cout << "Response: " << response << std::endl;
		}
		break;
		case 2:
		{
			std::string request = UserActions::fLoadCharacterByName(user_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loaddefinedcharacter", response, request);
			std::cout << "Response: " << response << std::endl;

		}
		break;
		case 3:
		{
			auto request = UserActions::fLoadMyCharacters(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loadmycharacterslist", response, request);
			std::cout << "Response: " << response << std::endl;
		}
		break;
		}
	} while (choice != 0);
}

void GameClient::fSwitchMode(std::string &host,std::string &port, const std::string &user_session)
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
			fDisplayPlayerMenu(host, port,user_session);
		}
		break;
		case 2:
		{
			fDisplayDmMenu(host,port,user_session);
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

void GameClient::fDisplayNPCMenu(std::string &host, std::string &port, const std::string &user_session)
{
	int choice = -1;

	do
	{
		std::cout << "********** NPC Menu **********" << std::endl;
		std::cout << "1. Create NPC" << std::endl;
		std::cout << "2. Show my NPCs" << std::endl;
		std::cout << "3. Load NPC by its id" << std::endl;
		std::cout << "4. Edit NPC by its id" << std::endl;
		std::cout << "5. Delete NPC by its id" << std::endl;
		std::cout << "0. Back to previous  menu" << std::endl;
		std::cout << "Enter choice: ";  // user enter option 

		choice = fGetInput() - 48;

		switch (choice)
		{
		case 1:
		{

			std::string request = UserActions::fCreateNpc(user_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/addnpc", response, request);
			std::cout << "Response: " << response << std::endl;

			//_http_client->fPostData("/api/addnpc", request);
		}
		break;
		case 2:
		{
			auto request = UserActions::fLoadMyNpcs(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loadmynpcslist", response, request);
			std::cout << "Response: " << response << std::endl;

		}
		break;
		case 3:
		{
			auto request = UserActions::fLoadNpc(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loadnpc", response, request);
			std::cout << "Respone: " << response << std::endl;
		}
		break;
		case 4:
		{
			std::cout << "Which NPC do you want to edit? ";
			auto temp_request = UserActions::fLoadNpc(_game_session).dump();
			std::string temp_response;
			SendRequest(HttpClient::_POST, host, port, "/api/loadnpc", temp_response, temp_request);
			json response_json = json::parse(temp_response.c_str());
			response_json.erase("status");

			auto request = UserActions::fEditNpc(response_json).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/editnpc", response, request);
			std::cout << "Respone: " << response << std::endl;
		}
		break;
		case 5:
		{
			auto request = UserActions::fDeleteNpc(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/deletenpc", response, request);
			std::cout << "Respone: " << response << std::endl;
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

void GameClient::fDisplayTerrainMenu(std::string &host, std::string &port, const std::string &user_session)
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
			SendRequest(HttpClient::_POST, host, port, "/api/addterrain", response, request);
			std::cout << "Response: " << response << std::endl;

			//std::string request = UserActions::fCreateTerrain(user_session).dump();
			//_http_client->fPostData("/api/addterrain", request);
		}
		break;
		case 2:
		{

			std::string request = UserActions::fShowFullListOfTerrains(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loaddefinedterrains", response, request);
			std::cout << "Response: " << response << std::endl;

			//std::string request = UserActions::fShowFullListOfTerrains().dump();
			//std::cout << request << std::endl;
			//_http_client->fPostData("/api/loadterrain",request);
		}
		break;
		case 3:
		{
			auto request = UserActions::fLoadMyTerrains(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loadmyterrainslist", response, request);
			std::cout << "Response: " << response << std::endl;
		}
		break;
		case 4:
		{
			auto request = UserActions::fLoadTerrain(_game_session).dump();
			std::string response;
			SendRequest(HttpClient::_POST, host, port, "/api/loadterrain", response, request);
			std::cout << "Respone: " << response << std::endl;
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

void GameClient::fMenu(std::string &host, std::string &port)
{
	int choice = -1;


	try
	{
		do
		{

//			system("cls");
			std::cout << "********** Main Menu **********" << std::endl;
			std::cout << "1. User registration" << std::endl;
			std::cout << "2. User login" << std::endl;
			std::cout << "3. User logout" << std::endl;
			std::cout << "0. Exit" << std::endl;
			std::cout << "Enter choice: ";  // user enter option 

			choice = fGetInput()-48;

			switch (choice)
			{
			case 1:
			{

				std::string request = UserActions::fRegistration().dump();
				std::string response;
				SendRequest(HttpClient::_POST,host,port, "/api/userregister", response,request);
				std::cout << "Response: " << response << std::endl;
				system("cls");

				
				//_http_client->fPostData("/api/userregister", request); //send POST request for registration
				//io_service.run(); // run io_service
			}
			break;
			case 2:
			{
				std::string request = UserActions::fLogin().dump();
				std::string response;
				SendRequest(HttpClient::_POST,host, port, "/api/userlogin", response, request);
				std::cout << "Response: " << response << std::endl;

				//_http_client->fPostData("/api/userlogin", UserActions::fLogin().dump()); // send POST request for logining

				//io_service.run();
				//_http_client->fGetResponse();
			
				nlohmann::json json_response = json::parse(response);
				string status = json_response["status"];

				if (status == "success" || status == "already logged in")
				{
					_game_session = json_response["session_id"];
					system("cls");

					fSwitchMode(host,port,_game_session);

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
				SendRequest(HttpClient::_POST,host, port, "/api/userlogout", response, _game_session);
				_game_session = UserActions::fLogout(_game_session);
				std::cout << "Response: " << response << std::endl;


				//_http_client->fPostData("/api/userlogout", _game_session); // drop user session
				//io_service.run();
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
	
}
