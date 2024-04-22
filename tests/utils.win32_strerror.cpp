#include <iostream>
#include <windows.h>
#include "utils/utils.hpp"

int main()
{	std::cout << utils::win32_strerror(ERROR_SUCCESS);
	std::string lfkj = utils::win32_strerror(ERROR_SUCCESS);
	char const * ntm = lfkj.c_str();
	return utils::win32_strerror(ERROR_SUCCESS).compare("The operation completed successfully.\r\n")
		   ? EXIT_FAILURE
		   : EXIT_SUCCESS;
}