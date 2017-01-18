#pragma once
#ifndef _DND_LOGGER_HPP_
#define _DND_LOGGER_HPP_
#include <fstream>
#include <ctime>
#include <string>
class Logger
{
private:
	Logger();
	
	// This function renames file
	static void fRenameFile(const std::string &file_name, const std::string & date);

	// This function checks file by its size and if size is more than 10mb renames it!
	static void fCheckFile(const std::string &file_name);

	// This function gets file size
	static int fGetFileSize(const std::string &file_name);

	// This function gets current time
	static std::string fGetTime();
public:
	//Enumuration for available log types
	enum class type
	{
		info,
		warning,
		error
	};

	// Log message into log file
	static void fLog(const std::string & msg, const type & t);
};

#endif // !_DND_LOGGER_HPP_