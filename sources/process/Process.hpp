#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "utils/libraries.hpp"
#include "utils/utils.hpp"
#include "utils/FileDescriptors.hpp"

/**
 * @brief Class representing a process launched
 *
 */
class Process
{
private:

	/**
	 * @brief Process ID
	 *
	 */
	pid_t						pid;

	/**
	 * @brief Reported status value
	 *
	 */
	int							status;

	/**
	 * @brief File descriptors
	 *
	 */
	FileDescriptors				fd;

public:

	/**
	 * @brief Construct a process
	 *
	 */
								Process();

	/**
	 * @brief Construct a process form another
	 *
	 * @param other the process to copy
	 */
								Process(const Process& other) = default;

	/**
	 * @brief Construct a process from file descriptors
	 *
	 * @param fd the file descriptors
	 */
								Process(FileDescriptors fd);

	/**
	 * @brief Construct a process from these arguments
	 *
	 * @param arguments the input arguments
	 * @param pipe true if the process is in a pipeline, false otherwise
	 */
								Process(const std::vector<std::string>& arguments, bool pipe);

	/**
	 * @brief Construct a process from theses arguments and file descriptors
	 *
	 * @param arguments the input arguments
	 * @param fd the file descriptors
	 * @param pipe true if the process is in a pipeline, false otherwise
	 */
								Process(const std::vector<std::string>& arguments, FileDescriptors fd, bool pipe);

	/**
	 * @brief Destroy the process
	 *
	 */
								~Process();

	/**
	 * @brief Equal operator
	 *
	 * @param other the process to copy
	 * @return A reference to the current process
	 */
	Process&					operator=(const Process& other) = default;

	/**
	 * @brief Launch the process from these arguments
	 *
	 * @param arguments the input arguments
	 * @param pipe true if the process is in a pipeline, false otherwise
	 */
	void						launch(std::vector<std::string> arguments, bool pipe);

	/**
	 * @brief Launch the process from theses arguments and file descriptors
	 *
	 * @param arguments the input arguments
	 * @param fd the file descriptors
	 * @param pipe true if the process is in a pipeline, false otherwise
	 */
	void						launch(std::vector<std::string> arguments, FileDescriptors fd, bool pipe);

	/**
	 * @brief Change file descriptors to handle redirections in the command
	 *
	 * @param arguments the input arguments
	 * @return the arguments without redirections
	 */
	std::vector<std::string>	redirections(const std::vector<std::string>& arguments);

	/**
	 * @brief Get the pid of the process
	 *
	 * @return the pid of the process
	 */
	pid_t						get_pid() const;

	/**
	 * @brief Set the pid of the process
	 *
	 * @param pid the new pid
	 */
	void						set_pid(pid_t pid);

	/**
	 * @brief Get the status of the process
	 *
	 * @return the status of the process
	 */
	int							get_status() const;

	/**
	 * @brief Set the status of the process
	 *
	 * @param status the new status
	 */
	void						set_status(int status);

	/**
	 * @brief The cd command of Shell
	 *
	 * @param arguments "cd" and the file to move
	 * @param child true if it's in the child process, false otherwise
	 * @return false on failure and true otherwise
	 */
	void						cd(const std::vector<std::string>& arguments, bool child);

	/**
	 * @brief The cp command of Shell
	 *
	 * @param arguments "cp" and the file to copy
	 * @return false on failure and true otherwise
	 */
	void						cp(const std::vector<std::string>& arguments);

	/**
	 * @brief The mkdir command of Shell
	 *
	 * @param arguments "mkdir" and the directory to create
	 * @return false on failure and true otherwise
	 */
	void						mkdir(const std::vector<std::string>& arguments);

	/**
	 * @brief Run a binary
	 *
	 * @param arguments the name of the binary and its arguments
	 * @return false on failure and true otherwise
	 */
	void						binary(const std::vector<std::string>& arguments);

	/**
	 * @brief Wait until the child process stop
	 *
	 */
	void						wait();
};

#endif
