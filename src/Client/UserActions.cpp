#include "Includes/UserActions.hpp"


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
		std::cout << "Invalid login! Empty spaces, ' or \" found!\nEnter login: ";
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
		std::cout << "Invalid username! Empty spaces, ' or \" found! Enter username: \n";
		std::getline(std::cin, user_username);
	}


	std::cout << "Enter email: (field may be empty)" << std::endl;;
	std::getline(std::cin, user_email);                   // get user email
	while (user_email.find(' ') != std::string::npos ||  // check for invalid symbols
		user_email.find('\'') != std::string::npos ||
		user_email.find('"') != std::string::npos)
	{
		std::cout << "Invalid email! Empty spaces, ' or \" found! Enter email: \n";
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

/*
 Method for loading the list of NPCs of mine
 */
json UserActions::fLoadMyNpcs(const std::string &session)
{
    json request;
    request["session_id"] = session;
    return request;
}

/*
 Method for loading one my NPC by its id
 */
json UserActions::fLoadNpc(const std::string &session)
{
    json request;
    request["session_id"] = session;
    
    std::string npc_id;
    do
    {
        std::cout << "Input the NPC's id:\n";
        std::getline(std::cin, npc_id);
        try
        {
            std::stoi(npc_id);
        }
        catch (const std::exception&)
        {
            npc_id = "*";
        }
    } while (!DataValidator::fValidate(npc_id, DataValidator::SQL_INJECTION));
    
    request["npc_id"] = npc_id;
    
    return request;
}

/*
 Method for editting one NPC of mine
 */
json UserActions::fEditNpc(json &json_npc)
{
    cout << "********** NPC **********" << endl;
    Npc npc(json_npc);
    npc.fShowNpc();
    
    string npc_id = json_npc["id"];
    string strength = json_npc["strength"];
    string dexterity = json_npc["dexterity"];
    string constitution = json_npc["constitution"];
    string intelligence = json_npc["intelligence"];
    string wisdom = json_npc["wisdom"];
    string charisma = json_npc["charisma"];
    int sum = stoi(strength) + stoi(dexterity) + stoi(constitution) + stoi(intelligence) + stoi(wisdom) + stoi(charisma);
    const int MAX_SUM = 80;
    
    enum npc_fields
    {
        NAME = 1,
        TYPE = 2,
        LEVEL = 3,
        STRENGTH = 4,
        DEXTERITY = 5,
        CONSTITUTION = 6,
        INTELLIGENCE = 7,
        WISDOM = 7,
        CHARISMA = 9
    };
    
    bool exit = false;
    do{
        system("cls");
        string choice;
        cout << "Choose some data you want to edit (to exit press \"0\":" << std::endl;
        cout << "1 - name;" << endl;
        cout << "2 - type;" << endl;
        cout << "3 - level;" << endl;
        cout << "4 - strength;" << endl;
        cout << "5 - dexterity;" << endl;
        cout << "6 - constitution;" << endl;
        cout << "7 - intelligence;" << endl;
        cout << "8 - wisdom;" << endl;
        cout << "9 - charisma." << endl;
        cout << "0 - back to previous menu" << std::endl;
        getline(cin, choice);
        
        switch (stoi(choice))
        {
            case NAME:
            {
                cout << "The old data for name: " << npc.fGetName() << endl;
                string name;
                do
                {
                    cout << "Input a name: ";
                    getline(cin, name);
                    
                    if (!DataValidator::fValidate(name, DataValidator::SQL_INJECTION)) // name validation
                        cout << "This datum should consist of letters!\n";
                    else
                        npc.fSetName(name);
                } while (!DataValidator::fValidate(name, DataValidator::SQL_INJECTION));
            }
                break;
            case TYPE:
            {
                cout << "The old data for type: " << npc.fGetType() << endl;
                string type_npc;
                do
                {
                    cout << "Input the NPC's type: ";
                    getline(cin, type_npc);
                    
                    if (!DataValidator::fValidate(type_npc, DataValidator::SQL_INJECTION)) // type_NPC validation
                        cout << "This datum should consist of letters!" << endl;
                    else
                        npc.fSetType(type_npc);
                } while (!DataValidator::fValidate(type_npc, DataValidator::SQL_INJECTION));
            }
                break;
            case LEVEL:
            {
                cout << "The old data for level: " << npc.fGetLevel() << endl;
                string level;
                do
                {
                    cout << "Input the level: ";
                    getline(cin, level);
                    
                    if (stoi(level) > 0) // level validation
                        npc.fSetLevel(level);
                } while (stoi(level) <= 0);
            }
                break;
            case STRENGTH:
            {
                cout << "The old data for strength: " << npc.fGetStrength() << endl;
                sum -= stoi(strength);
                cout << fShowMaxValue(sum) << endl;
                
                do
                {
                    cout << "Input the strength-value: ";
                    getline(cin, strength);
                    
                    if (!DataValidator::fValidate(strength, DataValidator::ABILITY))
                        cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
                    else if ((sum + stoi(strength)) > MAX_SUM)
                        cout << "The sum of all vability-values should be less than (or equal to) 80!" << endl;
                    else
                    {
                        npc.fSetStrength(strength);
                        sum += stoi(strength);
                    }
                } while (!DataValidator::fValidate(strength, DataValidator::ABILITY) && sum <= MAX_SUM);
            }
                break;
            case DEXTERITY:
            {
                cout << "The old data for dexterity: " << npc.fGetDexterity() << endl;
                sum -= stoi(dexterity);
                cout << fShowMaxValue(sum) << endl;
                
                do
                {
                    cout << "Input the dexterity-value: ";
                    getline(cin, dexterity);
                    
                    if (!DataValidator::fValidate(dexterity, DataValidator::ABILITY))
                        cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
                    else if ((sum + stoi(dexterity)) > MAX_SUM)
                        cout << "The sum of all vability-values should be less than (or equal to) 80!" << endl;
                    else
                    {
                        npc.fSetDexterity(dexterity);
                        sum += stoi(dexterity);
                    }
                } while (!DataValidator::fValidate(dexterity, DataValidator::ABILITY) && sum <= MAX_SUM);
            }
                break;
            case CONSTITUTION:
            {
                cout << "The old data for constitution: " << npc.fGetConstitution() << endl;
                sum -= stoi(constitution);
                cout << fShowMaxValue(sum) << endl;
                
                do
                {
                    cout << "Input the constitution-value: ";
                    getline(cin, constitution);
                    
                    if (!DataValidator::fValidate(constitution, DataValidator::ABILITY))
                        cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
                    else if ((sum + stoi(constitution)) > MAX_SUM)
                        cout << "The sum of all vability-values should be less than (or equal to) 80!" << endl;
                    else
                    {
                        npc.fSetConstitution(constitution);
                        sum += stoi(constitution);
                    }
                } while (!DataValidator::fValidate(constitution, DataValidator::ABILITY) && sum <= MAX_SUM);
            }
                break;
            case INTELLIGENCE:
            {
                cout << "The old data for intelligence: " << npc.fGetIntelligence() << endl;
                sum -= stoi(intelligence);
                cout << fShowMaxValue(sum) << endl;
                
                do
                {
                    cout << "Input the intelligence-value: ";
                    getline(cin, intelligence);
                    
                    if (!DataValidator::fValidate(intelligence, DataValidator::ABILITY))
                        cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
                    else if ((sum + stoi(intelligence)) > MAX_SUM)
                        cout << "The sum of all vability-values should be less than (or equal to) 80!" << endl;
                    else
                    {
                        npc.fSetIntelligence(intelligence);
                        sum += stoi(intelligence);
                    }
                } while (!DataValidator::fValidate(intelligence, DataValidator::ABILITY) && sum <= MAX_SUM);
            }
                break;
            case WISDOM:
            {
                cout << "The old data for wisdom: " << npc.fGetWisdom() << endl;
                sum -= stoi(wisdom);
                cout << fShowMaxValue(sum) << endl;
                
                do
                {
                    cout << "Input the wisdom-value: ";
                    getline(cin, wisdom);
                    
                    if (!DataValidator::fValidate(wisdom, DataValidator::ABILITY))
                        cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
                    else if ((sum + stoi(wisdom)) > MAX_SUM)
                        cout << "The sum of all vability-values should be less than (or equal to) 80!" << endl;
                    else
                    {
                        npc.fSetWisdom(wisdom);
                        sum += stoi(wisdom);
                    }
                } while (!DataValidator::fValidate(wisdom, DataValidator::ABILITY) && sum <= MAX_SUM);
            }
                break;
            case CHARISMA:
            {
                cout << "The old data for charisma: " << npc.fGetCharisma() << endl;
                sum -= stoi(charisma);
                cout << fShowMaxValue(sum) << endl;
                
                do
                {
                    cout << "This ability is the last." << endl;
                    cout << "Input the charisma-value: ";
                    getline(cin, charisma);
                    
                    if (!DataValidator::fValidate(charisma, DataValidator::ABILITY))
                        cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
                    else if ((sum + stoi(charisma)) > MAX_SUM)
                        cout << "The sum of all vability-values should be less than (or equal to) 80!" << endl;
                    else
                    {
                        npc.fSetCharisma(charisma);
                        sum += stoi(charisma);
                    }
                } while (!DataValidator::fValidate(charisma, DataValidator::ABILITY) && sum <= MAX_SUM);
            }
                break;
            case 0:
                system("cls");
                exit = true;
                break;
            default:
                cout << "\nUnexpected operation..." << endl;
        }
    } while (!exit);
    
    json request;
    request = npc.fToJson();
    request["npc_id"] = npc_id;
    
    return request;
}

/*
 Method for deleting one NPC of mine
 */
json UserActions::fDeleteNpc(const std::string &session)
{
    json request;
    request["session_id"] = session;
    
    std::string npc_id;
    do
    {
        std::cout << "Input the NPC's id:\n";
        std::getline(std::cin, npc_id);
        try
        {
            std::stoi(npc_id);
        }
        catch (const std::exception&)
        {
            npc_id = "*";
        }
    } while (!DataValidator::fValidate(npc_id, DataValidator::SQL_INJECTION));
    
    request["npc_id"] = npc_id;
    
    return request;
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
		cin.clear();
		cin.ignore();
		cout << "Input terrain name: ";
		getline(cin, terrain_name);

		if (!DataValidator::fValidate(terrain_name, DataValidator::SQL_INJECTION)) // name validation
			cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
		else
			terrain.SetName(terrain_name);
	} while (!DataValidator::fValidate(terrain_name, DataValidator::SQL_INJECTION));


	do
	{
		std::cout << "Enter terrain width:" << std::endl;
		std::getline(std::cin, width);

		if (!DataValidator::fValidate(width, DataValidator::LENGTH))
		{
			std::cout << "You input value less then 1 or more the 32 !!!" << std::endl;
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
			std::cout << "You input value less then 1 or more the 32 !!!" << std::endl;
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

json UserActions::fCreateCharacter(const std::string &session)
{

	Character character;

	character.fSetOwner(session);
	character.fAddCharacter();
	return character.fToJson();
}

json UserActions::fLoadCharacterByName(const std::string &session)
{
	json request;
	std::string character_name;

	request["session_id"] = session;
	do
	{
		std::cout << "Enter terrain name:" << std::endl;
		std::getline(std::cin, character_name);
		
	} while (!DataValidator::fValidate(character_name, DataValidator::SQL_INJECTION));

	request["character"] = character_name;

	return request;
}
json UserActions::fLoadMyCharacters(const std::string &session)
{
	json request;
	request["session_id"] = session;
	return request;
}

std::string UserActions::fLogout(std::string &session)
{
	session.clear();
	return session;
}
