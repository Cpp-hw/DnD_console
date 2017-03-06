//
//  NPC.cpp
//  Client
//
//  Created by Olha Leskovska on 1/15/17.
//  Copyright © 2017 Olha Leskovska. All rights reserved.
//

#include "Includes/NPC.hpp"

Npc::Npc()
{
}

Npc::Npc(const string &owner,
         const string &name,
         const string &type,
         const string &level,
         const string &hitpoints,
         const string &strength,
         const string &dexterity,
         const string &constitution,
         const string &intelligence,
         const string &wisdom,
         const string &charisma)
{
    _owner = owner,
    _name = name;
    _type = type;
    _level = level;
    _hitpoints = hitpoints;
    _strength = strength;
    _dexterity = dexterity;
    _constitution = constitution;
    _intelligence = intelligence;
    _wisdom = wisdom;
    _charisma = charisma;
}

Npc::Npc(json &jsonData)
{
    _owner = jsonData["id_owner"];
    _name = jsonData["npc"];
    _type = jsonData["id_type"];
    _level = jsonData["level"];
    _hitpoints = jsonData["hitpoints"];
    _strength = jsonData["strength"];
    _dexterity = jsonData["dexterity"];
    _constitution = jsonData["constitution"];
    _intelligence = jsonData["intelligence"];
    _wisdom = jsonData["wisdom"];
    _charisma = jsonData["charisma"];
}

Npc::Npc(const Npc &npc)
{
    _owner = npc._owner;
    _name = npc._name;
    _type = npc._type;
    _hitpoints = npc._hitpoints;
    _level = npc._level;
    _strength = npc._strength;
    _dexterity = npc._dexterity;
    _constitution = npc._constitution;
    _intelligence = npc._intelligence;
    _wisdom = npc._wisdom;
    _charisma = npc._charisma;
}

Npc &Npc::operator = (const Npc &npc)
{
    if (this == &npc)
        return * this;
    
    _owner = npc._owner;
    _name = npc._name;
    _type = npc._type;
    _hitpoints = npc._hitpoints;
    _level = npc._level;
    _strength = npc._strength;
    _dexterity = npc._dexterity;
    _constitution = npc._constitution;
    _intelligence = npc._intelligence;
    _wisdom = npc._wisdom;
    _charisma = npc._charisma;
    return * this;
}

void Npc::fSetOwner(const string owner)
{
    _owner = owner;
}

void Npc::fSetName(const string &name)
{
    _name = name;
}

void Npc::fSetType(const string &type)
{
    _type = type;
}

void Npc::fSetLevel(const string &level)
{
    _level = level;
}

void Npc::fSetHitpoints(const string &hitpoints)
{
    _hitpoints = hitpoints;
}

void Npc::fSetStrength(const string &strength)
{
    _strength = strength;
}

void Npc::fSetDexterity(const string &dexterity)
{
    _dexterity = dexterity;
}

void Npc::fSetConstitution(const string &constitution)
{
    _constitution = constitution;
}

void Npc::fSetIntelligence(const string &intelligence)
{
    _intelligence = intelligence;
}

void Npc::fSetWisdom(const string &wisdom)
{
    _wisdom = wisdom;
}

void Npc::fSetCharisma(const string &charisma)
{
    _charisma = charisma;
}

string Npc::fGetName()
{
    return _name;
}

string Npc::fGetType()
{
    return _type;
}

string Npc::fGetLevel()
{
    return _level;
}

string Npc::fGetHitpoints()
{
    return _hitpoints;
}

string Npc::fGetStrength()
{
    return _strength;
}

string Npc::fGetDexterity()
{
    return _dexterity;
}

string Npc::fGetConstitution()
{
    return _constitution;
}

string Npc::fGetIntelligence()
{
    return _intelligence;
}

string Npc::fGetWisdom()
{
    return _wisdom;
}

string Npc::fGetCharisma()
{
    return _charisma;
}

void Npc::fRandomizeAbilities() // fills the character abilities by rolling dices
{
    _level = "1";
    _hitpoints = "100";
    
    int sum;
    do
    {
		sum = 0;
        string strength;
        do
        {
            strength = to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));
            
            if (DataValidator::fValidate(strength, DataValidator::ABILITY))
                _strength = strength;
        } while (!DataValidator::fValidate(strength, DataValidator::ABILITY));
        sum += stoi(strength);
        
        string dexterity;
        do
        {
            dexterity = to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));
            
            if (DataValidator::fValidate(dexterity, DataValidator::ABILITY))
                _dexterity = dexterity;
        } while (!DataValidator::fValidate(dexterity, DataValidator::ABILITY));
        sum += stoi(dexterity);
        
        string constitution;
        do
        {
            constitution = to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));
            
            if (DataValidator::fValidate(constitution, DataValidator::ABILITY))
                _constitution = constitution;
        } while (!DataValidator::fValidate(constitution, DataValidator::ABILITY));
        sum += stoi(constitution);
        
        string intelligence;
        do
        {
            intelligence = to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));
            
            if (DataValidator::fValidate(intelligence, DataValidator::ABILITY))
                _intelligence = intelligence;
        } while (!DataValidator::fValidate(intelligence, DataValidator::ABILITY));
        sum += stoi(intelligence);
        
        string wisdom;
        do
        {
            wisdom = to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));
            
            if (DataValidator::fValidate(wisdom, DataValidator::ABILITY))
                _wisdom = wisdom;
        } while (!DataValidator::fValidate(wisdom, DataValidator::ABILITY));
        sum += stoi(wisdom);
        
        string charisma;
        do
        {
            charisma = to_string(Dice::fMultipleRollSum(Dice::D4, 4, 3));
            
            if (DataValidator::fValidate(charisma, DataValidator::ABILITY))
                _charisma = charisma;
        } while (!DataValidator::fValidate(charisma, DataValidator::ABILITY));
        sum += stoi(charisma);
    } while (sum > MAX_ABILITIES_SUM);
}

void Npc::fSetAbilities() // asks user for defining abilities points
{
    string level;
    do
    {
        cout << "Input the level: ";
        getline(cin, level);
        
        if (stoi(level) > 0) // level validation
            _level = level;
    } while (stoi(level) <= 0);
    
    string hitpoints;
    do
    {
        cout << "Input the hitpoints ammount: ";
        getline(cin, hitpoints);
        
        if (stoi(hitpoints) > 0) // hitpoints validation
            _hitpoints = hitpoints;
    } while (stoi(hitpoints) <= 0);
    
    int sum;
    do
    {
        sum = 0;
        string strength;
        do
        {
            cout << "Input the strength value: ";
            getline(cin, strength);
            
            if (!DataValidator::fValidate(strength, DataValidator::ABILITY))
                cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
            else
                _strength = strength;
        } while (!DataValidator::fValidate(strength, DataValidator::ABILITY));
        sum += stoi(strength);
        
        cout << MAX_ABILITIES_SUM - sum << " points left." << endl;
        string dexterity;
        do
        {
            cout << "Input the dexterity value: ";
            getline(cin, dexterity);
            
            if (!DataValidator::fValidate(dexterity, DataValidator::ABILITY))
                cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
            else
                _dexterity = dexterity;
        } while (!DataValidator::fValidate(dexterity, DataValidator::ABILITY));
        sum += stoi(dexterity);
        
        cout << MAX_ABILITIES_SUM - sum << " points left." << endl;
        string constitution;
        do
        {
            cout << "Input the constitution value: ";
            getline(cin, constitution);
            
            if (!DataValidator::fValidate(constitution, DataValidator::ABILITY))
                cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
            else
                _constitution = constitution;
        } while (!DataValidator::fValidate(constitution, DataValidator::ABILITY));
        sum += stoi(constitution);
        
        cout << MAX_ABILITIES_SUM - sum << " points left." << endl;
        string intelligence;
        do
        {
            cout << "Input the intelligence value: ";
            getline(cin, intelligence);
            
            if (!DataValidator::fValidate(intelligence, DataValidator::ABILITY))
                cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
            else
                _intelligence = intelligence;
        } while (!DataValidator::fValidate(intelligence, DataValidator::ABILITY));
        sum += stoi(intelligence);
        
        cout << MAX_ABILITIES_SUM - sum << " points left." << endl;
        string wisdom;
        do
        {
            cout << "Input the wisdom value: ";
            getline(cin, wisdom);
            
            if (!DataValidator::fValidate(wisdom, DataValidator::ABILITY))
                cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
            else
                _wisdom = wisdom;
        } while (!DataValidator::fValidate(wisdom, DataValidator::ABILITY));
        sum += stoi(wisdom);
        
        cout << MAX_ABILITIES_SUM - sum << " points left." << endl;
        string charisma;
        do
        {
            cout << "This ability is the last." << endl;
            cout << "Input the charisma value: ";
            getline(cin, charisma);
            
            if (!DataValidator::fValidate(charisma, DataValidator::ABILITY))
                cout << "This datum should be more than 0 and less than (or equal to) 20!" << endl;
            else
                _charisma = charisma;
        } while (!DataValidator::fValidate(charisma, DataValidator::ABILITY));
        sum += stoi(charisma);
        
        if (sum > MAX_ABILITIES_SUM)
            cout << "The sum of all ability values should be less than (or equal to) 80!\nTry again!" << endl;
    } while (sum > MAX_ABILITIES_SUM);
}

void Npc::fAddNpc() // creates NPC
{
    cin.clear();
    cin.ignore();
    cout << "********** NPC **********" << endl;
    string name;
    do
    {
        cout << "Input a name: ";
        getline(cin, name);
        
        if (!DataValidator::fValidate(name, DataValidator::SQL_INJECTION)) // name validation
            cout << "This datum should consist of letters!\n";
        else
            _name = name;
    } while (!DataValidator::fValidate(name, DataValidator::SQL_INJECTION));
    
    string type;
    do
    {
        cout << "Input the NPC's type: ";
        getline(cin, type);
        
        if (!DataValidator::fValidate(type, DataValidator::SQL_INJECTION)) // type validation
            cout << "This datum should consist of letters!" << endl;
        else
            _type = type;
    } while (!DataValidator::fValidate(type, DataValidator::SQL_INJECTION));
    
    int choice;
    cout << "Choose the way to fill the NPC's abilities:" << endl;
    cout << "1. by rolling dices;" << endl;
    cout << "2. manually" << endl;
    cout << "Your choice: ";
    cin >> choice;
    cin.clear();
    cin.ignore();
    
    switch (choice)
    {
        case 1:
            Npc::fRandomizeAbilities();
            break;
        case 2:
            Npc::fSetAbilities();
            break;
        default:
            cout << "Unexpected operation." << endl;
            break;
    }
}

void Npc::fShowNpc()
{
    cout << "Name: " << _name << endl;
    cout << "Type: " << _type << endl;
    cout << "Level: " << _level << endl;
    cout << "Hitpoints: " << _hitpoints << endl;
    cout << "Strength: " << _strength << endl;
    cout << "Dexterity: " << _dexterity << endl;
    cout << "Constitution: " << _constitution << endl;
    cout << "Intelligence: " << _intelligence << endl;
    cout << "Wisdom: " << _wisdom << endl;
    cout << "Charisma: " << _charisma << endl;
}

json Npc::fToJson()
{
    json npc;
    npc["session_id"] = _owner;
    npc["npc"] = _name;
    npc["id_type"] = _type;
    npc["level"] = _level;
    npc["hitpoints"] = _hitpoints;
    npc["strength"] = _strength;
    npc["dexterity"] = _dexterity;
    npc["constitution"] = _constitution;
    npc["intelligence"] = _intelligence;
    npc["wisdom"] = _wisdom;
    npc["charisma"] = _charisma;
    
    return npc;
}

Npc::~Npc() {}

string fShowMaxValue(const int sum)
{
    string answer;
    if ((MAX_ABILITIES_SUM - sum) <= 20)
    {
        answer = "The maximum value for the hitpoints is " + to_string(MAX_ABILITIES_SUM - sum) + ".";
    }
    else
        answer = "The maximum value for the hitpoints is 20.";
    
    return answer;
}
