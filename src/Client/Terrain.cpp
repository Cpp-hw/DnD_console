#include "Includes/Terrain.hpp"


Terrain::Terrain() {}

Terrain::Terrain(const std::string &owner, const std::string &name, const std::string &type, const std::string &width, const std::string &height, const std::string &description)
{
	this->_owner = owner;
	this->_name = name;
	this->_type = type;
	this->_height = height;
	this->_width = width;
	this->_description = description;
}

Terrain::Terrain(json &jsonData)
{
    _owner = jsonData["id_owner"];
    _name = jsonData["npc"];
    _type = jsonData["type"];
    _height = jsonData["height"];
    _width = jsonData["width"];
    _description = jsonData["description"];
}

void Terrain:: fShowTerrain()
{
	std::cout << "Name:" << _name << std::endl;
	std::cout << "Type:" << _type << std::endl;
	std::cout << "Width:" << _width << std::endl;
	std::cout << "Height:" << _height << std::endl;
	std::cout << "Description:" << _description << std::endl;

}


void Terrain::SetName(const std::string name)
{
	this->_name = name;
}
void Terrain::SetType(const std::string type)
{
	this->_type = type;
}

void Terrain::SetHeight(const std::string height)
{
	this->_height = height;
}
void Terrain::SetWidth(const std::string width)
{
	this->_width = width;
}
void Terrain::SetDescription(const std::string description)
{
	this->_description = description;
}

void Terrain::SetOwner(const std::string owner)
{
	this->_owner = owner;
}

json Terrain:: fTerrain_To_Json()
{
	json resualt;

	resualt["session_id"] = _owner;
	resualt["name"] = _name;
	resualt["type"] = _type;
	resualt["width"] = _width;
	resualt["height"] = _height;
	resualt["description"] = _description;

	return resualt;
}
Terrain::~Terrain(){}
