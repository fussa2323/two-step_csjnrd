#include "confusion_network.h"
#include "main.h"
using namespace std;



void confusion_network::display(){
	for(unsigned int i = 0; i < cn.size(); i++){
		cn[i].display();
		std::cout << std::endl;
	}
}
void confusion_network::push(confusion_set cs){
	cn.push_back(cs);
}

int confusion_network::get_size(){
	return cn.size();
}

void confusion_network::clear(){
	cn.clear();
	size = 0;
}

std::string confusion_network::best_hyp(){
	std::string best_word = "";

	for(std::vector<confusion_set>::iterator it = cn.begin(); it != cn.end(); it++){
		std::string bw = (*it).get_word_at(0);
		if(bw == "-" || bw == "<sil>" || bw == "<sp>"){
			continue;
		}
		best_word += bw + " ";
	}
	return best_word;
}

std::string confusion_network::oracle_hyp(){
	std::string oracle_word = "";
	for(int i = 0; i < (int)cn.size(); i++){
		std::string ow = cn[i].get_oracle_word();
		if(ow == "-" || ow == "<sil>" || ow == "<sp>"){
			continue;
		}
		oracle_word += ow + " ";
	}
	return oracle_word;
}

void confusion_network::labeling(std::vector<std::string> ref){

	int rcount = (int)ref.size();	//正解文書の単語数
	int hcount = (int)cn.size();	//コンフュージョンセットの数
	
	std::vector<int> n(hcount+1);		//min_nullの位置
	std::vector<std::vector<int> > d(rcount+1, std::vector<int>(hcount+1));		//一致しているか
	std::vector<std::vector<int> > w(rcount+1, std::vector<int>(hcount+1));		//一致単語
	std::vector<std::vector<int> > g(rcount+1, std::vector<int>(hcount+1));		//距離計算
	std::vector<std::vector<int> > r(rcount+1, std::vector<int>(hcount+1));		//置換，削除，挿入

	//nを求める
	for(int j = 1; j <= hcount; j++){
		int min_null = -1;
		int a = cn[j-1].find("-");
		int b = cn[j-1].find("<sil>");
		int c = cn[j-1].find("<sp>");
		if(a >= 0){
			if(a < b || b < 0){
				if(a < c || c < 0){
					min_null = a;
				}
				else{
					min_null = c;
				}
			}
			else{
				if(b < c || c < 0){
					min_null = b;
				}
				else{
					min_null = c;
				}
			}
		}
		else if(b >= 0){
			if(b < c || c < 0){
				min_null = b;
			}
			else{
				min_null = c;
			}
		}
		else if(c >= 0){
			min_null = c;
		}
		n[j] = min_null;
	}

	//dの算出(0:一致，1:不一致)
	for(int j = 1; j <= hcount; j++){
		w[0][j] = n[j];

		for(int i = 1; i <= rcount; i++){
//			std::cout << d.size() << ":" << d[i].size() <<  std::endl;
			if(cn[j-1].find(ref[i-1]) >= 0){
				d[i][j] = 0;
				w[i][j] = cn[j-1].find(ref[i-1]);
			}
			else{
				d[i][j] = 1;
				w[i][j] = -1;
			}
		}
	}

	//g，rの端に値を代入
	g[0][0] = 0;
	r[0][0] = 0;

	for(int i = 1; i <= rcount; i++){
		g[i][0] = g[i-1][0] + 1;
		r[i][0] = 1;
	}

	for(int j = 1; j <= hcount; j++){
		//g[0][j] = (cn[j-1].find("-") >= 0 || cn[j-1].find("<sil>") >= 0 || cn[j-1].find("<sp>") >= 0 ? g[0][j-1] : g[0][j-1] +1);
		g[0][j] = g[0][j-1] +1;
		r[0][j] = 2;
	}
	
	//g，rの中の計算
	for(int i = 1; i <= rcount; i++){
		for(int j = 1; j <= hcount; j++){
			int l = g[i-1][j] + 1;
			//int b = (cn[j-1].find("-") >= 0 || cn[j-1].find("<sil>") >= 0 || cn[j-1].find("<sp>") >= 0 ? g[i][j-1] : g[i][j-1] + 1);
			int b = g[i][j-1] + 1;
			int lb = g[i-1][j-1] + d[i][j];
			if(l <= b){
				if(l <= lb){
					g[i][j] = l;
					r[i][j] = 1;
				}
				else{
					g[i][j] = lb;
					r[i][j] = 3;
				}
			}
			else{
				if(b <= lb){
					g[i][j] = b;
					r[i][j] = 2;
				}
				else{
					g[i][j] = lb;
					r[i][j] = 3;
				}
			}
		}
	}

	//最短経路を探索しながら各単語に正誤のラベリング
	int i = rcount;
	int j = hcount;
	int prevright = 0;
	for(;;){
		if(r[i][j] == 1){
			prevright = 1;
			i--;
		}
		else if(r[i][j] == 2){
//			if(prevright == 1){
//				cn[j-1].labeling(-1);
//				cn_elem new_cn_elem(ref[i], 0, true);
//				cn[j-1].cs.push_back(new_cn_elem);
//			}
			if(n[j] >= 0){
				cn[j-1].labeling(n[j]);
			}
			else{
				cn[j-1].labeling(-1);
//				cn_elem new_cn_elem("-", 0, true);
//				cn[j-1].cs.push_back(new_cn_elem);
			}

			prevright = 0;
			j--;

		}
		else if(r[i][j] == 3){
			cn[j-1].labeling(w[i][j]);
			if(w[i][j] == -1){
//				cn_elem new_cn_elem(ref[i-1], 0, true);
//				cn[j-1].cs.push_back(new_cn_elem);
			}
			prevright = 0;
			i--;
			j--;
		}
		else if(i == 0 && j == 0){
			break;
		}
	}
	
	/*
	for(int j = hcount; j >= 1; j--){
		cn[j-1].display();
		std::cout << "\t";
		for(int i = 0; i <= rcount; i++){
			std::cout << g[i][j] << "\t";
		}
		std::cout << std::endl;
	}

	std::cout << "\t";
	for(int i = 0; i <= rcount; i++){
		std::cout << g[i][0] << "\t";
	}
	std::cout << std::endl;

	std::cout << "\t\t";
	for(int i = 1; i <= rcount; i++){
		std::cout << ref[i-1] << "\t";
	}
	std::cout << std::endl;
	*/
	
	/*
	for(std::vector<std::string>::iterator refit = ref.begin(); refit != ref.end(); refit++){
		for(std::vector<confusion_set>::iterator cnit = cn.begin(); cnit != cn.end(); cnit++){
			if((*cnit).find(*refit)){
				//std::cout << *refit << " ";
				break;
			}
		}
	}
	std::cout << std::endl;
	*/
}

void confusion_network::output(std::ofstream& ofs){
	std::string outstr;
	for(int i = 0; i < (int)cn.size(); i++){
		ofs << cn[i].cs_to_string() << std::endl;
	}
	ofs << std::endl;
}

std::string confusion_network::most_correct_hyp(std::ofstream& ofs){
	std::string best_word = "";
	std::string best_pp = "";

	for(int i = 0; i < (int)cn.size(); i++){
		std::string bw = cn[i].get_correct_word();
		if(bw == "-" || bw == "<sil>" || bw == "<sp>"){
			continue;
		}
		std::ostringstream oss;
		oss << cn[i].calc_entropy();
		//oss << (*it).get_best_label();

		best_word += cn[i].get_correct_word() + " ";
		best_pp += oss.str() + " ";
	}
	ofs << best_word << std::endl;
	ofs << "cmscore: " << best_pp << std::endl;

	return best_word;
}

void confusion_network::out_perceptron_data(std::ofstream& ofs){
	//表層単語	事後確率	（エントロピー）	（番目）（意味スコア）	ラベル
	for(int i = 0; i < (int)cn.size(); i++){
		for(int j = 0; j < (int)cn[i].get_size(); j++){
			ofs << cn[i].get_word_at(j) << "/" << cn[i].get_pp_at(j) << "/" << j << "/" << (cn[i].get_label_at(j) ? "C" : "E") << "\t";
//			ofs << cn[i].get_word_at(j) << "/" << (floor(cn[i].get_pp_at(j) * 10) / 10) << "/" << j << "/" << (cn[i].get_label_at(j) ? "C" : "E") << "\t";
		}
		ofs << std::endl;
	}
	ofs << std::endl;
}

void confusion_network::out_sem_perceptron_data(std::ofstream& ofs){
	//表層単語	事後確率	（エントロピー）	（番目）（意味スコア）	ラベル
	for(int i = 0; i < (int)cn.size(); i++){
		for(int j = 0; j < (int)cn[i].get_size(); j++){
			if(cn[i].get_semantic_at(j) != 0){ //変更
				ofs << cn[i].get_word_at(j) << "/" << cn[i].get_pp_at(j) << "/" << j << "/" << cn[i].get_cos_at(j) << "/" << cn[i].get_semantic_at(j) << "/"<<"*"<<"/" << (cn[i].get_label_at(j) ? "C" : "E") << "\t";
			}
			else{
				ofs << cn[i].get_word_at(j) << "/" << cn[i].get_pp_at(j) << "/" << j << "/*/*/*/" << (cn[i].get_label_at(j) ? "C" : "E") << "\t";
			}
		}
		ofs << std::endl;
	}
	ofs << std::endl;
}

void confusion_network::get_context(std::vector<std::string>& context, std::map<std::string, int> tag){

	for(int i = 0; i < (int)cn.size(); i++){
		std::string bw = cn[i].get_word_at(0);
		if(tag.find(bw) != tag.end()){
			context.push_back(bw);
		}
	}
}

void confusion_network::semantic_scoring(int setnum,std::map<std::string,std::string> context, std::map<std::string,double> word1, std::map<std::string, int> tag,std::ofstream& ofs){
    
    std::string curword,tempword,tempsetnum;
	int curtag,tag1,tag2;

	double vs[100];
	double u1;
	double u2;
	double cos;
	double cursc;
	double tmpsc;
	double avgsc;
    std::string dot=",";
    for(int i = 0; i < (int)cn.size(); i++){
		for(int j = 0; j < (int)cn[i].get_size(); j++){
			avgsc = 0;
			curword = cn[i].get_word_at(j);
			if(tag.find(curword) != tag.end()){
               
                // 数字を文字列に変換するための操作
                stringstream ss1;
                ss1 << setnum<<","<<i;
                string setnumcombi=ss1.str();
            
                //std::cout<<"current word is "<<curword<<std::endl;
                map<string,string>::iterator it;
                map<string,string>::iterator ittemp;
                map<string,string>::iterator it1;


                int flagofoverlap=0;
                
                //Erase the ones with in the same cnsets
                for (it=context.begin();it!=context.end();it++) {
                    if (setnumcombi==(*it).first){
                        ittemp=it;
                        tempsetnum=(*it).first;
                        tempword=(*it).second;
                        //cout<<"tempsetnum:tempword"<<tempsetnum<<":"<<tempword<<endl;
                        context.erase(it);
                        flagofoverlap=1;
                       // cout<<"ERASED!!!!"<<endl;
                    }
                    
                    
                }
                
                
                cursc=0;
                
                for(it1=context.begin();it1!=context.end();it1++) {
                    
                    //cout<<(*it).first<<"::"<<setnumcombi<<endl;
                    std::string temp="";
                    
                    temp=curword+dot+(*it1).second;
                    tmpsc=word1[temp];
                    cursc=cursc+tmpsc;
                   // cout<<temp<<":"<<tmpsc<<":"<<flagofoverlap<<endl;
                    
                }
                
                
                 avgsc=cursc/(double)context.size();
                
                cn[i].set_cos_at(j,cursc);
				cn[i].set_semantic_at(j,avgsc);
				//cn[i].set_idf_at(j,word1[curtag]);
			
            
                if(flagofoverlap==1){
                   // context.insert(std::map <std::string, std::string>::value_type(tempsetnum, tempword));
                   context.insert(std::make_pair(tempsetnum,tempword));
                   //cout<<"INSERT Complete!"<<endl;
                }
            
            
            }
			else{
				cn[i].set_cos_at(j,NULL);
				cn[i].set_semantic_at(j,NULL);
				cn[i].set_idf_at(j,NULL);
			}
		}
	}

	
}

void confusion_network::culc_weight(std::map<std::string,double>& feature_list,std::vector<std::vector<std::vector<int> > > templ){

	std::vector<std::string> ref_feature;
	std::vector<std::string> tmp_feature;
	std::vector<std::string> max_feature;

	std::vector<double> ref_pp;
	std::vector<double> tmp_pp;
	std::vector<double> max_pp;

	std::map<std::string,double> test;

	std::map<int,std::string> zengoWord;
	std::map<int,std::string> zengoPp;

	std::vector<std::string> bestWord;
	std::vector<std::string> bestPp;

	std::ostringstream ss;
	std::string str;
	double val;

	bool flag;	//正解単語があるCSでのみ学習を行う

	double tmpscore;

	int maxid;
	std::string maxword;
	double maxpp;
	double maxscore;
	bool maxlabel;

	std::map<std::string,double>::iterator itr;
	std::map<std::string,double>::iterator titr;
	std::vector<std::string>::iterator vitr;

	int l;

	//templateにより見る範囲
	int min = 0;
	int max = 0;
	for(int k = 0; k < (int)templ.size(); k++){
		for(int l = 0; l < (int)templ[k].size(); l++){
			if(templ[k][l][0] < min){
				min = templ[k][l][0];
			}
			if(templ[k][l][0] > max){
				max = templ[k][l][0];
			}
		}
	}

	for(int i = 0; i < (int)cn.size(); i++){
		std::string cw = cn[i].get_word_at(0);
//		std::string cw = cn[i].get_correct_word();
		if(cw == "-" || cw == "<sil>" || cw == "<sp>"){
			bestWord.push_back("-");
			bestPp.push_back("-");
		}
		else{
			//スコア最大の単語
			bestWord.push_back(cn[i].get_word_at(0));
			//正解の単語
//			bestWord.push_back(cw);
			ss.clear();
			ss.str("");
			ss << cn[i].get_pp_at(0);
//			ss << cn[i].get_correct_pp();
			bestPp.push_back(ss.str());
		}
	}

	for(int i = 0; i < (int)cn.size(); i++){
		
		//正解単語があるCSでのみ学習
		flag = false;
		for(int j = 0; j < (int)cn[i].get_size(); j++){
			if(cn[i].get_label_at(j)){
				flag = true;
			}
		}
		if(!flag){
			continue;
		}

		maxscore = -DBL_MAX;

		zengoWord.clear();
		zengoPp.clear();

		//-パスを飛ばさずに前後の単語を求める
		l = i - 1;
		for(int k = -1; k >= min; k--){
			if(l < 0){
				ss.clear();
				ss.str("");
				ss << l;
				zengoWord.insert(std::pair<int,std::string>(k,"_B" + ss.str()));
			}
			else{
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
			}
			l--;
		}
		l = i + 1;
		for(int k = 1; k <= max; k++){
			if(l >= (int)cn.size()){
				ss.clear();
				ss.str("");
				ss << (l - (int)cn.size() + 1);
				zengoWord.insert(std::pair<int,std::string>(k,"_B+" + ss.str()));
			}
			else{
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
			}
			l++;
		}
/*			
		//-パスを飛ばしながら前後の単語を求める
		l = i - 1;
		for(int k = -1; k >= min; ){
//			std::cerr << l << std::endl;
			if(l < 0){
				ss.clear();
				ss.str("");
				ss << l;
				zengoWord.insert(std::pair<int,std::string>(k,"_B" + ss.str()));
				zengoPp.insert(std::pair<int,std::string>(k,"_B" + ss.str()));
				k--;
			}
			else if(bestWord[l] != "-"){
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
				zengoPp.insert(std::pair<int,std::string>(k,bestPp[l]));
				k--;
			}
			l--;
		}
		l = i + 1;
		for(int k = 1; k <= max; ){
			if(l >= (int)cn.size()){
				ss.clear();
				ss.str("");
				ss << (l - (int)cn.size() + 1);
				zengoWord.insert(std::pair<int,std::string>(k,"_B+" + ss.str()));
				zengoPp.insert(std::pair<int,std::string>(k,"_B+" + ss.str()));
				k++;
			}
			else if(bestWord[l] != "-"){
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
				zengoPp.insert(std::pair<int,std::string>(k,bestPp[l]));
				k++;
			}
			l++;
		}
		*/
		zengoWord.insert(std::pair<int,std::string>(0,""));
		zengoPp.insert(std::pair<int,std::string>(0,""));


		for(int j = 0; j < (int)cn[i].get_size(); j++){
			zengoWord[0] = cn[i].get_word_at(j);
			ss.clear();
			ss.str("");
			ss << cn[i].get_pp_at(j);
			zengoPp[0] = ss.str();
			
			//正解単語の素性ベクトルを求める
			if(cn[i].get_label_at(j) == true){
				ref_feature.clear();
				ref_pp.clear();
				ref_feature.push_back("RecScore");
				ref_pp.push_back(cn[i].get_pp_at(j));
				str = "SemFeature1@";
				ss.clear();
				ss.str("");
				ss << cn[i].get_cos_at(j);
				str += ss.str() + "/";
				ss.clear();
				ss.str("");
				ss << cn[i].get_idf_at(j);
				str += ss.str();
				ref_feature.push_back(str);
				ref_pp.push_back(cn[i].get_cos_at(j));
				str = "SemFeature2@";
				ss.clear();
				ss.str("");
				ss << cn[i].get_semantic_at(j);
				str += ss.str() + "/";
				ss.clear();
				ss.str("");
				ss << cn[i].get_idf_at(j);
				str += ss.str();
				ref_feature.push_back(str);
				ref_pp.push_back(cn[i].get_semantic_at(j));
				for(int k = 0; k < (int)templ.size(); k++){
					str = "";
					val = 1;
					for(int l = 0; l < (int)templ[k].size(); l++){
						if(templ[k][l][1] == 0){
							str += zengoWord[templ[k][l][0]];
							if(templ[k][l][0] != 0){
								val *= atof(zengoPp[templ[k][l][0]].c_str());
							}
						}
						else if(templ[k][l][1] == 1){
							str += zengoPp[templ[k][l][0]];
							if(templ[k][l][0] != 0){
								val *= atof(zengoPp[templ[k][l][0]].c_str());
							}
						}
						if(l < (int)templ[k].size() - 1){
							str += "/";
						}
					}
					ss.clear();
					ss.str("");
					ss << k;
					str = ss.str() + "@" + str;
					ref_feature.push_back(str);
					ref_pp.push_back(val);
				}
			}

			//現在の単語の素性べクトルを求める
			tmp_feature.clear();
			tmp_pp.clear();
			tmp_feature.push_back("RecScore");
			tmp_pp.push_back(cn[i].get_pp_at(j));
			str = "SemFeature1@";
			ss.clear();
			ss.str("");
			ss << cn[i].get_cos_at(j);
			str += ss.str() + "/";
			ss.clear();
			ss.str("");
			ss << cn[i].get_idf_at(j);
			str += ss.str();
			tmp_feature.push_back(str);
			tmp_pp.push_back(cn[i].get_cos_at(j));
			str = "SemFeature2@";
			ss.clear();
			ss.str("");
			ss << cn[i].get_semantic_at(j);
			str += ss.str() + "/";
			ss.clear();
			ss.str("");
			ss << cn[i].get_idf_at(j);
			str += ss.str();
			tmp_feature.push_back(str);
			tmp_pp.push_back(cn[i].get_semantic_at(j));
			for(int k = 0; k < (int)templ.size(); k++){
				str = "";
				val = 1;

				for(int l = 0; l < (int)templ[k].size(); l++){
					if(templ[k][l][1] == 0){
						str += zengoWord[templ[k][l][0]];
						if(templ[k][l][0] != 0){
							val *= atof(zengoPp[templ[k][l][0]].c_str());
						}
					}
					else if(templ[k][l][1] == 1){
						str += zengoPp[templ[k][l][0]];
						if(templ[k][l][0] != 0){
							val *= atof(zengoPp[templ[k][l][0]].c_str());
						}
					}
					if(l < (int)templ[k].size()-1){
						str += "/";
					}
				}
				ss.clear();
				ss.str("");
				ss << k;
				str = ss.str() + "@" + str;
//				std::cerr << str << std::endl;
				tmp_feature.push_back(str);
				tmp_pp.push_back(val);
			}

			//現在の単語のスコアを求める
			tmpscore = 0;
			for(int k = 0; k < (int)tmp_feature.size(); k++){
				itr = feature_list.find(tmp_feature[k]);
				//重み
				if(itr != feature_list.end()){
					if(tmp_feature[k] == "RecScore"){
//						tmpscore += itr->second * tmp_pp[k];
					}
					else{
						tmpscore += itr->second;
					}
				}
			}
//			std::cout << "("<< cn[i].get_word_at(j) << "," << tmpscore <<  ")";

			if(tmpscore > maxscore){
				maxid = j;
				maxscore = tmpscore;
				max_feature.clear();
				max_pp.clear();
				for(int k = 0; k < (int)tmp_feature.size(); k++){
					max_feature.push_back(tmp_feature[k]);
					max_pp.push_back(tmp_pp[k]);
				}
			}
//			std::cout << std::endl;
		}
		/*
		//リランキング
		if(maxid != 0){
			maxword = cn[i].get_word_at(maxid);
			maxpp = cn[i].get_pp_at(maxid);
			maxlabel = cn[i].get_label_at(maxid);
//			std::cerr << cn[i].get_word_at(0) << ":" << cn[i].get_word_at(maxid) << std::endl;
			cn[i].set_word_at(maxid,cn[i].get_word_at(0));
			cn[i].set_pp_at(maxid,cn[i].get_pp_at(0));
			cn[i].set_label_at(maxid,cn[i].get_label_at(0));
			cn[i].set_word_at(0,maxword);
			cn[i].set_pp_at(0,maxpp);
			cn[i].set_label_at(0,maxlabel);
		}
		*/

//		std::cout << std::endl;
		
		test.clear();	//更新すべき素性のリスト
		//正解単語の素性の重みは+1
		for(int k = 0; k < (int)ref_feature.size(); k++){
			itr = feature_list.find(ref_feature[k]);
			if(ref_feature[k] == "RecScore"){
				itr->second += ref_pp[k];
			}
			else if(itr == feature_list.end()){
//				feature_list.insert(std::pair<std::string,double>(ref_feature[k],ref_pp[k]));
				feature_list.insert(std::pair<std::string,double>(ref_feature[k],1));
			}
			else{
//				itr->second += ref_pp[k];
				itr->second += 1;
			}
		}
		//選択された単語の素性の重みは-1
		for(int k = 0; k < (int)max_feature.size(); k++){
			itr = feature_list.find(max_feature[k]);
			if(max_feature[k] == "RecScore"){
				itr->second -= max_pp[k];
			}
			else if(itr == feature_list.end()){
//				feature_list.insert(std::pair<std::string,double>(max_feature[k],-max_pp[k]));
				feature_list.insert(std::pair<std::string,double>(max_feature[k],-1));
			}
			else{
//				itr->second -= max_pp[k];
				itr->second -= 1;
			}
		}
//		std::cerr << std::endl;
	}
}

void confusion_network::culc_score(std::map<std::string,double>& feature_list,std::vector<std::vector<std::vector<int> > > templ,double cm_weight){
	
	std::vector<std::string> tmp_feature;

	std::map<std::string,double> test;

	std::map<int,std::string> zengoWord;
	std::map<int,std::string> zengoPp;

	std::vector<std::string> bestWord;
	std::vector<std::string> bestPp;

	std::stringstream ss;
	std::string str;
	std::string sm_str = "";

	double tmpscore;

	int maxid;
	std::string maxword;
	double maxpp;
	double maxscore;

	std::map<std::string,double>::iterator itr;
	std::map<std::string,double>::iterator titr;
	std::vector<std::string>::iterator vitr;

	int l;

	//templateにより見る範囲
	int min = 0;
	int max = 0;
	for(int k = 0; k < (int)templ.size(); k++){
		for(int l = 0; l < (int)templ[k].size(); l++){
			if(templ[k][l][0] < min){
				min = templ[k][l][0];
			}
			if(templ[k][l][0] > max){
				max = templ[k][l][0];
			}
		}
	}

	for(int i = 0; i < (int)cn.size(); i++){
		if(cn[i].get_word_at(0) == "-" || cn[i].get_word_at(0) == "<sil>" || cn[i].get_word_at(0) == "<sp>"){
			bestWord.push_back("-");
			bestPp.push_back("-");
		}
		else{
			bestWord.push_back(cn[i].get_word_at(0));
			ss.clear();
			ss.str("");
			ss << cn[i].get_pp_at(0);
			bestPp.push_back(ss.str());
		}
	}


	for(int i = 0; i < (int)cn.size(); i++){
		maxscore = -DBL_MAX;

		zengoWord.clear();
		zengoPp.clear();

		
		l = i - 1;
		for(int k = -1; k >= min; k--){
			if(l < 0){
				ss.clear();
				ss.str("");
				ss << l;
				zengoWord.insert(std::pair<int,std::string>(k,"_B" + ss.str()));
			}
			else{
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
			}
			l--;
		}
		l = i + 1;
		for(int k = 1; k <= max; k++){
			if(l >= (int)cn.size()){
				ss.clear();
				ss.str("");
				ss << (l - (int)cn.size() + 1);
				zengoWord.insert(std::pair<int,std::string>(k,"_B+" + ss.str()));
			}
			else{
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
			}
			l++;
		}
		
/*
		l = i - 1;
		for(int k = -1; k >= min; ){
//			std::cerr << l << std::endl;
			if(l < 0){
				ss.clear();
				ss.str("");
				ss << l;
				zengoWord.insert(std::pair<int,std::string>(k,"_B" + ss.str()));
				zengoPp.insert(std::pair<int,std::string>(k,"_B" + ss.str()));
				k--;
			}
			else if(bestWord[l] != "-"){
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
				zengoPp.insert(std::pair<int,std::string>(k,bestPp[l]));
				k--;
			}
			l--;
		}
		l = i + 1;
		for(int k = 1; k <= max; ){
			if(l >= (int)cn.size()){
				ss.clear();
				ss.str("");
				ss << (l - (int)cn.size() + 1);
				zengoWord.insert(std::pair<int,std::string>(k,"_B+" + ss.str()));
				zengoPp.insert(std::pair<int,std::string>(k,"_B+" + ss.str()));
				k++;
			}
			else if(bestWord[l] != "-"){
				zengoWord.insert(std::pair<int,std::string>(k,bestWord[l]));
				zengoPp.insert(std::pair<int,std::string>(k,bestPp[l]));
				k++;
			}
			l++;
		}
		*/
		
		zengoWord.insert(std::pair<int,std::string>(0,""));
		zengoPp.insert(std::pair<int,std::string>(0,""));

		for(int j = 0; j < (int)cn[i].get_size(); j++){
			zengoWord[0] = cn[i].get_word_at(j);

			//現在の単語の素性べクトルを求める
			tmp_feature.clear();
			str = "SemFeature1@";
			ss.clear();
			ss.str("");
			ss << cn[i].get_cos_at(j);
			str += ss.str() + "/";
			ss.clear();
			ss.str("");
			ss << cn[i].get_idf_at(j);
			str += ss.str();
			tmp_feature.push_back(str);
			str = "SemFeature2@";
			ss.clear();
			ss.str("");
			ss << cn[i].get_semantic_at(j);
			str += ss.str() + "/";
			ss.clear();
			ss.str("");
			ss << cn[i].get_idf_at(j);
			str += ss.str();
			tmp_feature.push_back(str);
			for(int k = 0; k < (int)templ.size(); k++){
				str = "";
				for(int l = 0; l < (int)templ[k].size(); l++){
					if(templ[k][l][1] == 0){
						str += zengoWord[templ[k][l][0]];
					}
					else if(templ[k][l][1] == 1){
						str += zengoPp[templ[k][l][0]];
					}
					if(l < (int)templ[k].size()-1){
						str += "/";
					}
				}
				ss.clear();
				ss.str("");
				ss << k;
				str = ss.str() + "@" + str;
//				std::cerr << str << std::endl;
				tmp_feature.push_back(str);
			}
//			std::cout << std::endl;
			//現在の単語のスコアを求める
			tmpscore = 0;
			for(vitr = tmp_feature.begin(); vitr != tmp_feature.end(); vitr++){
				itr = feature_list.find(*vitr);
				if(itr != feature_list.end()){
					tmpscore += itr->second * 1;
				}
			}
			tmpscore += cn[i].get_pp_at(j) * cm_weight;
			
//			std::cout << "("<< cn[i].get_word_at(j) << "," << tmpscore <<  ")";

			if(tmpscore > maxscore){
				maxid = j;
				maxscore = tmpscore;
			}
		}

		if(maxid != 0){
			maxword = cn[i].get_word_at(maxid);
			maxpp = cn[i].get_pp_at(maxid);
//			std::cerr << cn[i].get_word_at(0) << ":" << cn[i].get_word_at(maxid) << std::endl;
			cn[i].set_word_at(maxid,cn[i].get_word_at(0));
			cn[i].set_pp_at(maxid,cn[i].get_pp_at(0));
			cn[i].set_word_at(0,maxword);
			cn[i].set_pp_at(0,maxpp);
		}

//		std::cout << std::endl;
	}
//	std::cout << std::endl;
}

confusion_network::confusion_network(void)
{
}

confusion_network::~confusion_network(void)
{
}
