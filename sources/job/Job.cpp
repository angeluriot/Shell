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

	int nb_pipes = std::count(arguments.begin(), arguments.end(), "|");
	std::vector<FileDescriptors> fd_list = {};

	// Pipes case
	if (nb_pipes > 0)
	{
		// Open all pipes
		for (int i = 0; i < nb_pipes; i++)
		{
			fd_list.emplace_back();

			if (pipe(fd_list.back().get_ptr()) == -1)
			{
				perror("pipe");
				throw std::runtime_error("pipe");
			}
		}

		std::vector<std::string> process_arguments = {};
		int pipe_i = 0;

		// Launch all process
		for (int i = 0; i < arguments.size(); i++)
		{
			if (arguments[i] == "|")
			{
				if (process_arguments.empty())
					throw std::invalid_argument("syntax error");

				int fd_in = (pipe_i == 0 ? STDIN_FILENO : fd_list[pipe_i - 1].in);
				int fd_out = fd_list[pipe_i].out;

				process_list.emplace_back(process_arguments, FileDescriptors(fd_in, fd_out), true, this);
				process_arguments.clear();
				pipe_i++;
			}

			else
				process_arguments.push_back(arguments[i]);
		}

		process_list.emplace_back(process_arguments, FileDescriptors(fd_list.back().in, STDOUT_FILENO), true, this);
		close_fd();
	}

	// No pipes case
	else
		process_list.emplace_back(arguments, false, this);

	// Wait until all process finished
	wait();
}

void Job::wait()
{
	for (Process process : process_list)
		process.wait();
}

void Job::close_fd()
{
	for (auto fd : fd_list)
		fd.close();
}
