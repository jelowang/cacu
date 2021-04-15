
/*

	+	H3Util.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine

	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Util.h"

#ifdef WIN32
#include "iconv.h"
string H3UtilGbk2Utf8 ( const string &str )
{
	int gbkLenght = str.length();
	const char* strgbk = str.c_str();

	//×ã¹»µÄ¿Õ¼ä
	int utf8Length = (gbkLenght+1)*4;
	char *strutf8 = new char[utf8Length];
	memset(strutf8, 0, utf8Length);

	iconv_t cd = iconv_open("utf-8", "gb2312");

	if (cd == 0) return "";

	const char *strin = strgbk;
	char *strout = strutf8;

	size_t ret = iconv(cd, &strin,(size_t *)&gbkLenght, &strout,(size_t *)&utf8Length);

	iconv_close(cd);

	string out = strutf8;

	delete [] strutf8;

	return out;
}
#endif


iS32 H3UtilGetUtf8Length ( iS8* str )
{

	static unsigned char utf8_look_for_table[] = 
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
	};

	#define UTFLEN(x)  utf8_look_for_table[(x)]

	int clen = strlen(str);
	int len = 0;
 
	for(char *ptr = str; 
	*ptr!=0&&len<clen; 
	len++, ptr+=UTFLEN((unsigned char)*ptr));
 
	return len;
}


// Convert UTF-8 to 32-bits character, process single character input.
// Based on stb_from_utf8() from github.com/nothings/stb/
// We handle UTF-8 decoding error by skipping forward.
iS32 H3UtilConvertCharFromUtf8 (iU32* out_char, const iS8* in_text, const iS8* in_text_end) 
{
    //unsigned int c = (unsigned int)-1;
	unsigned int c = 0 ;
    const unsigned char* str = (const unsigned char*)in_text;
    if (!(*str & 0x80))
    {
        c = (unsigned int)(*str++);
        *out_char = c;
        return 1;
    }
    if ((*str & 0xe0) == 0xc0)
    {
        *out_char = 0xFFFD; // will be invalid but not end of string
		//	jelo committed start
		//	this codes has a big problem , if the address of 'in_tex_end' is smaller than 'str'
		//	it would be crash when 'in_text_end' sum 'str'
        if (in_text_end && in_text_end - (const char*)str < 2) return 1;
		//	jelo committed end
        if (*str < 0xc2) return 2;
        c = (unsigned int)((*str++ & 0x1f) << 6);
        if ((*str & 0xc0) != 0x80) return 2;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 2;
    }
    if ((*str & 0xf0) == 0xe0)
    {
        *out_char = 0xFFFD; // will be invalid but not end of string
		//	jelo committed start
		//	this codes has a big problem , if the address of 'in_tex_end' is smaller than 'str'
		//	it would be crash when 'in_text_end' sum 'str'
        //if (in_text_end && in_text_end - (const char*)str < 3) return 1; 
		//	jelo committed end
        if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
        if (*str == 0xed && str[1] > 0x9f) return 3; // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x0f) << 12);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 3;
        c += (*str++ & 0x3f);
        *out_char = c;
        return 3;
    }
    if ((*str & 0xf8) == 0xf0)
    {
        *out_char = 0xFFFD; // will be invalid but not end of string
		//	jelo committed start
		//	this codes has a big problem , if the address of 'in_tex_end' is smaller than 'str'
		//	it would be crash when 'in_text_end' sum 'str'
        //if (in_text_end && in_text_end - (const char*)str < 4) return 1;
		//	jelo committed end
        if (*str > 0xf4) return 4;
        if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
        if (*str == 0xf4 && str[1] > 0x8f) return 4; // str[1] < 0x80 is checked below
        c = (unsigned int)((*str++ & 0x07) << 18);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (unsigned int)((*str++ & 0x3f) << 12);
        if ((*str & 0xc0) != 0x80) return 4; 
        c += (unsigned int)((*str++ & 0x3f) << 6);
        if ((*str & 0xc0) != 0x80) return 4;
        c += (*str++ & 0x3f);
        // utf-8 encodings of values used in surrogate pairs are invalid
        if ((c & 0xFFFFF800) == 0xD800) return 4;
        *out_char = c;
        return 4;
    }
    *out_char = 0;
    return 0;
}
