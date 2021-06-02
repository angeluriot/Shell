#ifndef SHELL_HPP
#define SHELL_HPP

#include "libraries.hpp"
#include "Job.hpp"

class Shell
{
public:

	static pid_t	pgid;
	static termios	terminal_modes;
	static int		terminal;
	static int		is_interactive;

	static void		init();
	static bool		mark_process_status(pid_t pid, int status);
	static void		update_status();
};

#endif
