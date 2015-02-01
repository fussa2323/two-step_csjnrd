#!/usr/bin/perl

opendir(CN,"semantic_cn/test");
@cnfile = readdir(CN);
closedir(CN);

$count = 0;
$first = 0;
$semcount = 0;

foreach(@cnfile){
	if($_ !~ /.cn/){next;}
	$sem_cn = "semantic_cn/test/$_";
	open(CN,"$sem_cn");
	while($line = <CN>){
		if($line eq "\n"){next;}
		$first++;
		$line =~ s/\n//;
		@words = split(/\s/,$line);
		$count += @words;
		#foreach $elem (@words){
			#@elems = split(/\//,$elem);
			@elems = split(/\//,$words[0]);
			if($elems[3] ne "*"){
				$semcount++;
			}
		#}
	}
	close(CN);
}

print "Num of first words : $first\n";
print "Num of words : $count, Num of semantic words : $semcount\n";