#ifndef INTERFACE_H
#define INTERFACE_H

#include "graphics.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;


class DrawObject { // класс для объектов на экране
protected:
	int x1, y1, x2, y2; // координаты углов прямоугольника, в котором находится объект
public:
	DrawObject(int x1, int y1, int x2, int y2) :x1(x1), y1(y1), x2(x2), y2(y2) {} // конструктор получает координаты левого верхнего угла x1,y1 и координаты правого нижнего угла x2,y2
	virtual ~DrawObject() {} // деструктор
	bool in(int x, int y); // координаты x, y внутри объекта
	int get_top() { return y1 + 1; } // координаты
	int get_left() { return x1 + 1; } // верхнего угла
	int get_height() { return y2 - y1 - 1; } // высота
	int get_width() { return x2 - x1 - 1; } // ширина
	virtual void draw() = 0; // нарисовать объект
};

class SelectedColors : public DrawObject { // класс для отображения текущих цветов рисования
	int fc, // цвет рисования
		bc; // цвет закраски

	SelectedColors(int x1, int y1, int x2, int y2) : // конструктор получает координаты углов x1,y1,x2,y2
		DrawObject(x1, y1, x2, y2), fc(BLACK), bc(WHITE) {}
public:
	static SelectedColors& instance(); // объект для текущих цветов
	void draw(); // нарисовать объект
	void set_fc(int c); // установить цвет рисования
	int get_fc() { return fc; } // получить  цвет рисования
	void set_bc(int c); // установить цвет закраски
	int get_bc() { return bc; } // получить цвет закраски
};

class StatusLine : public DrawObject { // класс для отображения текущих координат мыши и имени файла
	int x, y; // координаты мыши
	bool edit;
	string name; // имя файла
	StatusLine(int x1, int y1, int x2, int y2) : // конструктор получает координаты углов x1,y1,x2,y2
		DrawObject(x1, y1, x2, y2), x(0), y(0), name("new.vec") {}
public:
	static StatusLine& instance(); // строка состояния
	void draw(); // нарисовать объект
	void set_xy(int x, int y); // изменить текущие координаты
	void set_name(const string& name); // установить имя файла
	string get_name() { return name; } // получить имя файла
	void edit_name(); // ввести имя файла
};

class ControlObject : public DrawObject { // класс для объектов, реагирующих на мышь
public:
	ControlObject(int x1, int y1, int x2, int y2) :DrawObject(x1, y1, x2, y2) {} // конструктор получает координаты углов x1,y1,x2,y2
	virtual void press() = 0; // реакция на нажатие
};

typedef void (*fun_ptr)(); // вспомогательный тип - указатель на функцию

class Figure {
	bool visible = 0; // видимость
protected:
	int x_1, y_1, //точки для построения
		x_2, y_2;
	int c; //цвет
public:
	Figure(int x_1, int y_1, int x_2, int y_2, int c) : x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2), c(c) {}
	virtual ~Figure() {}
	int get_color() const { return c; } //получить цвет
	void hide(); //спрятать
	void show(); //показать фигуру
	bool isvisible() const { return visible; } //видима?
	virtual void get_data(int& x1, int& y1, int& x2, int& y2, int& c); // получение точек и цветов фигуры
	virtual void draw() = 0; // метод для отрисовки 
};

class Rect : public Figure {
public:
	Rect(int x_1, int y_1, int x_2, int y_2, int c)
		:Figure(x_1, y_1, x_2, y_2, c) {}
	~Rect() { hide(); }
	void draw(); // метод для отрисовки 
};

class ColoredRect : public Rect {
protected:
	int inner_c; // цвет заливки
public:
	ColoredRect(int x_1, int y_1, int x_2, int y_2, int c, int inner_c)
		:Rect(x_1, y_1, x_2, y_2, c), inner_c(inner_c) {}
	~ColoredRect() { hide(); }
	void get_data(int& x_1, int& y_1, int& x_2, int& y_2, int& c, int& inner_c); // получение точек и цветов закрашенного прямоугольника
	void draw(); // метод для отрисовки 
};



class PaintArea : public ControlObject { // класс для поля рисования
	vector <Figure*> storage; // хранилище фигур
	fun_ptr tool; // текущий инструмент
	PaintArea(int x1, int y1, int x2, int y2) : // конструктор получает координаты углов x1,y1,x2,y2
		ControlObject(x1, y1, x2, y2), tool(nullptr) {}
public:
	static PaintArea& instance(); // поле для рисования
	void draw(); // нарисовать само поле 
	void create_figure(string type, int x_1, int y_1, int x_2, int y_2, int c, int inner_c = -1); // создание фигуры
	void press(); // реакция на нажатие
	void set_tool(fun_ptr t) { tool = t; } // установить инструмент рисования
	fun_ptr get_tool() { return tool; } // получить текущий инструмент рисования
	void save(const string& name); // сохранить изображение
	void load(const string& name); // загрузить изображение
	Figure* get_figure(int i) { return storage[i]; }; // доступ к i-ой фигуре
	int figure_amount() { return storage.size(); } // возвращает кол-во фигур
	void delete_figure(int i) { storage.erase(storage.begin() + i); } // удалить фигуру из вектора
	void refresh(); // обновить содержимое экрана
};

class Line : public Figure { // класс для отрезка
public:
	Line(int x_1, int y_1, int x_2, int y_2, int c)
		:Figure(x_1, y_1, x_2, y_2, c) {}
	~Line() { hide(); }
	void draw(); // метод для отрисовки 
};
#endif
