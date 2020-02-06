#ifndef INTERFACE_H
#define INTERFACE_H

#include "graphics.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;


class DrawObject { // ����� ��� �������� �� ������
protected:
	int x1, y1, x2, y2; // ���������� ����� ��������������, � ������� ��������� ������
public:
	DrawObject(int x1, int y1, int x2, int y2) :x1(x1), y1(y1), x2(x2), y2(y2) {} // ����������� �������� ���������� ������ �������� ���� x1,y1 � ���������� ������� ������� ���� x2,y2
	virtual ~DrawObject() {} // ����������
	bool in(int x, int y); // ���������� x, y ������ �������
	int get_top() { return y1 + 1; } // ����������
	int get_left() { return x1 + 1; } // �������� ����
	int get_height() { return y2 - y1 - 1; } // ������
	int get_width() { return x2 - x1 - 1; } // ������
	virtual void draw() = 0; // ���������� ������
};

class SelectedColors : public DrawObject { // ����� ��� ����������� ������� ������ ���������
	int fc, // ���� ���������
		bc; // ���� ��������

	SelectedColors(int x1, int y1, int x2, int y2) : // ����������� �������� ���������� ����� x1,y1,x2,y2
		DrawObject(x1, y1, x2, y2), fc(BLACK), bc(WHITE) {}
public:
	static SelectedColors& instance(); // ������ ��� ������� ������
	void draw(); // ���������� ������
	void set_fc(int c); // ���������� ���� ���������
	int get_fc() { return fc; } // ��������  ���� ���������
	void set_bc(int c); // ���������� ���� ��������
	int get_bc() { return bc; } // �������� ���� ��������
};

class StatusLine : public DrawObject { // ����� ��� ����������� ������� ��������� ���� � ����� �����
	int x, y; // ���������� ����
	bool edit;
	string name; // ��� �����
	StatusLine(int x1, int y1, int x2, int y2) : // ����������� �������� ���������� ����� x1,y1,x2,y2
		DrawObject(x1, y1, x2, y2), x(0), y(0), name("new.vec") {}
public:
	static StatusLine& instance(); // ������ ���������
	void draw(); // ���������� ������
	void set_xy(int x, int y); // �������� ������� ����������
	void set_name(const string& name); // ���������� ��� �����
	string get_name() { return name; } // �������� ��� �����
	void edit_name(); // ������ ��� �����
};

class ControlObject : public DrawObject { // ����� ��� ��������, ����������� �� ����
public:
	ControlObject(int x1, int y1, int x2, int y2) :DrawObject(x1, y1, x2, y2) {} // ����������� �������� ���������� ����� x1,y1,x2,y2
	virtual void press() = 0; // ������� �� �������
};

typedef void (*fun_ptr)(); // ��������������� ��� - ��������� �� �������

class Figure {
	bool visible = 0; // ���������
protected:
	int x_1, y_1, //����� ��� ����������
		x_2, y_2;
	int c; //����
public:
	Figure(int x_1, int y_1, int x_2, int y_2, int c) : x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2), c(c) {}
	virtual ~Figure() {}
	int get_color() const { return c; } //�������� ����
	void hide(); //��������
	void show(); //�������� ������
	bool isvisible() const { return visible; } //������?
	virtual void get_data(int& x1, int& y1, int& x2, int& y2, int& c); // ��������� ����� � ������ ������
	virtual void draw() = 0; // ����� ��� ��������� 
};

class Rect : public Figure {
public:
	Rect(int x_1, int y_1, int x_2, int y_2, int c)
		:Figure(x_1, y_1, x_2, y_2, c) {}
	~Rect() { hide(); }
	void draw(); // ����� ��� ��������� 
};

class ColoredRect : public Rect {
protected:
	int inner_c; // ���� �������
public:
	ColoredRect(int x_1, int y_1, int x_2, int y_2, int c, int inner_c)
		:Rect(x_1, y_1, x_2, y_2, c), inner_c(inner_c) {}
	~ColoredRect() { hide(); }
	void get_data(int& x_1, int& y_1, int& x_2, int& y_2, int& c, int& inner_c); // ��������� ����� � ������ ������������ ��������������
	void draw(); // ����� ��� ��������� 
};



class PaintArea : public ControlObject { // ����� ��� ���� ���������
	vector <Figure*> storage; // ��������� �����
	fun_ptr tool; // ������� ����������
	PaintArea(int x1, int y1, int x2, int y2) : // ����������� �������� ���������� ����� x1,y1,x2,y2
		ControlObject(x1, y1, x2, y2), tool(nullptr) {}
public:
	static PaintArea& instance(); // ���� ��� ���������
	void draw(); // ���������� ���� ���� 
	void create_figure(string type, int x_1, int y_1, int x_2, int y_2, int c, int inner_c = -1); // �������� ������
	void press(); // ������� �� �������
	void set_tool(fun_ptr t) { tool = t; } // ���������� ���������� ���������
	fun_ptr get_tool() { return tool; } // �������� ������� ���������� ���������
	void save(const string& name); // ��������� �����������
	void load(const string& name); // ��������� �����������
	Figure* get_figure(int i) { return storage[i]; }; // ������ � i-�� ������
	int figure_amount() { return storage.size(); } // ���������� ���-�� �����
	void delete_figure(int i) { storage.erase(storage.begin() + i); } // ������� ������ �� �������
	void refresh(); // �������� ���������� ������
};

class Line : public Figure { // ����� ��� �������
public:
	Line(int x_1, int y_1, int x_2, int y_2, int c)
		:Figure(x_1, y_1, x_2, y_2, c) {}
	~Line() { hide(); }
	void draw(); // ����� ��� ��������� 
};
#endif
