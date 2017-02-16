#include "Includes\GameClient.hpp"
#include "Includes\IniParser.hpp"


int main()
{

	IniParser pIni_parser("config.ini");
	auto params = pIni_parser.fGetParams();
	
	GameClient client(params["client.host"], params["client.port"]);   // create game client object
	client.fMenu();													   // call Menu function

	system("pause");
	return 0;
}
