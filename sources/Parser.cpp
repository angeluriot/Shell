#include "Parser.hpp"

std::list<std::string> Parser::separate_parts(std::string command)
{
	if (command.empty())
		return {};

	std::list<std::string> result = {};

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
