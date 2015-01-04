#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LOW 0
#define HIGH 1
#define IN 0
#define OUT 1
#define PIN 23
#define PATHLEN 35
#define BUFLEN 3
#define DEL 250000

int gpio=23;

static int GPIOExport( int pin ){
	char buffer[BUFLEN];
	ssize_t bytes_written;
	int fd;

	fd = open( "/sys/class/gpio/export", O_WRONLY );
	if ( fd == -1 ){
		fprintf( stderr, "Failed to open export for writing.\n" );
		return( -1 );
	}

	bytes_written = snprintf( buffer, BUFLEN, "%d", pin );
	write( fd, buffer, bytes_written );
	close( fd );
	return( 0 );
}


static int GPIOUnexport( int pin ){
	char buffer[BUFLEN];
	ssize_t bytes_written;
	int fd;

	fd = open( "/sys/class/gpio/unexport", O_WRONLY );
	if ( fd == -1 ){
		fprintf( stderr, "Failed to open unexport for writing.\n" );
		return( -1 );
	}

	bytes_written = snprintf( buffer, BUFLEN, "%d", pin );
	write( fd, buffer, bytes_written );
	close( fd );
	return( 0 );
}

static int GPIODirection( int pin, int dir ){
	static const char s_dir_str[] = "in\0out";
	char path[PATHLEN];
	int fd;

	snprintf( path, 35, "/sys/class/gpio/gpio%d/direction", pin );
	fd = open( path, O_WRONLY );
	if ( fd == -1 ){
		fprintf( stderr, "Failed to open gpio direction for writing.\n" );
		return( -1 );
	}
	if ( write( fd, &s_dir_str[ IN == dir ? 0 : 3 ], IN == dir ? 2 : 3 ) == -1 ){
		fprintf( stderr, "Failed to set direction.\n" );
		return( -1 );
	}
	close( fd );
	return( 0 );
}

static int GPIOWrite( int pin, int value ){
	static const char s_val_str[] = "01";
	char path[PATHLEN];
	int fd;
	
	snprintf( path, PATHLEN, "/sys/class/gpio/gpio%d/value", pin );
	fd = open( path, O_WRONLY );
	if ( fd == -1 ){
		fprintf( stderr, "Failed to open gpio value for writing.\n" );
		return( -1 );
	}

	if ( write( fd, &s_val_str[ LOW == value ? 0 : 1 ], 1 ) != 1 ){
		fprintf( stderr, "Failed to write value for gpio%d.\n", pin );
		return( -1 );
	}
	close( fd );
	return( 0 );
}

int main( int argc, char *argv[]){
	int state;
	if ( strstr( argv[0], "amp-on" ) != NULL ){
		state=LOW;
	}
	if ( strstr( argv[0], "amp-off" ) != NULL ){
		state=HIGH;
	}
	if ( GPIOExport( gpio ) == -1 )
		return( 1 );
	if ( GPIODirection( gpio, OUT ) == -1 )
		return( 2 );
	if ( GPIOWrite( gpio, state ) == -1 )
		return( 3 );
	if ( GPIOUnexport( gpio ) == -1 )
		return( 4 );
}
