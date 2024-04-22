#include "ui/ui.hpp"
#include "logic/logic.hpp"

int main()
{	ui::init();
	logic::bind_ui_elements();
	logic::load_themes();
	return logic::run();
}
