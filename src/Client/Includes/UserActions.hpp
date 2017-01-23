#pragma once
#ifndef __USER_ACTIONS_HPP__
#define __USER_ACTIONS_HPP__

#include <iostream>
#include <regex>
#include "Includes/json.hpp"
#include "Includes/sha256.hpp"
#include "Npc.hpp"

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
    static json fCreateNpc(const std::string &session // method for NPC creation
	static std::string fLogout(std::string session); //drop user session
};
#endif // !__USER_ACTIONS_HPP__
