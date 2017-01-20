#include "Includes\GameClient.hpp"

boost::asio::io_service io_service;  // io_service represents your program's link to the operating system's I/O services.




GameClient::GameClient()
{
	_http_client = new HttpClient(io_service, "localhost", "15000"); //create http-client with options: host="localhost", port="33000"

}



void GameClient::fMenu()
{

	try
	{
		std::cout << "********** Main Menu **********" << std::endl;
		std::cout << "1. User registration" << std::endl;
		std::cout << "2. User login" << std::endl;
		std::cout << "3. User logout" << std::endl;
		std::cout << "4. Exit" << std::endl;
		int choice = 0;
		std::cout << "Enter choice:" << std::endl;  // user enter option 
		do
		{
			std::cin >> choice;
			
			switch (choice)
			{
			case 1:
			{
				_http_client->fPostData("/api/userregister", UserActions::fRegistration().dump()); //send POST request for registration
				io_service.run(); // run io_service
			}
				break;
			case 2:
			{
				//_http_client->fPostData("/api/userlogin", UserActions::fLogin().dump()); // send POST request for logining

				//io_service.run();
				//_http_client->fGetResponse();
				
			}
				break;
			case 3:
			{
				_http_client->fPostData("/api/userlogout", _game_session); // drop user session
				io_service.run();
				_game_session = UserActions::fLogout(_game_session);
				//_http_client->fSetSession(_game_session);
			}
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
