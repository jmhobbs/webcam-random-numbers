#include <iostream>
#include <unistd.h>

// Awesomeness! http://graphics.stanford.edu/~seander/bithacks.html#MaskedMerge

void print_binary ( char n ) {
	std::cout << "== Print Binary ==" << std::endl;
	std::cout << "  decimal: " << (int) n << std::endl;
	std::cout << "  binary : ";
	for( int i = 7; i >= 0; i-- ) {
		int bit = ( ( n >> i ) & 1 );
		std::cout << bit;
	}
	std::cout << std::endl;
}

int main ( int argc, char ** argv ) {
	char a,b;
	a = (char) 100;
	b = (char) 5;
	char mask = (char) 1;
	print_binary( a );
	print_binary( b );
	print_binary ( mask );
	char r = a ^ ( ( a ^ b ) & mask );
	print_binary( r );
}