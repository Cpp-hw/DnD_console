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

using namespace std;

void fHandler(int signal);
void fParseRequest(std::string &path, std::map <std::string, std::string> &http_headers);
void fUserLogIn(std::string &json_response, nlohmann::json &json_request);
bool fRetrieveUserId(std::string &user_id, std::string &session_id);
void fSaveTerrain(std::string &json_response, nlohmann::json &json_request);
void fSendTerrain(std::string &json_response, nlohmann::json &json_request);
void fSendOwnTerrainsList(std::string &json_response, nlohmann::json &json_request);
void fUserRegistration(std::string &json_response, nlohmann::json &json_request);
HttpServer* pHttp_server;
DataBase data_base;


int main(int argc, char* argv[])
{
    // read data from config.ini file
    IniParser pIni_parser("config.ini");
    auto params = pIni_parser.fGetParams();

    // connect to database
    data_base.fConnection(params["database.host"], params["database.username"], params["database.password"], params["database.name"]);

    // start HTTP server with correct termination
    pHttp_server = new HttpServer(stoi(params["server.port"]), params["server.root"]);
    // pHttp_server = new HttpServer(15000, "Root/");

    pHttp_server->fGenerateResponse = &fParseRequest; // assign callback function
    signal(SIGINT, fHandler); // listen for SIGINT (aka control-c), if it comes call function named fHandler
    pHttp_server->fRun(); // server's loop waiting for user connections

    // clear memory and sclose socets (in destructors, etc)
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
    string response = "";
    if (path.find("/api/")) // if it is not found or if it is not in the beginning
        response = "{\"error\": \"incorrect api call\"}";
    else
    {
        string request_data_content = http_headers[(string)"Content"];
        if (!request_data_content.length())
        {
            response = "{\"error\": \"request data content is empty\"}";
        }
        else
        {
            nlohmann::json json_request = json::parse(request_data_content);
            if (path.find("/api/userlogin") != string::npos)
                fUserLogIn(response, json_request);
            else if (path.find("/api/userregister") != string::npos)
                fUserRegistration(response, json_request);
            else if (path.find("/api/addterrain") != string::npos)
                fSaveTerrain(response, json_request);
            else if (path.find("/api/loadterrain") != string::npos)
                fSendTerrain(response, json_request);
            else if (path.find("/api/loadmyterrainslist") != string::npos)
                fSendOwnTerrainsList(response, json_request);
            /*else if (path.find("/api/addnpc") != string::npos)
                f_____(response, json_request);*/
            else
                response = "{\"error\": \"script is not implemented\"}";
        }
    }
    pHttp_server->fSetResponse(response.data(), response.length(), "JSON");
}

/**
* parses request string
*/
void fUserLogIn(std::string &json_response, nlohmann::json &json_request)
{
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
                    query = "INSERT INTO Sessions (user_id) VALUES (" + user_id + ");";
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
        if (query_result == "Success")
            json_response = "{\"status\":\"success\", \"message\": \"registered\"}";
        else
            json_response = "{\"status\":\"fail\", \"message\": \"database error\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"invalid data\"}";
}

bool fRetrieveUserId(std::string &user_id, std::string &session_id)
{
    string query = "SELECT user_id FROM Sessions WHERE id = " + session_id;
    nlohmann::json json_result = data_base.fExecuteQuery(query);
    string query_result = json_result["result"];
    if (query_result == "Success")
    {
        string rows = json_result["rows"];
        if (stoi(rows) > 0)
        {
            string user_id = json_result["data"][0]["user_id"];
            return 1;
        }
    }
    return 0;
}


void fSaveTerrain(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string user_id;
    if (fRetrieveUserId(user_id, session_id))
    {
        string name = json_request["terrain"];
        string width = json_request["width"];
        string height = json_request["height"];
        string description = json_request["description"];
        string query = "INSERT INTO Terrain (name, width, height, description, id_owner) VALUES (" + name + ", " + width + ", " + height + ", " + description + ", " + user_id + ");";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        string query_result = json_result["result"];

        if (query_result == "Success")
        {
            query = "SELECT LAST_INSERT_ID() AS id FROM Terrain";
            json_result = data_base.fExecuteQuery(query);
            string terrain_id = json_result["data"][0]["id"];
            json_response = "{\"status\":\"success\", \"terrain_id\": \"" + terrain_id + "\"}";
        }
        else
            json_response = "{\"status\":\"fail\", \"message\": \"terrain is not added, sql query execution failed\"}";
    }
    else
        json_response = "{\"status\":\"fail\", \"message\": \"you are not logged in\"}";
}

void fSendTerrain(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string user_id;
    if (fRetrieveUserId(user_id, session_id))
    {
        string terrain_id = json_request["terrain_id"];
        string query = "SELECT name, width, height, description, id_owner FROM Terrain WHERE user_id=" + user_id + " AND id = " + terrain_id + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        string query_result = json_result["result"];

        if (query_result == "Success")
        {
            string rows = json_result["rows"];
            if (stoi(rows) > 0)
            {
                json_result = data_base.fExecuteQuery(query);
                string terrain = json_result["data"][0]["name"];
                string width = json_result["data"][0]["width"];
                string height = json_result["data"][0]["height"];
                string description = json_result["data"][0]["description"];
                string id_owner = json_result["data"][0]["id_owner"];
                json_response = "{\"status\":\"success\", \"terrain\": \"" + terrain + "\", \"terrain_id\": \"" + terrain_id + "\", \"width\": \"" + width + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
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

void fSendOwnTerrainsList(std::string &json_response, nlohmann::json &json_request)
{
    string session_id = json_request["session_id"];
    string user_id;
    if (fRetrieveUserId(user_id, session_id))
    {
        string query = "SELECT id AS terrain_id, name, width, height, description, id_owner FROM Terrain WHERE user_id=" + user_id + ";";
        nlohmann::json json_result = data_base.fExecuteQuery(query);
        string query_result = json_result["result"];

        if (query_result == "Success")
        {
            string rows = json_result["rows"];
            int rows_qtt = stoi(rows);
            if (rows_qtt > 0)
            {
                json_response = "{\"status\":\"success\", \"terrains_quantity\":\"" + rows + "\", \"list\": [";
                while (rows_qtt--)
                {
                    string terrain_id = json_result["data"][rows_qtt]["terrain_id"];
                    string terrain = json_result["data"][rows_qtt]["name"];
                    string width = json_result["data"][rows_qtt]["width"];
                    string height = json_result["data"][rows_qtt]["height"];
                    string description = json_result["data"][rows_qtt]["description"];
                    string id_owner = json_result["data"][rows_qtt]["id_owner"];
                    json_response += "{\"terrain\": \"" + terrain + "\", \"terrain_id\": \"" + terrain_id + "\", \"width\": \"" + width + "\", \"height\": \"" + height + "\", \"description\": \"" + description + "\", \"id_owner\": \"" + id_owner + "\"}";
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