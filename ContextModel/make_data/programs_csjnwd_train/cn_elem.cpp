#include "cn_elem.h"

cn_elem::cn_elem(std::string _word, double _pp){
	word = _word;
	pp = _pp;
}

cn_elem::cn_elem(std::string _word, double _pp, bool _label){
	word = _word;
	pp = _pp;
	label = _label;
}

cn_elem::cn_elem(std::string _word, double _pp, double _cos, double _semantic, double _idf, bool _label){
	word = _word;
	pp = _pp;
	cos = _cos;
	semantic = _semantic;
	idf = _idf;
	label = _label;
}

void cn_elem::display(){
	std::cout << word << ":" << label;
}

std::string cn_elem::cn_elem_to_string(){
	std::string str;
	str = word + ":" + (label ? "C" : "E");
	return str;
}

std::string cn_elem::get_word(){
	return word;
}

void cn_elem::set_word(std::string _word){
	word = _word;
}

void cn_elem::set_pp(double _pp){
	pp = _pp;
}

void cn_elem::set_semantic(double _semantic){
	semantic = _semantic;
}

void cn_elem::set_cos(double _cos){
	cos = _cos;
}

void cn_elem::set_idf(double _idf){
	idf = _idf;
}

double cn_elem::get_pp(){
	return pp;
}

double cn_elem::get_semantic(){
	return semantic;
}

double cn_elem::get_cos(){
	return cos;
}

double cn_elem::get_idf(){
	return idf;
}

void cn_elem::set_label(bool lb){
	label = lb;
}

bool cn_elem::get_label(){
	return label;
}

cn_elem::cn_elem(void)
{
}

cn_elem::~cn_elem(void)
{
}
