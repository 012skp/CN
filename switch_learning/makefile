default: node switch

node: node.o my_socket.o
	g++ node.o my_socket.o -o node
switch: switch.o my_socket.o
	g++ switch.o my_socket.o -o switch -lpthread
clean:
	rm *.o switch node
