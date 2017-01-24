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
    
    Npc(const Npc &npc); // copy constructor
    
    Npc &operator = (const Npc &npc); // assignment operator =
    
    void fSetOwner(const string owner); // setter for owner
    
    void fRandomizeAbilities(); // fills the character abilities by rolling dices
    
    void fSetAbilities(); // asks user for defining abilities points
    
    void fAddNpc(); // creates NPC
    
    void fShowNpc(); // shows NPC in console window
    
    json fToJson(); // converts NPC data into json
    
    ~Npc();
};

#endif /* Npc_hpp */
