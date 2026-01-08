#ifndef __TOOLS__
#define __TOOLS__
	#define DISABLE_DEBUGMSG
	
	#ifdef DISABLE_DEBUGMSG
		#define STUB()
		#define INCOMPLETE()
		#define LOG(...)
	#else
		#ifdef __APPLE
			#include <SDL.h>
		#else
			#include <SDL3/SDL.h>
		#endif
		
		#ifdef __cplusplus
			#define CODING_LANGUAGE "C++"
		#else
			#define CODING_LANGUAGE "C"
		#endif
		
		#define STUB() SDL_Log("STUB: [" CODING_LANGUAGE "] %s: %d - %s;", __FILE__, __LINE__, __PRETTY_FUNCTION__)
		#define INCOMPLETE() SDL_Log("INCOMPLETE: [" CODING_LANGUAGE "] %s: %d - %s;", __FILE__, __LINE__, __PRETTY_FUNCTION__)
		#define LOG(...) SDL_Log(PREFIX_DEBUG ": " __VA_ARGS__)
	#endif
#endif