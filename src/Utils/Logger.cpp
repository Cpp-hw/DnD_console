#include "Includes/Logger.hpp"


Logger::Logger()
{ }

void Logger::fLog(const std::string & m, const Logger::type & t)
{
	std::string file_name;
	
	switch (t)  // switch in log types
	{
	case Logger::type::info:
		file_name = "Logs/info.log";
		break;
	case Logger::type::warning:
		file_name = "Logs/warning.log";
		break;
	case Logger::type::error:
		file_name = "Logs/error.log";
		break;
	}

	fCheckFile(file_name);                                            // checks file by size
	std::ofstream log_file(file_name, std::ios::out | std::ios::app); // opens/creates file
	log_file << "[" << fGetTime()  <<  "] " << m.c_str() << '\n';     // inserts data into file
	log_file.close();                                                 // closes file
}


void Logger::fRenameFile(const std::string & file_name, const std::string & date)
{
	std::string new_filename = file_name;
	std::size_t dot_pos = new_filename.find('.');         // finds file extension
	new_filename.insert(dot_pos, ("_" + date));           // inserts date into file
	std::rename(file_name.c_str(), new_filename.c_str()); // renames file
}

void Logger::fCheckFile(const std::string & file_name)
{
	int size = fGetFileSize(file_name);     // get file size
	if (size >= 10485760)                   // check if size > 10mb
		fRenameFile(file_name, fGetTime()); 
}

int Logger::fGetFileSize(const std::string & file_name)
{
	std::ifstream file(file_name.c_str(), 
		               std::ifstream::in | std::ifstream::binary); // opens file
	if (!file.is_open())
		return -1;

	file.seekg(0, std::ios::end); // reads all file symbols
	int fileSize = file.tellg();  // gets file size in bytes
	file.close();

	return fileSize;
}

std::string Logger::fGetTime()
{
	std::string     time = "";
	std::time_t     current_time;
	struct std::tm* local_time = nullptr;
	std::time(&current_time);              //get current time
	local_time = localtime(&current_time); //convert current time to readable 

	time += std::to_string(local_time->tm_year+1900) + ".";
	time += std::to_string(local_time->tm_mon + 1)   + ".";
	time += std::to_string(local_time->tm_mday)      + "_-_";

	time += std::to_string(local_time->tm_hour)      + ".";
	time += std::to_string(local_time->tm_min)       + ".";
	time += std::to_string(local_time->tm_sec);

	return time;
}

