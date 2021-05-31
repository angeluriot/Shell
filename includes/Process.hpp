#ifndef PROCESS_HPP
#define PROCESS_HPP
#include <unistd.hpp>
#include <vector>
#include <string>
#include <sys/types.h>

class Process
{
	struct process *next;			/* next process in pipeline */
	std::vector<std::string> argv;	/* for exec */
	pid_t pid;						/* process ID */
	char completed;					/* true if process has completed */
	char stopped;					/* true if process has stopped */
	int status;						/* reported status value */
};

#endif
