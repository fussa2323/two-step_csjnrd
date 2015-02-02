#!/usr/bin/perl

$cndir = "cn";

$semcndir = "semantic_cn";

opendir(CN,"$cndir");
@cnfile = readdir(CN);
closedir(CN);

foreach(@cnfile){
	if($_ =~ /.cn/){
		$cn = "$cndir/$_";
		$sem_cn = "$semcndir/$_";
		print "make $sem_cn\n";
		# `./semantic $cn $sem_cn`;
		`./semantic_csjnrd $cn $sem_cn`;
	}
}
