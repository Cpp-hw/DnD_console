#include "Includes\GameClient.hpp"

//boost::asio::io_service io_service;  // io_service represents your program's link to the operating system's I/O services.




GameClient::GameClient()
{
	//_http_client = new HttpClient(io_service, "localhost", "27022"); //create http-client with options: host="localhost", port="33000"

}
char GameClient::fGetInput()
{
	char choice;
	std::cin >> choice;
	return choice;
}

void SendRequest(std::string &host, std::string &port,const std::string &url, std::string &response, std::string &data)
{
	boost::asio::io_service *io_service = new boost::asio::io_service;

	HttpClient *http_client = new HttpClient(*io_service, host, port);
	
	http_client->fPostData(url,data);
	io_service->run();
	response = http_client->fGetResponse();
	delete http_client;
	delete io_service;

}

void GameClient::fDisplayDmMenu(std::string &host, std::string &port, const std::string &user_session)
{
	int choice = 0;

	


	do
	{
		system("cls");

		std::cout << "********** DM Mode **********" << std::endl;
		std::cout << "1. Create NPC" << std::endl;
		std::cout << "2. Create Terrain" << std::endl;
		std::cout << "3. Full list of terrains" << std::endl;
		std::cout << "4. Show my terrains" << std::endl;
		std::cout << "5. Back to previous  menu" << std::endl;
		std::cout << "Enter choice: ";  // user enter option 
		
		choice = fGetInput()-48;


		switch (choice)
		{
		case 1:
		{

			std::string request = UserActions::fCreateNpc(user_session).dump();
			std::string response;
			SendRequest(host, port, "/api/addnpc", response, request);
			std::cout << "Response: " << response << std::endl;

            //_http_client->fPostData("/api/addnpc", request);
		}
		break;
		case 2:
		{

			std::string request = UserActions::fCreateTerrain(user_session).dump();
			std::string response;
			SendRequest(host, port, "/api/addterrain", response, request);
			std::cout << "Response: " << response << std::endl;

			//std::string request = UserActions::fCreateTerrain(user_session).dump();
			//_http_client->fPostData("/api/addterrain", request);

		}
		break;
		case 3:
		{

			std::string request = UserActions::fShowFullListOfTerrains().dump();
			std::string response;
			SendRequest(host, port, "/api/loaddefinedterrains", response, request);
			std::cout << "Response: " << response << std::endl;

			//std::string request = UserActions::fShowFullListOfTerrains().dump();
			//std::cout << request << std::endl;
			//_http_client->fPostData("/api/loadterrain",request);
		}
		case 4:
		{
			
		}
		case 5:
			break;
		default:
			std::cout << "Wrong menu option !!!" << std::endl;
			break;
		}
	} while (choice != 5);

}

void GameClient::fDisplayPlayerMenu(const std::string &user_session)
{

}

void GameClient::fSwitchMode(std::string &host,std::string &port, const std::string &user_session)
{
	
	int mode = 0;

	do
	{

		system("cls");

		std::cout << "********** Game Modes **********" << std::endl;
		std::cout << "1. Player Mode" << std::endl;
		std::cout << "2. DM Mode" << std::endl;
		std::cout << "3. Back to main menu" << std::endl;

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
			fDisplayDmMenu(host,port,user_session);
		}
		break;
		case 3:
		break;
		default:
			std::cout << "Wrong menu option !!!" << std::endl;
			break;
		}

	} while (mode != 3);
}

void GameClient::fDisplayMainMenu()
{
	std::cout << "********** Main Menu **********" << std::endl;
	std::cout << "1. User registration" << std::endl;
	std::cout << "2. User login" << std::endl;
	std::cout << "3. User logout" << std::endl;
	std::cout << "4. Exit" << std::endl;
	std::cout << "Enter choice: ";  // user enter option 
}

void GameClient::fMenu(std::string &host, std::string &port)
{
	int choice = 0;


	try
	{
		do
		{

			system("cls");
			fDisplayMainMenu();
			choice = fGetInput()-48;

			switch (choice)
			{
			case 1:
			{

				std::string request = UserActions::fRegistration().dump();
				std::string response;
				SendRequest(host,port, "/api/userregister", response,request);
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
				SendRequest(host, port, "/api/userlogin", response, request);
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
				std::string request = UserActions::fLogin().dump();
				std::string response;
				SendRequest(host, port, "/api/userlogout", response, _game_session);
				_game_session = UserActions::fLogout(_game_session);
				std::cout << "Response: " << response << std::endl;


				//_http_client->fPostData("/api/userlogout", _game_session); // drop user session
				//io_service.run();
			}
			break;
			case 4:
				break;
			default:
				std::cout << "Wrong menu option !!!" << std::endl;
				break;
			}

		} while (choice != 4);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}

}

GameClient::~GameClient()
{
	//delete _http_client;
}
