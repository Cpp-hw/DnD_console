#pragma once
#ifndef SRC_INIPARSER_HPP_
#define SRC_INIPARSER_HPP_

#include <fstream>
#include <map>
#include <string>


// Class usage for getting data
//  IniParser* parser = new IniParser("config.ini");
//  auto params = parser->fGetParams(); - gets all params from file
// 
// Class usage for inserting data
// 	map<string, string> ins;
// 	ins.insert({ "Game.board", "1" });
//	ins.insert({ "Hero.Id", "15" });
//  	parser->fSetParams(ins); - inserts maps data into file
//

class IniParser
{
private:
	std::string _path;                           // path to file
	std::map<std::string, std::string> _params;  // file params
public:
	// Constructor sets _path value
	// string path - path to file
	IniParser(const std::string & path);

	// fGetParams() - returns map with params data from file
	std::map<std::string, std::string> fGetParams();

	// fSetParams(map<string, string> params) update or insert new data into file
	// map<string, string> params - new params that will be inserted into file
	void fSetParams(const std::map<std::string, std::string> & params);

	virtual ~IniParser();
};

#endif
