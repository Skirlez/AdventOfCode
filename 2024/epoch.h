/****************************************************************
*
* Author: Sean Jackson
* Last Modified: 06/29/22
* Description: Header for "Epoch" cross-platform time lib
*
****************************************************************/

#ifndef EPOCH_H
#define EPOCH_H

/////////////////////////////////////////////////////////////////

#define EPOCH_LOCAL_TIME 0
#define EPOCH_UTC_TIME 1

/////////////////////////////////////////////////////////////////

#ifdef _WIN32

// for time functions:
#include "windows.h"

// type for keeping track of ticks
typedef unsigned long long epoch_int;

// struct for epoch:
typedef struct
{
	epoch_int last, now;
	SYSTEMTIME systime;
} Epoch_t;

/////////////////////////////////////////////////////////////////

#else

#include <time.h>
#include <sys/time.h>

// timespec holds seconds and nanoseconds
typedef struct timespec timespec;

// struct for epoch:
typedef struct
{
	timespec last, now;
} Epoch_t;

/////////////////////////////////////////////////////////////////

#endif

/*
* struct that holds basic date and time information
* from operating system
*/
typedef struct
{
	unsigned short year;
	unsigned short month;
	unsigned short weekday;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
} Epoch_SysInfo_t;

/////////////////////////////////////////////////////////////////

// initialize epochness
unsigned char Epoch__Init();
// start an epoch timer
void Epoch__Start(Epoch_t* e);
// return float time (seconds) since start
double Epoch__QueryChange(Epoch_t* e);
// return system date and time information
// for type, use either EPOCH_LOCAL_TIME or EPOCH_UTC_TIME
void Epoch__QuerySysInfo(Epoch_SysInfo_t* t, unsigned char type);

/////////////////////////////////////////////////////////////////

#endif //EPOCH_H

/////////////////////////////////////////////////////////////////