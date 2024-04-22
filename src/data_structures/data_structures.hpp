//
// Created by César B. on 4/11/2024.
//

#pragma once

#include <cstdint>
#include <windows.h>
#include <dbghelp.h>
#include <vector>
#include <cstdint>

namespace data_structures
{	struct imported_symbol_data
	{	std::string label;
		uintptr_t address;
		uint32_t size;
		uintptr_t base_address;
		std::string tag;
		uint64_t value;
		uint32_t register_value;
		uint32_t scope;
	};
	struct imported_dll_data
	{	// unix time stamp, in seconds
		uint64_t dll_import_timestamp;
		std::vector<imported_symbol_data> symbols;
	};
	struct theme_registry_item { std::string label; std::string resource_path; };
	struct theme_registry_category
	{	std::string label;
		std::vector<theme_registry_category> sub_categories;
		std::vector<theme_registry_item> items;
	};
	struct theme_registry
	{
		std::vector<theme_registry_category> categories;
		std::vector<theme_registry_item> global_items;
	};
}
