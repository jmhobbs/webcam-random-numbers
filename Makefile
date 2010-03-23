all: generator
generator: generator.cpp
	g++ generator.cpp -Wall -o Generator `pkg-config opencv --libs --cflags`
lwc: lwc.cpp
	g++ lwc.cpp -Wall -o lwc -lwebcam
clean:
	rm -f Generator
	rm -f lwc
