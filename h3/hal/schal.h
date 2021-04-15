
/*

+	Semo Compiler HAL 

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by QDL since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		-Thanks to Our Committers and Friends
+		-Best Wish to all who Contributed and Inspired
+		-Techniques of Knowledge 
+		-致伙伴们最美好祝愿
+		-突壳开源社区

*/ 

#if defined(_MSC_VER)
#pragma once
#endif

# ifndef __HAL
# define __HAL


# include <stdio.h>
# include <memory.h>
# include <stdarg.h>
# include <string.h>
# include <time.h>
# include <string>
using namespace std ;

extern void SCHalSetAbsoluteMode ( bool enable ) ;
extern bool SCHalGetAbsoluteMode () ;
extern const char* SCHalGetWritablePath () ;
extern void* SCHalFileOpen ( const char* path , const char* flag ) ;
extern int SCHalFileSeek( void* file , int offset , int direct ) ;
extern int SCHalFileRead ( void* file , void* buffer , long long totall ) ;
extern int SCHalFileWrite ( void* file , void* buffer , long long totall ) ;
extern short int SCHalFileGetc ( void* file ) ;
extern int SCHalFileEnd ( void* file ) ;
extern int SCHalFileLength( void* file ) ;
extern int SCHalFileClose ( void* file ) ;

extern const char* SCHalGetFilePath ( const char* name ) ;
extern long SCHalGetTick() ;

# endif