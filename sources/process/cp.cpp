#include "process/Process.hpp"

/**
 * @brief Copy a file (exit on failure)
 *
 * @param input_file the input file name
 * @param output_file the output file name
 */
void copy_file(const std::string& input_file, const std::string& output_file)
{
	// Open source and destination files
	int src_desc = open(input_file.c_str(), O_RDONLY);
	int dest_desc = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	// Check open errors
	if (src_desc < 0 || dest_desc < 0)
	{
		perror("open");
		exit(errno);
	}

	// Get informations from the source file
	struct stat input_stat;

	if (fstat(src_desc, &input_stat) < 0)
	{
		perror("fstat");
		exit(errno);
	}

	// Copy permissions
	if (fchmod(dest_desc, input_stat.st_mode) < 0)
	{
		perror("fchmod");
		exit(errno);
	}

	while (1)
	{
		// Copy data from the input file
		char buffer[4096];
		int read_count = read(src_desc, buffer, sizeof(buffer));

		// Check read errors
		if (read_count < 0)
		{
			perror("read");
			exit(errno);
		}

		if (read_count == 0)
			break;

		// Paste data in output file
		write(dest_desc, buffer, read_count);
	}

	// Close source and destination files
	close(src_desc);
	close(dest_desc);
}

/**
 * @brief Join a directory path with a file name (exit on failure)
 *
 * @param directory the directory path
 * @param file the file name
 * @return the full path to the file
 */
char* path_join(const std::string& directory, const std::string& file)
{
	// Set the length of the result
	size_t directory_length = directory.length();
	char* result = new char[directory_length + file.length() + 1 + (directory_length != 0 && directory[directory_length - 1] != '/')];

	// Check malloc
	if (result == NULL)
	{
		perror("malloc");
		exit(errno);
	}

	// Check if directory is empty
	if (directory_length == 0)
		return strcpy(result, file.c_str());

	// Add directory to the result
	strcpy(result, directory.c_str());

	if (directory[directory_length - 1] != '/')
		strcat(result, "/");

	// Add file to the result
	strcat(result, file.c_str());
	return result;
}

/**
 * @brief Copy a directory (exit on failure)
 *
 * @param input_directory the input directory name
 * @param output_directory the output directory name
 */
void copy_directory(const std::string& input_directory, const std::string& output_directory)
{
	// Open the directory
	struct stat input_stat;
	struct dirent* ent;
	DIR* directory = opendir(input_directory.c_str());

	// Check opendir errors
	if (directory == NULL)
	{
		perror("opendir");
		exit(errno);
	}

	// Get informations from the source directory
	if (stat(input_directory.c_str(), &input_stat) < 0)
	{
		perror("stat");
		exit(errno);
	}

	// Create the output directory
	if (mkdir(output_directory.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH) < 0)
	{
		perror("mkdir");
		exit(errno);
	}

	// Copy permissions
	if (chmod(output_directory.c_str(), input_stat.st_mode) < 0)
	{
		perror("chmod");
		exit(errno);
	}

	// Get the first file/directory in the directory
	ent = readdir(directory);

	// Check readdir errors
	if (ent == NULL)
	{
		perror("readdir");
		exit(errno);
	}

	while (ent != NULL)
	{
		// Skip if the directory is . or ..
		if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
		{
			struct stat input_stat;
			char* input_name = path_join(input_directory, ent->d_name);
			char* output_name = path_join(output_directory, ent->d_name);
			stat(input_name, &input_stat);

			// Copy the file or the directory
			if (S_ISDIR(input_stat.st_mode))
				copy_directory(input_name, output_name);
			else
				copy_file(input_name, output_name);

			free(input_name);
			free(output_name);
		}

		// Get the next file/directory in the directory
		ent = readdir(directory);
	}

	closedir(directory);
}

/**
 * @brief Check if two paths are the same file
 *
 * @param input_file the input file path
 * @param output_file the output file path
 * @return 1 if they are the same and 0 if not
 */
int check_same_files(const char* input_file, const char* output_file)
{
	// Check if the output file exist
	struct stat output_stat;

	if (stat(output_file, &output_stat) < 0)
		return 0;

	// Get absolute paths
	char* input_str = realpath(input_file, NULL);
	char* output_str = realpath(output_file, NULL);

	// Check realpath errors
	if (input_str == NULL || output_str == NULL)
	{
		perror("realpath");
		exit(errno);
	}

	// Check if absolute paths are the same
	int result = 1 - strcmp(input_str, output_str);
	free(input_str);
	free(output_str);
	return result;
}

void Process::cp(const std::vector<std::string>& arguments)
{
	if (arguments.size() == 3)
	{
		// Check if source and destination are the sames
		if (check_same_files(arguments.at(1).c_str(), arguments.at(2).c_str()))
		{
			fprintf(stderr, "error: input file is output file\n");
			exit(1);
		}

		struct stat input_stat;
		stat(arguments.at(1).c_str(), &input_stat);

		// Copy the file/directory
		if (S_ISDIR(input_stat.st_mode))
			copy_directory(arguments.at(1), arguments.at(2));
		else
			copy_file(arguments.at(1), arguments.at(2));
	}

	else if (arguments.size() < 3)
		fprintf(stderr, "error: missing arguments\n");

	else if (arguments.size() > 3)
		fprintf(stderr, "error: too many arguments\n");
}


