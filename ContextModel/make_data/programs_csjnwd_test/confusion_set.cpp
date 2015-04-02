#include "confusion_set.h"

void confusion_set::display(){
	for(int i = 0; i < (int)cs.size(); i++){
		cs[i].display();
		std::cout << ",";
	}
}

std::string confusion_set::cs_to_string(){
	std::string str = "";
	for(int i = 0; i < (int)cs.size(); i++){
		str += "(" + cs[i].cn_elem_to_string() +")";
	}
	return str;
}

void confusion_set::push(cn_elem in_cn_elem){
	cs.push_back(in_cn_elem);
}

int confusion_set::get_size(){
	return (int)cs.size();
}

void confusion_set::clear(){
	cs.clear();
}

std::string confusion_set::get_oracle_word(){
	std::string ow = cs[0].get_word();
	for(int i = 0; i < (int)cs.size(); i++){
		if(cs[i].get_label()){
			ow = cs[i].get_word();
			break;
		}
	}
	return ow;
}

void confusion_set::set_word_at(int i, std::string _word){
	cs[i].set_word(_word);
}

void confusion_set::set_pp_at(int i, double _pp){
	cs[i].set_pp(_pp);
}

void confusion_set::set_semantic_at(int i, double _semantic){
	cs[i].set_semantic(_semantic);
}

void confusion_set::set_cos_at(int i, double _cos){
	cs[i].set_cos(_cos);
}

void confusion_set::set_idf_at(int i, double _idf){
	cs[i].set_idf(_idf);
}

void confusion_set::set_label_at(int i, bool _label){
	cs[i].set_label(_label);
}

std::string confusion_set::get_word_at(int i){
	return cs[i].get_word();
}

double confusion_set::get_pp_at(int i){
	return cs[i].get_pp();
}

double confusion_set::get_semantic_at(int i){
	return cs[i].get_semantic();
}

double confusion_set::get_cos_at(int i){
	return cs[i].get_cos();
}

double confusion_set::get_idf_at(int i){
	return cs[i].get_idf();
}

bool confusion_set::get_label_at(int i){
	return cs[i].get_label();
}

double confusion_set::calc_entropy(){
	double entropy = 0;
	for(std::vector<cn_elem>::iterator it = cs.begin(); it != cs.end(); it++){
		if((*it).get_pp() != 0){
			entropy -= (*it).get_pp() * log((*it).get_pp()) / log(2.0);
		}
	}
	return entropy;
}

int confusion_set::find(std::string refword){
	for(int i = 0; i < (int)cs.size(); i++){
		if(cs[i].get_word() == refword){
			return i;
		}
	}
	return -1;
}

void confusion_set::labeling(int id){
	for(int i = 0; i < (int)cs.size(); i++){
		cs[i].set_label(i == id);
	}
}

std::string confusion_set::get_correct_word(){
	for(int i = 0; i < (int)cs.size(); i++){
		if(cs[i].get_label()){
			return cs[i].get_word();
		}
	}
	return cs[0].get_word();
}

double confusion_set::get_correct_pp(){
	for(int i = 0; i < (int)cs.size(); i++){
		if(cs[i].get_label()){
			return cs[i].get_pp();
		}
	}
	return cs[0].get_pp();
}

confusion_set::confusion_set(void)
{
}

confusion_set::~confusion_set(void)
{
}
