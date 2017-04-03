#create simulator
set ns [new Simulator]

#Open the NAM trace file
set nf [open out_sack.nam w]
$ns namtrace-all $nf

#to create nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

# to create the link between the nodes with bandwidth, delay and queue
$ns duplex-link $n0 $n2   2Mb  10ms DropTail
$ns duplex-link $n1 $n2   2Mb  10ms DropTail
$ns duplex-link $n2 $n3 0.3Mb 200ms DropTail
$ns duplex-link $n3 $n4 0.5Mb  40ms DropTail
$ns duplex-link $n3 $n5 0.5Mb  30ms DropTail

# Sending node is 0 with agent as Reno Agent
set tcp1 [new Agent/TCP/Sack1]
set trace_file [open sack.tr w]
$ns attach-agent $n0 $tcp1

# receiving (sink) node is n4
set sink1 [new Agent/TCPSink/Sack1]
$ns attach-agent $n4 $sink1

# establish the traffic between the source and sink
$ns connect $tcp1 $sink1

# Setup a FTP traffic generator on "tcp1"
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1
$ftp1 set type_ FTP

#Setup a UDP connection
set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp $null


#Setup a CBR over UDP connection
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set type_ CBR
$cbr set packet_size_ 1000
$cbr set rate_ 0.1mb
$cbr set random_ false



# start/stop the traffic
$ns at 0.1   "$ftp1 start"
$ns at 40.0 "$ftp1 stop"
$ns at 0.2 "$cbr start"
$ns at 40.0 "$cbr stop"

$ns  trace-queue  $n2  $n3  $trace_file

# Set simulation end time
$ns at 50.0 "finish"



# procedure to plot the congestion window
proc plotWindow {tcpSource outfile} {
   global ns
   set now [$ns now]
   set cwnd [$tcpSource set cwnd_]

# the data is recorded in a file called congestion.xg (this can be plotted # using xgraph or gnuplot. this example uses xgraph to plot the cwnd_
   puts  $outfile  "$now $cwnd"
   $ns at [expr $now+0.1] "plotWindow $tcpSource  $outfile"
}

set outfile [open  "congestion_sack.xg"  w]
$ns  at  0.0  "plotWindow $tcp1  $outfile"




proc finish {} {
 exec xgraph congestion_sack.xg -geometry 300x300 &
 global ns nf
 $ns flush-trace
 #Close the NAM trace file
 close $nf
 #Execute NAM on the trace file
 exec nam out_sack.nam &
 exit 0
}



# Run simulation
$ns run
