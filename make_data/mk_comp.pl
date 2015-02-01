#!/usr/bin/perl

#outの結果と比較するConfusion Networkの作成
#比較したいから行はきちんと合わせておく
#２連続の<sil>とcn_failedが曲者

$cndir = "semantic_cn";
$compdir = "comp_cn";

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

foreach(@file){
	if($_ =~ /.cn/){
		$flag_sil = 0; #<sil><sil>のとき改行まみれになるのを防止したいフラグ
		$flag_fail = 0; #cn_failedのとき改行まみれ以下略
		$cnfile = "$cndir/$_";
		open(CNDATA,"$cnfile");
		$compcnfile = "$compdir/$_";
		open(COMPDATA,"> $compcnfile");
		print "$cnfile -> $compcnfile\n";
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[0]);
			#<sil>を見つけた時
			if($feature[0] eq "<sil>"){
				#２連続<sil>のとき
				if($flag_sil == 1){
					$flag_sil++;
					next;
				}
				$flag_sil = 1;
				next;
			}
			#Cn\\cn_failedを含む時
			elsif($feature[0] =~ /cn_failed/){
				$flag_fail = 1;p;
				next;
			}
			#２連続<sil>の後の改行は抹殺
			elsif($feature[0] eq "\n" and $flag_sil == 2){
				$flag_sil = 0;
				next;
			}
			#cn_failedの後の改行は抹殺
			elsif($feature[0] eq "\n" and $flag_fail == 1){
				$flag_fail = 0;
				next;
			}
			else{
				$line =~ s/\t\n/\n/;
				print COMPDATA "$line";
				$flag_fail = 0;
				$flag_sil = 0;
			}
		}
		close(CNDATA);
		close(COMPDATA);
	}
}

$cndir = "cn";
$compdir = "comp_nosem";

opendir(CNDIR,"$cndir");
@file = readdir(CNDIR);
closedir(CNDIR);

foreach(@file){
	if($_ =~ /.cn/){
		$flag_sil = 0; #<sil><sil>のとき改行まみれになるのを防止したいフラグ
		$flag_fail = 0; #cn_failedのとき改行まみれ以下略
		$cnfile = "$cndir/$_";
		open(CNDATA,"$cnfile");
		$compcnfile = "$compdir/$_";
		open(COMPDATA,"> $compcnfile");
		print "$cnfile -> $compcnfile\n";
		while(<CNDATA>){
			$line = $_;
			@cnwords = split(/\t/,$line);
			@feature = split(/\//,$cnwords[0]);
			#<sil>を見つけた時
			if($feature[0] eq "<sil>"){
				#２連続<sil>のとき
				if($flag_sil == 1){
					$flag_sil++;
					next;
				}
				$flag_sil = 1;
				next;
			}
			#Cn\\cn_failedを含む時
			elsif($feature[0] =~ /cn_failed/){
				$flag_fail = 1;p;
				next;
			}
			#２連続<sil>の後の改行は抹殺
			elsif($feature[0] eq "\n" and $flag_sil == 2){
				$flag_sil = 0;
				next;
			}
			#cn_failedの後の改行は抹殺
			elsif($feature[0] eq "\n" and $flag_fail == 1){
				$flag_fail = 0;
				next;
			}
			else{
				$line =~ s/\t\n/\n/;
				print COMPDATA "$line";
				$flag_fail = 0;
				$flag_sil = 0;
			}
		}
		close(CNDATA);
		close(COMPDATA);
	}
}