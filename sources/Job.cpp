#include "Job.hpp"
#include "Shell.hpp"

std::list<Job> Job::list = {};

Job::Job(std::string command, int std_in, int std_out, int std_err) 
{
	this->command = command;
	pgid = 0;
	notified = false;
	terminal_modes = termios();
	this->std_in = std_in;
	this->std_out = std_out;
	this->std_err = std_err;
	process_list.clear();
}

void Job::add_process(const Process& process) 
{
	process_list.push_back(process);
}

void Job::add_process(std::list<std::string> arguments) 
{
	process_list.emplace_back(arguments);
}

void Job::launch(bool foreground)
{
	pid_t pid;
	int mypipe[2], infile, outfile;
	infile = std_in;

	for (auto process = process_list.begin(); process != process_list.end(); process++)
	{
		// Set up pipes, if necessary
		if (process != --process_list.end())
		{
			if (pipe(mypipe) < 0)
			{
				perror("pipe");
				exit(1);
			}

			outfile = mypipe[1];
		}

		else
			outfile = std_out;

		// Fork the child processes
		pid = fork();

		if (pid == 0)
			// This is the child process
			process->launch(pgid, infile, outfile, std_err, foreground);

		else if (pid < 0)
		{
			// The fork failed
			perror("fork");
			exit(1);
		}

		else
		{
			// This is the parent process
			process->set_pid(pid);

			if (Shell::is_interactive)
			{
				if (!pgid)
					pgid = pid;

				setpgid(pid, pgid);
			}
		}

		// Clean up after pipes
		if (infile != std_in)
			close(infile);

		if (outfile != std_out)
			close(outfile);

		infile = mypipe[0];
	}

	//format_info("launched");

	if (!Shell::is_interactive)
		wait();

	else if (foreground)
		put_in_foreground(0);

	else
		put_in_background(0);
}

void Job::put_in_foreground(bool cont)
{
	// Put the job into the foreground
	tcsetpgrp(Shell::terminal, pgid);

	// Send the job a continue signal, if necessary
	if (cont)
	{
		tcsetattr(Shell::terminal, TCSADRAIN, &terminal_modes);

		if (kill(-pgid, SIGCONT) < 0)
			perror("kill(SIGCONT)");
	}

	// Wait for it to report
	wait();

	// Put the shell back in the foreground
	tcsetpgrp(Shell::terminal, pgid);

	// Restore the shell’s terminal modes
	tcgetattr(Shell::terminal, &terminal_modes);
	tcsetattr(Shell::terminal, TCSADRAIN, &terminal_modes);
}

void Job::put_in_background(bool cont)
{
	// Send the job a continue signal, if necessary
	if (cont && kill(-pgid, SIGCONT) < 0)
		perror("kill(SIGCONT)");
}

void Job::wait()
{
	int status;
	pid_t pid;

	do
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
	while (!Shell::mark_process_status(pid, status) && !is_stopped() && !is_completed());
}

void Job::format_info(const char* status)
{
	fprintf(stderr, "%ld (%s): %s\n", (long)pgid, status, command.data());
}

void Job::mark_as_running()
{
	for (Process& process : process_list)
		process.set_stopped(false);

	notified = 0;
}

void Job::keep_on_going(bool foreground)
{
	mark_as_running();

	if (foreground)
		put_in_foreground(1);

	else
		put_in_background(1);
}

bool Job::is_stopped() const
{
	return std::all_of(process_list.begin(), process_list.end(), [](const Process& process) -> bool { return process.is_completed() || process.is_stopped(); });
}

bool Job::is_completed() const
{
	return std::all_of(process_list.begin(), process_list.end(), [](const Process& process) -> bool { return process.is_completed(); });
}

void Job::clear() 
{
	auto it = std::find_if(list.begin(), list.end(), [this](const Job& job) -> bool { return this == &job; });

	if (it != list.end())
		list.erase(it);
}

Job& Job::add(std::string command, int std_in, int std_out, int std_err) 
{
	list.emplace_back(command, std_in, std_out, std_err);
	return list.back();
}

Job* Job::find_job(pid_t pgid)
{
	auto it = std::find_if(list.begin(), list.end(), [pgid](const Job& job) -> bool { return job.pgid == pgid; });
	return (it == list.end() ? nullptr : &(*it));
}

void Job::do_notification()
{
	// Update status information for child processes
	Shell::update_status();

	for (auto job = list.begin(); job != list.end(); job++)
	{
		/* If all processes have completed, tell the user the job has
		 completed and delete it from the list of active jobs.  */
		if (job->is_completed())
		{
			job->format_info("completed");
			list.erase(job);
			continue;
		}

		/* Notify the user about stopped jobs,
		 marking them so that we won’t do this more than once.  */
		else if (job->is_stopped() && !job->notified)
		{
			job->format_info("stopped");
			job->notified = 1;
		}
	}
}
