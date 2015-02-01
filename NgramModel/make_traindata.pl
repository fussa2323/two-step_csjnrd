#!/usr/bin/perl

$cndir = "make_data/cn";

$cncrftrain = "ngram_cncrftrain.txt";

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

open(CRF,"> $cncrftrain");

$linecnt = 0;

foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$_";
		open(CNDATA,"$cnfile");
		
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[0]);
			$feature[3] =~ s/\n//;
			#print("$feature[0]\n");
			if($feature[0] eq "<sil>"){
				next;
			}
			
			if(($feature[3] ne "C") and ($feature[3] ne "E")){
				print CRF "\n";
				next;
			}
			else{
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\n";
			}
		}
		close(CNDATA);
	}
}

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

#第二候補
foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$_";
		open(CNDATA,"$cnfile");
		
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[1]);
			#第二候補が存在しないとき，第一候補を選択
			if($feature[0] eq "\n"){
				@feature = split(/\//,$cnwords[0]);
			}
			$feature[3] =~ s/\n//;
			#print("$feature[0]\n");
			if($feature[0] eq "<sil>"){
				next;
			}
			
			if(($feature[3] ne "C") and ($feature[3] ne "E")){
				print CRF "\n";
				next;
			}
			else{
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\n";
			}
		}
		close(CNDATA);
	}
}

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

#第三候補
foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$_";
		open(CNDATA,"$cnfile");
		
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[2]);
			#第三候補が存在しないとき，第二候補を選択
			if($feature[0] eq "\n" or $feature[0] eq ""){
				@feature = split(/\//,$cnwords[1]);
			}
			#第二候補が存在しないとき，第一候補を選択
			if($feature[0] eq "\n"){
				@feature = split(/\//,$cnwords[0]);
			}
			$feature[3] =~ s/\n//;
			if($feature[0] eq "<sil>"){
				next;
			}
			
			if(($feature[3] ne "C") and ($feature[3] ne "E")){
				print CRF "\n";
				next;
			}
			else{
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\n";
			}
		}
		close(CNDATA);
	}
}

close(CRF);
