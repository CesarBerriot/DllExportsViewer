//
// Created by César B. on 4/9/2024.
//

#include <cstring>
#include "utils/exceptions.hpp"
#include "utils/utils.hpp"

std::string utils::exception::what()
{	std::stringstream res;
	//GetError
	res << "utils exception thrown\n";
	res << "error code : " << error_code << '\n';
	res << "error message : " << error_message << '\n';
	if(clib_error_code)
	{	res << "clib error code : " << clib_error_code << '\n';
		res << "clib error message : " << strerror(clib_error_code) << '\n';
	}
	if(win32_error_code != ERROR_SUCCESS)
	{	res << "win32 api error code : " << win32_error_code << '\n';
		res << "win32 api error message : " << utils::win32_strerror(win32_error_code) << '\n';
	}
	return res.str();
}
