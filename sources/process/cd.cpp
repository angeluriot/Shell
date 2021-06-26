#include "process/Process.hpp"

void Process::cd(const std::vector<std::string>& arguments, bool child)
{
	std::string old_pwd = std::string(std::filesystem::current_path());
	std::string new_pwd;

	// Too many arguments case
	if (arguments.size() > 2)
	{
		if (child)
		{
			std::cerr << "cd: too many arguments" << std::endl;
			exit(EXIT_FAILURE);
		}

		else
			throw std::invalid_argument("cd: too many arguments");
	}

	// Handle cd without argument
	else if (arguments.size() == 1)
	{
		if (std::getenv("HOME") == nullptr)
		{
			if (child)
			{
				std::cerr << "cd: HOME not set" << std::endl;
				exit(EXIT_FAILURE);
			}

			else
				throw std::invalid_argument("cd: HOME not set");
		}

		new_pwd = getenv("HOME");
	}

	// Handle go back
	else if (arguments.back() == "-")
	{
		if (std::getenv("OLDPWD") == nullptr)
		{
			if (child)
			{
				std::cerr << "cd: OLDPWD not set" << std::endl;
				exit(EXIT_FAILURE);
			}

			else
				throw std::invalid_argument("cd: OLDPWD not set");
		}

		new_pwd = getenv("OLDPWD");
		write(fd.out, (new_pwd + '\n').data(), new_pwd.size() + 1);
	}

	// Handle going in a new directory
	else
		new_pwd = arguments.back();

	try
	{
		std::filesystem::current_path(new_pwd);
	}

	catch (...)
	{
		if (child)
		{
			std::cerr << "cd: " << arguments.back() << ": No such file or directory" << std::endl;
			exit(EXIT_FAILURE);
		}

		else
			throw std::invalid_argument("cd: " + arguments.back() + ": No such file or directory");
	}

	setenv("OLDPWD", old_pwd.data(), 1);
	setenv("PWD", std::string(std::filesystem::current_path()).data(), 1);
}
