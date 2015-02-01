#!/usr/bin/perl

#$cndir = "make_data/semantic_cn";
$cndir = "make_data/new_semantic_cn";
$traindir = "train2";

$cncrftrain = "sem_cncrftrain.txt";

opendir(CNDIR,"$cndir/$traindir");
@file = readdir(CNDIR);
closedir(CNDIR);

open(CRF,"> $cncrftrain");

$linecnt = 0;

foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$traindir/$_";
		open(CNDATA,"$cnfile");
		
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[0]);
			$feature[6] =~ s/\n//;
			#print("$feature[0]\n");
			if($feature[0] eq "<sil>"){
				next;
			}
			
			if(($feature[6] ne "C") and ($feature[6] ne "E")){
				print CRF "\n";
				next;
			}
			elsif($feature[3] ne "*"){
				$feature[3] = int($feature[3] * 10);
				$feature[4] = int($feature[4] * 10);
				$feature[5] = int($feature[5]);
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
			}
			else{
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
			}
		}
		close(CNDATA);
	}
}

opendir(CNDIR,"$cndir/$traindir");
@file = readdir(CNDIR);
closedir(CNDIR);

#第二候補
foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$traindir/$_";
		open(CNDATA,"$cnfile");
		
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[1]);
			#第二候補が存在しないとき，第一候補を選択
			if($feature[0] eq "\n"){
				@feature = split(/\//,$cnwords[0]);
			}
			$feature[6] =~ s/\n//;
			#print("$feature[0]\n");
			if($feature[0] eq "<sil>"){
				next;
			}
			
			if(($feature[6] ne "C") and ($feature[6] ne "E")){
				print CRF "\n";
				next;
			}
			elsif($feature[3] ne "*"){
				$feature[3] = int($feature[3] * 10);
				$feature[4] = int($feature[4] * 10);
				$feature[5] = int($feature[5]);
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
			}
			else{
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
			}
		}
		close(CNDATA);
	}
}

opendir(CNDIR,"$cndir/$traindir");
@file = readdir(CNDIR);
closedir(CNDIR);

#第三候補
foreach(@file){
	if($_ =~ /.cn/){
		$cnfile = "$cndir/$traindir/$_";
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
			$feature[6] =~ s/\n//;
			if($feature[0] eq "<sil>"){
				next;
			}
			
			if(($feature[6] ne "C") and ($feature[6] ne "E")){
				print CRF "\n";
				next;
			}
			elsif($feature[3] ne "*"){
				$feature[3] = int($feature[3] * 10);
				$feature[4] = int($feature[4] * 10);
				$feature[5] = int($feature[5]);
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
			}
			else{
				print CRF "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
			}
		}
		close(CNDATA);
	}
}

close(CRF);
