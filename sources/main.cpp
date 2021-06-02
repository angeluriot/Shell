#include "Process.hpp"
#include "Job.hpp"
#include "Shell.hpp"

int main(int argc, char** argv, char** env)
{
	Shell::init();

	std::cout << "test" << std::endl;

	return 0;
}
