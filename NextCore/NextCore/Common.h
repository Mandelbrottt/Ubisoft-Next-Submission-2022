#pragma once

#define NEXT_CORE_SHARED_LIB

#define NEXT_CORE_API __cdecl

// TODO: Add more platform support
#ifdef _WIN32
	#ifndef _WINDLL
		#undef NEXT_CORE_SHARED_LIB
	#endif
#else
	#undef NEXT_CORE_SHARED_LIB

	#error NextCore only supports windows!
#endif

// TODO: Fix SharedLib not working
// TODO: Find a better structure for platform dependent defines
#ifdef NEXT_CORE_SHARED_LIB
	#ifdef NEXT_CORE
		#ifdef _WIN32
			#define NEXT_CORE_EXPORT __declspec(dllexport)
		#endif
	#else
		#ifdef _WIN32
			#define NEXT_CORE_EXPORT __declspec(dllimport)
		#endif
	#endif
#else
	#define NEXT_CORE_EXPORT
#endif