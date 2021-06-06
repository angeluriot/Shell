#ifndef SHELL_HPP
#define SHELL_HPP

#include "libraries.hpp"
#include "Job.hpp"
#include "utils.hpp"

class Shell
{
private:

	static std::list<Job>			job_list;

public:

	static void						show_prompt();
	static std::vector<std::string>	separate_parts(std::string command);
	static void						parse_command(const std::vector<std::string>& arguments);
};

#endif
