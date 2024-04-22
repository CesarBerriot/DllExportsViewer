//
// Created by César B. on 4/8/2024.
//

#pragma once

#include <minwindef.h>
#include <winerror.h>

#define ensure_or_throw(expression, error_code, message) \
	do \
	{ \
		if(!(expression)) \
			throw utils::exception(utils::error_code, message, errno, GetLastError()); \
	} while(false)

namespace utils
{	class exception : std::exception
	{	public:
			/// error code
			int error_code;
			/// error message
			std::string error_message;
			/// optional, set to errno when the error is due to a failed clib call
			errno_t clib_error_code;
			/// optional, set to errno when the error is due to a failed clib call
			DWORD win32_error_code;
			exception(int error_code, std::string error_message,
					  errno_t clib_error_code = 0, DWORD win32_error_code = ERROR_SUCCESS)
				: error_code(error_code), error_message(error_message),
				  clib_error_code(clib_error_code), win32_error_code(win32_error_code) {}
			std::string what();
	};
	namespace load_dll_errors
	{	enum
		{	empty_path_name,
			clib_basename_failed,
			invalid_file_extension,
			syscall_GetCurrentProcess_failed,
			syscall_SymInitialize_failed,
			syscall_SymLoadModuleEx_failed,
			syscall_SymEnumSymbols_failed,
			syscall_SymCleanup_failed,
		};
	}
}
