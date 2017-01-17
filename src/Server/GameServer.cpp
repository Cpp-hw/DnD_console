// GameServer.cpp
//
// server
// for 
// SoftServe ITA
//
// Nicholas Tsaruk
// fbt.ksnv@gmail.com
//
// Olha Leskovska
//

#include "Includes/stdafx.hpp"
#include "Includes/HttpServer.hpp"
#include "Includes/IniParser.hpp"
#include "Includes/DataBase.hpp"

using namespace std;

void fHandler(int signal);
void fParseRequest(std::string &path, std::map <std::string, std::string> &http_headers);
void fUserLogIn(std::string &json_response, nlohmann::json &json_request);
void fUserRegistration(std::string &json_response, nlohmann::json &json_request);
HttpServer* pHttp_server;
DataBase data_base;


int main(int argc, char* argv[])
{
    
	// read data from config.in file
		//IniParser* pIni_parser = new IniParser("config.ini");
		//auto params = pIni_parser->fGetParams();



	// start HTTP server with correct termination
		//pHttp_server = new HttpServer(stoi(params["server.port"]), params["server.root"]);
		pHttp_server = new HttpServer(15000, "Root/");

		pHttp_server->fGenerateResponse = &fParseRequest;
		signal(SIGINT, fHandler); // listen for SIGINT (aka control-c), if it comes call function named fHandler
		pHttp_server->fRun(); // server's loop waiting for user connections

    // clear memory // sclose socets (in destructors, etc)
		delete pHttp_server;
		//delete pIni_parser;
}




/**
 * Handles user, OS signals.
 */
void fHandler(int signal)
{
    if (signal == SIGINT) // signal == Ctrl+C
    {
		delete pHttp_server;
		exit(-1);
    }
}

/**
 * parses request string 
 */
void fParseRequest(std::string &path, std::map <std::string, std::string> &http_headers)
{
	nlohmann::json json_request = json::parse(http_headers[(string)"Content"]);
	string response = "";
	if (path.find("/api/")) // if it is not found or if it is not in the beginning
	{
		response = "{\"error\": \"incorrect api call\"}";
	}
	else
	{
		if (path.find("/api/userlogin") != string::npos)
			fUserLogIn(response, json_request);
        else if (path.find("/api/userregister") != string::npos)
            fUserRegistration(response, json_request);
		else
			response = "{\"error\": \"script is not implemented\"}";
	}
	pHttp_server->fSetResponse(response.data(), response.length(), "JSON");
}

/**
 * parses request string 
 */
void fUserLogIn(std::string &json_response, nlohmann::json &json_request)
{
	data_base.fConnection("localhost", "DnD_db", "dbpass", "DnD");
	string username = json_request["username"];
	string password = json_request["password"];
	string query = "SELECT id, username, password FROM Users WHERE username='" + username + "' AND password='" + password + "';";

	nlohmann::json json_result = data_base.fExecuteQuery(query);
	cout << json_result << endl;
	string query_result = json_result["result"];
	if (query_result == "Success")
	{
		string rows = json_result["rows"];
		if (stoi(rows) > 0)
		{
			string user_id = json_result["data"][0]["id"];
			query = "SELECT user_id FROM Sessions WHERE user_id=" + user_id + ";";
			json_result = data_base.fExecuteQuery(query);
			if (query_result == "Success")
			{
				string rows = json_result["rows"];
				if (stoi(rows) > 0) // get current active session]
				{
					string session_id = json_result["data"][0]["id"];
					json_response = "{\"status\":\"already logged in\", \"session_id\": \"" + session_id + "\"}";
				}
				else // create new session
				{
					query = "INSERT INTO Sessions (user_id) VALUES ("+user_id+");";
					json_result = data_base.fExecuteQuery(query);
					query = "SELECT LAST_INSERT_ID() AS id FROM Sessions";
					json_result = data_base.fExecuteQuery(query);
					string session_id = json_result["data"][0]["id"];
					json_response = "{\"status\":\"success\", \"session_id\": \"" + session_id + "\"}";
				}
			}
			else
				json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
		}
		else
			json_response = "{\"status\":\"fail\", \"message\": \"no such user or provided password is incorrect\"}";
	}
	else
		json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
}

void fUserRegistration(std::string &json_response, nlohmann::json &json_request)
{
    data_base.fConnection("localhost", "DnD_db", "dbpass", "DnD");
    string username = json_request["username"];
    DataValidator::fValidate(username, DataValidator::NAME); // checks the username
    string password = json_request["password"];
    DataValidator::fValidate(password, DataValidator::PASSWORD); // checks the password
    string email = json_request["email"];
    DataValidator::fValidate(email, DataValidator::EMAIL); // checks the email
    
    
    string query = "INSERT INTO Users (username, password, email, is_active) VALUES ('" + username + "', '" + password + "', '" + email + "', 0);"; // creates query
    DataValidator::fValidate(query, DataValidator::SQL_INJECTION);
    
    nlohmann::json json_result = data_base.fExecuteQuery(query); // executes query
    cout << json_result << endl;
    string query_result = json_result["result"];
    if (query_result == "Success")
        json_response = "{\"status\":\"success\", \"message\": \"registered\"}";
    else
        json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
}
