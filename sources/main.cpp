#include <iostream>
#include <memory>

void add(const int& value_1, const int& value_2)
{
	std::cout << value_1;
}

int main()
{
	std::shared_ptr<int> ptr = std::make_shared<int>(5);

	std::cout << *ptr << std::endl;

	return 0;
}
