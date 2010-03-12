all:
	make clean
	make generator
	make bits
generator:
	g++ generator.cpp -o Generator `pkg-config opencv --libs --cflags`
bits:
	g++ bits.cpp -o Bits
clean:
	rm -f Generator
	rm -f Bits