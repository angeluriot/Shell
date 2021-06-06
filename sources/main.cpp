#include "Process.hpp"
#include "Job.hpp"
#include "Shell.hpp"

int main()
{
	while (true)
	{
		Shell::show_prompt();

		std::string command;
		getline(std::cin, command);

		std::vector<std::string> arguments = Shell::separate_parts(command);

		if (arguments.empty())
			continue;

		if (arguments.front() == "exit")
			break;

		try
		{
			Shell::parse_command(arguments);
		}

		catch (const std::invalid_argument& e)
		{
			if (e.what() != "no argument")
				std::cerr << e.what() << std::endl;
		}

		catch (const std::runtime_error& e) {}
	}

	return 0;
}
