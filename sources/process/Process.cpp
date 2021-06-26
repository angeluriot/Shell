#include "shell/Shell.hpp"
#include "process/Process.hpp"

Process::Process()
{
	pid = 0;
	status = 0;
}

Process::Process(FileDescriptors fd)
{
	pid = 0;
	status = 0;
	this->fd = fd;
}

Process::Process(const std::vector<std::string>& arguments, bool pipe)
{
	pid = 0;
	status = 0;
	launch(arguments, FileDescriptors(), pipe);
}

Process::Process(const std::vector<std::string>& arguments, FileDescriptors fd, bool pipe)
{
	pid = 0;
	status = 0;
	launch(arguments, fd, pipe);
}

Process::~Process()
{
	// Close file descriptors
	if (fd.in != STDIN_FILENO)
		close(fd.in);

	if (fd.out != STDOUT_FILENO)
		close(fd.out);
}

void Process::launch(std::vector<std::string> arguments, bool pipe)
{
	// Handle redirections
	arguments = redirections(arguments);

	// No argument case
	if (arguments.empty())
		throw std::invalid_argument("no argument");

	// Cd
	if (!pipe && arguments.front() == "cd")
	{
		cd(arguments, false);
		return;
	}

	// Fork
	pid = fork();

	// Fork error case
	if (pid == -1)
	{
		perror("fork");
		throw std::runtime_error("fork failed");
	}

	// Child case
	else if (pid == 0)
	{
		// Change file descriptors of the process
		if (this->fd.in != STDIN_FILENO)
		{
			close(STDIN_FILENO);

			if (dup2(this->fd.in, STDIN_FILENO) == -1)
				exit(errno);

			close(this->fd.in);
		}

		if (this->fd.out != STDOUT_FILENO)
		{
			close(STDOUT_FILENO);

			if (dup2(this->fd.out, STDOUT_FILENO) == -1)
				exit(errno);

			close(this->fd.out);
		}

		// Cd
		if (arguments.front() == "cd")
		{
			if (pipe)
				cd(arguments, true);
		}

		// Cp
		else if (arguments.front() == "cp")
			cp(arguments);

		// Mkdir
		else if (arguments.front() == "mkdir")
			mkdir(arguments);

		// Binary
		else
			binary(arguments);

		exit(EXIT_SUCCESS);
	}
}

void Process::launch(std::vector<std::string> arguments, FileDescriptors fd, bool pipe)
{
	this->fd = fd;
	launch(arguments, pipe);
}

std::vector<std::string> Process::redirections(const std::vector<std::string>& arguments)
{
	// Syntax error case
	if (arguments.empty() || arguments.back() == ">" || arguments.back() == "<" || arguments.back() == ">>")
		throw std::invalid_argument("syntax error");

	std::vector<std::string> result = {};

	// Change file descriptors and remove redirections
	for (int i = 0; i < arguments.size(); i++)
	{
		if (arguments[i] == ">")
		{
			if (arguments[i + 1] == ">" || arguments[i + 1] == "<" || arguments[i + 1] == ">>")
				throw std::invalid_argument("syntax error");

			if (fd.out != STDOUT_FILENO)
				close(fd.out);

			fd.out = open(arguments[i + 1].data(), O_CREAT | O_WRONLY | O_TRUNC, 0664);

			if (fd.out == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else if (arguments[i] == "<")
		{
			if (arguments[i + 1] == ">" || arguments[i + 1] == "<" || arguments[i + 1] == ">>")
				throw std::invalid_argument("syntax error");

			if (fd.in != STDIN_FILENO)
				close(fd.in);

			fd.in = open(arguments[i + 1].data(), O_RDONLY);

			if (fd.in == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else if (arguments[i] == ">>")
		{
			if (arguments[i + 1] == ">" || arguments[i + 1] == "<" || arguments[i + 1] == ">>")
				throw std::invalid_argument("syntax error");

			if (fd.out != STDOUT_FILENO)
				close(fd.out);

			fd.out = open(arguments[i + 1].data(), O_CREAT | O_WRONLY | O_APPEND, 0664);

			if (fd.out == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else
			result.push_back(arguments[i]);
	}

	return result;
}

pid_t Process::get_pid() const
{
	return pid;
}

void Process::set_pid(pid_t pid)
{
	this->pid = pid;
}

int Process::get_status() const
{
	return status;
}

void Process::set_status(int status)
{
	this->status = status;
}

void Process::wait()
{
	waitpid(pid, &status, 0);
}
