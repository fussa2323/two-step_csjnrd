#!/usr/bin/perl

$cndir = "new_cn";

$semcndir = "new_semantic_cn";

opendir(CN,"$cndir");
@cnfile = readdir(CN);
closedir(CN);

foreach(@cnfile){
	if($_ =~ /.cn/){
		$cn = "$cndir/$_";
		$sem_cn = "$semcndir/$_";
		print "make $sem_cn\n";
		`./semantic $cn $sem_cn`;
	}
}
