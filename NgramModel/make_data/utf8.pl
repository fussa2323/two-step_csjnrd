#!/usr/bin/perl

#-wでutf8に、-sでshiftjisに

=pod
$refdir = "../ref";
opendir(REFDIR,"$refdir");
@file = readdir(REFDIR);
closedir(REFDIR);

foreach(@file){
	if($_ !~ /\.ref/){next;}
	`nkf -s --overwrite $refdir/$_`;
}
=cut

$traindir1 = "cn";

opendir(CNDIR,"$traindir1");
@file = readdir(CNDIR);
closedir(CNDIR);

foreach(@file){
	if($_ !~ /\.cn/){next;}
	`nkf -w --overwrite $traindir1/$_`;
}
