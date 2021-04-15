
#ifndef HpUtil_H
#define HpUtil_H

class H3UtilString2Number
{
public :
	H3UtilString2Number ( const char* string ) { 
		value = 0 ;
		*this = 0;
		while( *string != '\0' ) {
			*this += *string - '0';
			string ++ ;
		}
	}
	~H3UtilString2Number () {}

    void operator *= ( int a){value = value * a ;}
    void operator += ( int a){value = value + a ;}
	void operator = ( int a){value = a ;}
	long long getValue () { return value ; } 
private :

	long long value ;
} ;

#endif