#include "Process.hpp"
#include "Job.hpp"
#include "Shell.hpp"
#include "Parser.hpp"

int main(int argc, char** argv, char** env)
{
	//Shell::init();

	while (true)
	{
		std::string command;
		getline(std::cin, command);

		std::list<std::string> arguments = Parser::separate_parts(command);

		if (arguments.front() == "exit")
			break;

		Job& job = Job::add(command, STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
		job.add_process(arguments);
		job.launch(true);
		job.clear();
	}

	return 0;
}
