
default: reno newreno sack vegas tahoe
a: a.cc
	g++ -o a a.cc

reno: a tcp_reno.tcl
	ns tcp_reno.tcl
	./a reno.tr
vegas: a tcp_vegas.tcl
	ns tcp_vegas.tcl
	./a vegas.tr
newreno: a tcp_newreno.tcl
	ns tcp_newreno.tcl
	./a newreno.tr
tahoe: a tcp_tahoe.tcl
	ns tcp_tahoe.tcl
	./a tahoe.tr
sack: a tcp_sack.tcl
	ns tcp_sack.tcl
	./a sack.tr

clean:
	rm *.tr a *.nam *.xg
