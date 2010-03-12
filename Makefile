all:
	g++ generator.cpp -o Generator `pkg-config opencv --libs --cflags`
clean:
	rm -f Generator
