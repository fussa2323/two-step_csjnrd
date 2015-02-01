#!/usr/bin/perl

$outdir = "../out";
$dir = "semantic_out";
$testdir = "out_semtest";
$tmp = "data.tmp";
$model = "semmodel";

opendir(CNDIR,"$outdir");
@file = readdir(CNDIR);
closedir(CNDIR);

foreach(@file){
	if($_ !~ /.txt/){next;}
	$outfile = "$outdir/$_";
	open(OUT,"$outfile");

	$output = "$dir/$_";
	open(TMP,"> $tmp");
	
	print "$outfile -> $output\n";
	
	while($line = <OUT>){
		if($line eq "\n"){
			print TMP "\n";
			next;
		}
		@feature = split(/\t/,$line);
		print TMP "$feature[0]/$feature[1]/0/$feature[5]\n";
	}
	close(OUT);
	close(TMP);
	#意味スコアの付与
	#`./make_data/semantic $tmp $output`;
}

#意味スコアの離散化
opendir(DIR,"$dir");
@file = readdir(DIR);
closedir(DIR);

foreach(@file){
	if($_ !~ /.txt/){next;}
	
	$semfile = "$dir/$_";
	open(SEM,"$semfile");
	
	$testfile = "$testdir/$_";
	open(TESTDATA,"> $tmp");
	
	print "$semfile -> $testfile\n";
	
	while($line = <SEM>){
		if($line eq "\n"){
			print TESTDATA "\n";
			next;
		}
		@feature = split(/\//,$line);
		$feature[6] =~ s/\n//;
		
		if($feature[3] eq "*"){
			print TESTDATA "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
		}
		else{
			$feature[3] = int($feature[3] * 10);
			$feature[4] = int($feature[4] * 10);
			$feature[5] = int($feature[5]);
			print TESTDATA "$feature[0]\t$feature[1]\t$feature[3]\t$feature[4]\t$feature[5]\t$feature[6]\n";
		}
	}
	close(SEM);
	close(TESTDATA);
	
	`../CRF_0.57/bin/crf_test -m $model $tmp > $testfile`;
}
