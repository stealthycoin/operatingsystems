#!/usr/bin/perl
# CREATED
#  Team Brilliant Squid
use strict;
use warnings;

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
    print "Running test: $desc\n";    
    print "\tCommand \t\t Priority\n";

    my $i = 0; 
    my @fullCmds = ();
    foreach (@cmds){
	print "\t$cmds[$i] \t\t $nices[$i]\n";
	push(@fullCmds, "/usr/bin/nice -n $nices[$i] $cmds[$i]");
	$i = $i + 1; 
    }
    my @times = shTimes(@fullCmds); 

    print "\nTests finished...\n";
    print "\tCommand \t\t Priority \t\t Time \n";
    $i = 0; 
    foreach (@cmds){
	print "\t$cmds[$i] \t\t $nices[$i] \t\t\t $times[$i]\n";
	$i = $i + 1; 
    }	
    print "==============================\n\n";
}


my @cmds = ("./cpu_bound 10", "./cpu_bound 10");
my @pris = (100, 100);
test("Show that running two equal CPU bound tasks with equal tickets lets them run at about the same speed", \@cmds, \@pris);

@cmds = ("./cpu_bound 10", "./cpu_bound 10");
@pris = (50, 100);
test("Show that running two CPU tasks where 1 has twice the tickets, the task with more tickets finishes in 1/2 the time", \@cmds, \@pris);


@cmds = ("./cpu_bound 10", "./cpu_bound 10", "./cpu_bound 10");
@pris = (25, 50, 100);
test("Show that running three CPU tasks with 25, 50, and 100 tickets runs the tasks in the right ratio", \@cmds, \@pris);


@cmds = ("./cpu_bound 10", "./cpu_bound 10", "./cpu_bound 10", "./cpu_bound 10");
@pris = (100, 100, 100, 1);
test("Show that running several CPU tasks with 100 tickets doesn't completely starve another task with just 1 ticket", \@cmds, \@pris);


