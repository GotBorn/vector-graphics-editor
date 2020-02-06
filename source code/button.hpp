#ifndef BUTTON_H
#define BUTTON_H

#include <cmath>
#include "graphics.h"
#include "interface.hpp"

// Базовый класс для кнопок управления
class Button : public ControlObject {
public:
	// конструктор получает координаты левого верхнего угла x,y
	Button(int x, int y) :ControlObject(x, y, x + 31, y + 31) {}
};
// Класс для кнопок выбора цвета
class ColorButton : public Button {
	int c; // цвет кнопки
public:
	// конструктор получает координаты x,y и номер цвета c
	ColorButton(int x, int y, int c) :Button(x, y), c(c) {}
	void draw(); // нарисовать объект
	void press(); // реакция на нажатие
};
// Класс для кнопок c картинкой
class IconButton : public Button {
	IMAGE* image; // загруженная картинка для кнопки
public:
	// конструктор получает координаты x,y, имя файла с картинкой icon
	IconButton(int x, int y, string icon) :Button(x, y), image(loadBMP(icon.c_str())) {}
	~IconButton() { freeimage(image); } // деструктор
	void draw(); // нарисовать объект
};
// Класс для кнопок выбора инструмента
class ToolButton : public IconButton {
	fun_ptr tool; // инструмент
public:
	// конструктор получает координаты x,y, имя файла с картинкой icon
	// и указатель на функцию-инструмент t
	ToolButton(int x, int y, string icon, fun_ptr t) :IconButton(x, y, icon), tool(t) {}
	void press(); // реакция на нажатие
};
// Класс для кнопок-команд
class CommandButton : public IconButton {
	fun_ptr action; // действие
public:
	// конструктор получает координаты x,y, имя файла с картинкой icon
	// и указатель на действие a
	CommandButton(int x, int y, string icon, fun_ptr a) :IconButton(x, y, icon), action(a) {}
	void press(); // реакция на нажатие
};

void delete_tool(); // инструмент для удаления
void line_tool(); // инструмент линия
void rect_tool(); // инструмент прямоугольник
void colloredrect_tool(); // инструмент закрашенный прямоугольник
void load_command(); // загрузка рисунка
void save_command(); // сохранение рисунка

#endif
