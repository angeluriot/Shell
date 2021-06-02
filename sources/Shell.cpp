#include "Shell.hpp"

pid_t	Shell::pgid;
termios	Shell::terminal_modes;
int		Shell::terminal;
int		Shell::is_interactive;

void Shell::init()
{
	// See if we are running interactively
	terminal = STDIN_FILENO;
	is_interactive = isatty(terminal);

	if (is_interactive)
	{
		// Loop until we are in the foreground
		while (tcgetpgrp(terminal) != (pgid = getpgrp()))
			kill(-pgid, SIGTTIN);

		// Ignore interactive and job-control signals
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);

		// Put ourselves in our own process group
		pgid = getpid();

		if (setpgid(pgid, pgid) < 0)
		{
			perror("Couldn't put the shell in its own process group");
			exit(1);
		}

		// Grab control of the terminal
		tcsetpgrp(terminal, pgid);

		// Save default terminal attributes for shell
		tcgetattr(terminal, &terminal_modes);
	}
}

bool Shell::mark_process_status(pid_t pid, int status)
{
	if (pid > 0)
	{
		// Update the record for the process
		for (Job& job : Job::list)
			for (Process& process : job.process_list)
				if (process.get_pid() == pid)
				{
					process.set_status(status);

					if (WIFSTOPPED(status))
						process.stop();

					else
					{
						process.complete();

						if (WIFSIGNALED(status))
							fprintf(stderr, "%d: Terminated by signal %d.\n", (int)pid, WTERMSIG(process.get_status()));
					}

					return true;
				}

			fprintf(stderr, "No child process %d.\n", pid);
			return false;
		}

	else if (pid == 0 || errno == ECHILD)
		// No processes ready to report
		return false;

	else
	{
		// Other weird errors
		perror("waitpid");
		return false;
	}
}

void Shell::update_status()
{
	int status;
	pid_t pid;

	do
		pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
	while (!mark_process_status(pid, status));
}
