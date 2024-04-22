//
// Created by César B. on 4/8/2024.
//

#pragma once

#include <string>

namespace logic
{	void bind_ui_elements();
	void load_themes();
	void load_dll(std::string path);
	/// called post-initialization, runs the Qt application
	int run();
	/// json export
	void export_loaded_symbols_to_file(std::string path);
}
