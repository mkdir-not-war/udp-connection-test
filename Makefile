make:
	g++ -std=c++11 -pthread mysocket.cpp server.cpp -o server
	g++ -std=c++11 -pthread mysocket.cpp client.cpp -o client

makewin:
	x86_64-w64-mingw32-g++-posix -std=c++11 -pthread mysocket.cpp server.cpp -o server.exe -lws2_32 -static-libgcc -static-libstdc++ -static -lpthread
	x86_64-w64-mingw32-g++-posix -std=c++11 -pthread mysocket.cpp client.cpp -o client.exe -lws2_32 -static-libgcc -static-libstdc++ -static -lpthread

makeserver:
	g++ -std=c++11 -pthread mysocket.cpp server.cpp -o server

makeclient:
	g++ -std=c++11 -pthread mysockset.cpp client.cpp -o client

clean:
	rm *~ ./client ./server

cleanexe:
	rm ./client.exe ./server.exe

cleanserver:
	rm *~ ./server

cleanclient:
	rm *~ ./client
