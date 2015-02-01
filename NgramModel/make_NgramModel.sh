#!/bin/sh
cd make_data
./make_label.pl
./utf8.pl
cd ../
./make_traindata.pl
../../CRF_0.57/bin/crf_learn -p 1 NgramTemplate.txt ngram_cncrftrain.txt Ngrammodel
