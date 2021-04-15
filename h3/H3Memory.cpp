
/*

	+	H3Memory.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Log.h"
#ifdef WIN32
#include "malloc.h"
#endif
#include "string.h"
#include "H3Memory.h"

#ifdef H3MEMORY_ENABLE_MEMORY_CHECK

typedef struct MMN {
	
	char* file ;
	int line ;
	int length ;
	long long address ;
	int totall ;
	
	struct MMN* next ;	
	struct MMN* last ;
	
} MEMORY_MONITOR ;

# define MMT_MASK_LENGTH 32
unsigned int* mmt_mask = 0 ;
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 , 0 } ;
int MMTInit () {

	//	author : QDL
	//	since : 20100418
	//	(C)TOK
	
	mem_monitor.file = 0 ;
	mem_monitor.line = 0 ;
	mem_monitor.length = 0 ;
	mem_monitor.address = 0 ;	
	mem_monitor.totall = 0 ;
	mem_monitor.next = 0 ;		
	mem_monitor.last = 0 ;

	return 1 ;
		
}

void MMTCreateMask ()
{

	//	author : QDL
	//	since : 20110705
	//	(C)TOK

	int looper = 0 ;
	
	mmt_mask = (unsigned int* ) malloc ( MMT_MASK_LENGTH * sizeof(unsigned int) ) ;

	for ( looper = 0 ; looper < MMT_MASK_LENGTH ; looper ++ )
	{
		mmt_mask [ looper ] = looper | 0xccff0000;
	}

}

void MMTAdd ( const char* file , int line , long long length , long long address ) {
	
	//	author : QDL
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;
		
	newmem	= (MEMORY_MONITOR* ) malloc ( sizeof(MEMORY_MONITOR) ) ;

	if ( 0 == mmt_mask ) MMTCreateMask () ;
	
	memcpy ( (void*)(address+length) , mmt_mask , MMT_MASK_LENGTH*sizeof(unsigned int) ) ;

	if ( file ) {
		newmem->file = (char* ) malloc ( strlen ( file ) + 1 ) ;
		strcpy ( newmem->file , file ) ;
	}

	newmem->line = line ; 
	newmem->length = length ;
	newmem->address = address ;
	newmem->next = 0 ;
	newmem->last = 0 ;
 
	if ( 0 == mem_monitor.next ) {
		mem_monitor.next = newmem ;
		mem_monitor.last = newmem ;
	} else {
		mem_monitor.last->next = newmem ;
		mem_monitor.last = newmem ;
	}

	mem_monitor.totall ++ ;

}

void MMTCheckOverflow () {

	//	author : Jelo
	//	since : 2011.4.10
	//	(C)TOK
	
	//	notes : 识别溢出内存块

	static int overtimes = 0 ;

	MEMORY_MONITOR* looper = 0 ;
	unsigned int* mask = 0 ;

	mask = (unsigned int* ) malloc ( MMT_MASK_LENGTH * sizeof(unsigned int) ) ;

	for ( looper = mem_monitor.next ; looper ; looper=looper->next ) 
	{

		int counter = 0 ;
		
		memcpy ( mask , (void*)(looper->address+looper->length) , MMT_MASK_LENGTH*sizeof(unsigned int) ) ;

		for ( counter = 0 ; counter < MMT_MASK_LENGTH ; counter ++ )
		{

			if ( mask [ counter ] != mmt_mask [ counter ] )
			{
				overtimes ++ ;
				iLog ( "!!!!! M : %x , In : '%s' , At line : '%d' - overflowed" , (void* )looper->address , looper->file , looper->line ) ;
				free ( mask ) ;
				return ;
			}
		}		

	}

	free ( mask ) ;
	
}

void MMTFree ( long long address ) {
	
	//	author : QDL
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor.next ;
	MEMORY_MONITOR* pre = &mem_monitor ;
	
	if ( !address ) return ;

	for ( ; walker ; ) {

		if ( address == walker->address ) {
			if ( walker == mem_monitor.next ) {
				mem_monitor.next = walker->next ;
			} 
			if ( mem_monitor.last == walker ) {
				mem_monitor.last = pre ;	
			}
			pre->next = walker->next ;

			if ( walker->file ) free ( walker->file ) ;
			free ( walker ) ;

			mem_monitor.totall -- ;

			if ( 0 == mem_monitor.totall ) {
				mem_monitor.next = 0 ;
				mem_monitor.last = 0 ;
			}

			return ;
			
		}

		pre = walker ;
		walker = walker->next ;
			
	}
	
}


void MMTDestroy () {
	
	//	author : QDL
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	for ( walker = mem_monitor.next ; walker ; ) {
		mem_monitor.next = walker->next ;
		if ( walker->file ) free ( walker->file ) ;
		free ( walker ) ;
		walker = mem_monitor.next ;
	}
	
}

void MMTTest () 
{	
}

# endif

/*
 brief : 
*/
void* H3MemoryMalloc ( long long length , const char* file , int line )
{
	void* buffer = 0 ;

	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		buffer = (void*) malloc ( length + MMT_MASK_LENGTH*sizeof(unsigned int) ) ;
		memset ( buffer , 0 , length + MMT_MASK_LENGTH*sizeof(unsigned int) ) ;
		MMTAdd ( file , line , length , (long long)buffer ) ;
		H3MemoryCheckOverflowed () ;
	# else
		buffer = (void*) malloc ( length ) ;
		if ( buffer ) memset ( buffer , 0 , length ) ;
	# endif
	
	return buffer ;

}

/*
 brief : 
*/
void* H3MemoryRealloc ( void* buffer , long long length , const char* file , int line ) 
{
	void* address = 0 ;
	
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		MMTFree ( (long long)buffer ) ;
		address = realloc ( buffer , length+MMT_MASK_LENGTH*sizeof(unsigned int) ) ;
		memset ( address , 0 , length+MMT_MASK_LENGTH*sizeof(unsigned int) ) ;
		MMTAdd ( file , line , length , (long long)address ) ;
		H3MemoryCheckOverflowed () ;
	# else
		address = realloc ( buffer , length ) ;
		if ( address ) memset ( address , 0 , length ) ;	
	# endif
	
 	return address ;
}
/*
 brief : 
*/
void* H3MemoryRemalloc ( void* buffer , long long bufferlen , long long length , const char* file , int line )  
{
	unsigned char* memory = 0 ;

	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		MMTFree ( (long long)buffer ) ;
		memory = (unsigned char* ) malloc ( length + MMT_MASK_LENGTH*sizeof(unsigned int) ) ; 
		memset ( memory , 0 , length + MMT_MASK_LENGTH*sizeof(unsigned int) ) ;
		MMTAdd ( file , line , length , (long long)memory ) ;
		H3MemoryCheckOverflowed () ;
	# else
		memory = (unsigned char* ) malloc ( length ) ; 
		if ( memory ) memset ( memory , 0 , length ) ;
	# endif

	if ( memory ) memcpy ( memory , buffer , bufferlen ) ;
	
 	free ( buffer ) ;

	return memory ;
}

/*
 brief : 
*/
void H3MemoryFree ( void* buffer ) 
{
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		MMTCheckOverflow () ; 
		MMTFree ( (long long)buffer ) ; 
	# endif

	free ( buffer ) ;
}

/*
 brief : 
*/
void H3MemoryFreeEx ( void** buffer ) 
{
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		MMTCheckOverflow () ;
		MMTFree ( (long long)*buffer ) ;
	# endif

 	free ( *buffer ) ;

	*buffer = 0 ;
}

/*
 brief : 
*/
void H3MemoryCheckOverflowed () 
{
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		MMTCheckOverflow () ;
	# endif
}

/*
 brief : 
*/
void H3MemoryCheckLeaked () 
{
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		
		int totall = 0 ;
		int leakedtimes = 0 ;
		
		MEMORY_MONITOR* walker = mem_monitor.next ;
	
		for ( ;walker;walker=walker->next) {

			iLog ("# M : %x , S : %d , In '%s' , At line : '%d' - leaked",
				(void* )walker->address,\
				walker->length,\
				walker->file,\
				walker->line\
			) ;\

			totall = totall + walker->length ;

			leakedtimes ++ ;
			
		}

		iLog ( "Leaked Totall : %1.3f kb" , (float )totall/1024.0 ) ;
		iLog ( "Leaked Times : %d" , leakedtimes ) ;

	# endif
}

void H3MemoryMmcClear () 
{
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		
		MMTDestroy () ;

	# endif
}

/*
 brief : 
*/
void H3MemoryCheckTotallMemoryUsed () 
{
	# ifdef H3MEMORY_ENABLE_MEMORY_CHECK
		
		int totall = 0 ;
		
		MEMORY_MONITOR* walker = mem_monitor.next ;
	
		for ( ;walker;walker=walker->next) {

			totall = totall + walker->length ;
			
		}

		iLog ( "Totall Memory Used : %1.3f kb" , (float )totall/1024.0 ) ;

	# endif
}

/* 
 brief : 
*/
H3Memory::H3Memory () 
{
}

/*
 brief : 
*/
H3Memory::~H3Memory () 
{
}

/*
 brief : 
*/
void* H3Memory::operator new ( size_t size , const char* file , int line )
{
	void* handle = H3MemoryMalloc ( size , file , line ) ;
	assert(handle) ;
	return handle ;
}

/*
 brief : overload delete for compiler if object constructing failed
*/
void H3Memory::operator delete ( void* handle , size_t size )
{
	::delete handle ;  
}
  
/*
 brief : overload delete for compiler if object constructing failed
*/
void H3Memory::operator delete ( void* handle , const char* file , int line )
{
	::delete handle ;  
}  
