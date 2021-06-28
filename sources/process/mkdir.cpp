#include "process/Process.hpp"

void Process::mkdir(const std::vector<std::string>& arguments, bool child)
{
	if (arguments.size() == 2)
	{
		char *path = new char[std::filesystem::current_path().string().length() + arguments.back().length()];
		strcpy(path, std::filesystem::current_path().c_str());
		strcat(path, "/");
		strcat(path, arguments.back().c_str());
		::mkdir(path, 0777);
	}

	// Too many arguments case
	else if (arguments.size() > 2)
	{
		throw std::invalid_argument("cd: too many arguments");
	}

	// Missing argument case
	else if (arguments.size() == 1)
	{
		throw std::invalid_argument("mkdir: missing directory name");
	}

	

	exit(EXIT_FAILURE);
}
