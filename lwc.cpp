#include <webcam.h>
#include <iostream>
#include <stdlib.h>

int main( int argc, char *argv[] ) {

	CResult result;
	CHandle handle;
	CDevice *devices = NULL;
	unsigned int csize, numDevices;
	
	std::cout << "Starting up." << std::endl;
	
	// Start libwebcam
	result = c_init();
	if ( result != C_SUCCESS ) {
		std::cerr << "libwebcam init failure: " << c_get_error_text( result ) << std::endl;
		return -1;
	}

	c_enum_devices( devices, &csize, &numDevices );
	devices = static_cast<CDevice*> ( malloc( csize ) );
	result = c_enum_devices( devices, &csize, &numDevices );
	std::cout << csize << ":" << numDevices << std::endl;
	if ( result != C_SUCCESS ) {
		std::cerr << "libwebcam enum_devices failure: " << c_get_error_text( result ) << std::endl;
		free( devices );
		return -1;
	}

	handle = c_open_device( devices->shortName );
	if ( ! handle ) {
		std::cerr << "libwebcam open failure: " << c_get_error_text( handle ) << std::endl;
		free( devices );
	}

	std::cout << "Shutting down." << std::endl;

	c_close_device( handle );
	c_cleanup();
}