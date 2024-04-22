//
// Created by César B. on 4/8/2024.
//

#include "utils/utils.hpp"
#include <libgen.h>
#include <cstring>
#include <windows.h>
#include <dbghelp.h>
#include "utils/exceptions.hpp"
#include "ui/ui.hpp"

std::vector<data_structures::imported_symbol_data> utils::load_dll(std::string path)
{	std::vector<data_structures::imported_symbol_data> res;
	try
	{	// check path len
		ensure_or_throw(
			path.size(),
			load_dll_errors::empty_path_name,
			"Empty path name."
		);

		// obtain file name
		char * file_name = basename((char *)path.c_str());
		ensure_or_throw(
			file_name,
			load_dll_errors::clib_basename_failed,
			make_string("basename() failed with error : " << strerror(errno))
		);

		// ensure the file is a dll
		size_t file_name_len = strlen(file_name);
		std::string expected_file_extension = ".dll";
		ensure_or_throw(
			// file name is long enough to fit ".dll"
			file_name_len >= expected_file_extension.length() &&
			// file name ends with ".dll"
			!expected_file_extension.compare(file_name + file_name_len - expected_file_extension.length()),
			load_dll_errors::invalid_file_extension,
			"Not a Dll."
		);

		// get the current process
		HANDLE hProcess = GetCurrentProcess();
		ensure_or_throw(hProcess, load_dll_errors::syscall_GetCurrentProcess_failed, "Failed to obtain a handle to the current process");

		// initialize the symbol loader for this process
		bool SymInitialize_res = SymInitialize(hProcess, nullptr, false);
		ensure_or_throw(SymInitialize_res, load_dll_errors::syscall_SymInitialize_failed, "Failed to initialize the symbol loader.");

		// load the dll at the given path as a module within this process
		DWORD64 dll_loaded_symbols_base_address =
			SymLoadModuleEx(hProcess, NULL, path.c_str(), NULL, 0, 0, NULL, 0);
		ensure_or_throw(
			dll_loaded_symbols_base_address,
			load_dll_errors::syscall_SymLoadModuleEx_failed,
			make_string("Failed to load symbols from file '" << file_name << "'.")
		);

		// enumerate loaded dll symbols
		// *INDENT-OFF*
		bool SymEnumSymbols_res = SymEnumSymbols(
			hProcess,
			dll_loaded_symbols_base_address,
			"*",
			[](PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
			{	auto * res_ptr= (std::vector<data_structures::imported_symbol_data>*)UserContext;
				res_ptr->push_back(utils::translate_win32_symbol(*pSymInfo));
				return TRUE;
			},
			&res
		);
		// *INDENT-ON*
		ensure_or_throw(
			SymEnumSymbols_res,
			load_dll_errors::syscall_SymEnumSymbols_failed,
			make_string("Failed to enumerate loaded dll symbols from file '" << file_name << "'.")
		);

		// symbol loader cleanup
		bool SymCleanup_res = SymCleanup(hProcess);
		ensure_or_throw(
			SymCleanup_res,
			load_dll_errors::syscall_SymCleanup_failed,
			make_string("Failed to clean up symbol loader data.")
		);
	}
	catch(utils::exception & exception)
	{	// cleanup the symbol loader if symbol loading failed
		if(exception.error_code == load_dll_errors::syscall_SymLoadModuleEx_failed ||
		   exception.error_code == load_dll_errors::syscall_SymEnumSymbols_failed)
		{	bool r = SymCleanup(GetCurrentProcess());
			assert(r);
		}
		throw exception;
	}
	return res;
}

std::string utils::win32_strerror(DWORD error_code)
{	char * buffer;
	size_t buflen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								   NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char *)&buffer, 0, NULL);
	assert(buflen);
	std::string res = buffer;
	HLOCAL LocalFree_res = LocalFree(buffer);
	assert(!LocalFree_res);
	return res;
}

std::string utils::symbol_tag_to_string(ULONG tag)
{	/**
	* @note
	* the SymTagEnum enum is just missing from mingw for some reason.
	* I couldn't find any information about why that might be.
	* normally you just include <cvconst.h>, or define _NO_CVCONST_H
	* before including <DbgHelp.h> on modern msvc.
	* I wish I didn't have to do this.
	* Here's the official doc :
	* https://learn.microsoft.com/en-us/visualstudio/debugger/debug-interface-access/symtagenum
	*/
	int counter = -1;
#define _(x) \
	++counter; \
	if(tag == counter) \
		return #x;
	_(Null)_(Exe)_(Compiland)_(CompilandDetails)_(CompilandEnv)_(Function)_(Block)_(Data)
	_(Annotation)_(Label)_(PublicSymbol)_(UDT)_(Enum)_(FunctionType)_(PointerType)_(ArrayType)
	_(BaseType)_(Typedef)_(BaseClass)_(Friend)_(FunctionArgType)_(FuncDebugStart)_(FuncDebugEnd)
	_(UsingNamespace)_(VTableShape)_(VTable)_(Custom)_(Thunk)_(CustomType)_(ManagedType)
	_(Dimension)_(CallSite)_(InlineSite)_(BaseInterface)_(VectorType)_(MatrixType)_(HLSLType)
	_(Caller)_(Callee)_(Export)_(HeapAllocationSite)_(CoffGroup)
#undef _
	return "Unknown";
}

data_structures::imported_symbol_data utils::translate_win32_symbol(SYMBOL_INFO & symbol)
{
	return
	{
		.label = symbol.Name,
		.address = symbol.Address,
		.size = symbol.Size,
		.base_address = symbol.ModBase,
		.tag = symbol_tag_to_string(symbol.Tag),
		.value = symbol.Value,
		.scope = symbol.Scope
	};
}
