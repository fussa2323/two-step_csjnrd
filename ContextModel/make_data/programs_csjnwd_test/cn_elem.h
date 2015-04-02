#pragma once

#include <iostream>
#include <string>

class cn_elem
{
private:
	std::string word;
	double pp;
	double cos;
	double semantic;
	double idf;
	bool label;
public:
	void display();
	std::string cn_elem_to_string();
	std::string get_word();
	void set_word(std::string);
	void set_pp(double);
	void set_semantic(double);
	void set_cos(double);
	void set_idf(double);
	double get_pp();
	double get_semantic();
	double get_cos();
	double get_idf();
	void set_label(bool);
	bool get_label();
public:
	cn_elem(void);
	cn_elem(std::string, double);
	cn_elem(std::string, double, bool);
	cn_elem(std::string, double, double, double, double, bool);
public:
	~cn_elem(void);
};
