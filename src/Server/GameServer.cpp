// GameServer.cpp
//
// server
// for 
// SoftServe ITA
//
// Nicholas Tsaruk and Olha Leskovska
// fbt.ksnv@gmail.com, olhalesk@gmail.com
//

#include "Includes/stdafx.hpp"
#include "Includes/HttpServer.hpp"
#include "Includes/IniParser.hpp"
#include "Includes/DataBase.hpp"
#include "Includes/DataValidator.hpp"

using namespace std;

void fHandler(int signal);
void fParseRequest(std::string &path, std::map <std::string, std::string> &http_headers);
void fUserLogIn(std::string &json_response, nlohmann::json &json_request);
bool fRetrieveUserId(std::string &id_user, std::string &session_id);
void fSaveTerrain(std::string &json_response, nlohmann::json &json_request);
void fSaveNpc(std::string &json_response, nlohmann::json &json_request);
void fSaveCharacter(std::string &json_response, nlohmann::json &json_request);
void fSendOwnNpcsList(std::string &json_response, nlohmann::json &json_request);
void fSendNpc(std::string &json_response, nlohmann::json &json_request);
void fEditNpc(std::string &json_response, nlohmann::json &json_request);
void fDeleteNpc(std::string &json_response, nlohmann::json &json_request);
void fSendTerrain(std::string &json_response, nlohmann::json &json_request);
void fSendDefinedTerrains(std::string &json_response, nlohmann::json &json_request);
void fSendOwnTerrainsList(std::string &json_response, nlohmann::json &json_request);
void fSendOwnCharacterList(std::string &json_response, nlohmann::json &json_request);
void fUserRegistration(std::string &json_response, nlohmann::json &json_request);
void fSendDefinedCharacter(std::string &json_response, nlohmann::json &json_request);

string fSetAbilityMod(std::string ability);
HttpServer* pHttp_server;
DataBase data_base;


int main(int argc, char* argv[])
{
    // read data from config.ini file
        cout << "(1/3) The following keys were parsed out from the configuration file:\n";
        IniParser pIni_parser("config.ini");
        auto params = pIni_parser.fGetParams();
        for (auto & param: params)
    	    cout << "\t" << param.first + ":" + param.second + "\n";
        cout << "\n";

    // check for DB connection
        cout << "(2/3) ";
        nlohmann::json json_result = data_base.fConnection(params["database.host"], params["database.username"], params["database.password"], params["database.name"]);
        string db_connection_result = json_result["result"];
        if (db_connection_result == "error")
        {
            string db_connection_message = json_result["message"];
            cout << "Unable to connect to DB: " << db_connection_message << endl;
            return -10;
        }
        cout << "DB connection is available\n\n";

    // start HTTP server with correct termination
        cout << "(3/3) ";
        pHttp_server = new HttpServer(stoi(params["server.port"]), params["server.root"]);
        // pHttp_server = new HttpServer(15000, "Root/");
        if (pHttp_server->state_error)
        {
            cout << "Unable to create HTTP server: " << pHttp_server->state_info << endl;
            return -20;
        }
        pHttp_server->fGenerateResponse = &fParseRequest; // assign callback function
        signal(SIGINT, fHandler); // listen for SIGINT (aka control-c), if it comes call function named fHandler
        cout << "HTTP server is started:\n" << pHttp_server->state_info << endl;
        pHttp_server->state_info = "";

        pHttp_server->fRun(); // blocking function. Server's loop waits for user connections

    // clear memory and close socets (via destructors, etc.)
        delete pHttp_server;
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
    cout << "\n\nAPI call:\n";
    string response = "";
    if (path.find("/api/")) // if it is not found or if it is not in the beginning
        response = "{\"error\": \"incorrect api call\"}";
    else
    {
    	if (http_headers.find((string)"Content") == http_headers.end())
            response = "{\"status\": \"fail\",\"message\": \"Unable to gain content from http request.\"}";
    	else
    	{
        	string request_data_content = http_headers[(string)"Content"];
            if (!request_data_content.length())
                response = "{\"error\": \"request content is empty\"}";
            else
            {
                try
                {
                    nlohmann::json json_request = json::parse(request_data_content.c_str());

					if (path.find("/api/userlogin") != string::npos)
						fUserLogIn(response, json_request);
					else if (path.find("/api/userregister") != string::npos)
						fUserRegistration(response, json_request);
					else if (path.find("/api/addterrain") != string::npos)
						fSaveTerrain(response, json_request);
					else if (path.find("/api/loadterrain") != string::npos)
						fSendTerrain(response, json_request);
					else if (path.find("/api/loaddefinedterrains") != string::npos)
						fSendDefinedTerrains(response, json_request);
					else if (path.find("/api/loadmyterrainslist") != string::npos)
						fSendOwnTerrainsList(response, json_request);
					else if (path.find("/api/addnpc") != string::npos)
						fSaveNpc(response, json_request);
					else if (path.find("/api/loadmynpcslist") != string::npos)
						fSendOwnNpcsList(response, json_request);
					else if (path.find("/api/loadnpc") != string::npos)
						fSendNpc(response, json_request);
					else if (path.find("/api/editnpc") != string::npos)
						fEditNpc(response, json_request);
					else if (path.find("/api/deletenpc") != string::npos)
						fDeleteNpc(response, json_request);
					else if (path.find("/api/addcharacter") != string::npos)
						fSaveCharacter(response, json_request);
					else if (path.find("/api/loadmycharacterslist") != string::npos)
						fSendOwnCharacterList(response, json_request);
                    else if (path.find("/api/loaddefinedcharacter") != string::npos)
                        fSendDefinedCharacter(response, json_request)
                    else
                    	response = "{\"status\": \"fail\",\"message\": \"requested API is not implemented\"}";
                }
                catch (const exception &)
                {
                    response = "{\"status\": \"fail\",\"message\": \"JSON parse failed or JSON keys are invalid\"}";
                }
            }
    	}
    }
    cout << "RESPONSE:\n" << response << endl;
    pHttp_server->fSetResponse(response.data(), response.length(), "JSON");
}

/**
* parses request string
*/
void fUserLogIn(std::string &json_response, nlohmann::json &json_request)
{
    string username = json_request["username"];
    string password = json_request["password"];
    if (DataValidator::fValidate(username, DataValidator::NAME) &&
        DataValidator::fValidate(password, DataValidator::PASSWORD)) // checks data
    {
		string query = "SELECT id, username, password FROM Users WHERE username='" + username + "' AND password='" + password + "';";
		nlohmann::json json_result = data_base.fExecuteQuery(query);
		cout << query << "\nRESULT:\n" << json_result << endl;
		string query_result = json_result["result"];
		if (query_result == "success")
		{
			string rows = json_result["rows"];
			if (stoi(rows) > 0)
			{
				string id_user = json_result["data"][0]["id"];

				query = "SELECT id FROM Sessions WHERE id_user=" + id_user + ";";
				json_result = data_base.fExecuteQuery(query);
				cout << query << "\nRESULT:\n" << json_result << endl;
				query_result = json_result["result"];

				if (query_result == "success")
				{
					rows = json_result["rows"];
					if (stoi(rows) > 0) // get current active session
					{
						string session_id = json_result["data"][0]["id"];
						json_response = "{\"status\":\"already logged in\", \"session_id\": \"" + session_id + "\"}";
					}
					else // create new session
					{
						query = "INSERT INTO Sessions (id_user) VALUES (" + id_user + ");";
						json_result = data_base.fExecuteQuery(query);
						cout << query << "\nRESULT:\n" << json_result << endl;
						query = "SELECT LAST_INSERT_ID() AS id";
						json_result = data_base.fExecuteQuery(query);
						cout << query << "\nRESULT:\n" << json_result << endl;
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
    else
        json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
}

void fUserRegistration(std::string &json_response, nlohmann::json &json_request)
{
    string username = json_request["username"];
    string password = json_request["password"];
    string email = json_request["email"];

    if (DataValidator::fValidate(username, DataValidator::NAME) &&
        DataValidator::fValidate(password, DataValidator::PASSWORD) &&
        DataValidator::fValidate(email, DataValidator::EMAIL)) // checks data
    {
        string query = "INSERT INTO Users (username, password, email, is_active) VALUES ('" + username + "', '" + password + "', '" + email + "', 0);"; // creates query
        nlohmann::json json_result = data_base.fExecuteQuery(query); // executes query
        cout << json_result << endl;
        string query_result = json_result["result"];
        if (query_result == "success")
            json_response = "{\"status\":\"success\", \"message\": \"registered\"}";
        else
            json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"invalid data\"}";
}

bool fRetrieveUserId(std::string &id_user, std::string &session_id)
{
    if (DataValidator::fValidate(session_id,     DataValidator::SQL_INJECTION)) // checks data
    {
		string query = "SELECT id_user FROM Sessions WHERE id = " + session_id;
		nlohmann::json json_result = data_base.fExecuteQuery(query);
		cout << query << "\nRESULT:\n" << json_result << endl;
		string query_result = json_result["result"];
		if (query_result == "success")
		{
			string rows = json_result["rows"];
			if (stoi(rows) > 0)
			{
				id_user = json_result["data"][0]["id_user"];
				return 1;
			}
		}
    }
    return 0;
}


void fSaveTerrain(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];

    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
    	cout<<"USER("<<id_user<<") is logged in\n";
        string name = json_request["name"];
        string type = json_request["type"];
        string width = json_request["width"];
        string height = json_request["height"];
        string description = json_request["description"];

        if (DataValidator::fValidate(name,        DataValidator::SQL_INJECTION) &&
        	DataValidator::fValidate(type,        DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(width,       DataValidator::LENGTH) &&
			DataValidator::fValidate(height,      DataValidator::LENGTH) &&
			DataValidator::fValidate(description, DataValidator::SQL_INJECTION)) // checks data
        {
			string query = "INSERT INTO Terrain (name, type, width, height, description, id_owner) VALUES ('" + name + "', '" + type + "', " + width + ", " + height + ", '" + description + "', " + id_user + ");";
			nlohmann::json json_result = data_base.fExecuteQuery(query);
			cout << query << "\nRESULT:\n" << json_result << endl;
			string query_result = json_result["result"];

			if (query_result == "success")
			{
				query = "SELECT LAST_INSERT_ID() AS id";
				json_result = data_base.fExecuteQuery(query);
				cout << query << "\nRESULT:\n" << json_result << endl;
				string terrain_id = json_result["data"][0]["id"];
				json_response = "{\"status\":\"success\", \"terrain_id\": \"" + terrain_id + "\"}";
			}
			else
				json_response = "{\"status\":\"fail\", \"message\": \"terrain is not added, sql query execution failed\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}


void fSaveNpc(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string name         = json_request["npc"];
        string type         = json_request["type"];
        string hitpoints    = json_request["hitpoints"];
        string level        = json_request["level"];
        string strength     = json_request["strength"];
        string dexterity    = json_request["dexterity"];
        string constitution = json_request["constitution"];
        string intelligence = json_request["intelligence"];
        string wisdom       = json_request["wisdom"];
        string charisma     = json_request["charisma"];

        if (DataValidator::fValidate(name,         DataValidator::SQL_INJECTION) &&
    		DataValidator::fValidate(type,         DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(hitpoints,    DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(level,        DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(strength,     DataValidator::ABILITY) &&
			DataValidator::fValidate(dexterity,    DataValidator::ABILITY) &&
			DataValidator::fValidate(constitution, DataValidator::ABILITY) &&
			DataValidator::fValidate(intelligence, DataValidator::ABILITY) &&
			DataValidator::fValidate(wisdom,       DataValidator::ABILITY) &&
			DataValidator::fValidate(charisma,     DataValidator::ABILITY)) // checks data
        {
			string query = "INSERT INTO NPCs (name, type, hitpoints, level, strength, dexterity, constitution, intelligence, wisdom, charisma, id_owner) VALUES ('"+name+"', '"+type+"', '"+hitpoints+"', '"+level+"', '"+strength+"', '"+dexterity+"', '"+constitution+"', '"+intelligence+"', '"+wisdom+"', '"+charisma+"', '"+id_user+"');";
			nlohmann::json json_result = data_base.fExecuteQuery(query);
			cout << query << "\nRESULT:\n" << json_result << endl;
			string query_result = json_result["result"];

			if (query_result == "success")
			{
				query = "SELECT LAST_INSERT_ID() AS id";
				json_result = data_base.fExecuteQuery(query);
				cout << query << "\nRESULT:\n" << json_result << endl;
				string npc_id = json_result["data"][0]["id"];
				json_response = "{\"status\":\"success\", \"npc_id\": \"" + npc_id + "\"}";
			}
			else
				json_response = "{\"status\":\"fail\", \"message\": \"npc is not added, sql query execution failed\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendTerrain(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string terrain_id = json_request["terrain_id"];
        string query = "SELECT name, type, width, height, description, id_owner FROM Terrain WHERE id = " + terrain_id + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];

        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string terrain = json_result["data"][0]["name"];
                string type = json_result["data"][0]["type"];
                string width = json_result["data"][0]["width"];
                string height = json_result["data"][0]["height"];
                string description = json_result["data"][0]["description"];
                string id_owner = json_result["data"][0]["id_owner"];
                json_response = "{\"status\":\"success\", \"terrain\": \"" + terrain + "\", \"terrain_id\": \"" + terrain_id + "\", \"width\": \"" + width + "\", \"type\": \"" + type + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"no terrain that you own with the specified id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"terrain is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendDefinedTerrains(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string type = json_request["type"];
        string count = json_request["count"];
        string query = "SELECT id, name, width, height, description, id_owner FROM Terrain LIMIT "+count+";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];

        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"terrains_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string terrain_id = json_result["data"][rows_qtt]["id"];
                    string terrain = json_result["data"][rows_qtt]["name"];
                    string type = json_result["data"][rows_qtt]["type"];
                    string width = json_result["data"][rows_qtt]["width"];
                    string height = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner = json_result["data"][rows_qtt]["id_owner"];
                    json_response += "{\"terrain\": \"" + terrain + "\", \"terrain_id\": \"" + terrain_id + "\", \"width\": \"" + width + "\", \"type\": \"" + type + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of your terrains is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of your terrains is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendOwnTerrainsList(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name, width, height, description, id_owner FROM Terrain WHERE id_owner=" + id_user + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];

        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"terrains_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string terrain_id = json_result["data"][rows_qtt]["id"];
                    string terrain = json_result["data"][rows_qtt]["name"];
                    string type = json_result["data"][rows_qtt]["type"];
                    string width = json_result["data"][rows_qtt]["width"];
                    string height = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner = json_result["data"][rows_qtt]["id_owner"];
                    json_response += "{\"terrain\": \"" + terrain + "\", \"terrain_id\": \"" + terrain_id + "\", \"width\": \"" + width + "\", \"type\": \"" + type + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of your terrains is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of your terrains is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendOwnNpcsList(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name, type, hitpoints, level, strength, dexterity, constitution, intelligence, wisdom, charisma, id_owner FROM NPCs WHERE id_owner = " + id_user + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"npcs_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string npc_id = json_result["data"][rows_qtt]["id"];
                    string npc = json_result["data"][rows_qtt]["name"];
                    string type = json_result["data"][rows_qtt]["type"];
                    string hitpoints = json_result["data"][rows_qtt]["hitpoints"];
                    string level = json_result["data"][rows_qtt]["level"];
                    string strength = json_result["data"][rows_qtt]["strength"];
                    string dexterity = json_result["data"][rows_qtt]["dexterity"];
                    string constitution = json_result["data"][rows_qtt]["constitution"];
                    string intelligence = json_result["data"][rows_qtt]["intelligence"];
                    string wisdom = json_result["data"][rows_qtt]["wisdom"];
                    string charisma = json_result["data"][rows_qtt]["charisma"];
                    string id_owner = json_result["data"][rows_qtt]["id_owner"];
                    json_response += "{\"npc\": \"" + npc + "\", \"npc_id\": \"" + npc_id + "\", \"type\": \"" + type + "\", \"hitpoints\": \"" + hitpoints + "\", \"level\": \"" + level + "\", \"strength\": \"" + strength + "\", \"dexterity\": \"" + dexterity + "\", \"constitution\": \"" + constitution + "\", \"intelligence\": \"" + intelligence + "\", \"wisdom\": \"" + wisdom + "\", \"charisma\": \"" + charisma + "\", \"id_owner\": \"" + id_owner + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of your npcs is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of your npcs is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendNpc(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string npc_id = json_request["npc_id"];
        string query = "SELECT id, name, type, level, hitpoints, strength, dexterity, constitution, intelligence, wisdom, charisma, id_owner FROM NPCs WHERE id = " + npc_id + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string npc = json_result["data"][0]["name"];
                string type = json_result["data"][0]["type"];
                string level = json_result["data"][0]["level"];
                string hitpoints = json_result["data"][0]["hitpoints"];
                string strength = json_result["data"][0]["strength"];
                string dexterity = json_result["data"][0]["dexterity"];
                string constitution = json_result["data"][0]["constitution"];
                string intelligence = json_result["data"][0]["intelligence"];
                string wisdom = json_result["data"][0]["wisdom"];
                string charisma = json_result["data"][0]["charisma"];
                string id_owner = json_result["data"][0]["id_owner"];
                
                json_response = "{\"status\":\"success\", \"npc\":\"" + npc + "\", \"id\": \"" + npc_id + "\", \"type\": \"" + type + "\", \"level\": \"" + level + "\", \"hitpoints\": \"" + hitpoints + "\", \"strength\": \"" + strength + "\", \"dexterity\": \"" + dexterity + "\", \"constitution\": \"" + constitution + "\", \"intelligence\": \"" + intelligence + "\", \"wisdom\": \"" + wisdom + "\", \"charisma\": \"" + charisma + "\", \"id_owner\": \"" + id_owner + "\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"no your npc with this id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"npc is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fEditNpc(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string npc_id       = json_request["npc_id"];
        string name         = json_request["npc"];
        string type         = json_request["type"];
        string hitpoints    = json_request["hitpoints"];
        string level        = json_request["level"];
        string strength     = json_request["strength"];
        string dexterity    = json_request["dexterity"];
        string constitution = json_request["constitution"];
        string intelligence = json_request["intelligence"];
        string wisdom       = json_request["wisdom"];
        string charisma     = json_request["charisma"];
        
        if (DataValidator::fValidate(name,         DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(type,         DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(hitpoints,    DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(level,        DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(strength,     DataValidator::ABILITY) &&
            DataValidator::fValidate(dexterity,    DataValidator::ABILITY) &&
            DataValidator::fValidate(constitution, DataValidator::ABILITY) &&
            DataValidator::fValidate(intelligence, DataValidator::ABILITY) &&
            DataValidator::fValidate(wisdom,       DataValidator::ABILITY) &&
            DataValidator::fValidate(charisma,     DataValidator::ABILITY))
        {
            string query = "UPDATE NPCs SET name = '" + name + "', type = '" + type + "', hitpoints = '" + hitpoints + "', level = '" + level + "', strength = '" + strength + "', dexterity = '" + dexterity + "', constitution = '" + constitution + "', intelligence = '" + intelligence + "', wisdom = '" + wisdom + "', charisma = '" + charisma + "' WHERE id_owner = '" + id_user + "' AND id = '" + npc_id + "';";
            nlohmann::json json_result = data_base.fExecuteQuery(query);
            cout << query << "\nRESULT:\n" << json_result << endl;
            string query_result = json_result["result"];
            
            if (query_result == "success")
            {
                query = "SELECT id, name, type, level, hitpoints, strength, dexterity, constitution, intelligence, wisdom, charisma, id_owner FROM NPCs WHERE id_owner = '" + id_user + "' AND id = '" + npc_id + "';";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                string npc_id = json_result["data"][0]["id"];
                json_response = "{\"status\":\"success\", \"npc_id\": \"" + npc_id + "\"}";
                
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"npc was not updated, sql query execution failed\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fDeleteNpc(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string npc_id = json_request["npc_id"];
        string query = "DELETE FROM NPCs WHERE id_owner = '" + id_user + "' AND id = '" + npc_id + "' LIMIT 1;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
            json_response = "{\"status\":\"success\", \"message\": \"your npc with this id was deleted\"}";
        else
            json_response = "{\"status\":\"fail\", \"message\": \"npc is not deleted, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSaveCharacter(std::string &json_response, nlohmann::json &json_request)
{
	string session_id = json_request["session_id"];
	string id_user;
	if (fRetrieveUserId(id_user, session_id))
	{
		std::string name = json_request["character"];
		std::string race = json_request["race"];
		std::string _class = json_request["class"];
		std::string experience = json_request["experience"];
		std::string hitpoints = json_request["hitpoints"];
		std::string level = json_request["level"];
		std::string strength = json_request["strength"];
		std::string strength_mod = fSetAbilityMod(strength);
		std::string dexterity = json_request["dexterity"];
		std::string dexterity_mod = fSetAbilityMod(dexterity);
		std::string constitution = json_request["constitution"];
		std::string constitution_mod = fSetAbilityMod(constitution);
		std::string intelligence = json_request["intelligence"];
		std::string intelligence_mod = fSetAbilityMod(intelligence);
		std::string charisma = json_request["charisma"];
		std::string charisma_mod = fSetAbilityMod(charisma);
		std::string wisdom = json_request["wisdom"];
		std::string wisdom_mod = fSetAbilityMod(wisdom);

		if (DataValidator::fValidate(name, DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(race, DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(_class, DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(hitpoints, DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(level, DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(strength, DataValidator::ABILITY) &&
			DataValidator::fValidate(dexterity, DataValidator::ABILITY) &&
			DataValidator::fValidate(constitution, DataValidator::ABILITY) &&
			DataValidator::fValidate(intelligence, DataValidator::ABILITY) &&
			DataValidator::fValidate(wisdom, DataValidator::ABILITY) &&
			DataValidator::fValidate(charisma, DataValidator::ABILITY)) // checks data
		{
			string query = "INSERT INTO CHARACTERs (name, race, class, experience,hitpoints, level, id_user) VALUES ('" + name + "', '" + race + "', '" + _class + "', '" + experience + "', '" + hitpoints + "', '" + level + "', '" + id_user + "');";
			nlohmann::json json_result = data_base.fExecuteQuery(query);
			cout << query << "\nRESULT:\n" << json_result << endl;
			string query_result = json_result["result"];

			if (query_result == "success")
			{
				query = "SELECT LAST_INSERT_ID() AS id";
				json_result = data_base.fExecuteQuery(query);
				cout << query << "\nRESULT:\n" << json_result << endl;
				string character_id = json_result["data"][0]["id"];
				json_response = "{\"status\":\"success\", \"character_id\": \"" + character_id + "\"}";

				string query = "INSERT INTO ABILITIES (strength, str_mod, dexterity, dex_mod, constitution, con_mod, intelligence, int_mod, wisdom, wis_mod, charisma, cha_mod, id_character) VALUES ('" + strength + "', '"+strength_mod+"', '" + dexterity + "', '"+dexterity_mod +"', '" + constitution + "', '"+constitution_mod+"', '" + intelligence + "','"+intelligence_mod+"', '" + wisdom + "','"+wisdom_mod+"', '" + charisma + "','"+charisma_mod+"', '" + character_id + "');";
				nlohmann::json json_result = data_base.fExecuteQuery(query);
				cout << query << "\nRESULT:\n" << json_result << endl;
				string query_result = json_result["result"];
				
				if (query_result == "success")
				{
					query = "SELECT LAST_INSERT_ID() AS id";
					json_result = data_base.fExecuteQuery(query);
					cout << query << "\nRESULT:\n" << json_result << endl;
					string ability_id = json_result["data"][0]["id"];
					json_response = "{\"status\":\"success\", \"ability_id\": \"" + ability_id + "\"}";
				}
				else
					json_response = "{\"status\":\"fail\", \"message\": \"abilities is not added, sql query execution failed\"}";

			}
			else
				json_response = "{\"status\":\"fail\", \"message\": \"character is not added, sql query execution failed\"}";
		}
		else
			json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
	}
	else
		json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

string fSetAbilityMod(std::string ability)
{
	int ability_ = std::stoi(ability);

	if (ability_ >= 4 && ability_ <= 6)
	{
		return "-2";
	}
	else if (ability_ >= 7 && ability_ <= 9)
	{
		return "-1";
	}
	else if (ability_ == 10)
	{
		return "0";
	}
	else if (ability_ >= 11 && ability_ <= 13)
	{
		return "1";
	}
	else if (ability_ >= 14 && ability_ <= 16)
	{
		return "2";
	}
	else if (ability_ >= 17 && ability_ <= 19)
	{
		return "3";
	}
	else if (ability_ == 20)
	{
		return "5";
	}
	else
	{
		return "-3";
	}
}

void fSendOwnCharacterList(std::string &json_response, nlohmann::json &json_request)
{
	string session_id = json_request["session_id"];
	string id_user;
    
	if (fRetrieveUserId(id_user, session_id))
	{
		string query = "SELECT c.id, c.name, c.race, c.class, c.experience, c.hitpoints, c.level, c.id_user, a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma, a.id_character FROM Characters c, Abilities a WHERE id_user = " + id_user + " AND c.id = a.id_character;";
		nlohmann::json json_result = data_base.fExecuteQuery(query);
		cout << query << "\nRESULT:\n" << json_result << endl;
		string query_result = json_result["result"];

		if (query_result == "success")
		{
			string rows = json_result["rows"];
			int rows_qtt = stoi(rows);
			if (rows_qtt > 0)
			{
				json_response = "{\"status\":\"success\", \"character_quantity\":\"" + rows + "\", \"list\": [";
				while (rows_qtt--)
				{
					string character_id = json_result["data"][rows_qtt]["id"];
					string character = json_result["data"][rows_qtt]["name"];
					string race = json_result["data"][rows_qtt]["race"];
					string class_ = json_result["data"][rows_qtt]["class"];
					string experience = json_result["data"][rows_qtt]["experience"];
					string hitpoints = json_result["data"][rows_qtt]["hitpoints"];
					string level = json_result["data"][rows_qtt]["level"];
					string id_owner = json_result["data"][rows_qtt]["id_user"];
					json_response += "{\"character\": \"" + character + "\", \"character_id\": \"" + character_id + "\", \"race\": \"" + race + "\", \"class\": \"" + class_ + "\", \"experience\": \"" + experience + "\", \"hitpoints\": \"" + hitpoints + "\", \"level\": \"" + level + "\", \"id_owner\": \"" + id_owner + "\"}";
					if (rows_qtt)
						json_response += ",";
				}
				json_response += "]}";
			}
			else
				json_response = "{\"status\":\"warning\", \"message\": \"list of your characters is empty\"}";
		}
		else
			json_response = "{\"status\":\"fail\", \"message\": \"list of your characters is not loaded, sql query execution failed\"}";
	}
	else
		json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendDefinedCharacter(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string name = json_request["character"];
        string query = "SELECT c.id, c.name, c.race, c.class, c.experience, c.hitpoints, c.level, c.id_user, a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma, a.id_character FROM Characters c, Abilities a WHERE c.name = '" + name + "' AND c.id = a.id_character;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"characters_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string character_id = json_result["data"][rows_qtt]["id"];
                    string character = json_result["data"][rows_qtt]["name"];
                    string race = json_result["data"][rows_qtt]["race"];
                    string class_ = json_result["data"][rows_qtt]["class"];
                    string experience = json_result["data"][rows_qtt]["experience"];
                    string hitpoints = json_result["data"][rows_qtt]["hitpoints"];
                    string level = json_result["data"][rows_qtt]["level"];
                    string id_owner = json_result["data"][rows_qtt]["id_user"];
                    json_response += "{\"character\": \"" + character + "\", \"character_id\": \"" + character_id + "\", \"race\": \"" + race + "\", \"class\": \"" + class_ + "\", \"experience\": \"" + experience + "\", \"hitpoints\": \"" + hitpoints + "\", \"level\": \"" + level + "\", \"id_owner\": \"" + id_owner + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of your characters is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of characters is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}
