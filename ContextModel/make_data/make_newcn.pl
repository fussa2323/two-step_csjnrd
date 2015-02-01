#!/usr/bin/perl

#outを元に新しいConfusion Networkの作成

#訂正後、意味スコア付与、そして訂正を行うので、
#初回訂正に合わせてcnを書き換える

$cndir = "comp_nosem";
$outdir = "../out";
$newdir = "new_cn";

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

foreach $file (@file){
	if($file !~ /.cn/){next;}
	$cnfile = "$cndir/$file";
	$newcnfile = "$newdir/$file";
	
	$outfile = $file;
	$outfile =~ s/cn/txt/;
	$outfile = "$outdir/$outfile";
	
	print "$outfile --> $newcnfile\n";
	
	open(CN,"$cnfile");
	open(NEW,"> $newcnfile");
	open(OUT,"$outfile");
	
	while(!eof(OUT)){
		#outとcompは1行ずつ対応してる
		$line1 = <OUT>;
		$line2 = <CN>;
		
		if($line1 eq "\n"){
			print NEW "\n";
			next;
		}
		
		$line1 =~ s/\n//;
		$line2 =~ s/\n//;
		@out = split(/\t/,$line1);
		@words = split(/\t/,$line2);
		
		$i = 0; #wordsの何番目を見てるかカウントする
		foreach $feature (@words){
			@feature = split(/\//,$feature);
			if($feature[0] eq $out[0]){
				while($i > 0){
					$tmp = $words[$i-1];
					$words[$i-1] = $words[$i];
					$words[$i] = $tmp;
					$i--;
				}
			}
			$i++;
		}
		$i = 0;
		foreach $word (@words){
			if($i == $#words){
				print NEW "$word\n";
				last;
			}
			print NEW "$word\t";
			$i++;
		}
	}
	
	close(CN);
	close(NEW);
	close(TMP);
}
