#!/usr/bin/perl

##../cnsrr/testディレクトリを見て、
##../refに単語数をカウント

$testdir = "../cnsrr/train2";
#$testdir = "../cnsrr/test";
$refdir = "../ref"; #正解ファイルのディレクトリ
$count = 0;

opendir(TESTDIR,"$testdir");
@file = readdir(TESTDIR);
closedir(TESTDIR);

foreach(@file){
	if($_ !~ /.srr/){next}
	$srrfile = "$srrdir/$_";
	$reffile = $_;
	$reffile =~ s/srr/ref/;
	$reffile = "$refdir/$reffile";
	
	open(REF,$reffile);
	
	while($line = <REF>){
		$line =~ s/\n//;
		@words = split(/\s/,$line);
		$count += @words;
	}
	close(REF);
}

print "The num of words is $count.\n";
