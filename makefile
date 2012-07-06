
all:
	g++ -I./src ./src/instrumentor.cc -o instrumentor -lclang
	g++ -I./src ./src/path_record.cc -c

