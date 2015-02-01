#!/usr/bin/perl

$cnsrrdir = "../../cnsrr";
$refdir = "../../ref";

opendir(SRR,"$cnsrrdir/train2");
@hypfile = readdir(SRR);
closedir(SRR);

foreach(@hypfile){
	if($_ =~ /.srr/){
		$srr = "$cnsrrdir/train2/$_";
		$ref = "$refdir/$_";
		$ref =~ s/srr/ref/;
		$cn = "cn/$_";
		#srr→cnに置き換え
		$cn =~ s/srr/cn/;
		`./make_trainingdata $srr $ref $cn`;
		print "$srr $cn\n";
	}
}
