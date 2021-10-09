
#ifdef 	STD_INCLUDE_DECL

#define MAX_ASSET_ENTRY_COUNT  256
struct ASSET_ENTRY {
	uint32 offset;
	uint32 size;
};

struct ASSET_TEXTURE {
	int32 width;
	int32 height;
};

struct ASSET_MESH {
	MESH_DRAW_TYPE drawType;
	int32 vertex_count;
};

struct PLATFORM;
struct ASSET_SYSTEM {
	RENDER_TEXTURE	defaultTexture;
	FONT           defaultFont;
   AUDIO_DATA     defaultAudio;
	
	ASSET_ENTRY entry[ assetID_count ];
   uint8 * asset_data;
	
	#define ASSET_SYSTEM__MAX_TEXTURE_COUNT  64
	int32					texture_count;
	ASSET_ID 			textureID_list	[ ASSET_SYSTEM__MAX_TEXTURE_COUNT ];
	RENDER_TEXTURE		texture_list	[ ASSET_SYSTEM__MAX_TEXTURE_COUNT ];
	
	#define ASSET_SYSTEM__MAX_AUDIO_COUNT  32
	int32      audio_count;
	ASSET_ID   audioID_list	[ ASSET_SYSTEM__MAX_AUDIO_COUNT ];
	AUDIO_DATA audio_list	[ ASSET_SYSTEM__MAX_AUDIO_COUNT ];
	
	#define ASSET_SYSTEM__MAX_FONT_COUNT	12
	int32    font_count;
	ASSET_ID fontID_list[ ASSET_SYSTEM__MAX_FONT_COUNT ];
	FONT     font_list  [ ASSET_SYSTEM__MAX_FONT_COUNT ];
};

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

internal int32
findAsset( ASSET_SYSTEM * assetSystem, ASSET_ID assetID, ASSET_ID * list, int32 count ) {
	int32 result = -1;
	
	int32 index 	= 0;
	boo32 wasFound = false;
	while( ( !wasFound ) && ( index < count ) ) {
		ASSET_ID currID = list[ index ];
		if( assetID == currID ) {
			wasFound = true;
			result = index;
		}
		index++;
	}
	
	return result;
}

//----------
// audio asset functions
//----------

internal AUDIO_DATA
loadAudio( ASSET_SYSTEM * assetSystem, ASSET_ID audioID ) {
	AUDIO_DATA result = assetSystem->defaultAudio;
	if( assetSystem->audio_count < ASSET_SYSTEM__MAX_AUDIO_COUNT ) {
		ASSET_ENTRY entry = assetSystem->entry[ audioID ];
		if( entry.size > 0 ) {
         uint32 samples = *( ( uint32 * )( assetSystem->asset_data + entry.offset ) );
         uint8 * audio = assetSystem->asset_data + entry.offset + sizeof( uint32 );
         
         result.audio   = audio;
         result.samples = samples;
			
			assetSystem->audioID_list[ assetSystem->audio_count ] = audioID;
			assetSystem->audio_list	 [ assetSystem->audio_count ] = result;
			assetSystem->audio_count++;
		}
	} else {
		CONSOLE_STRING( "ERROR. AssetSystem audio list is full. Unable to load audio." );
	}
	return result;
}

internal AUDIO_DATA 
getAudio( ASSET_SYSTEM * assetSystem, ASSET_ID audioID ) {
	AUDIO_DATA result = assetSystem->defaultAudio;
	
	if( audioID != assetID_audio_default ) {
		int32 select_index = findAsset( assetSystem, audioID, assetSystem->audioID_list, assetSystem->audio_count );
		if( select_index > -1 ) {
			result = assetSystem->audio_list[ select_index ];
		} else {
			result = loadAudio( assetSystem, audioID );
		}
	}
	return result;
}

//----------
// texture asset functions
//----------

// internal void
// addTexture( ASSET_SYSTEM * assetSystem, ASSET_ID textureID, RENDER_TEXTURE texture ) {
	// // NOTE: Does NOT check for duplicates. Function assumes that texture was not already found in textureID_list or is being added at startup.
	// int32 count = assetSystem->texture_count;
	// Assert( count < ASSET_SYSTEM__MAX_TEXTURE_COUNT );
	// assetSystem->textureID_list[ count ] = textureID;
	// assetSystem->texture_list	[ count ] = texture;
	// assetSystem->texture_count++;
// }

// internal uint32
// GL_loadTexture_returnBufferID( int32 width, int32 height, void * texture ) {
	// uint32 bufferID = 0;
	// glGenTextures( 1, &bufferID );
	// glBindTexture( GL_TEXTURE_2D, bufferID );
	// glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture ); // android
	// // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, texture ); // win32
	// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	// glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	// return bufferID;
// }

// internal RENDER_TEXTURE
// GL_loadTexture( int32 width, int32 height, void * texture ) {
	// uint32 bufferID = GL_loadTexture_returnBufferID( width, height, texture );
	// RENDER_TEXTURE result = RenderTexture( bufferID, width, height );
	// return result;
// }

// internal RENDER_TEXTURE
// loadTexture( ASSET_SYSTEM * assetSystem, ASSET_ID textureID ) {
	// RENDER_TEXTURE result = assetSystem->defaultTexture;
	// if( assetSystem->texture_count < ASSET_SYSTEM__MAX_TEXTURE_COUNT ) {
		// ASSET_ENTRY entry = assetSystem->entry[ textureID ];
		// if( entry.size > 0 ) {
         // ASSET_TEXTURE * texture = ( ASSET_TEXTURE * )( assetSystem->asset_data + entry.offset );
			// result = GL_loadTexture( texture->width, texture->height, assetSystem->asset_data + entry.offset + sizeof( ASSET_TEXTURE ) );
			
			// assetSystem->textureID_list[ assetSystem->texture_count ] = textureID;
			// assetSystem->texture_list	[ assetSystem->texture_count ] = result;
			// assetSystem->texture_count++;
		// }
	// } else {
		// CONSOLE_STRING( "ERROR. AssetSystem texture list is full. Unable to load texture." );
	// }
	// return result;
// }

// internal RENDER_TEXTURE 
// getTexture( ASSET_SYSTEM * assetSystem, ASSET_ID textureID ) {
	// RENDER_TEXTURE result = assetSystem->defaultTexture;
	
	// if( textureID != assetID_texture_default ) {
		// int32 select_index = findAsset( assetSystem, textureID, assetSystem->textureID_list, assetSystem->texture_count );
		// if( select_index > -1 ) {
			// result = assetSystem->texture_list[ select_index ];
		// } else {
			// result = loadTexture( assetSystem, textureID );
		// }
	// }
	// return result;
// }

// internal FONT *
// loadMonoFont( ASSET_SYSTEM * assetSystem, ASSET_ID fontID ) {
	// FONT * result = &assetSystem->defaultFont;
   // // @Incomplete: load mono font
	// // if( assetSystem->font_count < ASSET_SYSTEM__MAX_FONT_COUNT ) {
		// // ASSET_ENTRY entry = assetSystem->entry[ fontID ];
		// // if( entry.size > 0 ) {
			// // PLATFORM * platform = assetSystem->platform;
			// // MEMORY * tempMemory = &platform->tempMemory;
			
			// // assetSystem->fontID_list[ assetSystem->font_count ] = fontID;
			// // result = assetSystem->font_list + assetSystem->font_count;
			// // assetSystem->font_count++;
			
			// // platform->readAsset_chunk( assetSystem->assetFile_handle, ( assetID_count * sizeof( ASSET_ENTRY ) ) + entry.offset, sizeof( FONT ), &result->font );
			
			// // int32 size_left = entry.size - sizeof( FONT );
			// // uint8 * memory = ( uint8 * )_pushSize_clear( tempMemory, size_left );
			// // platform->readAsset_chunk( assetSystem->assetFile_handle, ( assetID_count * sizeof( ASSET_ENTRY ) ) + entry.offset + sizeof( FONT ), size_left, memory );
			
			// // int32 width  = *( ( int32 * )memory );   memory += sizeof( int32 );
			// // int32 height = *( ( int32 * )memory );   memory += sizeof( int32 );
			// // uint8 * 	texture = memory;
			// // result->texture_bufferID = GL_loadTexture_returnBufferID( width, height, texture );
			
			// // _popSize( tempMemory, size_left );
		// // }
	// // } else {
		// // CONSOLE_STRING( "ERROR. AssetSystem font list is full. Unable to load font." );
	// // }
	// return result;
// }

internal boo32
isFontID( ASSET_ID assetID ) {
   boo32 result = ( ( assetID >= assetID_font_default ) && ( assetID < assetID_audio_default ) );
   return result;
}

internal FONT *
getFont( ASSET_SYSTEM * assetSystem, ASSET_ID fontID ) {
	FONT * result = &assetSystem->defaultFont;
   
   if( fontID != assetID_font_default ) {
      ASSET_ENTRY entry = assetSystem->entry[ fontID ];
      Assert( entry.size > 0 );
      
      result = ( FONT * )( assetSystem->asset_data + entry.offset );
   }
	return result;
}

#endif	// STD_INCLUDE_FUNC