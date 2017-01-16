#include "Includes\UserActions.hpp"

UserActions::UserActions()
{
}

json UserActions::fLogin()
{
	json        user_login_data;  // json login result
	std::string user_login;		  // user login input
	std::string user_pass;		  // user pass input
	SHA256      sha256;			  // sha256 obj

	std::cout << "Enter login: ";
	std::getline(std::cin, user_login);  // get user login
	std::cout << "Enter password: ";
	std::getline(std::cin, user_pass);   // get user pass
    user_pass = sha256(user_pass);       // hash user pass

	user_login_data["Login"]    = user_login; // insert into json key Login with value of user login input
	user_login_data["Password"] = user_pass;  // insert into json key Password with value of user hash pass

	return user_login_data;
}
