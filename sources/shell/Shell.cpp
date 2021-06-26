#include "shell/Shell.hpp"

std::list<Job> Shell::job_list = {};

void Shell::show_prompt()
{
	std::cout << "\033[1;36m" << std::string(std::filesystem::current_path()) << ">\033[0m " << std::flush;
}

std::vector<std::string> Shell::separate_parts(std::string command)
{
	// Empty command case
	if (command.empty())
		return {};

	// Add spaces around pipes/redirections
	for (int i = 0; i < command.size(); i++)
	{
		if (command[i] == '>' || command[i] == '<' || command[i] == '|')
		{
			if (i + 1 < command.size() && command[i] == '>' && command[i + 1] == '>')
			{
				command.insert(i + 2, 1, ' ');
				command.insert(i, 1, ' ');
				i += 1;
			}

			else
			{
				command.insert(i + 1, 1, ' ');
				command.insert(i, 1, ' ');
			}

			i += 2;
		}
	}

	std::vector<std::string> result = {};

	while (true)
	{
		// Remove start spaces
		while (command.front() == ' ' || command.front() == '\t')
			command = command.substr(1);

		// Add word in the array
		size_t end = command.find_first_of(" \t");

		// Last word case
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

void Shell::launch_command(const std::vector<std::string>& arguments)
{
	job_list.clear();
	job_list.emplace_back(arguments);
}
