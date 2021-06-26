#include "utils/utils.hpp"

char*const* to_char_array(std::vector<std::string> string_array)
{
	char** result = new char*[string_array.size() + 1];
	int i = 0;

	for (const std::string& string : string_array)
	{
		result[i] = new char[string.size() + 1];
		strcpy(result[i], string.data());
		i++;
	}

	result[string_array.size()] = nullptr;
	return result;
}
