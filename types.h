
#ifndef TYPES_H
#define	TYPES_H

typedef signed char int8;
typedef unsigned char uint8;
typedef int int16;
typedef unsigned int uint16;
#ifdef B1
typedef /* short */ long int24;
typedef unsigned /* short */ long uint24;
#endif
typedef long int32;
typedef unsigned long uint32;
typedef char bool;

#define true  1
#define false 0

#endif	/* TYPES_H */
