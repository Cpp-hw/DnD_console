#include "Includes\GameClient.hpp"

boost::asio::io_service io_service;  // io_service represents your program's link to the operating system's I/O services.




GameClient::GameClient()
{
	_http_client = new HttpClient(io_service, "localhost", "27022"); //create http-client with options: host="localhost", port="33000"

}
int GameClient::fGetInput()
{
	int choice;
	std::cin >> choice;
	return choice;
}

void GameClient::fDisplayDmMenu(const std::string &user_session)
{
	int choice = 0;

	std::cout << "********** DM Mode **********" << std::endl;
	std::cout << "1. Create NPC" << std::endl;
	std::cout << "2. Create Terrain" << std::endl;
	std::cout << "3. Full list of terrains" << std::endl;
	std::cout << "4. Show my terrains" << std::endl;
	std::cout << "5. Back to previous  menu" << std::endl;
	std::cout << "Enter choice: ";  // user enter option 


	do
	{

		choice = fGetInput();

		switch (choice)
		{
		case 1:
		{
            std::string request = UserActions::fCreateNpc(user_session).dump();
            _http_client->fPostData("/api/addnpc", request);
		}
		break;
		case 2:
		{
			std::string request = UserActions::fCreateTerrain(user_session).dump();
			_http_client->fPostData("/api/addterrain", request);

		}
		break;
		case 3:
		{
			std::string request = UserActions::fShowFullListOfTerrains().dump();
			std::cout << request << std::endl;
			//_http_client->fPostData("/api/loadterrain",request);
		}
		case 4:
		{
			//leave your functionality here
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

void GameClient::fSwitchMode(const std::string &user_session)
{
	int mode = 0;
	std::cout << "********** Game Modes **********" << std::endl;
	std::cout << "1. Player Mode" << std::endl;
	std::cout << "2. DM Mode" << std::endl;
	std::cout << "3. Back to main menu" << std::endl;

	std::cout << "Choose mode: ";

	do
	{

		mode = fGetInput();

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

void GameClient::fMenu()
{
	int choice = 0;

	try
	{
		do
		{

			system("cls");
			fDisplayMainMenu();
			choice = fGetInput();

			switch (choice)
			{
			case 1:
			{
				std::string request = UserActions::fRegistration().dump();
				_http_client->fPostData("/api/userregister", request); //send POST request for registration
				io_service.run(); // run io_service
			}
			break;
			case 2:
			{
				//_http_client->fPostData("/api/userlogin", UserActions::fLogin().dump()); // send POST request for logining

				//io_service.run();
				//_http_client->fGetResponse();
			

				fSwitchMode("123");


			}
			break;
			case 3:
			{
				_http_client->fPostData("/api/userlogout", _game_session); // drop user session
				//io_service.run();
				_game_session = UserActions::fLogout(_game_session);
			}
			break;
			case 4:
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
	delete _http_client;
}
