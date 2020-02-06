#include "graphics.h"
#include "interface.hpp"
#include "button.hpp"

Button* buttons[22];

void init_controls() {
	for (int i = 0; i < 16; ++i) {
		buttons[i] = new ColorButton(10 + (i / 8) * 35, 200 + (i % 8) * 35, i);
	}
	buttons[16] = new CommandButton(10, 5, "res/load.bmp", load_command);
	buttons[17] = new CommandButton(45, 5, "res/save.bmp", save_command);
	buttons[18] = new ToolButton(10, 40, "res/delete.bmp", delete_tool);
	buttons[19] = new ToolButton(45, 40, "res/line.bmp", line_tool);
	buttons[20] = new ToolButton(10, 75, "res/rect.bmp", rect_tool);
	buttons[21] = new ToolButton(45, 75, "res/frect.bmp", colloredrect_tool);
	PaintArea::instance().set_tool(line_tool);
}
void draw_controls() {
	setfillstyle(SOLID_FILL, WHITE);
	bar(0, 0, getmaxx(), getmaxy());
	for (int i = 0; i < 22; ++i) {
		buttons[i]->draw();
	}
	PaintArea::instance().draw();
	SelectedColors::instance().draw();
	StatusLine::instance().draw();
}
int main() {
	initwindow(800, 600);
	init_controls();
	draw_controls();
	int x, y;
	while (1) {
		if (mousebuttons()) {
			x = mousex();
			y = mousey();
			if (PaintArea::instance().in(x, y)) {
				PaintArea::instance().press();
			}
			else {
				for (int i = 0; i < 22; ++i) {
					if (buttons[i]->in(x, y)) {
						buttons[i]->press();
					}
				}
			}
		}
		else {
			x = mousex();
			y = mousey();
			if (PaintArea::instance().in(x, y)) {
				StatusLine::instance().set_xy(x - PaintArea::instance().get_left(), y - PaintArea::instance().get_top());
			}
		}
	}
	closegraph();
}
