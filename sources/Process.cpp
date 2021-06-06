#include "Process.hpp"
#include "Shell.hpp"

Process::Process()
{
	pid = 0;
	status = 0;
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
}

Process::Process(const std::vector<std::string>& arguments)
{
	pid = 0;
	status = 0;
	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;
	launch(arguments);
}

void Process::launch(std::vector<std::string> arguments)
{
	arguments = redirections(arguments);

	if (arguments.empty())
		throw std::invalid_argument("no argument");

	if (arguments.front() == "cd")
	{
		cd(arguments);
		return;
	}

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		throw std::runtime_error("fork failed");
	}

	else if (pid == 0)
	{
		if (fd_in != STDIN_FILENO)
		{
			close(STDIN_FILENO);

			if (dup2(fd_in, STDIN_FILENO) == -1)
				exit(errno);

			close(fd_in);
		}

		if (fd_out != STDOUT_FILENO)
		{
			close(STDOUT_FILENO);

			if (dup2(fd_out, STDOUT_FILENO) == -1)
				exit(errno);

			close(fd_out);
		}

		execvp(arguments.front().data(), to_char_array(arguments));

		if (errno == ENOENT)
			std::cerr << arguments.front() << ": command not found" << std::endl;

		else
			perror(arguments.front().data());

		exit(ENOENT);
	}

	else
		waitpid(pid, &status, 0);
}

std::vector<std::string> Process::redirections(const std::vector<std::string>& arguments)
{
	if (arguments.empty() || arguments.back() == ">" || arguments.back() == "<" || arguments.back() == ">>")
		throw std::invalid_argument("syntax error");

	std::vector<std::string> result = {};

	for (int i = 0; i < arguments.size(); i++)
	{
		if (arguments[i] == ">")
		{
			if (fd_out != STDOUT_FILENO)
				close(fd_out);

			fd_out = open(arguments[i + 1].data(), O_CREAT | O_WRONLY | O_TRUNC, 0664);

			if (fd_out == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else if (arguments[i] == "<")
		{
			if (fd_in != STDIN_FILENO)
				close(fd_in);

			fd_in = open(arguments[i + 1].data(), O_RDONLY);

			if (fd_in == -1)
			{
				perror("open");
				throw std::runtime_error("open");
			}

			i++;
		}

		else if (arguments[i] == ">>")
		{
			if (fd_out != STDOUT_FILENO)
				close(fd_out);

			fd_out = open(arguments[i + 1].data(), O_CREAT | O_WRONLY | O_APPEND, 0664);

			if (fd_out == -1)
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

int Process::cd(const std::vector<std::string>& arguments)
{
	std::string old_pwd = std::string(std::filesystem::current_path());
	std::string new_pwd;

	if (arguments.size() > 2)
	{
		std::cerr << "cd: too many arguments" << std::endl;
		return 1;
	}

	else if (arguments.size() == 1)
	{
		if (std::getenv("HOME") == nullptr)
		{
			std::cerr << "cd: HOME not set" << std::endl;
			return 1;
		}

		new_pwd = getenv("HOME");
	}

	else if (arguments.back() == "-")
	{
		if (std::getenv("OLDPWD") == nullptr)
		{
			std::cerr << "cd: OLDPWD not set" << std::endl;
			return 1;
		}

		new_pwd = getenv("OLDPWD");
		write(fd_out, (new_pwd + '\n').data(), new_pwd.size() + 1);
	}

	else
		new_pwd = arguments.back();

	try
	{
		std::filesystem::current_path(new_pwd);
	}

	catch (...)
	{
		std::cerr << "cd: " << arguments.back() << ": No such file or directory" << std::endl;
		return 1;
	}

	setenv("OLDPWD", old_pwd.data(), 1);
	setenv("PWD", std::string(std::filesystem::current_path()).data(), 1);
	return 0;
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
