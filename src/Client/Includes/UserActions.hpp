#pragma once
#ifndef __USER_ACTIONS_HPP__
#define __USER_ACTIONS_HPP__

#include <iostream>
#include <regex>
#include "json.hpp"
#include "sha256.hpp"
#include "DataValidator.hpp"
#include "NPC.hpp"
#include "Terrain.hpp"
#include "Character.hpp"

using json = nlohmann::json;

class UserActions
{
private:
	UserActions();
public:
	// Login method returns json result of user input Login and Password
	// { "Login": "username", "Password": "SHA256_hashed_password" }
	static json fLogin();
	static json fRegistration();
	static json fCreateNpc(const std::string &session); // method for NPC creation
    static json fLoadMyNpcs(const std::string &session); // loads full list of my NPCs
    static json fLoadNpc(const std::string &session); // loads one NPC by its id
    static json fEditNpc(json &json_npc); // edits one NPC according to its id
    static json fDeleteNpc(const std::string &session); // delete NPC by id
	static json fCreateTerrain(const std::string &session); //method for Terrain creation
	static json fShowFullListOfTerrains(const std::string &session);
	static json fLoadMyTerrains(const std::string &session);  //load user terrains
	static json fLoadTerrain(const std::string &session);     //load switched terrain
	static json fCreateCharacter(const std::string &session); //method for character creation
	static json fLoadCharacterByName(const std::string &session); //method for load user character
	static json fLoadMyCharacters(const std::string &session); //method for load user characters
	static std::string fLogout(std::string &session); //drop user session
};
#endif // !__USER_ACTIONS_HPP__
