#ifndef JOB_HPP
#define JOB_HPP

#include "utils/libraries.hpp"
#include "process/Process.hpp"

/**
 * @brief Class representing a list of process launched
 *
 */
class Job
{
private:

	/**
	 * @brief List of active process in the job
	 *
	 */
	std::list<Process>			process_list;

	/**
	 * @brief List of active pipes
	 *
	 */
	std::list<FileDescriptors>	fd_list;

public:

	/**
	 * @brief Construct a new job
	 *
	 */
								Job() = default;

	/**
	 * @brief Construct a job from another
	 *
	 * @param other the job to copy
	 */
								Job(const Job& other) = default;

	/**
	 * @brief Construct a job and start it from these arguments
	 *
	 * @param arguments the arguments of the job
	 */
								Job(const std::vector<std::string>& arguments);

	/**
	 * @brief Equal operator
	 *
	 * @param other the job to copy
	 * @return a reference to the current job after the copy
	 */
	Job&						operator=(const Job& other) = default;

	/**
	 * @brief Start the job from theses arguments
	 *
	 * @param arguments the arguments of the job
	 */
	void						launch(const std::vector<std::string>& arguments);

	/**
	 * @brief Wait for all the process of the job to stop
	 *
	 */
	void						wait();

	/**
	 * @brief Close all opened file descriptors
	 *
	 */
	void						close_fd();
};

#endif
