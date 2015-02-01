#!/usr/bin/perl

$outdir = "out";

opendir(OUTDIR,"$outdir");
@file = readdir(OUTDIR);
closedir(OUTDIR);

foreach(@file){
	if($_ !~ /.txt/){next;}
	$outfile = "$outdir/$_";
	system "perl SemCorrection.pl $outfile";
	#system "perl correct3.pl $outfile";
}
