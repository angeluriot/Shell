#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "libraries.hpp"

class Process
{
private:

	char**						arguments;	// for exec
	pid_t						pid;		// process ID
	bool						completed;	// true if process has completed
	bool						stopped;	// true if process has stopped
	int							status;		// reported status value

public:

								Process() = default;
								Process(const Process& other) = default;

	Process&					operator=(const Process& other) = default;

	void						launch(pid_t pgid, int infile, int outfile, int errfile, bool foreground);
	pid_t						get_pid() const;
	void						set_pid(pid_t pid);
	bool						is_completed() const;
	void						complete();
	bool						is_stopped() const;
	void						stop();
	void						set_stopped(bool stopped);
	int							get_status();
	void						set_status(int status);
};

#endif
