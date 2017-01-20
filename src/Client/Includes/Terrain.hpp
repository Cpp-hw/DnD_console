#pragma once

#include "json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;

class Terrain
{
	std::string _owner;
	std::string _name;
	std::string _width;
	std::string _height;
	std::string _description;

public:
	Terrain();
	Terrain(const std::string &owner, const std::string &name, const std::string &width, const std::string &height, const std::string &description);

	void fShowTerrain();
	json fTerrain_To_Json();
	void Terrain::SetName(const std::string name);
	void Terrain::SetHeight(const std::string height);
	void Terrain::SetWidth(const std::string width);
	void Terrain::SetDescription(const std::string description);
	void Terrain::SetOwner(const std::string owner);
	~Terrain();
};