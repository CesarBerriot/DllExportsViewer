//
// Created by César B. on 4/8/2024.
//

#include "logic.hpp"
#include <libgen.h>
#include <QApplication>
#include <QShortcut>
#include <QMainWindow>
#include "ui/ui.hpp"
#include "utils/utils.hpp"
#include "globals/globals.hpp"
#include "io/io.hpp"
#include "json/json.hpp"

void logic::bind_ui_elements()
{	ui::main_window::menu_bar::file_menu::open_action_clicked_cb.bind([] { ui::create_dll_open_dialog(load_dll); });
	ui::main_window::menu_bar::file_menu::load_external_theme_action_clicked_cb.bind([] { ui::create_external_theme_open_dialog(ui::load_theme); });
	ui::main_window::menu_bar::themes_menu::theme_action_clicked_cb.bind(ui::load_theme);
	ui::main_window::menu_bar::help_menu::about_action_clicked_cb.bind(ui::show_about_dialog);
	ui::main_window::menu_bar::help_menu::shortcuts_info_action_clicked_cb.bind(ui::show_shortcuts_info_dialog);
	ui::main_window::menu_bar::help_context_mode_action_clicked_cb.bind(ui::enter_help_context_mode);
	ui::main_window::central_widget::export_button::export_button_clicked_cb.bind([] { ui::create_json_create_file_dialog(export_loaded_symbols_to_file); });
}

void logic::load_dll(std::string path)
{	data_structures::imported_dll_data imported_dll_data;
	imported_dll_data.dll_import_timestamp = time(nullptr);
	try
	{	imported_dll_data.symbols = utils::load_dll(path);
	}
	catch(std::string error_msg)
	{	ui::display_status(error_msg);
		return;
	}
	globals::imported_dll_data = imported_dll_data;
	ui::enable_export_button();
	ui::display_status(make_string("Loaded " << basename((char *)path.c_str()) << '.'));
	ui::refresh_list(imported_dll_data.symbols);
}

int logic::run()
{	return QApplication::exec();
}

void logic::export_loaded_symbols_to_file(std::string path)
{	io::save_text_to_file(path, json::encode_imported_dll_data(globals::imported_dll_data));
}

void logic::load_themes()
{	ui::load_theme(globals::default_theme);
	ui::load_theme_registry(globals::theme_registry);
}
