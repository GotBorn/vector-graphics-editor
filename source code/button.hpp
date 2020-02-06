#ifndef BUTTON_H
#define BUTTON_H

#include <cmath>
#include "graphics.h"
#include "interface.hpp"

// ������� ����� ��� ������ ����������
class Button : public ControlObject {
public:
	// ����������� �������� ���������� ������ �������� ���� x,y
	Button(int x, int y) :ControlObject(x, y, x + 31, y + 31) {}
};
// ����� ��� ������ ������ �����
class ColorButton : public Button {
	int c; // ���� ������
public:
	// ����������� �������� ���������� x,y � ����� ����� c
	ColorButton(int x, int y, int c) :Button(x, y), c(c) {}
	void draw(); // ���������� ������
	void press(); // ������� �� �������
};
// ����� ��� ������ c ���������
class IconButton : public Button {
	IMAGE* image; // ����������� �������� ��� ������
public:
	// ����������� �������� ���������� x,y, ��� ����� � ��������� icon
	IconButton(int x, int y, string icon) :Button(x, y), image(loadBMP(icon.c_str())) {}
	~IconButton() { freeimage(image); } // ����������
	void draw(); // ���������� ������
};
// ����� ��� ������ ������ �����������
class ToolButton : public IconButton {
	fun_ptr tool; // ����������
public:
	// ����������� �������� ���������� x,y, ��� ����� � ��������� icon
	// � ��������� �� �������-���������� t
	ToolButton(int x, int y, string icon, fun_ptr t) :IconButton(x, y, icon), tool(t) {}
	void press(); // ������� �� �������
};
// ����� ��� ������-������
class CommandButton : public IconButton {
	fun_ptr action; // ��������
public:
	// ����������� �������� ���������� x,y, ��� ����� � ��������� icon
	// � ��������� �� �������� a
	CommandButton(int x, int y, string icon, fun_ptr a) :IconButton(x, y, icon), action(a) {}
	void press(); // ������� �� �������
};

void delete_tool(); // ���������� ��� ��������
void line_tool(); // ���������� �����
void rect_tool(); // ���������� �������������
void colloredrect_tool(); // ���������� ����������� �������������
void load_command(); // �������� �������
void save_command(); // ���������� �������

#endif
