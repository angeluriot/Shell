#include "job/Job.hpp"
#include "shell/Shell.hpp"

Job::Job(const std::vector<std::string>& arguments)
{
	process_list.clear();
	launch(arguments);
}

void Job::launch(const std::vector<std::string>& arguments)
{
	// Syntax error case
	if (arguments.empty() || arguments.front() == "|" || arguments.back() == "|")
		throw std::invalid_argument("syntax error");

	if (std::count(arguments.begin(), arguments.end(), "|") > 0)
	{
		// Pipes
		FileDescriptors previous_fd;
		FileDescriptors fd;
		std::vector<std::string> process_arguments = {};

		for (int i = 0; i < arguments.size(); i++)
		{
			if (arguments[i] == "|")
			{
				if (process_arguments.size() == 0)
					throw std::invalid_argument("syntax error");

				if (pipe(fd.get_ptr()) == -1)
				{
					perror("pipe");
					throw std::runtime_error("pipe");
				}

				process_list.emplace_back(process_arguments, FileDescriptors(previous_fd.in, fd.out), true);
				process_arguments.clear();
				previous_fd = fd;
				fd.clear();
			}

			else
				process_arguments.push_back(arguments[i]);
		}

		process_list.emplace_back(process_arguments, FileDescriptors(previous_fd.in, STDOUT_FILENO), true);
	}

	else
		process_list.emplace_back(arguments, false);

	wait();
}

void Job::wait()
{
	for (Process process : process_list)
		process.wait();
}
