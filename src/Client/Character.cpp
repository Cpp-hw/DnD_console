#include "Includes\Character.hpp"



Character::Character(){}

Character::Character(std::string &name,
	std::string &race,
	std::string &_class,
	std::string &experience,
	std::string &hitpoints,
	std::string &level,
	std::string &strength,
	std::string &dexterity,
	std::string &constitution,
	std::string &intelligence,
	std::string &wisdom,
	std::string &charisma,
	std::string &owner)
{
	_name = name;
	_race = race;
	this->_class = _class;
	_experience = experience;
	_hitpoints = hitpoints;
	_level = level;
	_strength = strength;
	_strength_mod = fSetAbilityMod(strength);
	_dexterity = dexterity;
	_dexterity_mod = fSetAbilityMod(_dexterity);
	_constitution = constitution;
	_constitution_mod = fSetAbilityMod(constitution);
	_intelligence = intelligence;
	_intelligence_mod = fSetAbilityMod(intelligence);
	_wisdom = wisdom;
	_wisdom_mod = fSetAbilityMod(wisdom);
	_charisma = charisma;
	_charisma_mod = fSetAbilityMod(charisma);
	_owner = owner;
}

Character::Character(json &jsonData)
{
	_owner = jsonData["session_id"];
	_name=jsonData["character"];
	_race=jsonData["race"];
	_class=jsonData["class"];
	_experience = jsonData["experience"];
	_level = jsonData["level"];
	_hitpoints=jsonData["hitpoints"];
	_strength=jsonData["strength"];
	_strength_mod = jsonData["str_mod"];
	_dexterity = jsonData["dexterity"];
	_dexterity_mod = jsonData["dex_mod"];
	_constitution = jsonData["constitution"];
	_constitution_mod = jsonData["con_mod"];
	_intelligence = jsonData["intelligence"];
	_intelligence_mod = jsonData["int_mod"];
	_wisdom = jsonData["wisdom"];
	_wisdom_mod=jsonData["wis_mod"];
	_charisma=jsonData["charisma"];
	_charisma_mod = jsonData["cha_mod"];
}

void Character::fSetName(const std::string &name)
{
	this->_name = name;
}

void Character::fSetClass(const std::string &class_)
{
	this->_class = class_;
}

void Character::fSetRace(const std::string &race)
{
	this->_race = race;
}

void Character::fSetLevel(const std::string &level)
{
	_level = level;
}

void Character::fSetHitpoints(const std::string &hitpoints)
{
	_hitpoints = hitpoints;
}

void  Character::fSetStrength(const std::string &strength)
{
	_strength = strength;
	_strength_mod = fSetAbilityMod(strength);
}

void Character::fSetDexterity(const std::string &dexterity)
{
	_dexterity = dexterity;
	_dexterity_mod = fSetAbilityMod(dexterity);
}

void Character::fSetConstitution(const std::string &constitution)
{
	_constitution = constitution;
	_constitution_mod = fSetAbilityMod(constitution);
}

void Character::fSetIntelligence(const std::string &intelligence)
{
	_intelligence = intelligence;
	_intelligence_mod = fSetAbilityMod(intelligence);
}

void Character::fSetWisdom(const std::string &wisdom)
{
	_wisdom = wisdom;
	_wisdom_mod = fSetAbilityMod(wisdom);
}

void Character::fSetCharisma(const std::string &charisma)
{
	_charisma = charisma;
	_charisma_mod = fSetAbilityMod(charisma);
}

std::string Character::fGetName()
{
	return _name;
}

std::string Character::fGetClass()
{
	return _class;
}

std::string Character::fGetRace()
{
	return _race;
}

std::string Character::fGetLevel()
{
	return _level;
}

std::string Character::fGetHitpoints()
{
	return _hitpoints;
}

std::string Character::fGetStrength()
{
	return _strength;
}

std::string Character::fGetDexterity()
{
	return _dexterity;
}

std::string Character::fGetConstitution()
{
	return _constitution;
}

std::string Character::fGetIntelligence()
{
	return _intelligence;
}

std::string Character::fGetWisdom()
{
	return _wisdom;
}

std::string Character::fGetCharisma()
{
	return _charisma;
}

std::string Character::fSetAbilityMod(std::string ability)
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

void Character::fSetOwner(const std::string &owner)
{
	this->_owner = owner;
}

void Character::fRandomizeAbilities()
{
	this->_level = "1";
	this->_hitpoints = "100";
	this->_experience = "0";

	int sum;
	do
	{
		sum = 0;

		std::string strength;
		do
		{
			strength = std::to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));

			if (DataValidator::fValidate(strength, DataValidator::ABILITY))
			{
				_strength = strength;
				_strength_mod = fSetAbilityMod(strength);
			}
		} while (!DataValidator::fValidate(strength, DataValidator::ABILITY));
		sum += stoi(strength);

		std::string dexterity;
		do
		{
			dexterity = std::to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));

			if (DataValidator::fValidate(dexterity, DataValidator::ABILITY))
			{
				_dexterity = dexterity;
				_dexterity_mod = fSetAbilityMod(dexterity);
			}
		} while (!DataValidator::fValidate(dexterity, DataValidator::ABILITY));
		sum += stoi(dexterity);

		std::string constitution;
		do
		{
			constitution = std::to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));

			if (DataValidator::fValidate(constitution, DataValidator::ABILITY))
			{
				_constitution = constitution;
				_constitution_mod = fSetAbilityMod(constitution);
			}
		} while (!DataValidator::fValidate(constitution, DataValidator::ABILITY));
		sum += stoi(constitution);

		std::string intelligence;
		do
		{
			intelligence = std::to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));

			if (DataValidator::fValidate(intelligence, DataValidator::ABILITY))
			{
				_intelligence = intelligence;
				_intelligence_mod = fSetAbilityMod(intelligence);
			}
		} while (!DataValidator::fValidate(intelligence, DataValidator::ABILITY));
		sum += stoi(intelligence);

		std::string wisdom;
		do
		{
			wisdom = std::to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));

			if (DataValidator::fValidate(wisdom, DataValidator::ABILITY))
			{
				_wisdom = wisdom;
				_wisdom_mod = fSetAbilityMod(wisdom);
			}
		} while (!DataValidator::fValidate(wisdom, DataValidator::ABILITY));
		sum += stoi(wisdom);

		std::string charisma;
		do
		{
			charisma = std::to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));

			if (DataValidator::fValidate(charisma, DataValidator::ABILITY))
			{
				_charisma = charisma;
				_charisma_mod = fSetAbilityMod(charisma);
			}
		} while (!DataValidator::fValidate(charisma, DataValidator::ABILITY));
		sum += stoi(charisma);
	} while (sum > MAX_ABILITIES_SUM_);
}

void Character::fAddCharacter()
{
	std::cout << "********** Character **********" << std::endl;

	std::string name;
	do
	{
		std::cin.clear();
		std::cin.ignore();
		std::cout << "Enter character name: ";
		std::getline(std::cin, name);

		if (!DataValidator::fValidate(name, DataValidator::SQL_INJECTION)) // name validation
			std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
		else
			_name = name;
	} while (!DataValidator::fValidate(name, DataValidator::SQL_INJECTION));

	std::string race;
	do
	{
		std::cout << "Enter the character race: ";
		std::getline(std::cin, race);

		if (!DataValidator::fValidate(race, DataValidator::SQL_INJECTION)) // race validation
			std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
		else
			_race = race;
	} while (!DataValidator::fValidate(race, DataValidator::SQL_INJECTION));

	std::string class_;

	do
	{
		std::cout << "Enter the character class: ";
		std::getline(std::cin, class_);

		if (!DataValidator::fValidate(class_, DataValidator::SQL_INJECTION)) // race validation
			std::cout << "You have entered invalid data, this value is prohibited signs !!!" << std::endl;
		else
			_class = class_;
	} while (!DataValidator::fValidate(class_, DataValidator::SQL_INJECTION));



	int choice;
	std::cout << "Choose the way to fill the Character abilities:" << std::endl;
	std::cout << "1. by rolling dices;" << std::endl;
	std::cout << "2. manually" << std::endl;
	std::cout << "Your choice: ";
	std::cin >> choice;
	std::cin.clear();
	std::cin.ignore();

	switch (choice)
	{
	case 1:
		fRandomizeAbilities();
		break;
	case 2:
		fSetAbilities();
		break;
	default:
		std::cout << "Unexpected operation." << std::endl;
		break;
	}
}


json Character::fToJson()
{
	json character;

	character["session_id"] = _owner;
	character["character"] = _name;
	character["race"] = _race;
	character["class"] = _class;
	character["experience"] = _experience;
	character["level"] = _level;
	character["hitpoints"] = _hitpoints;
	character["strength"] = _strength;
	character["str_mod"] = _strength_mod;
	character["dexterity"] = _dexterity;
	character["dex_mod"] = _dexterity_mod;
	character["constitution"] = _constitution;
	character["con_mod"] = _constitution_mod;
	character["intelligence"] = _intelligence;
	character["int_mod"] = _intelligence_mod;
	character["wisdom"] = _wisdom;
	character["wis_mod"] = _wisdom_mod;
	character["charisma"] = _charisma;
	character["cha_mod"] = _charisma_mod;
	
	return character;
}

void Character::fSetAbilities()
{
	std::string level;
	do
	{
		std::cout << "Enter the level: ";
		std::getline(std::cin, level);

		if (stoi(level) > 0) // level validation
			_level = level;
	} while (stoi(level) <= 0);

	int sum;
	do
	{
		sum = 0;
		std::string hitpoints;
		do
		{
			std::cout << "Enter the ammount of hitpoints: ";
			std::getline(std::cin, hitpoints);

			if (!DataValidator::fValidate(hitpoints, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_hitpoints = hitpoints;
			}
		} while (!DataValidator::fValidate(hitpoints, DataValidator::ABILITY));
		sum += stoi(hitpoints);

		std::cout << MAX_ABILITIES_SUM_ - sum << " points left." << std::endl;
		std::string strength;
		do
		{
			std::cout << "Enter the strength-value: ";
			std::getline(std::cin, strength);

			if (!DataValidator::fValidate(strength, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_strength = strength;
				_strength_mod = fSetAbilityMod(strength);
			}
		} while (!DataValidator::fValidate(strength, DataValidator::ABILITY));
		sum += stoi(strength);

		std::cout << MAX_ABILITIES_SUM_ - sum << " points left." << std::endl;
		std::string dexterity;
		do
		{
			std::cout << "Enter the dexterity-value: ";
			std::getline(std::cin, dexterity);

			if (!DataValidator::fValidate(dexterity, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_dexterity = dexterity;
				_dexterity_mod = fSetAbilityMod(dexterity);
			}
		} while (!DataValidator::fValidate(dexterity, DataValidator::ABILITY));
		sum += stoi(dexterity);

		std::cout << MAX_ABILITIES_SUM_ - sum << " points left." << std::endl;
		std::string constitution;
		do
		{
			std::cout << "Enter the constitution-value: ";
			std::getline(std::cin, constitution);

			if (!DataValidator::fValidate(constitution, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_constitution = constitution;
				_constitution_mod = fSetAbilityMod(constitution);
			}
		} while (!DataValidator::fValidate(constitution, DataValidator::ABILITY));
		sum += stoi(constitution);

		std::cout << MAX_ABILITIES_SUM_ - sum << " points left." << std::endl;
		std::string intelligence;
		do
		{
			std::cout << "Enter the intelligence-value: ";
			std::getline(std::cin, intelligence);
			
			if (!DataValidator::fValidate(intelligence, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_intelligence = intelligence;
				_intelligence_mod = fSetAbilityMod(intelligence);
			}
		} while (!DataValidator::fValidate(intelligence, DataValidator::ABILITY));
		sum += stoi(intelligence);

		std::cout << MAX_ABILITIES_SUM_ - sum << " points left." << std::endl;
		std::string wisdom;
		do
		{
			std::cout << "Enter the wisdom-value: ";
			std::getline(std::cin, wisdom);

			if (!DataValidator::fValidate(wisdom, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_wisdom = wisdom;
				_wisdom_mod = fSetAbilityMod(wisdom);
			}
		} while (!DataValidator::fValidate(wisdom, DataValidator::ABILITY));
		sum += stoi(wisdom);

		std::cout << MAX_ABILITIES_SUM_ - sum << " points left." << std::endl;
		std::string charisma;
		do
		{
			std::cout << "This ability is the last." << std::endl;
			std::cout << "Enter the charisma-value: ";
			std::getline(std::cin, charisma);

			if (!DataValidator::fValidate(charisma, DataValidator::ABILITY))
				std::cout << "This datum should be more than 0 and less than (or equal to) 20!" << std::endl;
			else
			{
				_charisma = charisma;
				_charisma_mod = fSetAbilityMod(charisma);
			}
		} while (!DataValidator::fValidate(charisma, DataValidator::ABILITY));
		sum += stoi(charisma);

		if (sum > MAX_ABILITIES_SUM_)
			std::cout << "The sum of all vability-values should be less than (or equal to) 80!\nTry again!" << std::endl;
	} while (sum > MAX_ABILITIES_SUM_);
}

Character::~Character(){}