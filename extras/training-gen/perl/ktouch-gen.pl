#!/usr/bin/perl -w




# genword( accumulated, core )
# This function will generate a random sequens of character
# containing only characters from accum or core and all words
# generated will contain at lest on character from core
sub genword
{
    my $res="";
    (my $accum, my $core) = @_;
    my $ran = int(rand()*2+2);

    $test=$accum.$core;


    for(my $i=0;$i<$ran;$i++)
    {
	$g=int(rand()*length($test));
	$res=$res.substr($test,$g,1);
    }
    if ($res =~ m/[$core]/)
    {
	return $res;
    }
    else
    {
	return genword($accum,$core);
    }

}



# This function will return a list of words containing only characters from
# accum or core. And all words will contain at lest on echaracter from core.
# If we can't find enough words, we will use genword to generate enough words
# to fill the list.
sub genlist
{
    ($accum, $core) = @_;
    my @res;
    $all=$accum.$core;
    foreach(@all_words)
    {
	chomp($_);
	if (m/[$core]/ && m/^[$all][$all]*$/){
	    push @res,$_;
	}
    }

    for(my $i=@res;$i<30;$i++)
    {
	push @res,genword($accum,$core);

    }

    return @res;
}


# Genlevel will generate a level
sub genlevel
{
    ($accum, $core) = @_;
    my $res = "";
    my @list = genlist($accum,$core);
    my $lines = 30;
    my $words = 60;
    while($lines >0)
    {
	while($words >0)
	{
	    my $tmp=$list[rand()*@list];
	    $res = $tmp." ".$res; 
	    $words = $words - (length($tmp) + 1); # +1 is for counting one extra space for each word
	}
	$res = "\n".$res;
	$words = 60;
	$lines = $lines - 1;
    }	
    return $core.$res;
}

sub print_heading
{
    print "######################################################################\n";
    print "##\n";
    print "# KTouch training file generated ".localtime(time())."\n";
    print "#\n";
    print "# Perl Script written by Steinar Theigmann & Håvard Frøiland.\n";
    print "#\n";
}

sub print_usage
  {
    die "usage: ktouch-gen config_file\n
           Example: ./ktouch-gen english-conf < english-word-file\n\n";
  }

# ------------------------ MAIN ----------------------------

if(@ARGV == 0)
  {
    print_usage;
    exit 0;
  }


print_heading;

# read in all files from standard inn and put it in @list
while (<STDIN>)
  {
    chomp($_);
    push @all_words, $_;
  }

open(CONFIG,$ARGV[0]) or
  die("The file $ARGV[0] did not exist\n"); # First argument should be config file

while (<CONFIG>)
  {
    chomp($_);
    push @config,$_;
  }

$accum="";

$count=0;
foreach(@config)
  {
    $count++;
    print "# Level $count\n";
    print genlevel($accum,$_);
    $accum=$accum.$_;
    print "\n\n";
  }




