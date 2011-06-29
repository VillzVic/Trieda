#ifndef _HASH_UTIL_H_
#define _HASH_UTIL_H_

#include <string>

#define HASH_PRIME 100711433U // a large prime number

/** Hash function for integers. */
unsigned int intHash( unsigned int );

/** Hash function for strings. */
unsigned int strHash( std::string );

#endif
