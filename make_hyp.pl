#!/usr/bin/perl

use IO::File;

$outdir = "out"; #訂正後のディレクトリ
#$outdir = "out_back"; #訂正前のディレクトリ
#$outdir = "make_data/oracle"; #WER最小のディレクトリ
$hypdir = "hyp"; #仮説を格納するディレクトリ

opendir(OUTDIR,"$outdir");
@file = readdir(OUTDIR);
closedir(OUTDIR);

foreach(@file){
	if($_ !~ /.txt/){next;}
	$outfile = "$outdir/$_";
	$hypfile = "$hypdir/$_";
	$hypfile =~ s/txt/hyp/;
	open(OUT,"$outfile");
	open(HYP,"> $hypfile");
	print "$hypfile\n";
	@words = ();
	
	while(<OUT>){
		$line = $_;
		@feature = split(/\t/,$line);
		if(($feature[0] eq "<sp>") or ($feature[0] eq "-")){next;}
		elsif($feature[0] eq "\n"){
			for($i = 0; $i <= $#words; $i++){
				print HYP "$words[$i]";
				print HYP " ";
			}
			@words = ();
			print HYP "\n";
		}
		else{push(@words,$feature[0]);}
	}
	
	close(HYP);
	close(OUT);
}

#ファイルの行数が違うとき<sil><sil>またはcn_failedがある
#そこは改行を挿入
#ヌル遷移だけの発話はたぶん大丈夫

$tmp = "data.tmp";
#$cndir = "make_data/semantic_cn/train2"; #CNのディレクトリ
#new_semantic_cnはsemanti_cn行数が異なるので、
#semantic_cnで行数合わせしなければならない
$cndir = "make_data/semantic_cn"; #CNのディレクトリ

opendir(HYPDIR,"$hypdir");
@hypfile = readdir(HYPDIR);
closedir(HYPDIR);

foreach(@hypfile){
	if($_ !~ /.hyp/){next;}
	open(TMP,"> $tmp");
	print "$_\n";
	$hypfile_name = $_;
	$hyp = "$hypdir/$_";
	$cnfile = "$cndir/$_";
	$cnfile =~ s/hyp/cn/;
	open(HYP,"$hyp");
	open(CN,"$cnfile");
	@cn = <CN>;
	$i = 0;
	$flag = 0;
	
	while(<HYP>){
		$line = $_;
		while($cn[$i] ne "\n"){
			@cs_first = split(/\t/,$cn[$i]);
			@feature_first = split(/\//,$cs_first[0]);
			@cs_second = split(/\t/,$cn[$i+1]);
			@feature_second = split(/\//,$cs_second[0]);
			
			if($feature_first[0] eq "<sil>" and $feature_second[0] eq "<sil>"){
				print TMP "\n";
				$i++;
				$flag++; #改行挿入フラグ
			}
			$i++;
			#改行を挿入した場合、TMPとHYPの間に以降行数の差が出るため、CNを先に進めて対処
			if($cn[$i] eq "\n" and $flag == 1){
				$i++;
				$flag = 0;
			}
		}
		
		print TMP "$line";
		$i++;
	}
	close(HYP);
	close(CN);
	close(TMP);
	`mv "$tmp" "$hypfile_name"`;
	`mv "$hypfile_name" "$hypdir"`
}