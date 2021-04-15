
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

# include "schal.h"

#ifndef WIN32
#define IOS
#endif

#ifdef IOS
static NSFileManager* s_fileManager = [NSFileManager defaultManager];

std::string getFullPathForDirectoryAndFilename( const char* path )
{
	
    std::string filename = SCHalGetFilePath(path) ;
    std::string strFilename = filename;
    std::string strDirectory ;
    size_t pos = filename.find_last_of("/");
    if (pos != std::string::npos)
    {
        strDirectory = filename.substr(0, pos+1);
        strFilename = filename.substr(pos+1);
    }
	
	
    if (strDirectory[0] != '/')
    {
        NSString* fullpath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:strFilename.c_str()]
                                                             ofType:nil
                                                        inDirectory:[NSString stringWithUTF8String:strDirectory.c_str()]];
        if (fullpath != nil) {
            return [fullpath UTF8String];
        }
    }
    else
    {
        std::string fullPath = strDirectory+strFilename;
        if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:fullPath.c_str()]]) {
            return fullPath;
        }
    }
    return "";
}

const char* SCHalGetWritablePath ()
{
    
    //test codes
    //NSString* filePath = [[NSBundle mainBundle] pathForResource:@"./H3/../shaders/ambient" ofType:@".vs"];
    //const char* path = [filePath cStringUsingEncoding:NSASCIIStringEncoding] ;
    //void* file = fopen( path , "rb") ;
    //printf("%s,%x\n",path,file) ;
    
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet.data();
}

#endif

void* SCHalFileOpen ( const char* path , const char* flag ) {

	//      author : QDL
	//      notes : fopen
	//      since : 20090809

#ifdef IOS
    
    std::string filepath = getFullPathForDirectoryAndFilename ( path ) ;
    void* filehandle = fopen ( filepath.data() , flag ) ;
    return filehandle ;
#else

    void* handle = fopen ( SCHalGetFilePath(path) , flag ) ;
    return handle ;
    
#endif
    
}

int SCHalFileSeek ( void* file , int offset , int direct ) {

	//      author : QDL
	//      notes : fseek
	//      since : 20090809
    
    int results = fseek ( (FILE*)file , offset , direct ) ;

    return results ;

}

int SCHalFileRead ( void* file , void* buffer , long int totall ) {

	//      author : QDL
	//      notes : fread
	//      since : 20090809

	int results = fread ( buffer , 1 , totall , (FILE*)file ) ;

    return results ;

}

int SCHalFileWrite ( void* file , void* buffer , long int totall ) {
        
	//      author : QDL
	//      notes : fwrite
	//      since : 20090809

	int results = fwrite ( buffer , 1 , totall , (FILE*)file ) ;

    return results ;
    
}

short int SCHalFileGetc ( void* file ) {
        
	//      author : QDL
	//      notes : fwrite
	//      since : 20090809

	short int results = fgetc ( (FILE*)file ) ;

    return results ;
    
}

int SCHalFileEnd ( void* file ) {
        
	//      author : QDL
	//      notes : fwrite
	//      since : 20090809

	int results = feof ( (FILE*)file ) ;

    return results ;
    
}
  

int SCHalFileLength ( void* file ) {

	 //      author : QDL
	 //      notes : fseek
	 //      since : 20090809
 
	 int length = 0;

	 SCHalFileSeek ( file , 0 , SEEK_SET ) ;

	 while( !SCHalFileEnd(file) ) {

		SCHalFileGetc ( file ) ;
		length ++ ;

	 }

	 SCHalFileSeek ( file , 0 , SEEK_SET ) ;
 
	 return length ;

}



int SCHalFileClose ( void* file ) {
        
	//      author : QDL
	//      notes : fclose
	//      since : 20090809
	
	int results = fclose ( (FILE*)file ) ;

    return results ;
    
}

const char* SCHalGetFilePath ( const char* name ) 
{

	//      author : QDL
	//      notes : SCHalGetFilePath
	//      since : 20151017

	static char buffer[512] = {0} ;
    
#ifdef IOS	
    sprintf ( buffer , "H3/%s" , name ) ;
#else
	sprintf ( buffer , "./Resources/H3/%s" , name ) ;
#endif
  
	return buffer ;

}

long SCHalGetTick() 
{

	//      author : QDL
	//      notes : fclose
	//      since : 20151024

	long tick = clock() ;
	return tick ;
}    