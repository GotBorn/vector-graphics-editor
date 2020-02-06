#include "interface.hpp"

bool DrawObject::in(int x, int y) {
	return x > x1&& y > y1&& x < x2 && y < y2;
}
SelectedColors& SelectedColors::instance() {
	static SelectedColors sc(20, 120, 70, 170);
	return sc;
}
void SelectedColors::draw() {
	setfillstyle(SOLID_FILL, fc);
	bar(x1, y1, x2, y2);
	setfillstyle(SOLID_FILL, bc);
	int dx = (x2 - x1) / 4;
	int dy = (y2 - y1) / 4;
	bar(x1 + dx, y1 + dx, x2 - dx, y2 - dy);
	setcolor(BLACK);
	rectangle(x1, y1, x2, y2);
}
void SelectedColors::set_fc(int c) {
	fc = c;
	draw();
}
void SelectedColors::set_bc(int c) {
	bc = c;
	draw();
}
StatusLine& StatusLine::instance() {
	static StatusLine sl(100, 550, 780, 580);
	return sl;
}
void StatusLine::draw() {
	setfillstyle(SOLID_FILL, WHITE);
	bar(x1, y1, x2, y2);
	setcolor(BLACK);
	rectangle(x1, y1, x2, y2);
	string s = "X: " + to_string(x) + " Y: " + to_string(x) + (edit ? " Введите имя файла: " : " Файл: ") + name;
	setbkcolor(WHITE);
	outtextxy(x1 + 10, y1 + 5, s.c_str());
}
void StatusLine::set_xy(int x, int y) {
	if (x == this->x && y == this->y) return;
	this->x = x;
	this->y = y;
	draw();
}
void StatusLine::set_name(const string& n) {
	name = n;
	draw();
}
void StatusLine::edit_name() {
	int ch;
	edit = 1;
	while (1) {
		draw();
		ch = getch();
		if (ch == KEY_ENTER || ch == KEY_ESC) break;
		else if (ch == KEY_BACKSPACE && name.size() > 0) name.pop_back();
		else if (ch > ' ' && ch <= 'z') name += (char)ch;
	}
	edit = 0;
	draw();
}
PaintArea& PaintArea::instance() {
	static PaintArea pa(100, 20, 780, 550);
	return pa;
}
void PaintArea::draw() {
	setfillstyle(SOLID_FILL, WHITE);
	bar(x1, y1, x2, y2);
	setcolor(BLACK);
	rectangle(x1, y1, x2, y2);
}
void PaintArea::press() {
	tool();
}
void PaintArea::save(const string& name) {
	ofstream file(name);		
	for (int i = 0; i < storage.size(); i++) {
		if (typeid(*storage[i]) == typeid(ColoredRect)) {
			int x1, y1, x2, y2, c, inner_c;
			ColoredRect* temp = dynamic_cast<ColoredRect*>(storage[i]);
			temp->get_data(x1, y1, x2, y2, c, inner_c);
			file << "ColoredRect " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << c << " " << inner_c << endl;
		}
		else {
			int x1, y1, x2, y2, c;
			storage[i]->get_data(x1, y1, x2, y2, c);
			if (typeid(*storage[i]) == typeid(Rect)) {
				file << "Rect ";
			}
			else {
				file << "Line ";
			}
			file << x1 << " " << y1 << " " << x2 << " " << y2 << " " << c << endl;
		}
	}
	file.close();
}
void PaintArea::load(const string& name) {
	ifstream file(name);
	string str;
	int x_1, y_1, x_2, y_2, c, inner_c;
	if (!file.is_open()) {
		StatusLine::instance().set_name("Файл не найден");
	}
	storage.clear(); // очистка вектора от старых фигур
	draw(); // очистка экрана
	while (file >> str >> x_1 >> y_1 >> x_2 >> y_2 >> c) {
		if (str == "ColoredRect") {
			file >> inner_c;
		}
		create_figure(str, x_1, y_1, x_2, y_2, c, inner_c);
	}
	file.close();
}
void PaintArea::refresh() {
	draw();
	for (int i = 0; i < storage.size(); i++) {
		storage[i]->draw();
	}
}
void PaintArea::create_figure(string type, int x_1, int y_1, int x_2, int y_2, int c, int inner_c) {	
	if (type == "Line") {
		storage.push_back(new Line(x_1, y_1, x_2, y_2, c));
	}
	else {
		if (type == "Rect") {
			storage.push_back(new Rect(min(x_1, x_2), min(y_1, y_2), max(x_1, x_2), max(y_1, y_2), c));
		}
		else {
			storage.push_back(new ColoredRect(min(x_1, x_2), min(y_1, y_2), max(x_1, x_2), max(y_1, y_2), c, inner_c));
		}
	}
	storage.back()->show();
}
void Figure::hide() {
	if (!visible) return;
	setfillstyle(SOLID_FILL, WHITE);
	bar(x_1, y_1, x_2, y_2);
	visible = 0;
}
void Figure::show() {
	if (visible) return;
	visible = 1;
	draw();
}
void Figure::get_data(int& x_1, int& y_1, int& x_2, int& y_2, int& c) {
	x_1 = this->x_1;
	y_1 = this->y_1;
	x_2 = this->x_2;
	y_2 = this->y_2;
	c = this->c;
}
void Line::draw() {
	setcolor(get_color());
	line(x_1, y_1, x_2, y_2);
}
void Rect::draw() {
	setcolor(get_color());
	rectangle(x_1, y_1, x_2, y_2);
}
void ColoredRect::draw() {
	setfillstyle(SOLID_FILL, inner_c);
	bar(x_1, y_1, x_2, y_2);
	setcolor(get_color());
	rectangle(x_1, y_1, x_2, y_2);
}
void ColoredRect::get_data(int& x_1, int& y_1, int& x_2, int& y_2, int& c, int& inner_c) {
	x_1 = this->x_1;
	y_1 = this->y_1;
	x_2 = this->x_2;
	y_2 = this->y_2;
	c = this->c;
	inner_c = this->inner_c;
}
