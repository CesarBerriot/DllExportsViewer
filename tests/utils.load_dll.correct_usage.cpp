#include <iostream>
#include "utils/utils.hpp"
#include "utils/exceptions.hpp"

int main()
{	std::vector<utils::found_symbol_data> found_symbols;
	try
	{	found_symbols = utils::load_dll("C:/Windows/System32/XInput9_1_0.dll");
	}
	catch(utils::exception exception)
	{	std::cout << exception.what();
		return EXIT_FAILURE;
	}
	catch(...)
	{	std::cout << "unknown error";
		return EXIT_FAILURE;
	}
	for(auto symbol : found_symbols)
		std::cout << "found symbol : " << symbol.label << '\n';
	return EXIT_SUCCESS;
}