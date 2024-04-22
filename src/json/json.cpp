//
// Created by César B. on 4/11/2024.
//

#include "json.hpp"
#include <jansson.h>
#include <memory>

std::string json::encode_imported_dll_data(data_structures::imported_dll_data imported_dll_data)
{
#define set_value(element, name, type, value) \
	json_object_set_new(element, name, json_##type(value))
#define set_integer(element, name, value) set_value(element, name, integer, value)
#define set_string(element, name, value) set_value(element, name, string, value)

	// create the main object
	json_auto_t * object = json_object();

	// add timestamps
	set_integer(object, "dll_import_timestamp", imported_dll_data.dll_import_timestamp);
	set_integer(object, "json_export_timestamp", time(nullptr));

	// create the symbol array
	json_t * symbols = json_array();

	// add symbols to the array
	for(auto symbol_data : imported_dll_data.symbols)
	{	json_t * symbol = json_object();
		set_string(symbol, "label", symbol_data.label.c_str());
		set_integer(symbol, "address", symbol_data.address);
		set_integer(symbol, "size", symbol_data.size);
		set_integer(symbol, "base_address", symbol_data.base_address);
		set_string(symbol, "tag", symbol_data.tag.c_str());
		set_integer(symbol, "value", symbol_data.value);
		set_integer(symbol, "register", symbol_data.register_value);
		set_integer(symbol, "scope", symbol_data.scope);
		json_array_append_new(symbols, symbol);
	}

	// add the symbol array
	json_object_set_new(object, "symbols", symbols);

	// dump and return
	auto dump = std::shared_ptr<char>(json_dumps(object, JSON_INDENT(4) | JSON_PRESERVE_ORDER), free);
	return dump.get();
}
