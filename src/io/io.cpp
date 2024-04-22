//
// Created by César B. on 4/11/2024.
//

#include "io.hpp"
#include <ui/ui.hpp>
#include <libgen.h>

void io::save_text_to_file(std::string path, std::string text)
{	FILE * file = nullptr;
	try
	{	using namespace std::string_literals;
		// check path len
		if(!path.length())
			throw "Empty file."s;
		// obtain dir path and file name separately
		char * buffer = strdup(path.c_str());
		std::string dir_path = dirname(buffer);
		memcpy(buffer, path.c_str(), path.length() + 1);
		std::string file_name = basename(buffer);
		free(buffer);
		if(!dir_path.length() || !file_name.length())
			throw make_string("Invalid path '" << path.c_str() << '\'');
		// open for writing
		file = fopen(path.c_str(), "w");
		if(!file)
			throw make_string("Failed to open file '" << file_name << "' for writing with error \"" << strerror(errno) << '"');
		// write the data to the file
		size_t fwrite_res = fwrite(text.c_str(), 1, text.length(), file);
		if(fwrite_res != text.length())
			throw make_string("Failed to write file '" << file_name << "' with error \"" << strerror(errno) << '"');
		// close the file
		int fclose_res = fclose(file);
		assert(!fclose_res);
		// display success status
		ui::display_status(make_string("File '" << file_name << "' saved to directory \"" << dir_path << '"'));
	}
	catch(std::string exception)
	{	if(file)
		{	int fclose_res = fclose(file);
			assert(!fclose_res);
		}
		ui::display_status(make_string("File save error : " << exception));
	}
}
