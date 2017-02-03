server: server.o gen_check_sum.o
	cc server.o gen_check_sum.o -o server
client: client.o gen_check_sum.o
	cc client.o gen_check_sum.o -o client
server.o: server.c gen_check_sum.h
	cc -c server.c
client.o: client.c gen_check_sum.h
	cc -c client.c
gen_check_sum.o: gen_check_sum.c gen_check_sum.h
	cc -c gen_check_sum.c
clean: 
	rm *.o server client
