//
// Created by César B. on 4/8/2024.
//

#pragma once

#include <windows.h>
#include <dbghelp.h>
#include <vector>
#include "data_structures/data_structures.hpp"

namespace utils
{	/// throws utils::exception on error with a code within utils::load_dll_errors
	std::vector<data_structures::imported_symbol_data> load_dll(std::string path);
	/// strerror for win32 api errors
	std::string win32_strerror(DWORD);
	std::string symbol_tag_to_string(ULONG);
	/// SYMBOL_INFO has to be passed by reference due to the way SYMBOL_INFO::Name is stored
	data_structures::imported_symbol_data translate_win32_symbol(SYMBOL_INFO &);
}
