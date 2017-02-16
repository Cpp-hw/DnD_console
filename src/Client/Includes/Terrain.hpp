#pragma once


#ifndef TERRAIN
#define TERRAIN

#include "json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

class Terrain
{
	std::string _owner;
	std::string _name;
	std::string _type;
	std::string _width;
	std::string _height;
	std::string _description;

public:
	Terrain();
	Terrain(const std::string &owner, const std::string &name, const std::string &type, const std::string &width, const std::string &height, const std::string &description);
    
    Terrain(json &jsonData); // creates Terrain from json

	void fShowTerrain();
	json fTerrain_To_Json();
	void SetName(const std::string name);
	void SetHeight(const std::string height);
	void SetWidth(const std::string width);
	void SetDescription(const std::string description);
	void SetOwner(const std::string owner);
	void SetType(const std::string type);
	~Terrain();
};
#endif
