
/*

+	Semo Compiler Common Library 

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

# ifndef __SCCL
# define __SCCL

# define SC_SCCL_TEST

//	the default length of SCCString
//	create a SCCString with SCClSLEN could accelerate it compare with realloc
# define SCClSLEN 1024

typedef enum {

	//	author : QDL
	//	since : 20100609
 
	//	notes : seek

	SCCLLISTSEEK_HEAD ,
	SCCLLISTSEEK_TAIL ,

} SCCLLISTSEEK ;

typedef struct ILN {

	//	author : QDL
	//	notes : SLIST
	//	since : 20091123
	//	(C)TOK

	//	data type of element
    int key ;
	int index ; 
	int eltype ;
	int totall ;

	void* element ;	
	struct ILN* next ;
	struct ILN* last ;
	
} SCClList ;

typedef struct ISTK {

	//	author : QDL
	//	since : 20090819
	//	(C)TOK
	
	void* element ;

	int deep ;
	
	struct ISTK* front ;		
	struct ISTK* next ;

} SCClStack ;

typedef struct QSTK {

	//	author : QDL
	//	since : 20100802
	//	(C)TOK
	
	int totall ;

	void* data ;
	struct QSTK* front ;
	struct QSTK* rear ;
	 
} SCClQueue ;

# endif

SCClList* SCClListNew () ;
void SCClListInit ( SCClList* list ) ;
void SCClListInsert ( SCClList* list , void* el ) ;
void SCClListInsertEx ( SCClList* list , void* el , int key ) ;
void SCClListDelete ( SCClList* list , void* el ) ;
int SCClListEmpty ( SCClList* list ) ;
void* SCClListSearch ( SCClList* lt , void* element ) ;
void* SCClListSearchEx ( SCClList* lt , int key ) ;
void SCClListClear ( SCClList* list ) ;
void SCClListDestroy ( SCClList** list ) ;

void SCClStackInit ( SCClStack* istack ) ;
void* SCClStackGet ( SCClStack* istack  ) ;
int SCClStackEmpty ( SCClStack* istack ) ;
void SCClStackPush ( SCClStack* istack , void* eelement ) ;
void* SCClStackPop ( SCClStack* istack ) ;
int SCClStackGetDeep ( SCClStack* istack ) ;
void SCClStackDestroy ( SCClStack* istack ) ;

void SCClQueueInit ( SCClQueue* queue ) ;
int SCClQueueEmpty ( SCClQueue* queue ) ;
void SCClQueueEnter ( SCClQueue* queue , void* element ) ;
void* SCClQueueOut ( SCClQueue* queue ) ;
void SCClQueueDestroy ( SCClQueue* queue ) ;