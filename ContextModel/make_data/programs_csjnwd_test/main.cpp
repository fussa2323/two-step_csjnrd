#include "main.h"

#define DIM 100
#define WINDOW_SIZE 10
#define ALPHA 10
using namespace std;


int main(int argc,char *argv[])
{
	//宣言
	int i,j,d;
	char buf[32768];

	fprintf(stderr,"Data reading...\n");

	/*//"word1.txt"の読み込み
	std::vector<std::string> word1;
	FILE *wordfile1 = fopen ("../tfidf/word1.txt", "r");
	while (fgets(buf, sizeof(buf), wordfile1)) {
        std::string ptr = buf;
        ptr=ptr.erase(ptr.find_last_not_of("\n")+1);
		word1.push_back(ptr);
//		weight.push_back(1-atof(ptr));
	}
	fclose(wordfile1);
	fprintf(stderr,"word1.txt read\n");
*/

    //"word2.txt"の読み込み


    //"word1.txt"の読み込み
	std::map<std::string, double> word1;
	FILE *wordfile = fopen ("../../tfidf/word1_hypertest.txt", "r");
	double gsd;
	char *keyword;
	while (fgets(buf, sizeof(buf), wordfile)) {
		char *ptr1 = buf;
		keyword= strtok(ptr1,"\t");
		gsd = atof(strtok(NULL,"\n"));
		word1.insert(std::map<std::string, double>::value_type(keyword, gsd));

	}
	fclose(wordfile);
	fprintf(stderr,"word1_hypertrain.txt read\n");




	//"500dic.txt"の読み込み
	std::map<std::string, int> tag;
	FILE *dicfile = fopen ("../../tfidf/dic.txt", "r");
	int id;
	char *ivword;
	while (fgets(buf, sizeof(buf), dicfile)) {
		char *ptr = buf;
		id = atoi(strtok(ptr,"\t"));
		ivword = strtok(NULL,"\n");
		tag.insert(std::map<std::string, int>::value_type(ivword, id));

	}
	fclose(dicfile);
	fprintf(stderr,"dic.txt read\n");


	/*//"s.txt"の読み込み
	std::vector<double> s;
	FILE *sfile = fopen ("../tfidf/s.txt", "r");
	i=0;
	while (fgets(buf, sizeof(buf), sfile) && (i < DIM)) {
		char *ptr = buf;
		j=0;
		while (ptr && *ptr) {
			while((strchr(ptr, ' ' ) - ptr) == sizeof(char));{
				ptr = ptr+1;
			}
			if(i == j){
				s.push_back(atof(ptr));
				break;
			}
			j++;
			if (j >= DIM) break;
			ptr = (strchr(ptr+1, ' ' ));
		}
		i++;
	}
	fclose(sfile);
	fprintf(stderr,"s.txt read\n");

	//"u.txt"の読み込み
	std::vector<double *> u;
	FILE *ufile = fopen ("../tfidf/u.txt", "r");
	while (fgets(buf, sizeof(buf), ufile)) {
		double *tmp = new double [DIM];
		char *ptr = buf;
		j=0;
		while (ptr && *ptr) {
			while((strchr(ptr, ' ' ) - ptr) == sizeof(char));{
				ptr = ptr+1;
			}
			tmp[j] = atof(ptr);
//			printf("%g\n",tmp[j]);
			j++;
			if (j >= DIM) break;
			ptr = (strchr(ptr+1, ' ' ));
		}
		u.push_back(tmp);
	}
	fclose(ufile);
	fprintf(stderr,"u.txt read\n");
     */

	//ここからいつもどおり
	int att_num;

	std::string str;

	std::map<std::string,std::string> context;

	std::vector<std::vector<std::vector<int> > > templ;
	std::vector<std::vector<int> > t;
    std::map <std::string,std::string>::iterator it1;
	std::vector<int> nums;

	std::vector<confusion_network> training_cn;
	confusion_network cn;
	confusion_set cs;

	std::map<std::string,double> feature_list;

	std::map<std::string,double>::iterator itr;

	char* cnfilename = argv[1];
	char* outfilename = argv[2];

	std::ofstream outfs(outfilename);
    std::ofstream testfile("testpair.txt",std::ios::app);

	//学習データの読み込み
	std::ifstream cnfs(cnfilename);
	att_num = 0;
	while(getline(cnfs,str)){
		if(str == "cn_failed"){
			cn_elem new_cn_elem("cn_failed", 0, false);
			cs.push(new_cn_elem);
			cn.push(cs);
			cs.clear();
		}
		else if(str != ""){
			std::vector<std::string> cnelems = split(str,"\t");
			for(int i = 0; i < (int)cnelems.size(); i++){
				std::vector<std::string> elemprop = split(cnelems[i],"/");

				std::string word = elemprop[0];
				double pp = (double)atof(elemprop[1].c_str());
				bool label = (elemprop[3] == "C" ? true : false);

				cn_elem new_cn_elem(word, pp, label);
				cs.push(new_cn_elem);
			}
			cn.push(cs);
			cs.clear();
		}
		//改行のときの処理
		else{
			training_cn.push_back(cn);
			cn.clear();
			att_num++;
			std::cerr << att_num << "\r";
		}
	}
	std::cerr << std::endl;
	std::cerr << "Read learning data ... OK" << std::endl;

	//意味スコアの計算
	for(int i = 0; i < (int)training_cn.size(); i++){
		//コンテキストの単語を取得
		context.clear();

		//窓幅
		for(int j = i - 3; j <= i - 1; j++){
			if(j >= 0){
				//training_cn[j].get_context(context,tag);
				for(int g = 0; g < (int)(training_cn[j].get_size()); g++){
					std::string bw = (training_cn[j].cn)[g].get_word_at(0);
                    bool label = (training_cn[j].cn)[g].get_label_at(0);

					if(tag.find(bw) != tag.end()){
                        //cout<<bw<<endl;
                        int num=tag[bw];
                        /*if (num>9483) {
                            ostringstream os;;
                           // cout<<"before"<<bw<<endl;
                            os<<"echo"<<" "<<bw<<"|mecab -F \"%f[6]\" -E \" \" ";
                            //cout<<os.str()<<endl;
                             //int temp=system(os.str().c_str());
                            char* cstr =new char[os.str().length()+1];
                            strcpy(cstr,os.str().c_str());
                            bw=exec(cstr);
                           //cout<<"exstatus is:"<<  tem <<endl;
                        }*/
                        std::stringstream ss;
                        ss << j <<","<<g;
                        std::string key=ss.str();
                       // std::cout<<key<<endl;
						context.insert(std::map <std::string, std::string>::value_type(key, bw));
					}
				}
			}
		}
		//窓幅
		for(int j = i ; j <= i + 3; j++){
			if(j < (int)training_cn.size()){
				//training_cn[j].get_context(context,tag);
				for(int g = 0; g < (int)(training_cn[j].get_size()); g++){
					std::string bw = (training_cn[j].cn)[g].get_word_at(0);
                    bool label = (training_cn[j].cn)[g].get_label_at(0);

                    if(tag.find(bw) != tag.end()){
                        int num=tag[bw];
                        //if(num>9483){
                          //  ostringstream os;

                            //os<<"echo"<<" "<<bw<<"|mecab -F \"%f[6]\" -E \" \" ";
                            //char* cstr =new char[os.str().length()+1];
                            //strcpy(cstr,os.str().c_str());
                            //bw=exec(cstr);
                        //}
                        std::stringstream ss;
                        ss << j<< ","<<g;
                        std::string key=ss.str();
                        //std::cout<<key<<endl;
						context.insert(std::map <std::string , std::string>::value_type(key, bw));
					}
				}
			}
		}

        //for( it1= context.begin(); it1!=context.end(); it1++){
		//	std::cout << (*it1).second << " ";
		//}
		//std::cerr << std::endl;
		std::string best = training_cn[i].best_hyp();
		if(best == "cn_failed "){
			outfs << "cn_failed \n\n";
		}
		else{
			training_cn[i].semantic_scoring(i,context,word1,tag,testfile);
			training_cn[i].out_sem_perceptron_data(outfs);
		}
		std::cerr << i << "\r";
	}

	cnfs.close();
	outfs.close();
    testfile.close();
	std::cerr << std::endl;

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
std::string exec(char *cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}
