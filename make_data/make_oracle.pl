#!/usr/bin/perl

#CN_oracle単語列を求める

$cndir = "comp_cn";
$oracledir = "oracle";

opendir(CNDIR,"$cndir");
@cnfile = readdir(CNDIR);
closedir(CNDIR);

foreach(@cnfile){
	if($_ !~ /.cn/){next;}
	$file = "$cndir/$_";
	$oracle_file = "$oracledir/$_";
	$oracle_file =~ s/cn/txt/;
	print "$oracle_file\n";
	open(CN,"$file");
	open(OCL,"> $oracle_file");
	while(<CN>){
		$line = $_;
		if($line eq "\n"){
			print OCL "\n";
			next;
		}
		@cnset = split(/\t/,$line);
		foreach(@cnset){
			$cn_elem = $_;
			@elem = split(/\//,$cn_elem);
			$elem[6] =~ s/\n//;
			if($elem[6] eq "C"){
				print OCL "$elem[0]\t$elem[1]\t$elem[3]\t$elem[4]\t$elem[5]\t$elem[6]\n";
				last;
			}
			elsif($cn_elem eq $cnset[$#cnset]){
				@elem = split(/\//,$cnset[0]);
				$elem[6] =~ s/\n//;
				print OCL "$elem[0]\t$elem[1]\t$elem[3]\t$elem[4]\t$elem[5]\t$elem[6]\n";
			}
			else{}
		}
	}
	close(CN);
	close(OCL);
}
