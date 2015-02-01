#!/usr/bin/perl

#refのディーエヌエーをＤＮＡに変換

$olddir = "../utf8_ref";
$refdir = "../ref";

opendir(REFDIR,"$olddir");
@file = readdir(REFDIR);
closedir(REFDIR);

foreach(@file){
	if($_ !~ /.ref/){next;}
	$oldfile = "$olddir/$_";
	$reffile = "$refdir/$_";
	
	open(OLD,"$oldfile");
	open(REF,"> $reffile");
	print "$oldfile --> $reffile\n";
	
	while($line = <OLD>){
		if($line =~ /ディーエヌエー/){
			$line =~ s/ディーエヌエー/ＤＮＡ/;
		}
		print REF "$line";
	}
	close(OLD);
	close(REF);
}
