#!/usr/bin/perl

`rm wer.txt`;

$refdir = "utf8_ref"; #正解ファイルのディレクトリ
$hypdir = "hyp"; #仮説を格納するディレクトリ

#$num = 259901; #train
#$num = 311374; #train2
$num = 113289; #test

opendir(HYPDIR,"$hypdir");
@file = readdir(HYPDIR);
closedir(HYPDIR);

foreach(@file){
	if($_ !~ /.hyp/){next}
	$hypfile = "$hypdir/$_";
	$reffile = "$refdir/$_";
	$reffile =~ s/hyp/ref/;
	
	`./make_wer $reffile $hypfile`;
}

$sub = 0;
$del = 0;
$ins = 0;
$cor = 0;

open(WER,"wer.txt");
while(<WER>){
	$line = $_;
	@ele = split(/\s/,$line);
	if($ele[0] eq "sub"){$sub += int($ele[1]);}
	elsif($ele[0] eq "del"){$del += int($ele[1]);}
	elsif($ele[0] eq "ins"){$ins += int($ele[1]);}
	elsif($ele[0] eq "cor"){$cor += int($ele[1]);}
	else{next;}
}

$wer = ($sub + $del + $ins) / $num;

print "SUB : $sub\n";
print "DEL : $del\n";
print "INS : $ins\n";
print "COR : $cor\n";
print "WER : $wer\n";