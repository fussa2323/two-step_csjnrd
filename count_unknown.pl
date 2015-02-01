#!/usr/bin/perl

##oovref/testに含まれる未知語数をカウント

$refdir = "utf8_ref"; #正解ファイルのディレクトリ
$testdir = "cnsrr/test"; #テストファイルを知るためのディレクトリ
$wordfile = ">words.txt";
$count = 0;
$oov = 0;
$speeches = 0;
open(DIC,$wordfile);
opendir(REFDIR,"$testdir");
@file = readdir(REFDIR);
closedir(REFDIR);
foreach(@file){
	if($_ !~ /.srr/){next}
	$reffile = $_;
	$reffile =~ s/srr/ref/;
	$reffile = "$refdir/$reffile";
	print "$reffile\n";
	
	open(REF,$reffile);
	while($line = <REF>){
		$line =~ s/\n//;
		@words = split(/\s/,$line);
		$count += @words;
		$speeches++;
		foreach $word (@words){
            if ($.% 2 ){next;}
            else{
                print DIC "$word\n";}
			if($word eq "<UNKNOWN>"){$oov++;}
		}
	}
    close(REF);
}

print "The num of speeches is $speeches.\n";
print "The num of words is $count.\n";
print "The num of <UNKNOWN> is $oov.\n";
close(DIC);
