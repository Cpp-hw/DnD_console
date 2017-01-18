#include "Includes/IniParser.hpp"

IniParser::IniParser(const std::string & path) :
	                            _path(path)
{
}

std::map<std::string, std::string> IniParser::fGetParams()
{
	std::ifstream file(_path.c_str());  // open file
	std::string   buff;                 // buffer for line in file
	std::string   category_name;        // category name in file [Example]
	std::string   insert;               // insert variable with type Category.Key=Value
	std::string   key;                  // map key
	std::string   value;                // map value
	bool          is_key = true;        // flag for checking

	if (file.is_open())
	{
		for (int i = 0; std::getline(file, buff); ++i)
		{
			insert = "";                      // reset insert value;

			if (buff.length() == 0) continue; // check if line is empty
			if (buff[0] == '[')               // if new category detected update category name
			{
				category_name = "";           // reset category_name if new finded
				for (int ch = 1; ch < buff.length() - 1; ch++)
					category_name += buff[ch];

				category_name += ".";
				continue;
			}

			insert = category_name + buff;
			key = "";                         // reset key
			value = "";                       // reset value

			for (int j = 0; j < insert.length(); j++)
			{
				if (insert[j] == ' ' || insert[j] == '"')
					continue;                 // check for empty spaces and " signs
				if (insert[j] == '=')         // if equal sign then trigger key flag to false
				{
					is_key = false;
					continue;
				}
				if (is_key)                   // if key flag insert data in key
					key += insert[j];
				else
					value += insert[j];       // if not key flag insert data in value
			}
			is_key = true;                    // reset key flag
			_params.insert({ key, value });   // insert data into map
		}
		file.close();
	}
	else
	{
		_params.clear();
		_params.insert({"Error","#1 File not found"});
	}
	return _params;
}


void IniParser::fSetParams(const std::map<std::string, std::string> & new_params)
{
	std::ofstream                      file(_path.c_str());  // open file
	std::map<std::string, std::string> old_params;           // old file params
	std::string                        map_category_name;    // category name
	std::string                        tmp_category_name;    // tmp category name
	std::string                        key;                  // map key without category

	if (file.is_open())                                      // check for file
	{
		old_params = fGetParams();                           // get old params
		for (auto &el : new_params)                          // insert or update old params
		{
			auto param_value = old_params.find(el.first);
			if (param_value == old_params.end())
				old_params.insert(el);                       // insert new values
			else
				param_value->second = el.second;             // update old values
		}

		tmp_category_name = "";
		map_category_name = "";
		for (auto &el : old_params)                          // write in file
		{
			tmp_category_name = "";
			for (int i = 0; i < el.first.length(); i++)      // update category name
			{
				if (el.first[i] == '.')
					break;
				tmp_category_name += el.first[i];
			}

			if (map_category_name != tmp_category_name)      // check for new category
			{
				map_category_name = tmp_category_name;
				file << "[" << map_category_name << "]\n";   // write new category
			}

			key = el.first;                                  // get key
			key.erase(0, map_category_name.length() + 1);    // erase category from key
			file << key << " = " << el.second << "\n";       // write in file 'key = value'
		}
		file.close();
	}
	else
	{
		_params.clear();
		_params.insert({"Error","#1 File not found"});
	}
}


IniParser::~IniParser()
{
}

