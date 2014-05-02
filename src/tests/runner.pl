#!/usr/bin/perl
# CREATED
#  Team Brilliant Squid
use strict;
use threads;
use warnings;
use Time::HiRes;


#...


#...


sub timeIt {
    my $str = $_[0];
    my $start_time = [Time::HiRes::gettimeofday()];
    my $output = `$str`;
    my $diff = Time::HiRes::tv_interval($start_time);
    return $diff;
}

sub timeThem {
    my @strs= @_;
    my @threads = ();
    my @times = (); 
    foreach my $str (@strs){
	print "Command: $str\n";
	push(@threads, async{ return timeIt($str) });
    }

    foreach my $t (@threads){
	my $out = $t->join;
	push(@times, $out);
    }

    foreach my $time (@times){
	print "Time: $time\n";
    }
    return @times; 
}

my $start_time = [Time::HiRes::gettimeofday()];
timeThem("./cpu_bound 10", "./cpu_bound 10", "./IO_bound cpu_bound");
my $diff = Time::HiRes::tv_interval($start_time);
print "Total time for all tests: $diff s\n";
