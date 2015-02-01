#include "main.h"

int sub = 0, del = 0, ins = 0, cor = 0;

int main(int argc, char* argv[]){

	std::ifstream reffs(argv[1]);
	std::ifstream hypfs(argv[2]);
	std::ofstream wer("wer.txt", std::ios::out | std::ios::app);

	std::string refstr;
	std::vector<std::string> ref;
	std::string hypstr;
	std::vector<std::string> hyp;

	int att_num = 0;

	while(getline(hypfs,hypstr)){

		att_num++;
		getline(reffs,refstr);
		getline(reffs,refstr);

		hyp = split(hypstr," ");
		ref = split(refstr," ");
		for(int i = 0; i < (int)ref.size(); i++){
			std::cout << ref[i] << " ";
		}
		std::cout << std::endl;
		for(int i = 0; i < (int)hyp.size(); i++){
			std::cout << hyp[i] << " ";
		}
		std::cout << std::endl;
		if(refstr.find("×",0) == refstr.npos){
			dp(ref,hyp);
		}
	}

	wer << argv[1] << std::endl;
	wer << "sub\t" << sub << std::endl;
	wer << "del\t" << del << std::endl;
	wer << "ins\t" << ins << std::endl;
	wer << "cor\t" << cor << std::endl;

	reffs.close();
	hypfs.close();
	wer.close();

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

void dp(std::vector<std::string> ref, std::vector<std::string> hyp){
	int rcount = (int)ref.size();	//正解文書の単語数
	int hcount = (int)hyp.size();	//コンフュージョンセットの数
	
	std::vector<std::vector<bool> > d(rcount+1, std::vector<bool>(hcount+1));	//一致しているか
	std::vector<std::vector<int> > g(rcount+1, std::vector<int>(hcount+1));		//距離計算
	std::vector<std::vector<int> > r(rcount+1, std::vector<int>(hcount+1));		//置換，削除，挿入

	//dの算出(0:一致，1:不一致)
	for(int i = 1; i <= rcount; i++){
		for(int j = 1; j <= hcount; j++){
			d[i][j] = (ref[i-1] != hyp[j-1]);
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
		g[0][j] = g[0][j-1] + 1;
		r[0][j] = 2;
	}
	
	//g，rの中の計算
	for(int i = 1; i <= rcount; i++){
		for(int j = 1; j <= hcount; j++){
			int l = g[i-1][j] + 1;
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
	
	int i = rcount;
	int j = hcount;
	
	for(;;){
		if(r[i][j] == 1){
			del++;
			i--;
		}
		else if(r[i][j] == 2){
			ins++;
			j--;
		}
		else if(r[i][j] == 3){
			if(d[i][j] == 1){
				sub++;
			}
			else{
				cor++;
			}
			i--;
			j--;
		}
		else if(i == 0 && j == 0){
			break;
		}
	}
}