
#ifdef	STD_INCLUDE_DECL

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

// internal void
// addTask( RENDER_SYSTEM * renderSystem, MESH_DRAW_TYPE drawType, ASSET_ID textureID, rect bound, vec4 color ) {
	// VERTEX_TEXMESH * vertex = beginTask( renderSystem, drawType, transformID_overlay, textureID, color );
   
   // vec2 pos[ 4 ] = {
      // Vec2( bound.left,  bound.bottom ),
      // Vec2( bound.right, bound.bottom ),
      // Vec2( bound.left,  bound.top ),
      // Vec2( bound.right, bound.top ),
   // };
   
   // vec2 tex[ 4 ] = {
      // Vec2( 0.0f, 0.0f ),
      // Vec2( 1.0f, 0.0f ),
      // Vec2( 0.0f, 1.0f ),
      // Vec2( 1.0f, 1.0f ),
   // };
   
   // *vertex++ = VertexTexMesh2D( pos[ 0 ], tex[ 0 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 1 ], tex[ 1 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 2 ], tex[ 2 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 3 ], tex[ 3 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 2 ], tex[ 2 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 1 ], tex[ 1 ] );
   // int32 vertex_count = 6;
   
	// endTask( renderSystem, vertex_count );
// }

// internal void
// drawRect( RENDER_SYSTEM * renderSystem, ASSET_ID textureID, rect bound, vec4 color ) {
	// addTask( renderSystem, meshDrawType_triangles, textureID, bound, color );
// }

// internal void
// drawRect( RENDER_SYSTEM * renderSystem, rect bound, vec4 color ) {
	// uint32 texture_bufferID = getTexture( renderSystem->assetSystem, assetID_texture_whiteTexture ).bufferID;
	// addTask( renderSystem, meshDrawType_triangleFan, texture_bufferID, bound, color );
// }

// internal void
// drawRect( RENDER_SYSTEM * renderSystem, ASSET_ID textureID, vec2 center, vec2 dim, flo32 radians, vec4 color ) {
	// uint32 texture_bufferID = getTexture( renderSystem->assetSystem, textureID ).bufferID;
   
	// VERTEX_TEXMESH * vertex = beginTask( renderSystem, meshDrawType_triangleFan, texture_bufferID, color );
   
   // vec2 halfDim = dim * 0.5f;
   
   // vec2 xAxis = Vec2( cosf( radians ), sinf( radians ) );
   // vec2 yAxis = getPerp( xAxis );
   
   // xAxis *= halfDim.x;
   // yAxis *= halfDim.y;
   
   // vec2 tex[ 4 ] = { Vec2( 0.0f, 0.0f ), Vec2( 1.0f, 0.0f ), Vec2( 1.0f, 1.0f ), Vec2( 0.0f, 1.0f ) };
   // vec2 pos[ 4 ] = {
      // center - xAxis - yAxis,
      // center + xAxis - yAxis,
      // center + xAxis + yAxis,
      // center - xAxis + yAxis,
   // };
   
   // *vertex++ = VertexTexMesh2D( pos[ 0 ], tex[ 0 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 1 ], tex[ 1 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 2 ], tex[ 2 ] );
   // *vertex++ = VertexTexMesh2D( pos[ 3 ], tex[ 3 ] );
   
	// endTask( renderSystem, 4 );
// }

// internal void
// drawRectOutline( RENDER_SYSTEM * renderSystem, rect bound, vec4 color ) {
	// uint32 texture_bufferID = getTexture( renderSystem->assetSystem, assetID_texture_whiteTexture ).bufferID;
	// addTask( renderSystem, meshDrawType_lineLoop, texture_bufferID, bound, color );
// }

// internal void
// drawLine( RENDER_SYSTEM * renderSystem, vec2 point0, vec2 point1, vec4 color ) {
	// uint32 texture_bufferID = getTexture( renderSystem->assetSystem, assetID_texture_whiteTexture ).bufferID;
	// beginTask( renderSystem, meshDrawType_lines, texture_bufferID, color );
	// addLine( renderSystem, point0, point1 );
	// endTask( renderSystem );
// }

// internal void
// drawCircleOutline( RENDER_SYSTEM * renderSystem, vec2 center, flo32 radius, vec4 color ) {
	// uint32 texture_bufferID = getTexture( renderSystem->assetSystem, assetID_texture_whiteTexture ).bufferID;
   
   // int32 segment_count = 64;
	// beginTask( renderSystem, meshDrawType_lineLoop, texture_bufferID, color );
   // for( int32 segment_index = 0; segment_index < segment_count; segment_index++ ) {
      // flo32 radians = ( ( flo32 )segment_index / ( flo32 )segment_count ) * 2.0f * PI;
      
      // flo32 x = center.x + cosf( radians ) * radius;
      // flo32 y = center.y + sinf( radians ) * radius;
      // addPoint( renderSystem, Vec2( x, y ) );
   // }
	// endTask( renderSystem );
// }

// internal void
// drawCircle( RENDER_SYSTEM * renderSystem, vec2 center, flo32 radius, vec4 color ) {
	// uint32 texture_bufferID = getTexture( renderSystem->assetSystem, assetID_texture_whiteTexture ).bufferID;
   
   // int32 segment_count = 32;
	// VERTEX_TEXMESH * vertex = beginTask( renderSystem, meshDrawType_triangleFan, texture_bufferID, color );
   
   // *vertex++ = VertexTexMesh2D( center, 0.0f, 0.0f );
   // int32 vertex_count = 1;
   
   // for( int32 segment_index = 0; segment_index <= segment_count; segment_index++ ) {
      // flo32 radians = ( ( flo32 )segment_index / ( flo32 )segment_count ) * 2.0f * PI;
      
      // flo32 x = center.x + cosf( radians ) * radius;
      // flo32 y = center.y + sinf( radians ) * radius;
      // *vertex++ = VertexTexMesh2D( x, y, 0.0f, 0.0f );
      // vertex_count++;
   // }
   // Assert( ( renderSystem->vertexBuffer->temp_count + vertex_count ) < renderSystem->vertexBuffer->temp_maxCount );
	// endTask( renderSystem, vertex_count );
// }

internal DEBUG_VERTEX
DebugVertex( vec2 position, vec2 texCoord ) {
   DEBUG_VERTEX result = { position, texCoord };
   return result;
}

internal void
addObject( RENDERER * renderer, int32 passID, RENDER_OBJECT object ) {
   Assert( renderer->object_count[ passID ] < renderer->object_maxCount[ passID ] );
   renderer->object[ passID ][ renderer->object_count[ passID ]++ ] = object;
}

internal void
addVertex( RENDERER * renderer, int32 bufferID, void * vertex, int32 vertex_count ) {
   Assert( ( renderer->vertex_count[ bufferID ] + vertex_count ) < renderer->vertex_maxCount[ bufferID ] );
   void * dest = ( uint8 * )renderer->vertex_ptr[ bufferID ] + ( renderer->vertex_count[ bufferID ] * renderer->vertex_stride[ bufferID ] );
   memcpy( dest, vertex, ( vertex_count * renderer->vertex_stride[ bufferID ] ) );
   renderer->vertex_count[ bufferID ] += vertex_count;
}

internal void
addVertex( RENDERER * renderer, int32 bufferID, int32 vertex_count ) {
   Assert( ( renderer->vertex_count[ bufferID ] + vertex_count ) < renderer->vertex_maxCount[ bufferID ] );
   renderer->vertex_count[ bufferID ] += vertex_count;
}

internal void
drawString( RENDERER * renderer, RENDER_PASS_ID passID, ASSET_ID fontID, char * string, vec2 position, vec4 color ) {
	FONT * font = getFont( renderer->assetSystem, fontID );
   
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
   addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertex_index, vertex_count, fontID, color ) );
}

internal void
drawString( RENDERER * renderer, RENDER_PASS_ID passID, ASSET_ID fontID, char * string, vec2 position, TEXT_ALIGNMENT align, vec4 color ) {
   FONT * font = getFont( renderer->assetSystem, fontID );
   vec2 offsetToBasePoint = getOffset( font, string, align );
   
   drawString( renderer, passID, fontID, string, position + offsetToBasePoint, color );
}

internal void
drawString( RENDERER * renderer, ASSET_ID fontID, char * string, vec2 position, TEXT_ALIGNMENT align, vec4 color ) {
   FONT * font = getFont( renderer->assetSystem, fontID );
   vec2 offsetToBasePoint = getOffset( font, string, align );
   
   drawString( renderer, renderPassID_overlay, fontID, string, position + offsetToBasePoint, color );
}

#endif	// STD_INCLUDE_FUNC
