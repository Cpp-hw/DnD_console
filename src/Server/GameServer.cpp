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
void fSendMyNpc(std::string &json_response, nlohmann::json &json_request);
void fEditNpc(std::string &json_response, nlohmann::json &json_request);
void fDeleteNpc(std::string &json_response, nlohmann::json &json_request);
void fSendTerrain(std::string &json_response, nlohmann::json &json_request);
void fDeleteTerrain(std::string &json_response, nlohmann::json &json_request);
void fSendDefinedTerrains(std::string &json_response, nlohmann::json &json_request);
void fSendOwnTerrainsList(std::string &json_response, nlohmann::json &json_request);
void fSendOwnCharacterList(std::string &json_response, nlohmann::json &json_request);
void fUserRegistration(std::string &json_response, nlohmann::json &json_request);
void fSendDefinedCharacter(std::string &json_response, nlohmann::json &json_request);
void fSaveBoard(std::string &json_response, nlohmann::json &json_request);
void fSaveObjectsOnBoard(std::string &json_response, nlohmann::json &json_request);
void fSendOwnBoard(std::string &json_response, nlohmann::json &json_request);
void fSendBoard(std::string &json_response, nlohmann::json &json_request);
void fEditBoard(std::string &json_response, nlohmann::json &json_request);
void fSendOwnBoardsList(std::string &json_response, nlohmann::json &json_request);
void fSendBoardsList(std::string &json_response, nlohmann::json &json_request);
void fDeleteCharacter(std::string &json_response, nlohmann::json &json_request);
void fEditCharacter(std::string &json_response, nlohmann::json &json_request);
void fSendClasses(std::string &json_response, nlohmann::json &json_request);
void fSendRaces(std::string &json_response, nlohmann::json &json_request);
void fSendNpcTypes(std::string &json_response, nlohmann::json &json_request);
void fSendTerrainTypes(std::string &json_response, nlohmann::json &json_request);
void fUserLogOut(std::string &json_response, nlohmann::json &json_request);
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
                    else if (path.find("/api/userlogout") != string::npos)
                        fUserLogOut(response, json_request);
                    else if (path.find("/api/addterrain") != string::npos)
                        fSaveTerrain(response, json_request);
                    else if (path.find("/api/loadterrain") != string::npos)
                        fSendTerrain(response, json_request);
                    else if (path.find("/api/deleteterrain") != string::npos)
                        fDeleteTerrain(response, json_request);
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
                    else if (path.find("/api/loadmynpc") != string::npos)
                        fSendMyNpc(response, json_request);
                    else if (path.find("/api/editnpc") != string::npos)
                        fEditNpc(response, json_request);
                    else if (path.find("/api/deletenpc") != string::npos)
                        fDeleteNpc(response, json_request);
                    else if (path.find("/api/addcharacter") != string::npos)
                        fSaveCharacter(response, json_request);
                    else if (path.find("/api/loadmycharacterslist") != string::npos)
                        fSendOwnCharacterList(response, json_request);
                    else if (path.find("/api/loaddefinedcharacter") != string::npos)
                        fSendDefinedCharacter(response, json_request);
                    else if (path.find("/api/deletecharacter") != string::npos)
                        fDeleteCharacter(response, json_request);
                    else if (path.find("/api/editcharacter") != string::npos)
                        fEditCharacter(response, json_request);
                    else if (path.find("/api/addboard") != string::npos)
                        fSaveBoard(response, json_request);
                    else if (path.find("/api/addobjectonboard") != string::npos)
                        fSaveObjectsOnBoard(response, json_request);
                    else if (path.find("/api/loadmyboard") != string::npos)
                        fSendOwnBoard(response, json_request);
                    else if (path.find("/api/loadboard") != string::npos)
                        fSendBoard(response, json_request);
                    else if (path.find("/api/editboard") != string::npos)
                        fEditBoard(response, json_request);
                    else if (path.find("/api/loadmyboardslist") != string::npos)
                        fSendOwnBoardsList(response, json_request);
                    else if (path.find("/api/loadboardslist") != string::npos)
                        fSendBoardsList(response, json_request);
                    else if (path.find("/api/loadclasses") != string::npos)
                        fSendClasses(response, json_request);
                    else if (path.find("/api/loadraces") != string::npos)
                        fSendRaces(response, json_request);
                    else if (path.find("/api/loadtypesnpc") != string::npos)
                        fSendNpcTypes(response, json_request);
                    else if (path.find("/api/loadtypesterrain") != string::npos)
                        fSendTerrainTypes(response, json_request);
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
    string email    = json_request["email"];

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

void fUserLogOut(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "DELETE FROM Sessions WHERE id = " + session_id;
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
            json_response = "{\"status\":\"success\", \"message\": \"you are logged out\"}";
        else
            json_response = "{\"status\":\"fail\", \"message\": \"you are not logged out, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSaveTerrain(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];

    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string name        = json_request["name"];
        string type        = json_request["type"];
        string width       = json_request["width"];
        string height      = json_request["height"];
        string description = json_request["description"];

        if (DataValidator::fValidate(name,        DataValidator::SQL_INJECTION) &&
        	DataValidator::fValidate(type,        DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(width,       DataValidator::LENGTH) &&
			DataValidator::fValidate(height,      DataValidator::LENGTH) &&
			DataValidator::fValidate(description, DataValidator::SQL_INJECTION)) // checks data
        {
            string query = "SELECT id, name From TerrainTypes WHERE name = '" + type + "';";
            nlohmann::json json_result = data_base.fExecuteQuery(query);
            cout << query << "\nRESULT:\n" << json_result << endl;
            
            if (json_result["result"] == "success" && json_result["rows"] == "1")
            {
                string id_type = json_result["data"][0]["id"];
                query = "INSERT INTO Terrain (name, id_type, width, height, description, id_owner) VALUES ('" + name + "', '" + id_type + "', " + width + ", " + height + ", '" + description + "', " + id_user + ");";
                json_result = data_base.fExecuteQuery(query);
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
                json_response = "{\"status\":\"fail\", \"message\": \"no such Terrain's type\"}";
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
        string id_type      = json_request["id_type"];
        string hitpoints    = json_request["hitpoints"];
        string level        = json_request["level"];
        string strength     = json_request["strength"];
        string dexterity    = json_request["dexterity"];
        string constitution = json_request["constitution"];
        string intelligence = json_request["intelligence"];
        string wisdom       = json_request["wisdom"];
        string charisma     = json_request["charisma"];

        if (DataValidator::fValidate(name,         DataValidator::SQL_INJECTION) &&
    		DataValidator::fValidate(id_type,      DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(hitpoints,    DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(level,        DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(strength,     DataValidator::ABILITY) &&
			DataValidator::fValidate(dexterity,    DataValidator::ABILITY) &&
			DataValidator::fValidate(constitution, DataValidator::ABILITY) &&
			DataValidator::fValidate(intelligence, DataValidator::ABILITY) &&
			DataValidator::fValidate(wisdom,       DataValidator::ABILITY) &&
			DataValidator::fValidate(charisma,     DataValidator::ABILITY)) // checks data
        {
			string query = "INSERT INTO NPCs (name, id_type, hitpoints, level, strength, dexterity, constitution, intelligence, wisdom, charisma, id_owner) VALUES ('" + name + "', '" + id_type + "', '" + hitpoints + "', '" + level + "', '" + strength + "', '" + dexterity + "', '" + constitution + "', '" + intelligence + "', '" + wisdom + "', '" + charisma + "', '" + id_user + "');";
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
        string query = "SELECT t.id, t.name, type.name as type, t.width, t.height, t.description, t.id_owner FROM Terrain t, TerrainTypes type  WHERE t.id = " + terrain_id + " AND t.id_type = type.id;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];

        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string terrain     = json_result["data"][0]["name"];
                string type        = json_result["data"][0]["type"];
                string width       = json_result["data"][0]["width"];
                string height      = json_result["data"][0]["height"];
                string description = json_result["data"][0]["description"];
                string id_owner    = json_result["data"][0]["id_owner"];
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

void fDeleteTerrain(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string terrain_id = json_request["terrain_id"];
        string query = "SELECT t.id, t.name, type.name as type, t.width, t.height, t.description, t.id_owner FROM Terrain t, TerrainTypes type  WHERE t.id = " + terrain_id + " AND t.id_type = type.id;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                query = "DELETE FROM Terrain WHERE id_owner = '" + id_user + "' AND id = '" + terrain_id + "' LIMIT 1;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                string query_result = json_result["result"];
                
                if (query_result == "success")
                    json_response = "{\"status\":\"success\", \"message\": \"your terrain with this id was deleted\"}";
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"terrain is not deleted, sql query execution failed\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"there is no your terrain with this id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
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
        string query = "SELECT t.id, t.name, type.name as type, t.width, t.height, t.description, t.id_owner FROM Terrain t, TerrainTypes type  WHERE type.name = '" + type + "' LIMIT " + count + ";";
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
                    string terrain_id  = json_result["data"][rows_qtt]["id"];
                    string terrain     = json_result["data"][rows_qtt]["name"];
                    string type        = json_result["data"][rows_qtt]["type"];
                    string width       = json_result["data"][rows_qtt]["width"];
                    string height      = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner    = json_result["data"][rows_qtt]["id_owner"];
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
        string query = "SELECT t.id, t.name, type.name as type, t.width, t.height, t.description, t.id_owner FROM Terrain t, TerrainTypes type  WHERE t.id_owner = '" + id_user + "' AND t.id_type = type.id;";
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
                    string terrain_id  = json_result["data"][rows_qtt]["id"];
                    string terrain     = json_result["data"][rows_qtt]["name"];
                    string type        = json_result["data"][rows_qtt]["type"];
                    string width       = json_result["data"][rows_qtt]["width"];
                    string height      = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner    = json_result["data"][rows_qtt]["id_owner"];
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
        string query = "SELECT n.id, n.name, t.name as type, n.level, n.hitpoints, n.strength, n.dexterity, n.constitution, n.intelligence, n.wisdom, n.charisma, n.id_owner FROM NPCs n, NpcTypes t WHERE n.id_owner = " + id_user + " AND n.id_type = t.id;";
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
                    string npc_id       = json_result["data"][rows_qtt]["id"];
                    string npc          = json_result["data"][rows_qtt]["name"];
                    string type         = json_result["data"][rows_qtt]["type"];
                    string hitpoints    = json_result["data"][rows_qtt]["hitpoints"];
                    string level        = json_result["data"][rows_qtt]["level"];
                    string strength     = json_result["data"][rows_qtt]["strength"];
                    string dexterity    = json_result["data"][rows_qtt]["dexterity"];
                    string constitution = json_result["data"][rows_qtt]["constitution"];
                    string intelligence = json_result["data"][rows_qtt]["intelligence"];
                    string wisdom       = json_result["data"][rows_qtt]["wisdom"];
                    string charisma     = json_result["data"][rows_qtt]["charisma"];
                    string id_owner     = json_result["data"][rows_qtt]["id_owner"];
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
        string query = "SELECT n.id, n.name, t.name as type, n.level, n.hitpoints, n.strength, n.dexterity, n.constitution, n.intelligence, n.wisdom, n.charisma, n.id_owner FROM NPCs n, NpcTypes t WHERE n.id = " + npc_id + " AND n.id_type = t.id;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string npc          = json_result["data"][0]["name"];
                string type         = json_result["data"][0]["type"];
                string level        = json_result["data"][0]["level"];
                string hitpoints    = json_result["data"][0]["hitpoints"];
                string strength     = json_result["data"][0]["strength"];
                string dexterity    = json_result["data"][0]["dexterity"];
                string constitution = json_result["data"][0]["constitution"];
                string intelligence = json_result["data"][0]["intelligence"];
                string wisdom       = json_result["data"][0]["wisdom"];
                string charisma     = json_result["data"][0]["charisma"];
                string id_owner     = json_result["data"][0]["id_owner"];
                
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

void fSendMyNpc(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string npc_id = json_request["npc_id"];
        string query = "SELECT n.id, n.name, t.name as type, n.level, n.hitpoints, n.strength, n.dexterity, n.constitution, n.intelligence, n.wisdom, n.charisma, n.id_owner FROM NPCs n, NpcTypes t WHERE n.id = " + npc_id + " AND n.id_type = t.id AND id_owner = '" + id_user + "';";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string npc          = json_result["data"][0]["name"];
                string type         = json_result["data"][0]["type"];
                string level        = json_result["data"][0]["level"];
                string hitpoints    = json_result["data"][0]["hitpoints"];
                string strength     = json_result["data"][0]["strength"];
                string dexterity    = json_result["data"][0]["dexterity"];
                string constitution = json_result["data"][0]["constitution"];
                string intelligence = json_result["data"][0]["intelligence"];
                string wisdom       = json_result["data"][0]["wisdom"];
                string charisma     = json_result["data"][0]["charisma"];
                string id_owner     = json_result["data"][0]["id_owner"];
                
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
        string npc_id = json_request["npc_id"];
        string query = "SELECT n.id, n.name, t.name as type, n.level, n.hitpoints, n.strength, n.dexterity, n.constitution, n.intelligence, n.wisdom, n.charisma, n.id_owner FROM NPCs n, NpcTypes t WHERE n.id = " + npc_id + " AND n.id_type = t.id AND id_owner = '" + id_user + "';";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        
        if (json_result["result"] == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
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
                    query = "SELECT id, name From NpcTypes WHERE name = '" + type + "';";
                    json_result = data_base.fExecuteQuery(query);
                    cout << query << "\nRESULT:\n" << json_result << endl;
                    
                    if (json_result["result"] == "success" && json_result["rows"] == "1")
                    {
                        string id_type = json_result["data"][0]["id"];
                        query = "UPDATE NPCs SET name = '" + name + "', id_type = '" + id_type + "', hitpoints = '" + hitpoints + "', level = '" + level + "', strength = '" + strength + "', dexterity = '" + dexterity + "', constitution = '" + constitution + "', intelligence = '" + intelligence + "', wisdom = '" + wisdom + "', charisma = '" + charisma + "' WHERE id_owner = '" + id_user + "' AND id = '" + npc_id + "';";
                        json_result = data_base.fExecuteQuery(query);
                        cout << query << "\nRESULT:\n" << json_result << endl;
                    
                        if (json_result["result"] == "success")
                        {
                            query = "SELECT n.id, n.name, t.name as type, n.level, n.hitpoints, n.strength, n.dexterity, n.constitution, n.intelligence, n.wisdom, n.charisma, n.id_owner FROM NPCs n, NpcTypes t WHERE n.id = " + npc_id + " AND n.id_type = t.id AND id_owner = '" + id_user + "';";
                            json_result = data_base.fExecuteQuery(query);
                            cout << query << "\nRESULT:\n" << json_result << endl;
                            string npc_id = json_result["data"][0]["id"];
                            json_response = "{\"status\":\"success\", \"npc_id\": \"" + npc_id + "\"}";
                        }
                        else
                            json_response = "{\"status\":\"fail\", \"message\": \"npc was not updated, sql query execution failed\"}";
                    }
                    else
                        json_response = "{\"status\":\"fail\", \"message\": \"no such NPC's type\"}";
                }
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"there is no your npc with this id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
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
        string query = "SELECT n.id, n.name, t.name as type, n.level, n.hitpoints, n.strength, n.dexterity, n.constitution, n.intelligence, n.wisdom, n.charisma, n.id_owner FROM NPCs n, NpcTypes t WHERE n.id = " + npc_id + " AND n.id_type = t.id AND id_owner = '" + id_user + "';";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                query = "DELETE FROM NPCs WHERE id_owner = '" + id_user + "' AND id = '" + npc_id + "' LIMIT 1;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                string query_result = json_result["result"];
                
                if (query_result == "success")
                    json_response = "{\"status\":\"success\", \"message\": \"your npc with this id was deleted\"}";
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"npc is not deleted, sql query execution failed\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"there is no your npc with this id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
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
		std::string name             = json_request["character"];
		std::string race             = json_request["race"];
		std::string c_class          = json_request["class"];
		std::string experience       = json_request["experience"];
		std::string hitpoints        = json_request["hitpoints"];
		std::string level            = json_request["level"];
		std::string strength         = json_request["strength"];
		std::string strength_mod     = fSetAbilityMod(strength);
		std::string dexterity        = json_request["dexterity"];
		std::string dexterity_mod    = fSetAbilityMod(dexterity);
		std::string constitution     = json_request["constitution"];
		std::string constitution_mod = fSetAbilityMod(constitution);
		std::string intelligence     = json_request["intelligence"];
		std::string intelligence_mod = fSetAbilityMod(intelligence);
		std::string charisma         = json_request["charisma"];
		std::string charisma_mod     = fSetAbilityMod(charisma);
		std::string wisdom           = json_request["wisdom"];
		std::string wisdom_mod       = fSetAbilityMod(wisdom);

		if (DataValidator::fValidate(name,         DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(race,         DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(c_class,      DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(hitpoints,    DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(level,        DataValidator::SQL_INJECTION) &&
			DataValidator::fValidate(strength,     DataValidator::ABILITY) &&
			DataValidator::fValidate(dexterity,    DataValidator::ABILITY) &&
			DataValidator::fValidate(constitution, DataValidator::ABILITY) &&
			DataValidator::fValidate(intelligence, DataValidator::ABILITY) &&
			DataValidator::fValidate(wisdom,       DataValidator::ABILITY) &&
			DataValidator::fValidate(charisma,     DataValidator::ABILITY)) // checks data
		{
            string query = "SELECT id, name From Classes WHERE name = '" + c_class + "';";
            nlohmann::json json_result = data_base.fExecuteQuery(query);
            cout << query << "\nRESULT:\n" << json_result << endl;
            
            if (json_result["result"] == "success" && json_result["rows"] == "1")
            {
                string id_class = json_result["data"][0]["id"];
                query = "SELECT id, name From Races WHERE name = '" + race + "';";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                
                if (json_result["result"] == "success" && json_result["rows"] == "1")
                {
                    string id_race = json_result["data"][0]["id"];
                    query = "INSERT INTO CHARACTERs (name, id_race, id_class, experience, hitpoints, level, id_user) VALUES ('" + name + "', '" + id_race + "', '" + id_class + "', '" + experience + "', '" + hitpoints + "', '" + level + "', '" + id_user + "');";
                    json_result = data_base.fExecuteQuery(query);
                    cout << query << "\nRESULT:\n" << json_result << endl;
                    string query_result = json_result["result"];
                    
                    if (query_result == "success")
                    {
                        query = "SELECT LAST_INSERT_ID() AS id";
                        json_result = data_base.fExecuteQuery(query);
                        cout << query << "\nRESULT:\n" << json_result << endl;
                        string character_id = json_result["data"][0]["id"];
                        json_response = "{\"status\":\"success\", \"character_id\": \"" + character_id + "\"}";
                        
                        string query = "INSERT INTO ABILITIES (strength, str_mod, dexterity, dex_mod, constitution, con_mod, intelligence, int_mod, wisdom, wis_mod, charisma, cha_mod, id_character) VALUES ('" + strength + "', '" + strength_mod + "', '" + dexterity + "', '" + dexterity_mod + "', '" + constitution + "', '" + constitution_mod + "', '" + intelligence + "','" + intelligence_mod + "', '" + wisdom + "','" + wisdom_mod + "', '" + charisma + "','" + charisma_mod + "', '" + character_id + "');";
                        nlohmann::json json_result = data_base.fExecuteQuery(query);
                        cout << query << "\nRESULT:\n" << json_result << endl;
                        string query_result = json_result["result"];
                        
                        if (query_result == "success")
                        {
                            query = "SELECT LAST_INSERT_ID() AS id";
                            json_result = data_base.fExecuteQuery(query);
                            cout << query << "\nRESULT:\n" << json_result << endl;
                            string ability_id = json_result["data"][0]["id"];
                            //json_response = "{\"status\":\"success\", \"ability_id\": \"" + ability_id + "\"}";
                        }
                        else
                            json_response += "{\"status\":\"fail\", \"message\": \"abilities is not added, sql query execution failed\"}";
                    }
                    else
                        json_response = "{\"status\":\"fail\", \"message\": \"character is not added, sql query execution failed\"}";
                }
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"no such Character's race\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"no such Character's class\"}";
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
        string query = "SELECT ch.id, ch.name, r.name as race, c.name as class, ch.experience, ch.hitpoints, ch.level, ch.id_user, a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma, a.id_character FROM Characters ch, Abilities a, Classes c, Races r WHERE ch.id_user = " + id_user + " AND ch.id = a.id_character AND ch.id_class = c.id AND ch.id_race = r.id;";
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
					string character    = json_result["data"][rows_qtt]["name"];
					string race         = json_result["data"][rows_qtt]["race"];
					string class_       = json_result["data"][rows_qtt]["class"];
					string experience   = json_result["data"][rows_qtt]["experience"];
					string hitpoints    = json_result["data"][rows_qtt]["hitpoints"];
					string level        = json_result["data"][rows_qtt]["level"];
					string id_owner     = json_result["data"][rows_qtt]["id_user"];
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

void fSaveBoard(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_owner;
    if (fRetrieveUserId(id_owner, session_id))
    {
        string name        = json_request["board"];
        string width       = json_request["width"];
        string height      = json_request["height"];
        string description = json_request["description"];
        string spawn_x = "";
        string spawn_y = "";
        
        if (json_request.count("spawn_x") > 0)
            spawn_x = json_request["spawn_x"];
        if (json_request.count("spawn_y") > 0)
            spawn_y = json_request["spawn_y"];
        
        if (DataValidator::fValidate(name,         DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(width,        DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(height,       DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(description,  DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(spawn_x,      DataValidator::SQL_INJECTION) &&
            DataValidator::fValidate(spawn_y,      DataValidator::SQL_INJECTION)) // checks data
        {
            string query = "INSERT INTO Boards (name, width, height, description, spawn_x, spawn_y, id_owner) VALUES ('" + name + "', '" + width + "', '" + height + "', '" + description + "', '" + spawn_x + "', '" + spawn_y + "', '" + id_owner + "');";
            nlohmann::json json_result = data_base.fExecuteQuery(query);
            cout << query << "\nRESULT:\n" << json_result << endl;
            string query_result = json_result["result"];
            
            if (query_result == "success")
            {
                query = "SELECT LAST_INSERT_ID() AS id";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                string board_id = json_result["data"][0]["id"];
                json_response = "{\"status\":\"success\", \"board_id\": \"" + board_id + "\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"board is not added, sql query execution failed\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSaveObjectsOnBoard(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_owner;
    if (fRetrieveUserId(id_owner, session_id))
    {
        std::string id_board = json_request["id_board"];
        string data_count    = json_request["data_count"];
        int data_qtt = stoi(data_count);
        if (data_qtt > 0)
        {
            while (data_qtt--)
            {
                if (json_request["data"][data_qtt]["type"] == "npc")
                {
                    string id_npc = json_request["data"][data_qtt]["id"];
                    string pos_x  = json_request["data"][data_qtt]["pos_x"];
                    string pos_y  = json_request["data"][data_qtt]["pos_y"];
                    
                    if (DataValidator::fValidate(pos_x, DataValidator::SQL_INJECTION)&&
                        DataValidator::fValidate(pos_y, DataValidator::SQL_INJECTION)) // checks data
                    {
                        string query = "SELECT id_board, id_npc, npc_x, npc_y FROM BN_Map WHERE id_board = '" + id_board + "' AND id_npc = '" + id_npc + "' AND npc_x = '" + pos_x + "' AND npc_y = '" + pos_y + "';";
                        nlohmann::json json_result = data_base.fExecuteQuery(query);
                        cout << query << "\nRESULT:\n" << json_result << endl;
                        string query_result = json_result["result"];
                        
                        if (query_result == "success")
                        {
                            string rows = json_result["rows"];
                            int row_qtt = stoi(rows);
                            if (row_qtt > 0)
                                json_response += "{\"status\":\"fail\", \"message\": \"position duplication of npc with id " + id_npc +"\"}";
                            else
                            {
                                query = "INSERT INTO BN_Map (id_board, id_npc, npc_x, npc_y) VALUES ('" + id_board + "', '" + id_npc + "', '" + pos_x + "', '" + pos_y + "');";
                                json_result = data_base.fExecuteQuery(query);
                                cout << query << "\nRESULT:\n" << json_result << endl;
                                query_result = json_result["result"];
                                
                                if (query_result == "success")
                                    json_response += "{\"status\":\"success\", \"message\": \"npc with id " + id_npc + " is added\"}";
                                else
                                    json_response += "{\"status\":\"fail\", \"message\": \"npc with id " + id_npc + " is not added, sql query execution failed\"}";
                            }
                        }
                        else
                            json_response += "{\"status\":\"fail\", \"message\": \"database error\"}";
                    }
                    else
                        json_response += "{\"status\":\"fail\", \"message\": \"invalid npc's data passed\"}";
                }
                else if (json_request["data"][data_qtt]["type"] == "terrain")
                {
                    string id_terrain = json_request["data"][data_qtt]["id"];
                    string pos_x      = json_request["data"][data_qtt]["pos_x"];
                    string pos_y      = json_request["data"][data_qtt]["pos_y"];
                    
                    if (DataValidator::fValidate(pos_x, DataValidator::SQL_INJECTION)&&
                        DataValidator::fValidate(pos_y, DataValidator::SQL_INJECTION)) // checks data
                    {
                        string query = "SELECT id_board, id_terrain, terrain_x, terrain_y FROM BT_Map WHERE id_board = '" + id_board + "' AND id_terrain = '" + id_terrain + "' AND terrain_x = '" + pos_x + "' AND terrain_y = '" + pos_y + "';";
                        nlohmann::json json_result = data_base.fExecuteQuery(query);
                        cout << query << "\nRESULT:\n" << json_result << endl;
                        string query_result = json_result["result"];
                        
                        if (query_result == "success")
                        {
                            string rows = json_result["rows"];
                            int row_qtt = stoi(rows);
                            if (row_qtt > 0)
                                json_response += "{\"status\":\"fail\", \"message\": \"position duplication of terrain with id " + id_terrain +"\"}";
                            else
                            {
                                query = "INSERT INTO BT_Map (id_board, id_terrain, terrain_x, terrain_y) VALUES ('" + id_board + "', '" + id_terrain + "', '" + pos_x + "', '" + pos_y + "');";
                                json_result = data_base.fExecuteQuery(query);
                                cout << query << "\nRESULT:\n" << json_result << endl;
                                query_result = json_result["result"];
                                
                                if (query_result == "success")
                                    json_response += "{\"status\":\"success\", \"message\": \"terrain with id " + id_terrain + " is added\"}";
                                else
                                    json_response += "{\"status\":\"fail\", \"message\": \"terrain with id " + id_terrain + " is not added, sql query execution failed\"}";
                            }
                        }
                        else
                            json_response += "{\"status\":\"fail\", \"message\": \"database error\"}";
                    }
                    else
                        json_response += "{\"status\":\"fail\", \"message\": \"invalid terrain's data passed\"}";
                }
            }
        }
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
        string query = "SELECT ch.id, ch.name, r.name as race, c.name as class, ch.experience, ch.hitpoints, ch.level, ch.id_user, a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma FROM Characters ch, Abilities a, Classes c, Races r WHERE ch.name = '" + name + "' AND ch.id = a.id_character AND ch.id_class = c.id AND ch.id_race = r.id;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", ";
                
                string character_id = json_result["data"][0]["id"];
                string character    = json_result["data"][0]["name"];
                string race         = json_result["data"][0]["race"];
                string class_       = json_result["data"][0]["class"];
                string experience   = json_result["data"][0]["experience"];
                string hitpoints    = json_result["data"][0]["hitpoints"];
                string level        = json_result["data"][0]["level"];
                string id_owner     = json_result["data"][0]["id_user"];
                string strength     = json_result["data"][0]["strength"];
                string dexterity    = json_result["data"][0]["dexterity"];
                string constitution = json_result["data"][0]["constitution"];
                string intelligence = json_result["data"][0]["intelligence"];
                string wisdom       = json_result["data"][0]["wisdom"];
                string charisma     = json_result["data"][0]["charisma"];
                
                json_response += "\"character\": \"" + character + "\", \"id\": \"" + character_id + "\", \"race\": \"" + race + "\", \"class\": \"" + class_ + "\", \"experience\": \"" + experience + "\", \"hitpoints\": \"" + hitpoints + "\", \"level\": \"" + level + "\", \"id_owner\": \"" + id_owner + "\", \"strength\": \"" + strength + "\", \"dexterity\": \"" + dexterity + "\", \"constitution\": \"" + constitution + "\", \"intelligence\": \"" + intelligence + "\", \"wisdom\": \"" + wisdom + "\", \"charisma\": \"" + charisma + "\"}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"you have no such character\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fDeleteCharacter(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string character_id = json_request["character_id"];
        string query = "Select id, name FROM Characters WHERE id_user = '" + id_user + "' AND id = '" + character_id + "';";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                query = "START TRANSACTION;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                query = "DELETE FROM Abilities WHERE id_character = '" + character_id + "' LIMIT 1;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                query = "DELETE FROM Characters WHERE id_user = '" + id_user + "' AND id = '" + character_id + "' LIMIT 1;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                query = "COMMIT;";
                json_result = data_base.fExecuteQuery(query);
                
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                
                if (query_result == "success")
                    json_response = "{\"status\":\"success\", \"message\": \"your character with this id was deleted\"}";
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"character is not deleted, sql query execution failed\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"you have no such character\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fEditCharacter(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string character_id = json_request["character_id"];
        string query = "SELECT ch.id, ch.name, r.name as race, c.name as class, ch.experience, ch.hitpoints, ch.level, ch.id_user, a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma FROM Characters ch, Abilities a, Classes c, Races r WHERE ch.id = '" + character_id + "' AND ch.id = a.id_character AND ch.id_class = c.id AND ch.id_race = r.id AND id_user = '" + id_user + "';";
        
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        
        if (json_result["result"] == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string name             = json_request["character"];
                string race_name        = json_request["race"];
                string class_name       = json_request["class"];
                string experience       = json_request["experience"];
                string hitpoints        = json_request["hitpoints"];
                string level            = json_request["level"];
                string strength         = json_request["strength"];
                string strength_mod     = fSetAbilityMod(strength);
                string dexterity        = json_request["dexterity"];
                string dexterity_mod    = fSetAbilityMod(dexterity);
                string constitution     = json_request["constitution"];
                string constitution_mod = fSetAbilityMod(constitution);
                string intelligence     = json_request["intelligence"];
                string intelligence_mod = fSetAbilityMod(intelligence);
                string charisma         = json_request["charisma"];
                string charisma_mod     = fSetAbilityMod(charisma);
                string wisdom           = json_request["wisdom"];
                string wisdom_mod       = fSetAbilityMod(wisdom);
                
                if (DataValidator::fValidate(name,         DataValidator::SQL_INJECTION) &&
                    DataValidator::fValidate(race_name,    DataValidator::SQL_INJECTION) &&
                    DataValidator::fValidate(class_name,   DataValidator::SQL_INJECTION) &&
                    DataValidator::fValidate(hitpoints,    DataValidator::SQL_INJECTION) &&
                    DataValidator::fValidate(level,        DataValidator::SQL_INJECTION) &&
                    DataValidator::fValidate(strength,     DataValidator::ABILITY) &&
                    DataValidator::fValidate(dexterity,    DataValidator::ABILITY) &&
                    DataValidator::fValidate(constitution, DataValidator::ABILITY) &&
                    DataValidator::fValidate(intelligence, DataValidator::ABILITY) &&
                    DataValidator::fValidate(wisdom,       DataValidator::ABILITY) &&
                    DataValidator::fValidate(charisma,     DataValidator::ABILITY))
                {
                    query = "SELECT id, name From Races WHERE name = '" + race_name + "';";
                    json_result = data_base.fExecuteQuery(query);
                    cout << query << "\nRESULT:\n" << json_result << endl;
                    
                    if (json_result["result"] == "success" && json_result["rows"] == "1")
                    {
                        string id_new_race = json_result["data"][0]["id"];
                        query = "SELECT id, name From Classes WHERE name = '" + class_name + "';";
                        json_result = data_base.fExecuteQuery(query);
                        cout << query << "\nRESULT:\n" << json_result << endl;
                        
                        if (json_result["result"] == "success" && json_result["rows"] == "1")
                        {
                            string id_new_class = json_result["data"][0]["id"];
                            query = "UPDATE Characters SET name = '" + name + "', id_race = '" + id_new_race + "', id_class = '" + id_new_class + "', experience = '" + experience + "', hitpoints = '" + hitpoints + "', level = '" + level + "' WHERE id_user = '" + id_user + "' AND id = '" + character_id + "';";
                            json_result = data_base.fExecuteQuery(query);
                            cout << query << "\nRESULT:\n" << json_result << endl;
                            
                            if (json_result["result"] == "success")
                            {
                                query = "UPDATE Abilities SET strength = '" + strength + "', str_mod = '" + strength_mod + "', dexterity = '" + dexterity + "', dex_mod = '" + dexterity_mod + "', constitution = '" + constitution + "', con_mod = '" + constitution_mod + "', intelligence = '" + intelligence + "', int_mod = '" + intelligence_mod + "', wisdom = '" + wisdom + "', wis_mod = '" + wisdom_mod + "', charisma = '" + charisma + "', cha_mod = '" + charisma_mod + "' WHERE id_character = '" + character_id + "';";
                                json_result = data_base.fExecuteQuery(query);
                                cout << query << "\nRESULT:\n" << json_result << endl;
                                string query_result = json_result["result"];
                                
                                if (query_result == "success")
                                {
                                    query = "SELECT ch.id, ch.name, r.name as race, c.name as class, ch.experience, ch.hitpoints, ch.level, ch.id_user, a.strength, a.dexterity, a.constitution, a.intelligence, a.wisdom, a.charisma FROM Characters ch, Abilities a, Classes c, Races r WHERE ch.id = '" + character_id + "' AND ch.id = a.id_character AND ch.id_class = c.id AND ch.id_race = r.id AND id_owner = '" + id_user + "';";
                                    json_result = data_base.fExecuteQuery(query);
                                    cout << query << "\nRESULT:\n" << json_result << endl;
                                    if (query_result == "success")
                                    {
                                        string rows = json_result["rows"];
                                        int rows_qtt = stoi(rows);
                                        if (rows_qtt > 0)
                                        {
                                            json_response = "{\"status\":\"success\", ";
                                            
                                            string character_id = json_result["data"][0]["id"];
                                            string character    = json_result["data"][0]["name"];
                                            string race         = json_result["data"][0]["race"];
                                            string class_       = json_result["data"][0]["class"];
                                            string experience   = json_result["data"][0]["experience"];
                                            string hitpoints    = json_result["data"][0]["hitpoints"];
                                            string level        = json_result["data"][0]["level"];
                                            string id_owner     = json_result["data"][0]["id_user"];
                                            string strength     = json_result["data"][0]["strength"];
                                            string dexterity    = json_result["data"][0]["dexterity"];
                                            string constitution = json_result["data"][0]["constitution"];
                                            string intelligence = json_result["data"][0]["intelligence"];
                                            string wisdom       = json_result["data"][0]["wisdom"];
                                            string charisma     = json_result["data"][0]["charisma"];
                                            
                                            json_response += "\"character\": \"" + character + "\", \"id\": \"" + character_id + "\", \"race\": \"" + race + "\", \"class\": \"" + class_ + "\", \"experience\": \"" + experience + "\", \"hitpoints\": \"" + hitpoints + "\", \"level\": \"" + level + "\", \"id_owner\": \"" + id_owner + "\", \"strength\": \"" + strength + "\", \"dexterity\": \"" + dexterity + "\", \"constitution\": \"" + constitution + "\", \"intelligence\": \"" + intelligence + "\", \"wisdom\": \"" + wisdom + "\", \"charisma\": \"" + charisma + "\"}";
                                        }
                                    }
                                    else
                                        json_response = "{\"status\":\"fail\", \"message\": \"sql query execution failed\"}";
                                }
                                else
                                    json_response = "{\"status\":\"fail\", \"message\": \"sql query execution failed\"}";
                            }
                            else
                                json_response = "{\"status\":\"fail\", \"message\": \"character was not updated, sql query execution failed\"}";
                        }
                        else
                            json_response = "{\"status\":\"fail\", \"message\": \"no such Character's class\"}";
                    }
                    else
                        json_response = "{\"status\":\"fail\", \"message\": \"no such Character's race\"}";
                }
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"invalid data passed\"}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"there is no your character with this id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendOwnBoard(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string board_id = json_request["board_id"];
        string query = "SELECT b.name, b.width, b.height, b.spawn_x, b.spawn_y, b.description, b.id_owner FROM Boards b WHERE b.id = '" + board_id + "' AND b.id_owner = '" + id_user + "';";
        json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string board       = json_result["data"][0]["name"];
                string width       = json_result["data"][0]["width"];
                string height      = json_result["data"][0]["height"];
                string spawn_x     = json_result["data"][0]["spawn_x"];
                string spawn_y     = json_result["data"][0]["spawn_y"];
                string description = json_result["data"][0]["description"];
                string id_owner    = json_result["data"][0]["id_owner"];
                json_response = "{\"status\":\"success\", \"board\": \"" + board + "\", \"board_id\": \"" + board_id + "\", \"width\": \"" + width + "\", \"height\": \"" + height + "\", \"spawn_x\": \"" + spawn_x + "\", \"spawn_y\": \"" + spawn_y + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"";
                
                query = "SELECT n.id_npc, n.npc_x, n.npc_y FROM Boards b, BN_Map n WHERE b.id = '" + board_id + "' AND b.id_owner = '" + id_user + "' AND n.id_board = b.id;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                
                int npc_rows_qtt = 0;
                if (query_result == "success")
                {
                    rows = json_result["rows"];
                    npc_rows_qtt = stoi(rows);
                    int rows_qtt = stoi(rows);
                    if (rows_qtt > 0)
                    {
                        json_response += ", \"data\": [";
                        
                        while (rows_qtt--)
                        {
                            string type   = "npc";
                            string id_npc = json_result["data"][rows_qtt]["id_npc"];
                            string npc_x  = json_result["data"][rows_qtt]["npc_x"];
                            string npc_y  = json_result["data"][rows_qtt]["npc_y"];
                            
                            json_response += "{\"type\": \"" + type + "\", \"id\": \"" + id_npc + "\", \"pos_x\": \"" + npc_x + "\", \"pos_y\": \"" + npc_y + "\"}";
                            
                            if (rows_qtt)
                                json_response += ",";
                        }
                    }
                }
                
                query = "SELECT t.id_terrain, t.terrain_x, t.terrain_y FROM Boards b, BT_Map t WHERE b.id = '" + board_id + "' AND b.id_owner = '" + id_user + "' AND t.id_board = b.id;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                
                int terrain_rows_qtt = 0;
                if (query_result == "success")
                {
                    rows = json_result["rows"];
                    terrain_rows_qtt = stoi(rows);
                    int rows_qtt = stoi(rows);
                    if (rows_qtt > 0)
                    {
                        if (npc_rows_qtt <= 0)
                            json_response += ", \"data\": [";
                        
                        while (rows_qtt--)
                        {
                            string type       = "npc";
                            string id_terrain = json_result["data"][rows_qtt]["id_terrain"];
                            string terrain_x  = json_result["data"][rows_qtt]["terrain_x"];
                            string terrain_y  = json_result["data"][rows_qtt]["terrain_y"];
                            
                            json_response += "{\"type\": \"" + type + "\", \"id\": \"" + id_terrain + "\", \"pos_x\": \"" + terrain_x + "\", \"pos_y\": \"" + terrain_y + "\"}";
                            
                            if (rows_qtt)
                                json_response += ",";
                        }
                    }
                }
                int data_count = npc_rows_qtt + terrain_rows_qtt;
                cout << "npc_rows_qtt " << npc_rows_qtt << endl;
                cout << "terrain_rows_qtt " << terrain_rows_qtt << endl;
                cout << "data_count " << data_count << endl;
                if (npc_rows_qtt > 0 || terrain_rows_qtt > 0)
                    json_response += "], \"data_count\":\"" + to_string(data_count) + "\"";
                 json_response += "}";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"no board that you own with the specified id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"board is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendBoard(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string board_id = json_request["board_id"];
        string query = "SELECT b.name, b.width, b.height, b.spawn_x, b.spawn_y, b.description, b.id_owner FROM Boards b WHERE b.id = '" + board_id + "';";
        json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                string board       = json_result["data"][0]["name"];
                string width       = json_result["data"][0]["width"];
                string height      = json_result["data"][0]["height"];
                string spawn_x     = json_result["data"][0]["spawn_x"];
                string spawn_y     = json_result["data"][0]["spawn_y"];
                string description = json_result["data"][0]["description"];
                json_response = "{\"status\":\"success\", \"board\": \"" + board + "\", \"board_id\": \"" + board_id + "\", \"width\": \"" + width + "\", \"height\": \"" + height + "\", \"spawn_x\": \"" + spawn_x + "\", \"spawn_y\": \"" + spawn_y + "\", \"description\": \"" + description + "\"";
                
                query = "SELECT n.id_npc, n.npc_x, n.npc_y FROM Boards b, BN_Map n WHERE b.id = '" + board_id + "' AND n.id_board = b.id;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                
                int npc_rows_qtt = 0;
                if (query_result == "success")
                {
                    rows = json_result["rows"];
                    npc_rows_qtt = stoi(rows);
                    int rows_qtt = stoi(rows);
                    if (rows_qtt > 0)
                    {
                        json_response += ", \"data\": [";
                        
                        while (rows_qtt--)
                        {
                            string type   = "npc";
                            string id_npc = json_result["data"][rows_qtt]["id_npc"];
                            string npc_x  = json_result["data"][rows_qtt]["npc_x"];
                            string npc_y  = json_result["data"][rows_qtt]["npc_y"];
                            
                            json_response += "{\"type\": \"" + type + "\", \"id\": \"" + id_npc + "\", \"pos_x\": \"" + npc_x + "\", \"pos_y\": \"" + npc_y + "\"}";
                            
                            if (rows_qtt)
                                json_response += ",";
                        }
                    }
                }
                
                query = "SELECT t.id_terrain, t.terrain_x, t.terrain_y FROM Boards b, BT_Map t WHERE b.id = '" + board_id + "' AND t.id_board = b.id;";
                json_result = data_base.fExecuteQuery(query);
                cout << query << "\nRESULT:\n" << json_result << endl;
                query_result = json_result["result"];
                
                int terrain_rows_qtt = 0;
                if (query_result == "success")
                {
                    rows = json_result["rows"];
                    terrain_rows_qtt = stoi(rows);
                    int rows_qtt = stoi(rows);
                    if (rows_qtt > 0)
                    {
                        if (npc_rows_qtt <= 0)
                            json_response += ", \"data\": [";
                        
                        while (rows_qtt--)
                        {
                            string type       = "terrain";
                            string id_terrain = json_result["data"][rows_qtt]["id_terrain"];
                            string terrain_x  = json_result["data"][rows_qtt]["terrain_x"];
                            string terrain_y  = json_result["data"][rows_qtt]["terrain_y"];
                            
                            json_response += "{\"type\": \"" + type + "\", \"id\": \"" + id_terrain + "\", \"pos_x\": \"" + terrain_x + "\", \"pos_y\": \"" + terrain_y + "\"}";
                            
                            if (rows_qtt)
                                json_response += ",";
                        }
                    }
                }
                int data_count = npc_rows_qtt + terrain_rows_qtt;
                if (npc_rows_qtt > 0 || terrain_rows_qtt > 0)
                    json_response += "], \"data_count\":\"" + to_string(data_count) + "\"";
            }
            else
                json_response = "{\"status\":\"fail\", \"message\": \"no board with the specified id\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"board is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fEditBoard(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string board_id = json_request["board_id"];
        string query = "SELECT b.name, b.width, b.height, b.description, b.id_owner FROM Boards b WHERE b.id = '" + board_id + "' AND b.id_owner = '" + id_user + "';";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string data_count = json_request["data_count"];
            int data_qtt = stoi(data_count);
            if (data_qtt > 0)
            {
                string board = json_request["name"];
                
                if (DataValidator::fValidate(board, DataValidator::SQL_INJECTION))
                {
                    query = "UPDATE Boards SET name = '" + board + "' WHERE id_owner = '" + id_user + "' AND id = '" + board_id + "';";
                    json_result = data_base.fExecuteQuery(query);
                    cout << query << "\nRESULT:\n" << json_result << endl;
                    query_result = json_result["result"];
                    
                    if (query_result == "success")
                    {
                        json_response += "{\"status\":\"success\", \"message\": \"board is updated";
                        while (data_qtt--)
                        {
                            if (json_request["type"] == "npc")
                            {
                                string id_npc = json_request["data"][data_qtt]["id"];
                                string npc_x  = json_request["data"][data_qtt]["pos_x"];
                                string npc_y  = json_request["data"][data_qtt]["pos_y"];
                                
                                if (DataValidator::fValidate(npc_x,    DataValidator::SQL_INJECTION) &&
                                    DataValidator::fValidate(npc_y,    DataValidator::SQL_INJECTION))
                                {
                                    query = "UPDATE BN_Map SET id_npc = '" + id_npc + "', npc_x = '" + npc_x + "', npc_y = '" + npc_y + "' WHERE id_board = '" + board_id + "' AND id_npc = '" + id_npc + "';";
                                    json_result = data_base.fExecuteQuery(query);
                                    cout << query << "\nRESULT:\n" << json_result << endl;
                                    query_result = json_result["result"];
                                    
                                    if (query_result == "success")
                                        json_response += ", npc with id " + id_npc + " is updated";
                                    else
                                        json_response += ", npc with id " + id_npc + " is not updated, sql query execution failed";
                                }
                                else
                                    json_response += ", invalid npc's data passed";
                            }
                            else if (json_request["type"] == "terrain")
                            {
                                string id_terrain = json_request["data"][data_qtt]["id"];
                                string terrain_x  = json_request["data"][data_qtt]["pos_x"];
                                string terrain_y  = json_request["data"][data_qtt]["pos_y"];
                                
                                if (DataValidator::fValidate(terrain_x,    DataValidator::SQL_INJECTION) &&
                                    DataValidator::fValidate(terrain_y,    DataValidator::SQL_INJECTION))
                                {
                                    query = "UPDATE BT_Map SET id_terrain = '" + id_terrain + "', terrain_x = '" + terrain_x + "', terrain_y = '" + terrain_y + "' WHERE id_board = '" + board_id + "' AND id_terrain = '" + id_terrain + "';";
                                    json_result = data_base.fExecuteQuery(query);
                                    cout << query << "\nRESULT:\n" << json_result << endl;
                                    query_result = json_result["result"];
                                    
                                    if (query_result == "success")
                                        json_response += ", terrain with id " + id_terrain + " is updated";
                                    else
                                        json_response += ", terrain with id " + id_terrain + " is not updated, sql query execution failed";
                                }
                                else
                                    json_response += ", invalid terrain's data passed";
                            }
                            else if (json_request["type"] == "spawn")
                            {
                                string spawn_x = json_request["data"][data_qtt]["pos_x"];
                                string spawn_y = json_request["data"][data_qtt]["pos_y"];
                                
                                if (DataValidator::fValidate(spawn_x,    DataValidator::SQL_INJECTION) &&
                                    DataValidator::fValidate(spawn_y,    DataValidator::SQL_INJECTION))
                                {
                                    query = "UPDATE Boards SET spawn_x = '" + spawn_x + "', spawn_y = '" + spawn_y + "' WHERE id_owner = '" + id_user + "' AND id = '" + board_id + "';";
                                    json_result = data_base.fExecuteQuery(query);
                                    cout << query << "\nRESULT:\n" << json_result << endl;
                                    query_result = json_result["result"];
                                    
                                    if (query_result == "success")
                                        json_response += ", spawn point is updated";
                                    else
                                        json_response += ", spawn point is not updated, sql query execution failed";
                                }
                                else
                                    json_response += ", invalid spawn point's data passed";
                            }
                        }
                        json_response += "\"}";
                    }
                    else
                    json_response = "{\"status\":\"success\", \"message\": \"board is not updated, sql query execution failed\"}";
                }
                else
                    json_response = "{\"status\":\"fail\", \"message\": \"board is not updated, invalid data passed\"}";
            }
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"board is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendOwnBoardsList(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name, width, height, description, id_owner FROM Boards WHERE id_owner = " + id_user + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"boards_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string board_id    = json_result["data"][rows_qtt]["id"];
                    string board       = json_result["data"][rows_qtt]["name"];
                    string width       = json_result["data"][rows_qtt]["width"];
                    string height      = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner    = json_result["data"][rows_qtt]["id_owner"];
                    json_response += "{\"board\": \"" + board + "\", \"id\": \"" + board_id + "\", \"width\": \"" + width + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
                    
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of your boards is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of your boards is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendBoardsList(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name, width, height, description, id_owner FROM Boards;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"boards_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string board_id    = json_result["data"][rows_qtt]["id"];
                    string board       = json_result["data"][rows_qtt]["name"];
                    string width       = json_result["data"][rows_qtt]["width"];
                    string height      = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner    = json_result["data"][rows_qtt]["id_owner"];
                    json_response += "{\"board\": \"" + board + "\", \"id\": \"" + board_id + "\", \"width\": \"" + width + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
                    
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of boards is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of boards is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendClasses(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name FROM Classes;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"classes_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string id   = json_result["data"][rows_qtt]["id"];
                    string name = json_result["data"][rows_qtt]["name"];
                    json_response += "{\"class\": \"" + name + "\", \"class_id\": \"" + id + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of classes is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of classes is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendRaces(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name FROM Races;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"races_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string id   = json_result["data"][rows_qtt]["id"];
                    string name = json_result["data"][rows_qtt]["name"];
                    json_response += "{\"race\": \"" + name + "\", \"race_id\": \"" + id + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of races is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of races is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendNpcTypes(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name FROM NpcTypes;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"types_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string id   = json_result["data"][rows_qtt]["id"];
                    string name = json_result["data"][rows_qtt]["name"];
                    json_response += "{\"type\": \"" + name + "\", \"type_id\": \"" + id + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of npc's types is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of npc's types is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendTerrainTypes(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string id_user;
    if (fRetrieveUserId(id_user, session_id))
    {
        string query = "SELECT id, name FROM TerrainTypes;";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        cout << query << "\nRESULT:\n" << json_result << endl;
        string query_result = json_result["result"];
        
        if (query_result == "success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"types_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string id   = json_result["data"][rows_qtt]["id"];
                    string name = json_result["data"][rows_qtt]["name"];
                    json_response += "{\"type\": \"" + name + "\", \"type_id\": \"" + id + "\"}";
                    if (rows_qtt)
                        json_response += ",";
                }
                json_response += "]}";
            }
            else
                json_response = "{\"status\":\"warning\", \"message\": \"list of terrain types is empty\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"list of terrain types is not loaded, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}
