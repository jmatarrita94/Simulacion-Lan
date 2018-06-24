#include "Chrono.h"
/*
struct timespec {
        time_t  tv_sec;         // seconds
        long    tv_nsec;        // nanoseconds 
};
*/

Chrono::Chrono() {
   ts.tv_sec = 0;
   ts.tv_nsec = 0;
}

Chrono::Chrono( struct timespec & t ) {

   ts.tv_sec = t.tv_sec;
   ts.tv_nsec = t.tv_nsec;

}

Chrono::~Chrono() {
}

int Chrono::getTime() {

   struct timespec t;

   clock_gettime( CLOCK_MONOTONIC, &t );
   ts.tv_sec = t.tv_sec;
   ts.tv_nsec = t.tv_nsec;

}

int Chrono::getSecs() {

   return ts.tv_sec;

}

int Chrono::getnSecs() {

   return ts.tv_nsec;

}

Chrono & Chrono::operator = ( const Chrono &rhs ) {

   if ( this != &rhs ) {
      ts.tv_sec = rhs.ts.tv_sec;
      ts.tv_nsec = rhs.ts.tv_nsec;
   }

   return *this;
}

Chrono & Chrono::operator - ( const Chrono &rhs ) {

   if ( this != &rhs ) {
      if ( (ts.tv_nsec - rhs.ts.tv_nsec) < 0) {	// nanoseconds difference is negative
         ts.tv_sec = ts.tv_sec - rhs.ts.tv_sec - 1;	// Substract one from seconds count
         ts.tv_nsec = 1000000000 + ts.tv_nsec - rhs.ts.tv_nsec;	// Add one second to diff
      } else {
         ts.tv_sec -= rhs.ts.tv_sec;
         ts.tv_nsec -= rhs.ts.tv_sec;
      }
   }

   return *this;
}

Chrono & Chrono::operator + ( const Chrono &rhs ) {

  if ( this != &rhs ) {
    ts.tv_sec += rhs.ts.tv_sec;
    ts.tv_nsec += rhs.ts.tv_sec;
  }

  return *this;
}

Chrono & Chrono::operator -= ( const Chrono &rhs ) {

   return *this - rhs;

}

Chrono & Chrono::operator += ( const Chrono &rhs ) {

   return *this + rhs;

}
