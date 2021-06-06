#ifndef JOB_HPP
#define JOB_HPP

#include "libraries.hpp"
#include "Process.hpp"

class Job
{
private:

	std::list<Process>	process_list;

public:

						Job() = default;
						Job(const Job& other) = default;
						Job(const std::vector<std::string>& arguments);

	Job&				operator=(const Job& other) = default;

	void				launch(const std::vector<std::string>& arguments);
};

#endif
