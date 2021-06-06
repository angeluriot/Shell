#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "libraries.hpp"

class Process
{
private:

	pid_t						pid;		// process ID
	int							status;		// reported status value
	int							fd_in;		// file descriptor in
	int							fd_out;		// file descriptor out

public:

								Process();
								Process(const Process& other) = default;
								Process(const std::vector<std::string>& arguments);

	Process&					operator=(const Process& other) = default;

	void						launch(std::vector<std::string> arguments);
	std::vector<std::string>	redirections(const std::vector<std::string>& arguments);
	int							cd(const std::vector<std::string>& arguments);
	pid_t						get_pid() const;
	void						set_pid(pid_t pid);
	int							get_status() const;
	void						set_status(int status);
};

#endif
