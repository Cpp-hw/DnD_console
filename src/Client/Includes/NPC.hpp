//
//  NPC.hpp
//  Client
//
//  Created by Olha Leskovska on 1/15/17.
//  Copyright © 2017 Olha Leskovska. All rights reserved.
//

#ifndef Npc_hpp
#define Npc_hpp

#include <stdio.h>
#include <string>
#include "json.hpp"
#include "DataValidator.hpp"
#include "Dice.hpp"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::to_string;
using json = nlohmann::json;

const int MAX_ABILITIES_SUM = 80;

class Npc
{
private:
    string _owner;
    string _name;
    string _type_npc;
    string _level;
    string _hitpoints;
    string _strength;
    string _dexterity;
    string _constitution;
    string _intelligence;
    string _wisdom;
    string _charisma;
    
public:
    
    Npc();
    
    Npc(string &owner,
        string &name,
        string &type_npc,
        string &level,
        string &hitpoints,
        string &strength,
        string &dexterity,
        string &constitution,
        string &intelligence,
        string &wisdom,
        string &charisma);
    
    Npc(json &jsonData); // creates NPC from json
    
    Npc(const Npc &npc); // copy constructor
    
    Npc &operator = (const Npc &npc); // assignment operator =
    
    /*
     Setters
     */
    void fSetOwner(const string owner);
    void fSetName(const string &name);
    void fSetType(const string &type);
    void fSetLevel(const string &level);
    void fSetHitpoints(const string &hitpoints);
    void fSetStrength(const string &strength);
    void fSetDexterity(const string &dexterity);
    void fSetConstitution(const string &constitution);
    void fSetIntelligence(const string &intelligence);
    void fSetWisdom(const string &wisdom);
    void fSetCharisma(const string &charisma);
    
    /*
     Getters
     */
    string fGetName();
    string fGetType();
    string fGetLevel();
    string fGetHitpoints();
    string fGetStrength();
    string fGetDexterity();
    string fGetConstitution();
    string fGetIntelligence();
    string fGetWisdom();
    string fGetCharisma();
    
    void fRandomizeAbilities(); // fills the character abilities by rolling dices
    
    void fSetAbilities(); // asks user for defining abilities points
    
    void fAddNpc(); // creates NPC
    
    void fShowNpc(); // shows NPC in console window
    
    json fToJson(); // converts NPC data into json
    
    ~Npc();
};

string fShowMaxValue(const int sum);

#endif /* Npc_hpp */
