#include "Process.hpp"
#include "Shell.hpp"

void Process::launch(pid_t pgid, int infile, int outfile, int errfile, bool foreground)
{
	if (Shell::is_interactive)
	{
		/* Put the process into the process group and give the process group
		 the terminal, if appropriate.
		 This has to be done both by the shell and in the individual
		 child processes because of potential race conditions. */
		pid = getpid();

		if (pgid == 0)
			pgid = pid;

		setpgid(pid, pgid);

		if (foreground)
			tcsetpgrp(Shell::terminal, pgid);

		// Set the handling for job control signals back to the default
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
	}

	// Set the standard input/output channels of the new process
	if (infile != STDIN_FILENO)
	{
		dup2(infile, STDIN_FILENO);
		close(infile);
	}

	if (outfile != STDOUT_FILENO)
	{
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
	}

	if (errfile != STDERR_FILENO)
	{
		dup2(errfile, STDERR_FILENO);
		close(errfile);
	}

	// Exec the new process. Make sure we exit
	execvp(arguments[0], arguments);
	perror("execvp");
	exit(1);
}

pid_t Process::get_pid() const
{
	return pid;
}

void Process::set_pid(pid_t pid)
{
	this->pid = pid;
}

bool Process::is_completed() const
{
	return completed;
}

void Process::complete()
{
	completed = true;
}

bool Process::is_stopped() const
{
	return stopped;
}

void Process::stop()
{
	stopped = true;
}

void Process::set_stopped(bool stopped)
{
	this->stopped = stopped;
}

int Process::get_status()
{
	return status;
}

void Process::set_status(int status)
{
	this->status = status;
}
