#include "Includes/DataValidator.hpp"



DataValidator::DataValidator()
{
}

bool DataValidator::fValidateEmail(const std::string & email)
{
	if (email.find(' ')  != std::string::npos  ||  // check for invalid symbols
		!fValidateSqlInjection(email)          ||
		email.length()   == 0)
	{
		return false;
	}

	// regex email pattern
	const std::regex  validate_email("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	if (!std::regex_match(email, validate_email))
	{
		return false;
	}

	return true;
}

bool DataValidator::fValidateName(const std::string & name)
{
	if (name.find(' ')  != std::string::npos ||  // check for invalid symbols
		!fValidateSqlInjection(name)         ||
		name.length()    < 4 )
	{
		return false;
	}
	return true;
}

bool DataValidator::fValidatePassword(const std::string & password)
{
	if (password.find(' ')  != std::string::npos ||  // check for invalid symbols
		!fValidateSqlInjection(password)         ||
		password.length()   != 64)
	{
		return false;
	}

	return true;
}

bool DataValidator::fValidateAbility(const std::string & ability)
{
	int _ability;
	try 
	{
		_ability = std::stoi(ability);
	}
	catch (std::exception & e) // if can't convert
	{
		return false;
	}

	if (_ability > 20 || _ability < 0)
		return false;

	return true;
}

bool DataValidator::fValidateAbilities(const std::string & abilities)
{
	json validate = json::parse(abilities);
	int sum = 0;

	// iterate through every ability
	for (auto & el : validate)
	{
		if (!fValidateAbility(el)) // checking every ability
			return false;
		try
		{
			std::string sum_str = el;
			sum+=std::stoi(sum_str); // adding every ability
		}
		catch (std::exception & e)
		{
			return false;
		}
	}

	return sum <= 80 && sum >= 0;
}

bool DataValidator::fValidateLength(const std::string & length)
{
	int len;
	try
	{
		len = std::stoi(length);
	}
	catch (std::exception & e)
	{
		return false;
	}

	return len >= 4 && len < 32;
}


bool DataValidator::fValidateSqlInjection(const std::string & msg)
{
	if (msg.find('\'') != std::string::npos ||
		msg.find('"')  != std::string::npos ||
		msg.find('*')  != std::string::npos)
		return false;
	return true;
}




bool DataValidator::fValidate(const std::string & to_validate, const type & t)
{
	
	switch (t)
	{
	case DataValidator::EMAIL:
		return fValidateEmail(to_validate);
		break;
	case DataValidator::NAME:
		return fValidateName(to_validate);
		break;
	case DataValidator::PASSWORD:
		return fValidatePassword(to_validate);
		break;
	case DataValidator::ABILITY:
		return fValidateAbility(to_validate);
		break;
	case DataValidator::ABILITIES:
		return fValidateAbilities(to_validate);
		break;
	case DataValidator::LENGTH:
		return fValidateLength(to_validate);
		break;
	case DataValidator::SQL_INJECTION:
		return fValidateSqlInjection(to_validate);
		break;
	default:
		return false;
	}
}

json DataValidator::fValidate(const json & to_validate)
{
	json result;
	// loop for iterating over json
	for (auto it = to_validate.begin(); it != to_validate.end(); ++it)
	{
		if (it.key() == "email")
		{
			bool res = fValidateEmail(to_validate[it.key()]);
			result[it.key()] = res ? "Validated" : "Failed";
		}
		else if (it.key() == "name"     ||
				 it.key() == "username" ||
				 it.key() == "terrain"  ||
				 it.key() == "npc"      ||
				 it.key() == "character")
		{
			bool res = fValidateName(to_validate[it.key()]);
			result[it.key()] = res ? "Validated" : "Failed";
		}
		else if (it.key() == "password")
		{
			bool res = fValidatePassword(to_validate[it.key()]);
			result[it.key()] = res ? "Validated" : "Failed";
		}
		else if (it.key() == "abilities")
		{
			bool res = fValidateAbilities(to_validate[it.key()].dump());
			result[it.key()] = res ? "Validated" : "Failed";
		}
		else if (it.key() == "width" || it.key() == "height")
		{
			bool res = fValidateLength(to_validate[it.key()]);
			result[it.key()] = res ? "Validated" : "Failed";
		}
		else if (it.key() == "description")
		{
			bool res = fValidateSqlInjection(to_validate[it.key()]);
			result[it.key()] = res ? "Validated" : "Failed";
		}
		else
		{
			result[it.key()] = "Not implemented";
		}

	}
	return result;
}

bool DataValidator::fValidate(const std::string & password, const std::string & retry_password)
{
	return password == retry_password;
}




