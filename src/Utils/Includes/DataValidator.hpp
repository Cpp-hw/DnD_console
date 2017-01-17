#pragma once
#ifndef _DATA_VALIDATOR_HPP_
#define _DATA_VALIDATOR_HPP_

#include <string>
#include <regex>
#include "json.hpp"

using json = nlohmann::json;

class DataValidator
{
private:
	DataValidator();

	// Method for email validation
	static bool fValidateEmail(const std::string & email);

	// Method for name/username validation
	static bool fValidateName(const std::string & name);

	// Method for password validation
	static bool fValidatePassword(const std::string & password);

	// Method for ability validation
	static bool fValidateAbility(const std::string & ability);

	// Method for abilities validation
	static bool fValidateAbilities(const std::string & abilities);

	// Method for terrain width/height validation
	static bool fValidateLength(const std::string & length);

	// Method for checking data on SQL Injection
	static bool fValidateSqlInjection(const std::string & msg);

public:
	// Available validation types
	enum type {
		EMAIL,
		NAME,
		PASSWORD,
		ABILITY,
		ABILITIES,
		LENGTH,
		SQL_INJECTION
	};

	// Main validation method for checking single data
	static bool fValidate(const std::string & to_validate, const type & t);

	// Main validation method for checking json data
	// returns json with same keys but values will be ["Validated", "Failed", "Not implemented"]
	//
	static json fValidate(const json & to_validate);

	// Validation method for comparing passwords
	static bool fValidate(const std::string & password, const std::string & retry_password);

};

#endif // !_DATA_VALIDATOR_HPP_

