#include <stdlib.h>
#include <time.h>

class Chrono{

    public:
	Chrono();
	Chrono( struct timespec & );
	~Chrono();
	int getTime();	// Reads system time
	int getSecs();	// Returns the seconds part of time in variable
	int getnSecs();	// Returns the nanoseconds part of time in variable
	Chrono & operator =  ( const Chrono & rhs );
	Chrono & operator +  ( const Chrono & rhs );
	Chrono & operator -  ( const Chrono & rhs );
	Chrono & operator += ( const Chrono & rhs );
	Chrono & operator -= ( const Chrono & rhs );

    private:
      struct timespec ts;
};
