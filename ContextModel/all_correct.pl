#!/usr/bin/perl

$outdir = "out";

opendir(OUTDIR,"$outdir");
@file = readdir(OUTDIR);
closedir(OUTDIR);

foreach(@file){
	if($_ !~ /.txt/){next;}
	$outfile = "$outdir/$_";
	system "perl correct.pl $outfile";
}
