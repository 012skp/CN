default: server client
	
server: server.o my_socket.o server.cc my_socket.cc 
	g++ server.o my_socket.o -o server
client: client.o my_socket.o client.cc my_socket.cc
	g++ client.o my_socket.o -o client
clean:
	rm *.o server client

