#include "main.h"


int main(int argc, char* argv[]){


	std::ifstream srrfs(argv[1]);
	std::ifstream reffs(argv[2]);
	std::ofstream hypfs(argv[3]);

	//std::ifstream srrfs("A01F0066.srr");
	//std::ifstream reffs("A01F0066.ref");
	//std::ofstream hypfs("data.txt");

	std::string refstr;
	std::vector<std::string> ref;
	std::string hypstr;
	std::vector<std::string> hyp;
//	for(unsigned int i = 0; i < ref.size(); i++){
//		std::cout << ref.at(i) << " ";
//	}
//	std::cout << std::endl;

	int att_num = 0;

	std::string str,tmpstr,pass1;
	bool in_cn = false;
	
	int lp,rp,colon;
	
	confusion_network cn;
	confusion_set cs;
	
	std::string best_hyp;
	std::string best_pp;
	
	while(getline(srrfs,str)){
		if(str.find("pass1_best:") != str.npos){
			pass1 = str.substr(12) + " ";
			att_num++;

			getline(reffs,refstr);
			getline(reffs,refstr);
			ref = split(refstr," ");
//			for(int i = 0; i < (int)ref.size(); i++){
//				std::cout << ref[i] << " ";
//			}
//			std::cout << std::endl;
		}
		else if(str.find("---- begin confusion network ---") != str.npos){
			in_cn = true;
			cn.clear();
		}
		else if(str.find("---- end confusion network ---") != str.npos){
			in_cn = false;
			cn.labeling(ref);
			cn.out_perceptron_data(hypfs);
		}
		else if(str.find("<search failed>") != str.npos){
			hypfs << "cn_failed" << std::endl;
			hypfs << std::endl;
			for(int pos = (int)pass1.find("<sil> "); pos != pass1.npos; pos = (int)pass1.find("<sil> ",pos)){
				pass1.replace(pos,6,"");
			}
			for(int pos = (int)pass1.find("<sp> "); pos != pass1.npos; pos = (int)pass1.find("<sp> ",pos)){
				pass1.replace(pos,5,"");
			}
			hyp = split(pass1," ");
		}
		else if(in_cn == true){
			cs.clear();
			while((lp = (int)str.find_first_of("(")) != str.npos){
				rp = (int)str.find_first_of(")");
				
				std::string word;
				float pp;
				
				tmpstr = str.substr(lp+1,rp-lp-1);
				colon = (int)tmpstr.find_first_of(":");
				word = tmpstr.substr(0,colon);
				
				pp = (float)atof((tmpstr.substr(colon+1)).c_str());
				
				cn_elem new_cn_elem(word,pp);
				
				cs.push(new_cn_elem);
				
				str = str.substr(rp+1);
			}
			cn.push(cs);
		}
	}

	srrfs.close();
	reffs.close();
	hypfs.close();

	return 0;
}

std::vector<std::string> split(std::string str, std::string delim){
    std::vector<std::string> result;
    int cutAt;
    while( (cutAt = (int)str.find_first_of(delim)) != str.npos ){
        if(cutAt > 0){
            result.push_back(str.substr(0, cutAt));
        }
        str = str.substr(cutAt + 1);
    }
    if(str.length() > 0)
    {
        result.push_back(str);
    }
	return result;
}

