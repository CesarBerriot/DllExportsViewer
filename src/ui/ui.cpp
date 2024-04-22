//
// Created by César B. on 4/8/2024.
//

#include "ui.hpp"
#include <QStatusBar>
#include <QMenuBar>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QSpacerItem>
#include <QHeaderView>
#include <QWhatsThis>
#include <vector>
#include "utils/exceptions.hpp"

void ui::init()
{	int argc = 0;
	application = new QApplication(argc, nullptr);
	main_window::init();
	about_dialog::init();
	shortcuts_info_dialog::init();
}

void ui::create_dll_open_dialog(void (*callback)(std::string))
{	QString result = QFileDialog::getOpenFileName(ui::main_window::main_window, "Open a Dll", QDir::homePath(), "*.dll");
	if(result.length())
		callback(result.toStdString());
	else
		display_status("Dll import file dialog cancelled.");
}

void ui::refresh_list(std::vector<data_structures::imported_symbol_data> found_symbols)
{	auto * list = main_window::central_widget::exports_list::list;
	list->clear();

	struct column_data { std::string label; std::string description; };
	std::vector<column_data> columns =
	{	{ "Label", "The exported name of a symbol." },
		{ "Address", "The address of a constant value symbol." },
		{ "Size", "The symbol size, in bytes (or bits, if the symbol is a bitfield member)." },
		// @todo make the markdown work
		{ "Base Addr.", "The base address of a symbol's export table." },
		{ "Tag", "PDB classification. See [Official Microsoft Documentation](https://learn.microsoft.com/en-us/visualstudio/debugger/debug-interface-access/symtagenum)." },
		{ "Value", "The value of a variable symbol." },
		{ "Register", "I ain't got no idea wtf that is." },
		{ "Scope", "Debug Interface Access Scope" },
	};
	// set the row count
	list->setColumnCount(columns.size());
	list->setRowCount(fmax(found_symbols.size(), 25));
	// set column names
	QStringList horizontal_header_labels;
	for(auto column_data : columns)
		horizontal_header_labels.push_back(column_data.label.c_str());
	list->setHorizontalHeaderLabels(horizontal_header_labels);
	// set columns what's this and tooltips
	for(int i = 0; i < columns.size(); ++i)
	{	auto * item = list->horizontalHeaderItem(i);
		auto column_data = columns[i];
		item->setWhatsThis(column_data.description.c_str());
		item->setToolTip(column_data.description.c_str());
	}
	// fill rows
	for(int i = 0; i < found_symbols.size(); ++i)
	{	data_structures::imported_symbol_data symbol = found_symbols[i];
		int column_counter = -1;
#define push_item(format, ...) ++column_counter; list->setItem(i, column_counter, new QTableWidgetItem(QString().asprintf(format, __VA_ARGS__)))
		push_item("%s", symbol.label.c_str());
		push_item("%p", (void *)symbol.address);
		push_item("%ib", symbol.size);
		push_item("%p", (void *)symbol.base_address);
		push_item("%s", symbol.tag.c_str());
		push_item("%lli", symbol.value);
		push_item("%i", symbol.register_value);
		push_item("%i", symbol.scope);
#undef push_item
	}
	// compute column widths to fit contents
	list->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void ui::enter_help_context_mode()
{	QWhatsThis::enterWhatsThisMode();
}

void ui::show_about_dialog()
{	about_dialog::about_dialog->show();
}

void ui::create_json_create_file_dialog(void (* callback)(std::string))
{	QString result = QFileDialog::getSaveFileName(ui::main_window::main_window, "Export to json", QDir::homePath(), "*.json");
	if(result.length())
		callback(result.toStdString());
	else
		display_status("Json export file dialog cancelled.");
}

void ui::enable_export_button()
{	main_window::central_widget::export_button::export_button->setEnabled(true);
}

void ui::show_shortcuts_info_dialog()
{	shortcuts_info_dialog::shortcuts_info_dialog->show();
}

void ui::load_theme(std::string resource_path)
{	if(!resource_path.length())
	{	ui::main_window::main_window->setStyleSheet("");
		return;
	}
	QFile theme = QFile(resource_path.c_str());
	if(!theme.exists())
		throw utils::exception(EXIT_FAILURE, make_string("Failed to load a theme : '" << resource_path << '\''));
	theme.open(QIODevice::ReadOnly);
	main_window::main_window->setStyleSheet(theme.readAll());
	theme.close();
}

void ui::load_theme_registry(data_structures::theme_registry theme_registry)
{	namespace themes_menu = main_window::menu_bar::themes_menu;
	themes_menu::clear();
	for(auto category : theme_registry.categories)
		themes_menu::create_category_menu(themes_menu::themes_menu, category);
	for(auto item : theme_registry.global_items)
		themes_menu::create_item_action(themes_menu::themes_menu, item);
}

void ui::create_external_theme_open_dialog(void (* callback)(std::string))
{	QString result = QFileDialog::getOpenFileName(ui::main_window::main_window, "External Theme", QDir::homePath(), "*.qss");
	if(result.length())
		callback(result.toStdString());
	else
		display_status("External Theme import file dialog cancelled.");
}

void ui::main_window::init()
{	// main_window
	main_window = new QMainWindow;
	main_window->show();
	main_window->resize(size::w, size::h);
	// menu bar
	menu_bar::init();
	main_window->setMenuBar(menu_bar::menu_bar);
	// central widget
	central_widget::init();
	main_window->setCentralWidget(central_widget::central_widget);
	// status bar
	status_bar::init();
	main_window->setStatusBar(status_bar::status_bar);
}

void ui::main_window::menu_bar::init()
{	menu_bar = new QMenuBar;
	// file
	file_menu::init();
	menu_bar->addMenu(file_menu::file_menu);
	// themes
	themes_menu::init();
	menu_bar->addMenu(themes_menu::themes_menu);
	// help
	help_menu::init();
	menu_bar->addMenu(help_menu::help_menu);
	// ?
	help_context_mode_action = new QAction("?");
	QObject::connect(help_context_mode_action, &QAction::triggered, [] { help_context_mode_action_clicked_cb(); });
	menu_bar->addAction(help_context_mode_action);
}

void ui::main_window::central_widget::init()
{	central_widget = new QWidget;
	layout = new QVBoxLayout;
	central_widget->setLayout(layout);
	exports_list::init();
	layout->addWidget(exports_list::exports_list);
	export_button::init();
	layout->addLayout(export_button::layout);
	assert(layout->count());
}

void ui::main_window::status_bar::init()
{	status_bar = new QStatusBar;
	display("Ready. Select File -> Open to display a DLL's exports list.");
}

void ui::main_window::status_bar::display(std::string info)
{	status_bar->showMessage(QString::fromStdString(info));
}

void ui::main_window::menu_bar::file_menu::init()
{	file_menu = new QMenu("&File");
	// open
	open_action = new QAction("&Open");
	open_action->setShortcut(QKeySequence("Ctrl+O"));
	open_action->setShortcutVisibleInContextMenu(true);
	open_action->setWhatsThis("Open a Dll file for exploration.");
	QObject::connect(open_action, &QAction::triggered, [] { open_action_clicked_cb(); });
	file_menu->addAction(open_action);
	// load external theme
	load_external_theme_action = new QAction("&Load External Theme");
	load_external_theme_action->setShortcut(QKeySequence("Ctrl+T"));
	load_external_theme_action->setWhatsThis("Load an external theme from user storage.");
	QObject::connect(load_external_theme_action, &QAction::triggered, [] { load_external_theme_action_clicked_cb(); });
	file_menu->addAction(load_external_theme_action);
}

void ui::main_window::menu_bar::help_menu::init()
{	help_menu = new QMenu("&Help");
	// about
	about_action = new QAction("&About");
	QObject::connect(about_action, &QAction::triggered, [] { about_action_clicked_cb(); });
	help_menu->addAction(about_action);
	// shortcuts info
	shortcuts_info_action = new QAction("&Shortcuts");
	QObject::connect(shortcuts_info_action, &QAction::triggered, [] { shortcuts_info_action_clicked_cb(); });
	help_menu->addAction(shortcuts_info_action);
}

void ui::main_window::central_widget::exports_list::init()
{	// container widget
	exports_list = new QWidget;
	// layout
	layout = new QVBoxLayout;
	exports_list->setLayout(layout);
	// label
	label = new QLabel("Exports List");
	layout->addWidget(label);
	// list
	list = new QTableWidget;
	refresh_list({});
	list->verticalHeader()->setVisible(false);
	list->setEditTriggers(QAbstractItemView::NoEditTriggers);
	list->setSortingEnabled(true);
	layout->addWidget(list);
	// list horizontal header
	QHeaderView * header = list->horizontalHeader();
	//header->setSortIndicatorShown(true);
	header->setSectionResizeMode(QHeaderView::Interactive);
	header->resizeSection(0, 175);
	for(int i = 1; i < header->count(); ++i)
		header->resizeSection(i, header->sectionSize(i) + 5);
}

void ui::main_window::central_widget::export_button::init()
{	// layout
	layout = new QHBoxLayout;
	layout->setContentsMargins(10, 0, 0, 0);
	// button
	export_button = new QPushButton("Export to json");
	export_button->setEnabled(false);
	export_button->setWhatsThis("Export the displayed data to a json file.");
	QObject::connect(export_button, &QPushButton::clicked, [] { export_button_clicked_cb(); });
	layout->addWidget(export_button);
	// shortcuts action
	shortcuts_action = new QAction;
	main_window->addAction(shortcuts_action);
	shortcuts_action->setShortcuts({ { "Ctrl+E" }, { "Ctrl+S" } });
	QObject::connect(shortcuts_action, &QAction::triggered, [] { export_button->click(); });
	// spacer
	spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
	layout->addItem(spacer);
}

void ui::about_dialog::init()
{	about_dialog = new QDialog(main_window::main_window);
	about_dialog->setModal(true);
	layout = new QVBoxLayout;
	label = new QLabel;
	layout->addWidget(label);
	about_dialog->setLayout(layout);
	// dialog
	about_dialog->setContentsMargins(QMargins() + 10);
	about_dialog->setWindowTitle("About");
	// layout
	layout->setSizeConstraint(QLayout::SetFixedSize);
	// label
	label->setOpenExternalLinks(true);
	label->setTextFormat(Qt::MarkdownText);
	label->setText(
		"# DllExportsViewer  \n"
		"Made with ♥ by César Berriot  \n"
		"<a style=color:blue; href=\"https://www.linkedin.com/in/c%C3%A9sar-berriot/\">LinkedIn</a> • "
		"<a style=color:blue; href=\"https://github.com/CesarBerriot\">Github</a> • "
		"<a style=color:blue; href=\"https://github.com/CesarBerriot/DllExportsViewer\">This project</a>"
	);
}

void ui::shortcuts_info_dialog::init()
{	shortcuts_info_dialog = new QDialog(main_window::main_window);
	shortcuts_info_dialog->setModal(true);
	layout = new QVBoxLayout;
	shortcuts_info_dialog->setLayout(layout);
	// dialog
	shortcuts_info_dialog->setContentsMargins(QMargins() + 10);
	shortcuts_info_dialog->setWindowTitle("Shortcuts");
	// layout
	layout->setSizeConstraint(QLayout::SetFixedSize);
	// shortcuts
#define add_shortcut(label, key_sequence) \
	do \
	{	QHBoxLayout * shortcut_layout = new QHBoxLayout; \
		shortcut_layout->addWidget(new QLabel(label)); \
		shortcut_layout->addItem(new QSpacerItem(70, 0, QSizePolicy::MinimumExpanding)); \
		shortcut_layout->addWidget(new QLabel(key_sequence)); \
		layout->addLayout(shortcut_layout); \
	} while(false)
	add_shortcut("Open Dll for examination", "Ctrl + O");
	add_shortcut("Export to json", "Ctrl + E, Ctrl + S");
	add_shortcut("Load theme from user storage", "Ctrl + T");
}

void ui::main_window::menu_bar::themes_menu::init()
{	themes_menu::themes_menu = new QMenu("&Themes");
}

void ui::main_window::menu_bar::themes_menu::create_category_menu(QMenu * parent, data_structures::theme_registry_category category)
{	QMenu * menu = new QMenu(make_string('&' << category.label).c_str());
	for(auto sub_category : category.sub_categories)
		create_category_menu(menu, sub_category);
	for(auto item : category.items)
		create_item_action(menu, item);
	parent->addMenu(menu);
}

void ui::main_window::menu_bar::themes_menu::create_item_action(QMenu * parent, data_structures::theme_registry_item item)
{	QAction * action = new QAction(make_string('&' << item.label).c_str());
	QObject::connect(action, &QAction::triggered, [item] { theme_action_clicked_cb(item.resource_path); });
	parent->addAction(action);
}

void ui::main_window::menu_bar::themes_menu::clear()
{	themes_menu->clear();
}
