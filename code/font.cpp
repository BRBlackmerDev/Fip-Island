
#ifdef	STD_INCLUDE_DECL

#define FONT__START_CHAR      ( '!' )
#define FONT__END_CHAR        ( '~' )
#define FONT__CHAR_COUNT      ( '~' - '!' + 1 )

struct FONT_CHAR {
   vec2 dim;
   vec2 offset;
   vec2 texCoord_min;
   vec2 texCoord_max;
   flo32 advanceWidth;
};

struct FONT {
   flo32 advanceWidth_space;
   flo32 ascent;
   flo32 descent;
   flo32 lineGap;
   flo32 advanceHeight;
   FONT_CHAR alphabet[ FONT__CHAR_COUNT ];
   int8  kernValue[ FONT__CHAR_COUNT * FONT__CHAR_COUNT ];
};

enum TEXT_ALIGNMENT {
	ALIGN_DEFAULT,
	
	ALIGN_BOTTOMLEFT,
	ALIGN_BOTTOMCENTER,
	ALIGN_BOTTOMRIGHT,
	
	ALIGN_CENTERLEFT,
	ALIGN_CENTER,
	ALIGN_CENTERRIGHT,
	
	ALIGN_TOPLEFT,
	ALIGN_TOPCENTER,
	ALIGN_TOPRIGHT,
	
	ALIGN_BASELEFT,
	ALIGN_BASECENTER,
	ALIGN_BASERIGHT,
};

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

internal flo32
getWidth( FONT * font, char * string ) {
	flo32 result = {};
	char * ptr = string;
	while( *ptr ) {
		int32 charIndex = *ptr - FONT__START_CHAR;
		if( *ptr != ' ' ) {
			result += font->alphabet[ charIndex ].advanceWidth;
			if( *( ptr + 1 ) && ( *( ptr + 1 ) != ' ' ) ) {
				int32 nextCharIndex = *( ptr + 1 ) - FONT__START_CHAR;
				int32 kernValue = font->kernValue[ charIndex * FONT__CHAR_COUNT + nextCharIndex ];
				result += kernValue;
			}
		} else {
			result += font->advanceWidth_space;
		}
		ptr++;
	}
	return result;
}

internal vec2
getOffset( FONT * font, char * string, TEXT_ALIGNMENT align, flo32 scale = 1.0f ) {
	vec2 result = {};
   
   flo32 string_width = getWidth( font, string );
   flo32 font_ascent  = font->ascent;
   flo32 font_descent = font->descent;
	
	flo32 width     = string_width * scale;
	flo32 halfWidth = width        * 0.5f;
	
	flo32 ascent  = font_ascent  * scale;
	flo32 descent = font_descent * scale;
	
	flo32 height     = ascent + descent;
	flo32 halfHeight = height * 0.5f;
	
	switch( align ) {
		case ALIGN_BOTTOMLEFT:   { result = Vec2(       0.0f, descent ); } break;
		case ALIGN_BOTTOMCENTER: { result = Vec2( -halfWidth, descent ); } break;
		case ALIGN_BOTTOMRIGHT:  { result = Vec2(     -width, descent ); } break;
		
		case ALIGN_CENTERLEFT:   { result = Vec2(       0.0f, -halfHeight + descent ); } break;
		case ALIGN_CENTER: 	    { result = Vec2( -halfWidth, -halfHeight + descent ); } break;
		case ALIGN_CENTERRIGHT:  { result = Vec2(     -width, -halfHeight + descent ); } break;
		
		case ALIGN_TOPLEFT:      { result = Vec2(       0.0f, -ascent ); } break;
		case ALIGN_TOPCENTER:    {	result = Vec2( -halfWidth, -ascent ); } break;
		case ALIGN_TOPRIGHT:     { result = Vec2(     -width, -ascent ); } break;
		
		case ALIGN_DEFAULT:
		case ALIGN_BASELEFT:		 { /* no change */ } break;
		case ALIGN_BASECENTER:	 { result = Vec2( -halfWidth, 0.0f ); } break;
		case ALIGN_BASERIGHT:	 { result = Vec2(     -width, 0.0f ); } break;
		
		default: { InvalidCodePath; } break;
	};
	
	return result;
}

#endif	// STD_INCLUDE_FUNC