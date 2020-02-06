#include "button.hpp"

double count_distance(int x, int y, int x1, int y1, int x2, int y2) {
	double t = ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) / (pow(x2 - x1, 2) + pow(y2 - y1, 2));
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	double dist = sqrt(pow(x1 - x + (x2 - x1) * t, 2) + pow(y1 - y + (y2 - y1) * t, 2));
	return dist;
}
void ColorButton::draw() {
	setfillstyle(SOLID_FILL, c);
	bar(x1, y1, x2, y2);
	setcolor(BLACK);
	rectangle(x1, y1, x2, y2);
}
void ColorButton::press() {
	if (mousebuttons() == 1)
		SelectedColors::instance().set_fc(c);
	else
		SelectedColors::instance().set_bc(c);
}
void IconButton::draw() {
	setfillstyle(SOLID_FILL, WHITE);
	bar(x1, y1, x2, y2);
	putimage(x1 + 1, y1 + 1, image, COPY_PUT);
	setcolor(BLACK);
	rectangle(x1, y1, x2, y2);
}
void CommandButton::press() {
	action();
}
void ToolButton::press() {
	PaintArea::instance().set_tool(tool);
}
void delete_tool() {
	const int max_dist = 4;
	int x = mousex(),
		y = mousey();
	Figure* curr_fig;
	StatusLine::instance().set_xy(x - PaintArea::instance().get_left(), y - PaintArea::instance().get_top());
	for (int i = PaintArea::instance().figure_amount() - 1; i >= 0; i--) {
		curr_fig = PaintArea::instance().get_figure(i);
		if (typeid(*curr_fig) == typeid(Line)) {
			int x1, y1, x2, y2, c;
			curr_fig->get_data(x1, y1, x2, y2, c);
			double dist = count_distance(x, y, x1, y1, x2, y2);
			if (dist <= max_dist) {
				PaintArea::instance().delete_figure(i);
				PaintArea::instance().refresh();
				delay(200);
				break;
			}
		}
		if (typeid(*curr_fig) == typeid(Rect)) {
			int x1, y1, x2, y2, c;
			curr_fig->get_data(x1, y1, x2, y2, c);
			double dist1 = count_distance(x, y, x1, y1, x2, y1);
			double dist2 = count_distance(x, y, x2, y1, x2, y2);
			double dist3 = count_distance(x, y, x2, y2, x1, y2);
			double dist4 = count_distance(x, y, x1, y2, x1, y1);
			if (dist1 <= max_dist || dist2 <= max_dist || dist3 <= max_dist || dist4 <= max_dist) {
				PaintArea::instance().delete_figure(i);
				PaintArea::instance().refresh();
				delay(200);
				break;
			}
		}
		if (typeid(*curr_fig) == typeid(ColoredRect)) {
			int x1, y1, x2, y2, c, inner_c;
			ColoredRect* temp = dynamic_cast<ColoredRect*>(curr_fig);
			temp->get_data(x1, y1, x2, y2, c, inner_c);
			if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
				PaintArea::instance().delete_figure(i);
				PaintArea::instance().refresh();
				delay(200);
				break;
			}
		}
	}
}
void rect_shape(int x1, int y1, int x2, int y2) { //временное отображение прямоугольника
	setwritemode(XOR_PUT);
	setcolor(WHITE);
	rectangle(x1, y1, x2, y2);
	setwritemode(COPY_PUT);
}
void line_shape(int x1, int y1, int x2, int y2) { //временное отображение линии
	setwritemode(XOR_PUT);
	setcolor(WHITE);
	line(x1, y1, x2, y2);
	setwritemode(COPY_PUT);
}
bool elastic_tool(int& x1, int& y1, int& x2, int& y2, void (*shape)(int x1, int y1, int x2, int y2)) { //отвечает за динамическое изменение неготовой фигуры, shape редактирует текущие координаты дистанционно через указатели
	x1 = mousex();
	y1 = mousey();
	x2 = x1;
	y2 = y1;
	shape(x1, y1, x2, y2);
	while (1) {
		int b = mousebuttons();
		if (!b) break;
		int xn = mousex();
		int yn = mousey();
		if (!PaintArea::instance().in(xn, yn)) {
			shape(x1, y1, x2, y2);
			return 0;
		}
		if (xn != x2 || yn != y2) {
			shape(x1, y1, x2, y2);
			x2 = xn;
			y2 = yn;
			shape(x1, y1, x2, y2);
			StatusLine::instance().set_xy(x2 - PaintArea::instance().get_left(), y2 - PaintArea::instance().get_top());
		}
	}
	shape(x1, y1, x2, y2);
	return 1;
}
void line_tool() {
	int x1, y1, x2, y2;
	if (elastic_tool(x1, y1, x2, y2, line_shape)) {
		PaintArea::instance().create_figure("Line", x1, y1, x2, y2, SelectedColors::instance().get_fc());
	}
}
void rect_tool() {
	int x1, y1, x2, y2;
	if (elastic_tool(x1, y1, x2, y2, rect_shape)) {
		PaintArea::instance().create_figure("Rect", x1, y1, x2, y2, SelectedColors::instance().get_fc());
	}
}
void colloredrect_tool() {
	int x1, y1, x2, y2;
	if (elastic_tool(x1, y1, x2, y2, rect_shape)) {		
		PaintArea::instance().create_figure("ColoredRect", x1, y1, x2, y2, SelectedColors::instance().get_fc(), SelectedColors::instance().get_bc());
	}
}
void load_command() {
	StatusLine::instance().edit_name();
	PaintArea::instance().load(StatusLine::instance().get_name());
}
void save_command() {
	StatusLine::instance().edit_name();
	PaintArea::instance().save(StatusLine::instance().get_name());
}
