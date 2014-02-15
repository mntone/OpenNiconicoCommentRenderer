#ifdef NICO_RENDERER_EXPORTS
	#ifdef _WINDOWS
		#define NICORIMP __declspec( dllexport )
	#else
		#define NICORIMP
	#endif
#else
	#ifdef _WINDOWS
		#define NICORIMP __declspec( dllimport )
	#else
		#define NICORIMP
	#endif
#endif