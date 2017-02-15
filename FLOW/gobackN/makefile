defult: server client

server: server.o server.cc my_socket.o my_socket.cc
	g++ server.o my_socket.o  -o server
client: client.o client.cc my_socket.o my_socket.cc
	g++ client.o my_socket.o  -lpthread -o client

clean:
	rm *.o server client