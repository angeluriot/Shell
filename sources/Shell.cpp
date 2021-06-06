#include "Shell.hpp"

std::list<Job> Shell::job_list = {};

void Shell::show_prompt()
{
	std::cout << std::string(std::filesystem::current_path()) << "> " << std::flush;
}

std::vector<std::string> Shell::separate_parts(std::string command)
{
	if (command.empty())
		return {};

	std::vector<std::string> result = {};

	while (true)
	{
		while (command.front() == ' ' || command.front() == '\t')
			command = command.substr(1);

		size_t end = command.find_first_of(" \t");

		if (end == std::string::npos)
		{
			result.push_back(command);
			break;
		}

		result.push_back(command.substr(0, end));
		command = command.substr(end);
	}

	return result;
}

void Shell::parse_command(const std::vector<std::string>& arguments)
{
	job_list.clear();
	job_list.emplace_back(arguments);
}
