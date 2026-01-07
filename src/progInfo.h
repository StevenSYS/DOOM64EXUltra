#ifndef __PROGINFO__
#define __PROGINFO__

/* - Program - */
#ifndef PROGRAM_PRE_RELEASE
	#define PROGRAM_PRE_RELEASE 1
#endif

#define PROGRAM_NAME "DOOM 64 EX Ultra"
#if PROGRAM_PRE_RELEASE
	#define PROGRAM_VERSION "1.0pre"
#else
	#define PROGRAM_VERSION "1.0"
#endif

#define PROGRAM_COMPDATE __DATE__

/* - File - */
#define FILE_SRCPORT "doom64ex-ultra.wad"
#define FILE_QUICKSAVE "doomsavq.dsg"

/* - Path - */
#define PATH_PREF "doom64ex-ultra"

/* - Net - */
#define NET_VERSION "DOOM64EXUltra"

/* - Font - */
#define FONT_SIZE 26.0f

/* - Min - */
#define MIN_WIDTH 320.0f
#define MIN_HEIGHT 200.0f

/* - Slot - */
#define SLOT_QUICKSAVE 8

/* - Misc. - */
#define FPS 30

#endif
