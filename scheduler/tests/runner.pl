#!/usr/bin/perl
# CREATED
#  Team Brilliant Squid
use strict;
use POSIX; 
use warnings;

sub average{
        my($data) = @_;
        if (not @$data) {
                die("Empty array\n");
        }
        my $total = 0;
        foreach (@$data) {
                $total += $_;
        }
        my $average = $total / @$data;
        return $average;
}
sub stdev{
        my($data) = @_;
        if(@$data == 1){
                return 0;
        }
        my $average = &average($data);
        my $sqtotal = 0;
        foreach(@$data) {
                $sqtotal += ($average-$_) ** 2;
        }
        my $std = ($sqtotal / (@$data-1)) ** 0.5;
        return $std;
}

sub shTimes {
    my @cmds = @_; 
    my $cmdStr = '';
    my $i = 0; 
    foreach my $cmd (@cmds){
	my $inter = '';
	if($i != 0){ $inter = ""; }
	$cmdStr = $cmdStr . "{ /usr/bin/time $cmd ; } 2>&1 >/dev/null | grep real > timeout_$i& \n " ;
	$i = $i + 1;
    }
    $cmdStr = $cmdStr . "wait";
    
    open(my $fh, '>', "runner.sh");
    print $fh $cmdStr; 
    close $fh;
    my $z0 = `chmod +x runner.sh`;
    system("./runner.sh");
    system("rm runner.sh");

    $i = 0; 
    my @times = ();
    foreach my $cmd (@cmds){
	open(FILE, '<', "timeout_$i");
	my @lines = <FILE>;
	if(scalar @lines == 0){
	    print "File timeout_$i empty\n";
	    exit(1);
	}
	else {
	    my $match = $lines[0];
	    $match =~ m/(\d+.\d+)/g; 
	    push(@times, $1);
	}
	close($fh);
	system("rm timeout_$i");
	$i = $i + 1; 
    }
    return @times; 
}


sub test {
    my ($desc, $c, $n) = @_;
    my @cmds = @$c;
    my @nices = @$n;
    
    if(scalar @cmds != scalar @nices){
	print "Number cmds and priorities do not match\n";
	exit(1);
    }

    print "==============================\n";
    print "$desc\n";    
    print "\tCommand \t\t Tickets\n";

    my $i = 0; 
    my @fullCmds = ();
    foreach (@cmds){
	print "\t$cmds[$i] \t\t $nices[$i]\n";
	my $nc = $nices[$i] - 20;
	push(@fullCmds, "/usr/bin/nice -n $nc $cmds[$i]");
	$i = $i + 1; 
    }
    
    
    my @iterTimes = ();
    for(my $iters = 0; $iters < 5; $iters++){
	print "\tTrial $iters...\n";
	my @time = shTimes(@fullCmds);
	push(@iterTimes, \@time);
    }
    
    my @times = ();
    my @stdevs = ();
    for(my $tj = 0; $tj < scalar @cmds; $tj++){
	my @vals = ();
	for(my $ti = 0; $ti < scalar @iterTimes; $ti++){
	    push(@vals, $iterTimes[$ti][$tj]);
	}
	push(@times, floor(average(\@vals) * 1000) / 1000);
	push(@stdevs, floor(stdev(\@vals) * 1000) / 1000);
    }    
	
    print "\nTests finished...\n";
    print "\tCommand \t Tickets \t Avg Time \t StDev \t Ratio\n";
    $i = 0; 
    foreach (@cmds){
	my $ratio = floor($times[$i] / $times[0] * 1000) / 1000;
	print "\t$cmds[$i] \t $nices[$i] \t\t $times[$i] \t\t $stdevs[$i] \t $ratio\n";
	$i = $i + 1; 
    }	
    print "==============================\n\n";
}

my @cmds = ("./cpu_bound", "./cpu_bound");
my @pris = (100, 100);
test("Show that running two equal CPU bound tasks with equal tickets lets them run at about the same speed", \@cmds, \@pris);

@cmds = ("./cpu_bound", "./cpu_bound");
@pris = (50, 100);
test("Show that running two CPU tasks where 1 has twice the tickets, the task with more tickets finishes in 1/2 the time", \@cmds, \@pris);

@cmds = ("./cpu_bound", "./cpu_bound", "./cpu_bound");
@pris = (25, 50, 100);
test("Show that running three CPU tasks with 25, 50, and 100 tickets runs the tasks in the right ratio", \@cmds, \@pris);

@cmds = ("./cpu_bound", "./cpu_bound", "./cpu_bound", "./cpu_bound");
@pris = (100, 100, 100, 1);
test("Show that running several CPU tasks with 100 tickets doesn't completely starve another task with just 1 ticket", \@cmds, \@pris);

@cmds = ("./cpu_bound", "./IO_bound IO_bound.in");
@pris = (100, 100);
test("Show that your dynamic scheduler improves performance when you mix CPU and IO 
bound tasks compared to keeping a fixed number of tickets for each process", \@cmds, \@pris);
