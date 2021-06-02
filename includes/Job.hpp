#ifndef JOB_HPP
#define JOB_HPP

#include "libraries.hpp"
#include "Process.hpp"

class Job
{
private:

	std::string				command;					// command line, used for messages
	pid_t					pgid;						// process group ID
	bool					notified;					// true if user told about stopped job
	termios					terminal_modes;				// saved terminal modes
	int						std_in, std_out, std_err;	// standard i/o channels

public:

	std::list<Process>		process_list;				// list of processes in this job

	static std::list<Job>	list;						// The active jobs are linked into a list

							Job() = default;
							Job(const Job& other) = default;
							Job(std::string command, int std_in, int std_out, int std_err);

	Job&					operator=(const Job& other) = default;

	void					add_process(const Process& process);
	void					add_process(std::list<std::string> arguments);
	void					launch(bool foreground);
	void					put_in_foreground(bool cont);
	void					put_in_background(bool cont);
	void					wait();
	void					format_info(const char* status);
	void					mark_as_running();
	void					keep_on_going(bool foreground);
	bool					is_stopped() const;
	bool					is_completed() const;
	void					clear();

	static Job&				add(std::string command, int std_in, int std_out, int std_err);
	static Job*				find_job(pid_t pgid);
	static void				do_notification();
};

#endif
