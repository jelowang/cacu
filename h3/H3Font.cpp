
/*

	+	H3Font.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Util.h"
#include "H3Font.h"

#define STBRP_ASSERT(x)    H3ASSERT(x)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#include "stb_rect_pack.h"

#define STBTT_malloc(x,u)  ((void)(u), MMC_MALLOC(x))
#define STBTT_free(x,u)    ((void)(u), MMC_FREE(x))
#define STBTT_assert(x)    H3ASSERT(x)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#include "stb_truetype.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace H3Font 
{

typedef struct {
	float x ;
	float y ;
}Point2;

template<typename T>
class FontVector
{
public:
    int                         Size;
    int                         Capacity;
    T*                          Data;

    typedef T                   value_type;
    typedef value_type*         iterator;
    typedef const value_type*   const_iterator;

    FontVector()                  { Size = Capacity = 0; Data = NULL; }
    ~FontVector()                 { if (Data) MMC_FREE(Data); }

    inline bool                 empty() const                   { return Size == 0; }
    inline int                  size() const                    { return Size; }
    inline int                  capacity() const                { return Capacity; }

    inline value_type&          operator[](int i)               { assert(i < Size); return Data[i]; }
    inline const value_type&    operator[](int i) const         { assert(i < Size); return Data[i]; }

    inline void                 clear()                         { if (Data) { Size = Capacity = 0; MMC_FREE(Data); Data = NULL; } }
    inline iterator             begin()                         { return Data; }
    inline const_iterator       begin() const                   { return Data; }
    inline iterator             end()                           { return Data + Size; }
    inline const_iterator       end() const                     { return Data + Size; }
    inline value_type&          front()                         { assert(Size > 0); return Data[0]; }
    inline const value_type&    front() const                   { assert(Size > 0); return Data[0]; }
    inline value_type&          back()                          { assert(Size > 0); return Data[Size-1]; }
    inline const value_type&    back() const                    { assert(Size > 0); return Data[Size-1]; }
    inline void                 swap(FontVector<T>& rhs)          { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; value_type* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }

    inline int                  _grow_capacity(int new_size)    { int new_capacity = Capacity ? (Capacity + Capacity/2) : 8; return new_capacity > new_size ? new_capacity : new_size; }

    inline void                 resize(int new_size)            { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
    inline void                 reserve(int new_capacity)
    {
        if (new_capacity <= Capacity) return;
        T* new_data = (value_type*)MMC_MALLOC((size_t)new_capacity * sizeof(value_type));
        memcpy(new_data, Data, (size_t)Size * sizeof(value_type));
        MMC_FREE(Data);
        Data = new_data;
        Capacity = new_capacity;
    }

    inline void                 push_back(const value_type& v)  { if (Size == Capacity) reserve(_grow_capacity(Size+1)); Data[Size++] = v; }
    inline void                 pop_back()                      { assert(Size > 0); Size--; }

    inline iterator             erase(const_iterator it)        { assert(it >= begin() && it < end()); const ptrdiff_t off = it - begin(); memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(value_type)); Size--; return Data + off; }
    inline iterator             insert(const_iterator it, const value_type& v)  { assert(it >= begin() && it <= end()); const ptrdiff_t off = it - begin(); if (Size == Capacity) reserve(Capacity ? Capacity * 2 : 4); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(value_type)); Data[off] = v; Size++; return Data + off; }
};

typedef struct 
{
    void*           FontData;                   //          // TTF data
    int             FontDataSize;               //          // TTF data size
    bool            FontDataOwnedByAtlas;       // true     // TTF data ownership taken by the container H3FontAtlas (will delete memory itself). Set to true
    int             FontNo;                     // 0        // Index of font within TTF file
    float           SizePixels;                 //          // Size in pixels for rasterizer
    int             OversampleH, OversampleV;   // 3, 1     // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
    bool            PixelSnapH;                 // false    // Align every character to pixel boundary (if enabled, set OversampleH/V to 1)
    Point2          GlyphExtraSpacing;          // 0, 0     // Extra spacing (in pixels) between glyphs
    const unsigned short*  GlyphRanges;                //          // List of Unicode range (2 value per range, values are inclusive, zero-terminated list)
    bool            MergeMode;                  // false    // Merge into previous H3font, so you can combine multiple inputs font into one H3font (e.g. ASCII font + icons + Japanese glyphs).
    bool            MergeGlyphCenterV;          // false    // When merging (multiple ImFontInput for one H3font), vertically center new glyphs instead of aligning their baseline

    // [Internal]
    void*			DstFont;

}H3FontConfig;

struct FontGlyph 
{
    unsigned short          Codepoint;
    float                   XAdvance;
    float                   X0, Y0, X1, Y1;
    float                   U0, V0, U1, V1;     // Texture coordinates
};

typedef struct
{
	float                   U0, V0, U1, V1;
}glyphCoord ;

typedef struct 
{
    // Members: Settings
    float                       FontSize;           // <user set>      // Height of characters, set during loading (don't change after loading)
    float                       Scale;              // = 1.0f          // Base font scale, multiplied by the per-window font scale which you can adjust with SetFontScale()
    Point2                      DisplayOffset;      // = (0.0f,1.0f)   // Offset font rendering by xx pixels
    unsigned short              FallbackChar;       // = '?'           // Replacement glyph if one isn't found. Only set via SetFallbackChar()
    H3FontConfig*               ConfigData;         //                 // Pointer within H3FontAtlas->ConfigData
    int                         ConfigDataCount;    //

    float                       Ascent, Descent;    // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
    void*						ContainerAtlas;     // What we has been loaded into
    FontVector<FontGlyph>       Glyphs;
    const FontGlyph*            FallbackGlyph;      // == FindGlyph(FontFallbackChar)
    float                       FallbackXAdvance;   //
    //FontVector<float>         IndexXAdvance;      // Sparse. Glyphs->XAdvance directly indexable (more cache-friendly that reading from Glyphs, for CalcTextSize functions which are often bottleneck in large UI)
    FontVector<int>             IndexLookup;        // Sparse. Index glyphs by Unicode code-point.

}H3font;

typedef struct
{
    //void*                       TexID;              
    unsigned char*              TexPixelsAlpha8;    
    unsigned int*               TexPixelsRGBA32;    
    int                         TexWidth;           
    int                         TexHeight;          
    int                         TexDesiredWidth; 
    Point2                      TexUvWhitePixel;    
    FontVector<H3font*>			Fonts;
	SCClList                    fontList ;

    // Private
    FontVector<H3FontConfig>      ConfigData;         
}H3FontAtlas;



int gFontTextureWidth = 0 ;
int gFontTextureHeight = 0 ;
H3Texture* gFontTexture = NULL;
void* gFontData = NULL ;

FontVector<FontGlyph> gGlyphListRef ;

H3font* dst_font = NULL ;
H3font* extra_font = NULL ;
H3FontAtlas fontAtlas ;

//just for test  -- Begin
float gH3FontTestU0 = 0.0f ;
float gH3FontTestU1 = 0.0f ;
float gH3FontTestV0 = 0.0f ;
float gH3FontTestV1 = 0.0f ;
//......         -- End

SCClList gReleaseList ;

static const FontGlyph* FindGlyph( unsigned short c)
{
	H3ASSERT(dst_font != NULL) ;
	H3ASSERT(dst_font->IndexLookup.Size > 0) ;

	if (c < dst_font->IndexLookup.Size) 
    {
        const int i = dst_font->IndexLookup[c];
        if (i != -1)
		{
			return &gGlyphListRef[i] ;
		} 
    }
    return dst_font->FallbackGlyph;
}
#if 0
static const void buildLookupTable()
{
    int max_codepoint = 0 ;
	for( int i = 0 ; i != dst_font->Glyphs.Size ; i++ )
	{
		max_codepoint = max_codepoint >= (int)dst_font->Glyphs[i].Codepoint ? max_codepoint : (int)dst_font->Glyphs[i].Codepoint ;
	}

    dst_font->IndexLookup.resize(max_codepoint + 1);
    for( int i = 0 ; i < max_codepoint + 1 ; i++ )
    {
        dst_font->IndexLookup[i] = -1;
    }
    for( int i = 0 ; i < dst_font->Glyphs.Size ; i++ )
    {
        int codepoint = (int)dst_font->Glyphs[i].Codepoint ;
        dst_font->IndexLookup[codepoint] = i ;
    }

    // Create a glyph to handle TAB
    // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
	if( FindGlyph((unsigned short)' ') )
    {
        if( dst_font->Glyphs.back().Codepoint != '\t' )   // So we can call this function multiple times
            dst_font->Glyphs.resize(dst_font->Glyphs.Size + 1) ;
        FontGlyph tab_glyph = dst_font->Glyphs.back() ;
		tab_glyph = *FindGlyph((unsigned short)' ') ;
        tab_glyph.Codepoint = '\t';
        dst_font->IndexLookup[(int)tab_glyph.Codepoint] = (int)(dst_font->Glyphs.Size-1) ;
    }

    dst_font->FallbackGlyph = NULL ;
    dst_font->FallbackGlyph = FindGlyph(dst_font->FallbackChar) ;
}
#endif

static const void buildLookupTable()
{
    int max_codepoint = 0 ;
	for( int i = 0 ; i != gGlyphListRef.Size ; i++ )
	{
		max_codepoint = max_codepoint >= (int)gGlyphListRef[i].Codepoint ? max_codepoint : (int)gGlyphListRef[i].Codepoint ;
	}

    dst_font->IndexLookup.resize(max_codepoint + 1);
    for( int i = 0 ; i < max_codepoint + 1 ; i++ )
    {
        dst_font->IndexLookup[i] = -1;
    }
    for( int i = 0 ; i < gGlyphListRef.Size ; i++ )
    {
		int codepoint = (int)gGlyphListRef[i].Codepoint ;
        dst_font->IndexLookup[codepoint] = i ;
    }

    // Create a glyph to handle TAB
    // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
	if( FindGlyph((unsigned short)' ') )
    {
        if( gGlyphListRef.back().Codepoint != '\t' )   // So we can call this function multiple times
            gGlyphListRef.resize(gGlyphListRef.Size + 1) ;
        FontGlyph tab_glyph = gGlyphListRef.back() ;
		tab_glyph = *FindGlyph((unsigned short)' ') ;
        tab_glyph.Codepoint = '\t';
        dst_font->IndexLookup[(int)tab_glyph.Codepoint] = (int)(gGlyphListRef.Size-1) ;
    }

    dst_font->FallbackGlyph = NULL ;
    dst_font->FallbackGlyph = FindGlyph(dst_font->FallbackChar) ;

}

void* init ( iS8* ttf ) {

	SCClListInit( &gReleaseList ) ;	

	H3FontConfig font_cfg ;
	void* glyphRef = 0 ;

    void* f ;
	if ((f = SCHalFileOpen(ttf, "rb")) == NULL) {
		iLog ( "WR:%s is not exists" , ttf ) ;
        return NULL;
	}

    long file_size_signed;

	SCHalFileSeek(f, 0, SEEK_SET) ;

#if 1

    int file_size = SCHalFileLength ( f );
	void* file_data = ( void* ) MMC_MALLOC( file_size ) ;
	SCClListInsert( &gReleaseList , file_data ) ;

    if (file_data == NULL)
    {
		iLog ( "WR:%s is not enough memory" ) ;
        SCHalFileClose(f);
        return NULL;
    }
   
	file_size = file_size - 1 ;

	SCHalFileRead ( f , file_data , file_size ) ;

    SCHalFileClose(f);

	iS32 out_file_size = file_size ;

    static const unsigned short ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement                  32    - 255
        0x3000, 0x30FF, // Punctuations, Hiragana, Katakana                12288 - 12543
        0x31F0, 0x31FF, // Katakana Phonetic Extensions                    12784 - 12799
        0xFF00, 0xFFEF, // Half-width characters                           65280 - 65519
        0x4e00, 0x9FAF, // CJK Ideograms                                   19968 - 40879
        0,
    };

	//load Chinese .ttf  -- Begin
    H3FontConfig extraFontCfg ;
    extraFontCfg.FontData =  file_data ;
    extraFontCfg.FontDataSize = file_size ;
    extraFontCfg.SizePixels = 20.0f ;
	extraFontCfg.OversampleH = 3 ;
	extraFontCfg.OversampleV = 1 ;
    extraFontCfg.GlyphRanges = ranges ;
	//此处强行等于    -- Begin
	extraFontCfg.FontNo = 0 ;
	extraFontCfg.PixelSnapH = false ;
	extraFontCfg.GlyphExtraSpacing.x = 0 ;
	extraFontCfg.GlyphExtraSpacing.y = 0 ;
	extraFontCfg.MergeMode = false ;
	extraFontCfg.MergeGlyphCenterV = false ;
	//                -- End

	extra_font = ( H3font* ) MMC_MALLOC( sizeof( H3font ) ) ;
	SCClListInsert( &gReleaseList , extra_font ) ;
	assert( NULL != extra_font ) ;
	SCClListInit( &fontAtlas.fontList ) ;	
	SCClListInsert( &fontAtlas.fontList , extra_font ) ;

	extraFontCfg.DstFont = extra_font ;

    fontAtlas.ConfigData.push_back(extraFontCfg) ;
	
	/*
    ImFontConfig& new_font_cfg = ConfigData.back();
    new_font_cfg.DstFont = Fonts.back();
    if (!new_font_cfg.FontDataOwnedByAtlas)
    {
        new_font_cfg.FontData = MMC_MALLOC(new_font_cfg.FontDataSize);
        new_font_cfg.FontDataOwnedByAtlas = true;
        memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
    }*/
	//load Chinese .ttf  -- End

	//font_cfg.FontNo = 0 ;
	//font_cfg.GlyphExtraSpacing.x = 0.0f ;
	//font_cfg.GlyphExtraSpacing.y = 0.0f ;

    //fontatlas.TexID = NULL ;
    fontAtlas.TexWidth = 0 ;
	fontAtlas.TexHeight = 0 ;
	fontAtlas.TexDesiredWidth = 0 ;
	fontAtlas.TexUvWhitePixel.x = 0 ;
	fontAtlas.TexUvWhitePixel.y = 0 ;
    fontAtlas.TexPixelsAlpha8 = NULL ;

	struct H3FontTempBuildData
	{
		stbtt_fontinfo      FontInfo ;
		stbrp_rect*         Rects ;
		stbtt_pack_range*   Ranges ;
		int                 RangesCount ;
	} ;

	H3FontTempBuildData* tmp_array = (H3FontTempBuildData*)MMC_MALLOC((size_t)fontAtlas.ConfigData.Size * sizeof(H3FontTempBuildData)) ;
	SCClListInsert( &gReleaseList , tmp_array ) ;

    int total_glyph_count = 0 ;
    int total_glyph_range_count = 0 ;
    for (int input_i = 0; input_i < fontAtlas.ConfigData.Size; input_i++)
    {
        H3FontConfig cfg = fontAtlas.ConfigData[input_i] ;
		H3FontTempBuildData& tmp = tmp_array[input_i] ;

		const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData , cfg.FontNo) ;
		assert(font_offset >= 0) ;

		if (!stbtt_InitFont(&tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset)) {
			iLog ( "FE:stbtt_InitFont failure" ) ;
			release() ;
			return NULL;
		}

		for (const unsigned short* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2)
		{
		    total_glyph_count += (in_range[1] - in_range[0]) + 1 ;
		    total_glyph_range_count ++ ;
		}
	}

    // Start packing
    fontAtlas.TexWidth = (fontAtlas.TexDesiredWidth > 0) ? fontAtlas.TexDesiredWidth : (total_glyph_count > 2000) ? 2048 : (total_glyph_count > 1000) ? 1024 : 512 ; 
	fontAtlas.TexHeight = 0 ;
	const int max_tex_height = 1024*32 ;
    
	stbtt_pack_context  spc ;
    stbtt_PackBegin(&spc, NULL, fontAtlas.TexWidth, max_tex_height, 0, 1, NULL) ;

    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    
	FontVector<stbrp_rect> extra_rects ;
    const int TEX_DATA_W = 90 ;
    const int TEX_DATA_H = 27 ;
    const char texture_data[TEX_DATA_W*TEX_DATA_H+1] =
    {
        "..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX"
        "..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X"
        "---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X"
        "X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X"
        "XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X"
        "X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X"
        "X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX"
        "X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      "
        "X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       "
        "X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        "
        "X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         "
        "X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          "
        "X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           "
        "X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            "
        "X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           "
        "X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          "
        "X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          "
        "X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       ------------------------------------"
        "X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           "
        "XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           "
        "      X..X          -  X...X  -         X...X         -  X..X           X..X  -           "
        "       XX           -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           "
        "------------        -    X    -           X           -X.....................X-           "
        "                    ----------------------------------- X...XXXXXXXXXXXXX...X -           "
        "                                                      -  X..X           X..X  -           "
        "                                                      -   X.X           X.X   -           "
        "                                                      -    XX           XX    -           "
    };

    //Request rectangles
    stbrp_rect r ;
    memset(&r, 0, sizeof(r)) ;
	r.w = (TEX_DATA_W*2)+1 ;
	r.h = TEX_DATA_H+1 ;
	extra_rects.push_back(r) ;

    stbtt_PackSetOversampling(&spc, 1, 1) ;
    stbrp_pack_rects((stbrp_context*)spc.pack_info, &extra_rects[0], extra_rects.Size ) ;

    for (int i = 0; i < extra_rects.Size; i++)
        if (extra_rects[i].was_packed)
            fontAtlas.TexHeight = fontAtlas.TexHeight>= r.y + r.h ? fontAtlas.TexHeight: r.y + r.h ;
    
	//Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
    int buf_packedchars_n = 0, buf_rects_n = 0, buf_ranges_n = 0 ;
    stbtt_packedchar* buf_packedchars = (stbtt_packedchar*)MMC_MALLOC(total_glyph_count * sizeof(stbtt_packedchar)) ;
	SCClListInsert( &gReleaseList , buf_packedchars ) ;
    stbrp_rect* buf_rects = (stbrp_rect*)MMC_MALLOC(total_glyph_count * sizeof(stbrp_rect)) ;
	SCClListInsert( &gReleaseList , buf_rects ) ;
    stbtt_pack_range* buf_ranges = (stbtt_pack_range*)MMC_MALLOC(total_glyph_range_count * sizeof(stbtt_pack_range)) ;
	SCClListInsert( &gReleaseList , buf_ranges ) ;
    memset(buf_packedchars, 0, total_glyph_count * sizeof(stbtt_packedchar)) ;
    memset(buf_rects, 0, total_glyph_count * sizeof(stbrp_rect)) ;              // Unnecessary but let's clear this for the sake of sanity.
    memset(buf_ranges, 0, total_glyph_range_count * sizeof(stbtt_pack_range)) ;

    //First font pass: pack all glyphs (no rendering at this point, we are working with rectangles in an infinitely tall texture at this point)
    for (int input_i = 0; input_i < fontAtlas.ConfigData.Size; input_i++)
    {
        H3FontConfig cfg = fontAtlas.ConfigData[input_i] ;
        H3FontTempBuildData& tmp = tmp_array[input_i] ;
        //Setup ranges
        int glyph_count = 0 ;
        int glyph_ranges_count = 0 ;
        for (const unsigned short* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2)
        {
            glyph_count += (in_range[1] - in_range[0]) + 1 ;
            glyph_ranges_count ++ ;
        }

        tmp.Ranges = buf_ranges + buf_ranges_n ;
        tmp.RangesCount = glyph_ranges_count ;
        buf_ranges_n += glyph_ranges_count;
        for (int i = 0; i < glyph_ranges_count; i++)
        {
            const unsigned short* in_range = &cfg.GlyphRanges[i * 2];
            stbtt_pack_range& range = tmp.Ranges[i];
            range.font_size = cfg.SizePixels;
            range.first_unicode_codepoint_in_range = in_range[0];
            range.num_chars = (in_range[1] - in_range[0]) + 1;
            range.chardata_for_range = buf_packedchars + buf_packedchars_n ;
            buf_packedchars_n += range.num_chars;
        }

        //Pack
        tmp.Rects = buf_rects + buf_rects_n;
        buf_rects_n += glyph_count;
		stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV) ; 
		
		for( int i = 0 ; i<tmp.RangesCount ; i++ ) {
			tmp.Ranges[i].array_of_unicode_codepoints = NULL ;
		}

        int n = stbtt_PackFontRangesGatherRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
        stbrp_pack_rects((stbrp_context*)spc.pack_info, tmp.Rects, n);

        //Extend texture height
        for (int i = 0; i < n; i++)
            if (tmp.Rects[i].was_packed)
                fontAtlas.TexHeight = fontAtlas.TexHeight >= tmp.Rects[i].y + tmp.Rects[i].h ? fontAtlas.TexHeight : tmp.Rects[i].y + tmp.Rects[i].h;
    }
    assert(buf_rects_n == total_glyph_count);
    assert(buf_packedchars_n == total_glyph_count);
    assert(buf_ranges_n == total_glyph_range_count);

    // Create texture
	fontAtlas.TexHeight--; 
	fontAtlas.TexHeight |= fontAtlas.TexHeight >> 1; 
	fontAtlas.TexHeight |= fontAtlas.TexHeight >> 2; 
	fontAtlas.TexHeight |= fontAtlas.TexHeight >> 4; 
	fontAtlas.TexHeight |= fontAtlas.TexHeight >> 8; 
	fontAtlas.TexHeight |= fontAtlas.TexHeight >> 16; 
	fontAtlas.TexHeight++; 

    fontAtlas.TexPixelsAlpha8 = (unsigned char*)MMC_MALLOC(fontAtlas.TexWidth * fontAtlas.TexHeight);
	if ( NULL == fontAtlas.TexPixelsAlpha8 ) {
		iLog ( "FE:not enough memory for TexPixelsAlpha8" ) ;
		release() ;
		return 0 ;
	}
	SCClListInsert( &gReleaseList , fontAtlas.TexPixelsAlpha8 ) ;
    memset(fontAtlas.TexPixelsAlpha8, 0, fontAtlas.TexWidth * fontAtlas.TexHeight);
    spc.pixels = fontAtlas.TexPixelsAlpha8;
    spc.height = fontAtlas.TexHeight;

    // Second pass: render characters
    for (int input_i = 0; input_i < fontAtlas.ConfigData.Size; input_i++)
    {
        H3FontConfig cfg = fontAtlas.ConfigData[input_i];
        H3FontTempBuildData& tmp = tmp_array[input_i];
        stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
        stbtt_PackFontRangesRenderIntoRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
        tmp.Rects = NULL;
    }

    // End packing
    stbtt_PackEnd(&spc);
    buf_rects = NULL;

    // Third pass: setup H3font and glyphs for runtime
    for (int input_i = 0; input_i < fontAtlas.ConfigData.Size; input_i++)
    {
        H3FontConfig cfg = fontAtlas.ConfigData[input_i];
        H3FontTempBuildData& tmp = tmp_array[input_i];

		dst_font = (H3font*)cfg.DstFont ;

        float font_scale = stbtt_ScaleForPixelHeight(&tmp.FontInfo, 13.0);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

        float ascent = unscaled_ascent * font_scale;
        float descent = unscaled_descent * font_scale;
		
		//MergeMode取值不明
		cfg.MergeMode = false ;
        if (!cfg.MergeMode)
        {
            dst_font->ContainerAtlas = &fontAtlas ;
            dst_font->ConfigData = &cfg;
            dst_font->ConfigDataCount = 1;
            dst_font->FontSize = cfg.SizePixels;
            dst_font->Ascent = ascent;
            dst_font->Descent = descent;
            dst_font->Glyphs.resize(0);
        }
        dst_font->ConfigDataCount++;
        float off_y = 0.0f;

        dst_font->FallbackGlyph = NULL; // Always clear fallback so FindGlyph can return NULL. It will be set again in BuildLookupTable()
        for(int i = 0; i < tmp.RangesCount; i++)
        {
            stbtt_pack_range& range = tmp.Ranges[i];
            for(int char_idx = 0; char_idx < range.num_chars; char_idx += 1)
            {
                const stbtt_packedchar& pc = range.chardata_for_range[char_idx];
                if(!pc.x0 && !pc.x1 && !pc.y0 && !pc.y1)
                    continue;

                const int codepoint = range.first_unicode_codepoint_in_range + char_idx;
                if(cfg.MergeMode && FindGlyph((unsigned short)codepoint))
                    continue;

                stbtt_aligned_quad q;
                float dummy_x = 0.0f, dummy_y = 0.0f;
                stbtt_GetPackedQuad(range.chardata_for_range, fontAtlas.TexWidth, fontAtlas.TexHeight, char_idx, &dummy_x, &dummy_y, &q, 0);

                dst_font->Glyphs.resize(dst_font->Glyphs.Size + 1);
				glyphRef = (void*)&dst_font->Glyphs ;
                FontGlyph glyph = dst_font->Glyphs.back();

                glyph.Codepoint = (unsigned short)codepoint;
                glyph.X0 = q.x0; glyph.Y0 = q.y0; glyph.X1 = q.x1; glyph.Y1 = q.y1;
				glyph.U0 = q.s0; glyph.V0 = q.t0; glyph.U1 = q.s1; glyph.V1 = q.t1;
				glyph.Y0 += (float)(int)(dst_font->Ascent + off_y + 0.5f);
				glyph.Y1 += (float)(int)(dst_font->Ascent + off_y + 0.5f);
                glyph.XAdvance = (pc.xadvance + cfg.GlyphExtraSpacing.x);  // Bake spacing into XAdvance
                if(cfg.PixelSnapH)
                    glyph.XAdvance = (float)(int)(glyph.XAdvance + 0.5f);
				
				//save glyph in list (20160407)     -- Begin
				gGlyphListRef.push_back( glyph ) ;
				//                                  -- End 
            }
        }
	buildLookupTable() ;

    }

    // Cleanup temporaries

    static unsigned int* TexPixelsRGBA32 = (unsigned int*)MMC_MALLOC((size_t)(fontAtlas.TexWidth * fontAtlas.TexHeight * 4));
	if ( NULL == TexPixelsRGBA32 ) {
		iLog ( "FE:not enough memory for TexPixelsRGBA32" ) ;
		release() ;
		return 0 ;
	}
	SCClListInsert( &gReleaseList , TexPixelsRGBA32 ) ;
    const unsigned char* src = spc.pixels;
    unsigned int* dst = TexPixelsRGBA32;

    for (int n = fontAtlas.TexWidth * fontAtlas.TexHeight; n > 0; n--)
        *dst++ = ((unsigned int)(*src++) << 24) | 0x00FFFFFF;

	gFontData = (void*) TexPixelsRGBA32 ;
	gFontTextureWidth = fontAtlas.TexWidth ;
	gFontTextureHeight = fontAtlas.TexHeight ;
#endif

	return glyphRef ;
}
void release ()
{
	for( SCClList* walker = ( SCClList* )gReleaseList.next ; walker ; walker = walker->next ) {
		MMC_FREE( walker->element ) ;
	}
	SCClListClear(&gReleaseList) ;
	SCClListClear(&fontAtlas.fontList) ;
}
void draw ( H3Vector3f position , iS8* string ) {}

TexCoord getTexCoord( unsigned short c)
{
	TexCoord coord ;

	FontGlyph glyph = *FindGlyph(c) ;
	
	coord.U0 = glyph.U0 ;
	coord.U1 = glyph.U1 ;
	coord.V0 = glyph.V0 ;
	coord.V1 = glyph.V1 ;

	return coord ;
}

void* Texture () 
{
	return gFontTexture ;
}

void* Data () 
{
	return gFontData ;
}

int TextureWidth ()
{
	return gFontTextureWidth ;
}

int TextureHeight () 
{
	return gFontTextureHeight ;
}

int getWidth ( const iS8* words ) 
{
	iU32 out_char ;
	iS8 texttest [256] = {0} ;
	iS8 in_text [ 32 ] = {""} ;
	iS8 in_text_end [ 32 ] = {""} ;

	iS32 walker = 0 ;
	iS32 widthWalker = 0 ;
	iS32 heightWalker = 20 ;

	strcpy ( texttest , words ) ;
	  
	while(texttest[walker] != '\0') {
		unsigned int c = (unsigned int)0xffffff00 | texttest[walker] ;
		unsigned int c2 = c ;
		{
			H3UtilConvertCharFromUtf8 ( &c , texttest+walker , in_text_end ) ;	
            if (c == 0)
                break ;
			if ( c2 > 0xffffff80 ) {
				walker += 2 ;
			}
			
        }

		H3Font::TexCoord fontTexCoord = H3Font::getTexCoord( c ) ;

		int X0 = (float )fontTexCoord.U0 * gFontTextureWidth ;
		int X1 = (float )fontTexCoord.U1 * gFontTextureWidth ;
		int Y0 = (float )fontTexCoord.V0 * gFontTextureHeight ;
		int Y1 = (float )fontTexCoord.V1 * gFontTextureHeight ;

		iS32 width = X1 - X0 ;

		widthWalker += width ;

		walker++ ;
	}

	return widthWalker ;

}

} ;  