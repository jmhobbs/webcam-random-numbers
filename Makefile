all: generator
generator: generator.cpp
	@g++ generator.cpp -o Generator `pkg-config opencv --libs --cflags`
bits: bits.cpp
	@g++ bits.cpp -o Bits
clean:
	rm -f Generator
	rm -f Bits