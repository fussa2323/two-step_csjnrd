#!/bin/sh
cd make_data
./make_label.pl
./utf8.pl
#ここで文脈を計算する必要は無いが、プログラムを再利用するために形式を合わせている
./make_semdata.pl
./mk_comp.pl
cd ../
./make_testdata.pl
./ErrorDetection.pl
./all_correct.pl
cd make_data
./make_newcn.pl
./make_newsemdata.pl
cd ../
./make_traindata.pl
../../CRF_0.57/bin/crf_learn -p 1 SemTemplate.txt sem_cncrftrain.txt Semmodel
