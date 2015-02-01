#!/usr/bin/perl

$oov = 0;
open(W,"wordorg.txt");
open(W1,"wordssorted.txt");
open(O,">oovdic.txt");
@dicwords=();
@words=();
while(<W>){
    $line=$_;
    chomp($line);
    push(@dicwords,$line);

}

while(<W1>){
    $line=$_;
    chomp($line);
    push(@words,$line);
    
}

%dichash=map{$_=>1} @dicwords;
foreach $word(@words){
    if(exists($dichash{$word})){
        next;
    }
    else{
    print O "$word\n";
    $oov++
    }


}




print "The num of <UNKNOWN> is $oov.\n";
close(W);
close(W1);
close(O);
