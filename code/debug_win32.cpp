
#ifdef	STD_INCLUDE_DECL

//----------
// debug declarations
//----------

internal void CONSOLE_STRING( const char * string );

struct RENDERER;
struct DEBUG_SYSTEM {
	RENDERER * renderer;
    FONT     * defaultFont;
};

#if DEBUG_BUILD
#define DISPLAY_VALUE( type, name ) {\
char __debug_string[ 2048 ] = {};\
serialize_##type( __debug_string, #name, name );\
DISPLAY_STRING( __debug_string );\
}
#define CONSOLE_VALUE( type, name ) {\
char __debug_string[ 2048 ] = {};\
serialize_##type( __debug_string, #name, name );\
strcat( __debug_string, "\n" );\
OutputDebugString( __debug_string );\
}
#define CONSOLE_VALUE_HEX( type, name ) {\
char __debug_string[ 2048 ] = {};\
serialize_hex_##type( __debug_string, #name, name );\
strcat( __debug_string, "\n" );\
OutputDebugString( __debug_string );\
}

#else
#define DISPLAY_STRING( str )
#define DISPLAY_VALUE( type, name )
#define CONSOLE_STRING( str )

internal void PROFILE_string( char * string ) {}
internal void PROFILE_tooltip( char * string, vec2 position ) {}

#endif

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

//----------
// debug functions
//----------

#if DEBUG_BUILD
#define DEBUG_SYSTEM__MAX_VERTEX_COUNT		( 2097152 )
#define DEBUG_SYSTEM__MAX_MEMORY_SIZE    _MB( 2 )
#define DEBUG_SYSTEM__SIZE_IN_BYTES  		( sizeof( RENDER_SYSTEM ) + DEBUG_SYSTEM__MAX_MEMORY_SIZE )

DEBUG_SYSTEM globalVar_debugSystem = {};
vec2         globalVar_atPos = {};
flo32        globalVar_advanceHeight = 0.0f;

internal void
CONSOLE_STRING( const char * string ) {
	OutputDebugString( string );
	OutputDebugString( "\n" );
}

internal void
DEBUG_addTask( ASSET_ID textureID, rect bound, vec4 color ) {
    RENDERER * renderer = globalVar_debugSystem.renderer;
    
    int32 vertex_index    = renderer->vertex_count[ 0 ];
    int32 vertex_count    = 4;
	DEBUG_VERTEX * vertex = ( ( DEBUG_VERTEX * )renderer->vertex_ptr[ 0 ] ) + vertex_index;
    
    vec2 pos[ 4 ] = {
        Vec2( bound.left,  bound.bottom ),
        Vec2( bound.right, bound.bottom ),
        Vec2( bound.left,  bound.top ),
        Vec2( bound.right, bound.top ),
    };
    
    vec2 tex[ 4 ] = {
        Vec2( 0.0f, 0.0f ),
        Vec2( 1.0f, 0.0f ),
        Vec2( 0.0f, 1.0f ),
        Vec2( 1.0f, 1.0f ),
    };
    
    *vertex++ = DebugVertex( pos[ 0 ], tex[ 0 ] );
    *vertex++ = DebugVertex( pos[ 1 ], tex[ 1 ] );
    *vertex++ = DebugVertex( pos[ 2 ], tex[ 2 ] );
    *vertex++ = DebugVertex( pos[ 3 ], tex[ 3 ] );
    
    addVertex( renderer, 0, vertex_count );
    addObject( renderer, renderPassID_debug, RenderObject( meshDrawType_triangleStrip, vertex_index, vertex_count, textureID, color ) );
}

internal void
DEBUG_drawString( char * string, vec2 position, vec4 color ) {
	RENDERER * renderer = globalVar_debugSystem.renderer;
	FONT     * font     = globalVar_debugSystem.defaultFont;
    
    int32 vertex_index    = renderer->vertex_count[ 0 ];
    int32 vertex_count    = 0;
	DEBUG_VERTEX * vertex = ( ( DEBUG_VERTEX * )renderer->vertex_ptr[ 0 ] ) + vertex_index;
	
	int32 char_count = ( int32 )strlen( string );
	
	vec2 atPos = position;
	for( int32 counter = 0; counter < char_count; counter++ ) {
		char c = string[ counter ];
		if( c != ' ' ) {
			int32 char_index = c - FONT__START_CHAR;
			FONT_CHAR _char = font->alphabet[ char_index ];
			
			flo32 left   = atPos.x + _char.offset.x;
			flo32 right  = left    + _char.dim.x;
			flo32 top    = atPos.y + _char.offset.y;
			flo32 bottom = top     - _char.dim.y;
			rect bound = Rect( left, bottom, right, top );
            
            vec2 pos[ 4 ] = {
                Vec2( bound.left,  bound.bottom ),
                Vec2( bound.right, bound.bottom ),
                Vec2( bound.left,  bound.top ),
                Vec2( bound.right, bound.top ),
            };
            
            vec2 tex[ 4 ] = {
                Vec2( _char.texCoord_min.x, _char.texCoord_min.y ),
                Vec2( _char.texCoord_max.x, _char.texCoord_min.y ),
                Vec2( _char.texCoord_min.x, _char.texCoord_max.y ),
                Vec2( _char.texCoord_max.x, _char.texCoord_max.y ),
            };
            
            *vertex++ = DebugVertex( pos[ 0 ], tex[ 0 ] );
            *vertex++ = DebugVertex( pos[ 1 ], tex[ 1 ] );
            *vertex++ = DebugVertex( pos[ 2 ], tex[ 2 ] );
            *vertex++ = DebugVertex( pos[ 3 ], tex[ 3 ] );
            *vertex++ = DebugVertex( pos[ 2 ], tex[ 2 ] );
            *vertex++ = DebugVertex( pos[ 1 ], tex[ 1 ] );
            
            vertex_count += 6;
            
            atPos.x += _char.advanceWidth;
		} else {
            atPos.x += font->advanceWidth_space;
        }
	}
    
    addVertex( renderer, 0, vertex_count );
    addObject( renderer, renderPassID_debug, RenderObject( meshDrawType_triangles, vertex_index, vertex_count, assetID_font_default, color ) );
}

internal void
DEBUG_drawRect( rect bound, vec4 color ) {
	DEBUG_addTask( assetID_texture_whiteTexture, bound, color );
}

internal void
DISPLAY_STRING( char * string ) {
    DEBUG_drawString( string, globalVar_atPos, COLOR_BLACK );
    globalVar_atPos.y -= globalVar_advanceHeight;
}

internal void
DISPLAY_STRING( char * string, vec2 position ) {
    DEBUG_drawString( string, position, COLOR_BLACK );
}

internal void
PROFILE_string( char * string ) {
	DEBUG_drawString( string, globalVar_atPos, COLOR_WHITE );
	globalVar_atPos.y -= globalVar_advanceHeight;
}

internal void
PROFILE_tooltip( char * string, vec2 position ) {
	DEBUG_drawString( string, position, COLOR_WHITE );
}

internal void serialize_int8  ( char * buffer, const char * name, int8   value ) {
    sprintf( buffer, "%s: %hhd",    name, value ); }
internal void serialize_uint8 ( char * buffer, const char * name, uint8  value ) {
    sprintf( buffer, "%s: %hhu",    name, value ); }
internal void serialize_int16 ( char * buffer, const char * name, int16  value ) { sprintf( buffer, "%s: %hd",    name, value ); }
internal void serialize_int32 ( char * buffer, const char * name, int32  value ) { sprintf( buffer, "%s: %d",     name, value ); }
internal void serialize_uint16( char * buffer, const char * name, uint16 value ) { sprintf( buffer, "%s: %hu",    name, value ); }
internal void serialize_uint32( char * buffer, const char * name, uint32 value ) { sprintf( buffer, "%s: %u",     name, value ); }
internal void serialize_uint64( char * buffer, const char * name, uint64 value ) { sprintf( buffer, "%s: %llu",   name, value ); }
internal void serialize_boo32 ( char * buffer, const char * name, boo32  value ) { sprintf( buffer, "%s: %d",     name, value ); }
internal void serialize_flo32 ( char * buffer, const char * name, flo32  value ) { sprintf( buffer, "%s: %f",     name, value ); }
internal void serialize_vec2  ( char * buffer, const char * name, vec2   value ) { sprintf( buffer, "%s: %f, %f", name, value.x, value.y ); }
internal void serialize_vec3  ( char * buffer, const char * name, vec3   value ) { sprintf( buffer, "%s: %f, %f, %f", name, value.x, value.y, value.z ); }
internal void serialize_vec4  ( char * buffer, const char * name, vec4   value ) { sprintf( buffer, "%s: %f, %f, %f, %f", name, value.x, value.y, value.z, value.w ); }
internal void serialize_rect  ( char * buffer, const char * name, rect   value ) { sprintf( buffer, "%s: %f, %f, %f, %f", name, value.left, value.bottom, value.right, value.top ); }
internal void serialize_MEMORY ( char * buffer, const char * name, MEMORY memory ) { sprintf( buffer, "%s: base: %llx, size: %llu, used: %llu", name, ( uint64 )memory.base, memory.size, memory.used ); }

internal void serialize_hex_uint32( char * buffer, const char * name, uint32 value ) { sprintf( buffer, "%s: %X",     name, value ); }

#endif // DEBUG_BUILD
#endif	// STD_INCLUDE_FUNC