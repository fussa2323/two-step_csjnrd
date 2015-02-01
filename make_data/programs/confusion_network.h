#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <climits>
#include <cfloat>
#include <cstdlib>

#include "confusion_set.h"

class confusion_network
{
private:
	
	int size;
public:
	std::vector<confusion_set> cn;
	void display();
	void push(confusion_set);
	int get_size();
	void clear();
	std::string best_hyp();				//事後確率最大の単語を選択
	std::string oracle_hyp();				//事後確率最大の単語を選択
	void labeling(std::vector<std::string>);			//DPマッチングにより各単語に正誤のラベリングを行う
	void output(std::ofstream&);						//ラベル付きCNをファイルに出力
	std::string most_correct_hyp(std::ofstream&);		//正解の単語を選択
	void out_perceptron_data(std::ofstream&);			//パーセプトロンの学習用のデータを出力
	void out_sem_perceptron_data(std::ofstream&);						//意味スコア付きCNをファイルに出力
	void get_context(std::vector<std::string>&, std::map<std::string, int>);	//コンテキスト単語の取得
	void semantic_scoring(std::vector<std::string>, std::vector<double>, std::map<std::string, int>, std::vector<double>, std::vector<double *>);	//意味スコアの計算
	void culc_weight(std::map<std::string,double>&, std::vector<std::vector<std::vector<int> > >);	//重みの計算
	void culc_score(std::map<std::string,double>&, std::vector<std::vector<std::vector<int> > >,double);	//スコアが最大の単語を選択していく
public:
	confusion_network(void);
public:
	~confusion_network(void);
};

std::string get(int,int,int,int,std::vector<confusion_set>);
