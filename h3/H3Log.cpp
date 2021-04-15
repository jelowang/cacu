
/*

	+	H3Log.cpp
	+	QDL

	+	hello3d 
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/
 
#include "H3Log.h"

void H3Log ( int module , const char* log , ... ) 
{

	va_list ap ;   
	va_start ( ap , log ) ;   
	vprintf ( log , ap ) ;
	vprintf ( "\n" , ap ) ;
	va_end ( ap ) ;

	#ifdef WIN32
	{

		FILE *file = 0 ;
		file = fopen("./Resources/i51.log" , "rb") ;
		
		if ( file > 0 ) {

			fseek(file,0,SEEK_END) ;
			va_start ( ap , log ) ;   
			vfprintf( file , log , ap ) ;
			vprintf ( "\n" , ap ) ;
			va_end(ap) ;
			fclose(file) ;

		}
	}
	#endif
	
}
