server: server.o dijkstra.o digraph.o
	g++ -o server server.o dijkstra.o digraph.o

server.o: server.cpp
	g++ -c server.cpp

dijkstra.o: dijkstra.cpp dijkstra.h
	g++ -c dijkstra.cpp

digraph.o: digraph.cpp digraph.h wdigraph.h
	g++ -c digraph.cpp

clean:
	rm *.o 
	rm -f server
