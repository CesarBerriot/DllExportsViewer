//
// Created by César B. on 4/8/2024.
//

#pragma once

class QApplication;
class QMainWindow;
class QMenuBar;
class QMenu;
class QAction;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QTableWidget;
class QStatusBar;
class QStatusBar;
class QPushButton;
class QSpacerItem;
class QDialog;

#include <string>
#include "templates/delegate.hpp"
#include "utils/utils.hpp"

namespace ui
{	void init();
	inline QApplication * application;
	namespace main_window
	{	void init();
		namespace size { enum { w = 600, h = 400 }; }
		inline QMainWindow * main_window;
		namespace menu_bar
		{	void init();
			inline QMenuBar * menu_bar;
			namespace file_menu
			{	void init();
				inline QMenu * file_menu;
				inline QAction * open_action;
				inline QAction * load_external_theme_action;
				inline templates::delegate<void()> open_action_clicked_cb;
				inline templates::delegate<void()> load_external_theme_action_clicked_cb;
			}
			namespace themes_menu
			{ 	void init();
				void clear();
				void create_category_menu(QMenu * parent, data_structures::theme_registry_category);
				void create_item_action(QMenu * parent, data_structures::theme_registry_item);
				inline QMenu * themes_menu;
				inline templates::delegate<void(std::string resource_path)> theme_action_clicked_cb;
			}
			namespace help_menu
			{	void init();
				inline QMenu * help_menu;
				inline QAction * about_action;
				inline QAction * shortcuts_info_action;
				inline templates::delegate<void()> about_action_clicked_cb;
				inline templates::delegate<void()> shortcuts_info_action_clicked_cb;
			}
			inline QAction * help_context_mode_action;
			inline templates::delegate<void()> help_context_mode_action_clicked_cb;
		}
		namespace central_widget
		{	void init();
			inline QWidget * central_widget;
			inline QVBoxLayout * layout;
			namespace exports_list
			{	void init();
				inline QWidget * exports_list;
				inline QVBoxLayout * layout;
				inline QLabel * label;
				inline QTableWidget * list;
			}
			namespace export_button
			{	void init();
				inline QHBoxLayout * layout;
				/**
				 * since the export button has multiple shortcuts and
				 * those are not supported by QPushButton, they're bound
				 * to an action that triggers a button click instead
				 */
				inline QAction * shortcuts_action;
				inline QPushButton * export_button;
				inline QSpacerItem * spacer;
				inline templates::delegate<void()> export_button_clicked_cb;
			}
		}
		namespace status_bar
		{	void init();
			inline QStatusBar * status_bar;
			void display(std::string);
		}
	}
	namespace about_dialog
	{	void init();
		inline QDialog * about_dialog;
		inline QVBoxLayout * layout;
		inline QLabel * label;
	}
	namespace shortcuts_info_dialog
	{	void init();
		inline QDialog * shortcuts_info_dialog;
		inline QVBoxLayout * layout;
	}
	inline void display_status(std::string info) { main_window::status_bar::display(info); }
	void create_dll_open_dialog(void(*callback)(std::string));
	void create_external_theme_open_dialog(void(*callback)(std::string));
	void refresh_list(std::vector<data_structures::imported_symbol_data>);
	void enter_help_context_mode();
	void show_about_dialog();
	void show_shortcuts_info_dialog();
	void create_json_create_file_dialog(void(*callback)(std::string));
	void enable_export_button();
	void load_theme(std::string resource_path);
	void load_theme_registry(data_structures::theme_registry);
}
