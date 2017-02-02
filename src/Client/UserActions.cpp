#include "UserActions.hpp"


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
	std::getline(std::cin, user_login);                  // get user login
	while (user_login.find(' ') != std::string::npos ||  // check for invalid symbols
		user_login.find('\'') != std::string::npos ||
		user_login.find('"') != std::string::npos ||
		user_login.length() == 0)
	{
		std::cout << "Invalid login! Empty spaces, ' or \" finded!\nEnter login: ";
		std::getline(std::cin, user_login);
	}

	std::cout << "Enter password: ";
	std::getline(std::cin, user_pass);   // get user pass
	while (user_pass.length() < 6)       // check for password length
	{
		std::cout << "Password length less than 6 symbols!\nEnter password: ";
		std::getline(std::cin, user_pass);
	}

	user_pass = sha256(user_pass);       // hash user pass

	user_login_data["action"] = "login";
	user_login_data["username"] = user_login; // insert into json key Login with value of user login input
	user_login_data["password"] = user_pass;  // insert into json key Password with value of user hash pass
	return user_login_data;
}

json UserActions::fRegistration()
{
	json user_registration_data;  // json registration resualt

	std::string       user_username;      //  user input login  
	std::string       user_password;      //  user input password
	std::string       user_email;         //  user input e-mail;
	std::string       repeat_password;
	const std::regex  validate_email("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+"); // email regex pattern
	SHA256            sha256;			  // sha256 obj

	std::cout << "Enter username: (Username field must not be empty)" << std::endl;;
	std::getline(std::cin, user_username);                   // get user username
	while (user_username.find(' ') != std::string::npos ||  // check for invalid symbols
		user_username.find('\'') != std::string::npos ||
		user_username.find('"') != std::string::npos ||
		user_username.length() == 0)
	{
		std::cout << "Invalid username! Empty spaces, ' or \" finded! Enter username: \n";
		std::getline(std::cin, user_username);
	}


	std::cout << "Enter email: (field may be empty)" << std::endl;;
	std::getline(std::cin, user_email);                   // get user email
	while (user_email.find(' ') != std::string::npos ||  // check for invalid symbols
		user_email.find('\'') != std::string::npos ||
		user_email.find('"') != std::string::npos)
	{
		std::cout << "Invalid email! Empty spaces, ' or \" finded! Enter email: \n";
		std::getline(std::cin, user_email);
	}

	while (!std::regex_match(user_email, validate_email))
	{
		std::cout << "Invalid email address! Enter email: \n";
		std::getline(std::cin, user_email);
	}



	std::cout << "Enter password: (Password must be longer than 6 characters and not be empty)" << std::endl;
	std::getline(std::cin, user_password);       // get user password
	while (user_password.length() < 6)           // check for password length
	{
		std::cout << "Password length less than 6 symbols! Enter password: \n";
		std::getline(std::cin, user_password);
	}

	std::cout << "Please enter password again: " << std::endl;
	std::getline(std::cin, repeat_password);

	user_password = sha256(user_password);       // hash user pass
	repeat_password = sha256(repeat_password);

	while (user_password != repeat_password)     // check user passwords
	{
		std::cout << "Password does not match! Retype your password: \n";
		std::getline(std::cin, user_password);
		while (user_password.length() < 6)       // check for password length
		{
			std::cout << "Password length less than 6 symbols! Enter password: \n";
			std::getline(std::cin, user_password);
		}
		std::cout << "Please enter password again: " << std::endl;
		std::getline(std::cin, repeat_password);

		user_password = sha256(user_password);
		repeat_password = sha256(repeat_password);

	}

	user_registration_data["action"] = "registration";
	user_registration_data["email"] = user_email;
	user_registration_data["username"] = user_username;
	user_registration_data["password"] = user_password;

	return user_registration_data;

}

/*
	Method for NPC creation
 */
json UserActions::fCreateNpc(const std::string &session)
{
    Npc npc;
    
    npc.fSetOwner(session);
    npc.fAddNpc();
    
    cout << "Your NPC: " << endl;
    npc.fShowNpc();
    
    return npc.fToJson();
}

json UserActions::fCreateTerrain(const std::string &session)
{
	std::string terrain_name;
	std::string width;
	std::string height;
	std::string description;

	Terrain terrain;

	terrain.SetOwner(session);

	do
	{
		std::cout << "Enter terrain name:" << std::endl;
		std::getline(std::cin, terrain_name);

		std::cin.ignore();


		if (!DataValidator::fValidate(terrain_name, DataValidator::SQL_INJECTION))
		{
			std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
		}
		else
		{
			terrain.SetName(terrain_name);
		}

	} while (!DataValidator::fValidate(terrain_name, DataValidator::NAME));

	do
	{
		std::cout << "Enter terrain width:" << std::endl;
		std::getline(std::cin, width);

		if (!DataValidator::fValidate(width, DataValidator::LENGTH))
		{
			std::cout << "You input value less then 4 or more the 32 !!!" << std::endl;
		}
		else
		{
			terrain.SetWidth(width);
		}

	} while (!DataValidator::fValidate(width, DataValidator::LENGTH));

	do
	{
		std::cout << "Enter terrain height:" << std::endl;
		std::getline(std::cin, height);

		if (!DataValidator::fValidate(height, DataValidator::LENGTH))
		{
			std::cout << "You input value less then 4 or more the 32 !!!" << std::endl;
		}
		else
		{
			terrain.SetHeight(height);
		}

	} while (!DataValidator::fValidate(height, DataValidator::LENGTH));

	char desc;
	std::cout << "You want input terrain description (yes = y, no = n)" << std::endl;
	std::cin >> desc;
	
	std::cin.ignore();

	if (desc=='y')
	{
		do
		{
			std::cout << "Enter terrain description: "<<std::endl;
			std::getline(std::cin, description);

			if (!DataValidator::fValidate(description, DataValidator::SQL_INJECTION))
				std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
			else
				terrain.SetDescription(description);
		} while (!DataValidator::fValidate(description, DataValidator::SQL_INJECTION));
	}
	else
	{
		return terrain.fTerrain_To_Json();
	}


	return terrain.fTerrain_To_Json();
}

json UserActions::fShowFullListOfTerrains(const std::string &session)
{
	std::string type, count;

	json request;
	request["session_id"] = session;
	std::cin.ignore();

	do
	{
	std::cout << "Enter type of terrain which you want load: " << std::endl;
	std::getline(std::cin, type);


	if (!DataValidator::fValidate(type, DataValidator::SQL_INJECTION))
		std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
	else
		request["type"] = type;

	} while (!DataValidator::fValidate(type, DataValidator::SQL_INJECTION));

	do
	{
	std::cout << "Enter count of terrain which you want load: " << std::endl;
	std::getline(std::cin, count);
	if (!DataValidator::fValidate(type, DataValidator::SQL_INJECTION))
		std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
	else
		request["count"] = count;

	} while (!DataValidator::fValidate(type, DataValidator::SQL_INJECTION));
	

	return request;
}

json UserActions::fLoadMyTerrains(const std::string &session)
{
	json request;
	request["session_id"] = session;
	return request;
}

json UserActions::fLoadTerrain(const std::string &session)
{
	json request;
	std::string terrain_id;
	request["session_id"] = session;
	do
	{
		std::cout << "Enter terrain id:\n";
		std::getline(std::cin, terrain_id);
		try
		{
			std::stoi(terrain_id);
		}
		catch (const std::exception&)
		{
			terrain_id = "*";
		}
	} while (!DataValidator::fValidate(terrain_id, DataValidator::SQL_INJECTION));

	request["terrain_id"] = terrain_id;

	return request;
}

/*
	Method clear session string
*/
std::string UserActions::fLogout(std::string &session)
{
	session.clear();
	return session;
}
