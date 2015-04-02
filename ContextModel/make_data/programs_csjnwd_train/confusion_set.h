#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include "cn_elem.h"

class confusion_set{
private:
	
public:
	std::vector<cn_elem> cs;
	void display();
	std::string cs_to_string();
	void push(cn_elem);
	int get_size();
	std::string get_oracle_word();
	void clear();
	void set_word_at(int,std::string);
	void set_pp_at(int,double);
	void set_semantic_at(int,double);
	void set_cos_at(int,double);
	void set_idf_at(int,double);
	void set_label_at(int,bool);
	std::string get_word_at(int);
	double get_pp_at(int);
	double get_semantic_at(int);
	double get_cos_at(int);
	double get_idf_at(int);
	bool get_label_at(int);
	double calc_entropy();
	int find(std::string);
	void labeling(int);
	std::string get_correct_word();
	double get_correct_pp();
public:
	confusion_set(void);
public:
	~confusion_set(void);
};
