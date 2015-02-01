#!/usr/bin/perl

$cndir = "make_data/new_semantic_cn";

$testdir = "test_data";

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$_";
		open(CNDATA,"$cnfile");
		$testcnfile = "$testdir/$_";
		open(TESTDATA,"> $testcnfile");
		print "$cnfile -> $testcnfile\n";
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[0]);
			$feature[6] =~ s/\n//;
			if($feature[0] eq "<sil>"){next;}
			if(($feature[6] eq "C") or ($feature[6] eq "E")){
				if($feature[3] eq "*"){
#					$feature[1] = int($feature[1] * 100) / 100;
					print TESTDATA "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
				}
				else{
#					$feature[1] = int($feature[1] * 100) / 100;
					$feature[3] = int($feature[3] * 10);
					$feature[4] = int($feature[4] * 10);
					$feature[5] = int($feature[5]);
					print TESTDATA "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
				}
				next;
			}
			else{
				print TESTDATA "$line";
			}
		}
		close(CNDATA);
		close(TESTDATA);
	}
}
