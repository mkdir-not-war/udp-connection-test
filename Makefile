make:
	g++ -std=c++11 -pthread mysocket.cpp server.cpp -o server
	g++ -std=c++11 -pthread mysocket.cpp client.cpp -o client

makeserver:
	g++ -std=c++11 -pthread mysocket.cpp server.cpp -o server

makeclient:
	g++ -std=c++11 -pthread mysockset.cpp client.cpp -o client

clean:
	rm *~ ./client ./server

cleanserver:
	rm *~ ./server

cleanclient:
	rm *~ ./client
