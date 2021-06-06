#include "Job.hpp"
#include "Shell.hpp"

Job::Job(const std::vector<std::string>& arguments)
{
	process_list.clear();
	launch(arguments);
}

void Job::launch(const std::vector<std::string>& arguments)
{
	if (arguments.empty())
		throw std::invalid_argument("syntax error");

	process_list.emplace_back(arguments);
}
