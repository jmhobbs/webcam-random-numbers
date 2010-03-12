#include <iostream>
#include <unistd.h>

void print_binary ( char n ) {
	std::cout << "== Print Binary ==" << std::endl;
	std::cout << "  char   : " << n << std::endl;
	std::cout << "  decimal: " << (int) n << std::endl;
	std::cout << "  binary : ";
	for( int i = 7; i >= 0; i-- ) {
		int bit = ( ( n >> i ) & 1 );
		std::cout << bit;
	}
	std::cout << std::endl;
}

int main ( int argc, char ** argv ) {
	char byte;
	std::cout << "Size of char = " << sizeof( byte ) << " byte" << std::endl;
	byte = 'A';
	print_binary( byte );
}