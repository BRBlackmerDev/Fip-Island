
#ifdef 	STD_INCLUDE_DECL

struct RENDER_TEXTURE {
	uint32 bufferID;
	int32  width;
	int32  height;
};

struct RENDER_OBJECT {
	MESH_DRAW_TYPE drawType;
	
	int32 vertex_index;
	int32 vertex_count;
	
	ASSET_ID textureID;
	vec4 color;
};

// struct VERTEX_BUFFER {
	// VERTEX_TEXMESH * buffer_base;
   
   // boo32 taskIsActive;
	
   // // int32 temp_bufferCount;
	// int32 temp_maxCount;
	// int32 temp_count;
	// int32 temp_select;
   
   // // int32 perm_maxCount;
   // // int32 perm_count;
// };

// struct PERM_MESH {
   // int32 vertex_index;
   // int32 vertex_count;
// };

// struct VERTEX_TASK {
   // VERTEX_TEXMESH * vertex;
   // int32            count;
// };

struct ASSET_SYSTEM;
#define RENDER_SYSTEM__MAX_VERTEX_COUNT	( 2097152 )
#define RENDER_SYSTEM__MAX_OBJECT_COUNT	( 8192 )

struct RENDER_SYSTEM {
	// ASSET_SYSTEM  * assetSystem;
	// VERTEX_BUFFER * vertexBuffer;
   
	RENDER_OBJECT tempObject;
   // VERTEX_TASK   vertexTask;
	
   mat4           camera_transform;
   
	int32 			object_count;
	RENDER_OBJECT 	object[ RENDER_SYSTEM__MAX_OBJECT_COUNT ];
   
   int32          debug_count;
   RENDER_OBJECT  debug [ RENDER_SYSTEM__MAX_OBJECT_COUNT ];
};

#endif 	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

//----------
// render object functions
//----------

// internal RENDER_OBJECT
// RenderObject( MESH_DRAW_TYPE drawType, uint32 vertex_index, uint32 vertex_count, ASSET_ID textureID, vec4 color ) {
	// RENDER_OBJECT result = { drawType, vertex_index, vertex_count, textureID, color };
	// return result;
// }

internal RENDER_OBJECT
RenderObject( MESH_DRAW_TYPE drawType, uint32 vertex_index, uint32 vertex_count, ASSET_ID textureID, vec4 color ) {
	RENDER_OBJECT result = {};
   result.drawType     = drawType;
   result.vertex_index = vertex_index;
   result.vertex_count = vertex_count;
   result.textureID    = textureID;
   result.color        = color;
	return result;
}

internal RENDER_TEXTURE
RenderTexture( uint32 bufferID, int32 width, int32 height ) {
	RENDER_TEXTURE result = { bufferID, width, height };
	return result;
}

//----------
// vertex buffer functions
//----------

// internal int32
// getTempIndex( VERTEX_BUFFER * vertexBuffer ) {
	// int32 result = vertexBuffer->temp_select * vertexBuffer->temp_maxCount + vertexBuffer->temp_count;
	// return result;
// }

// internal int32
// getPermIndex( VERTEX_BUFFER * vertexBuffer ) {
   // int32 result = vertexBuffer->temp_bufferCount * vertexBuffer->temp_maxCount + vertexBuffer->perm_count;
   // return result;
// }

// internal PERM_MESH
// addPerm( VERTEX_BUFFER * vertexBuffer, MESH * mesh ) {
   // int32 vertex_index = getPermIndex( vertexBuffer );
   // int32 vertex_count = mesh->vertex_count;
   
   // memcpy( vertexBuffer->buffer_base + vertex_index, mesh->vertex, sizeof( VERTEX_TEXMESH ) * vertex_count );
   
   // PERM_MESH result = { vertex_index, vertex_count };
   // return result;
// }

// internal VERTEX_TEXMESH *
// beginTask( RENDER_SYSTEM * renderSystem, MESH_DRAW_TYPE drawType, ASSET_ID textureID, vec4 modColor ) {
	// VERTEX_BUFFER * vertexBuffer = renderSystem->vertexBuffer;
	
	// Assert( !vertexBuffer->taskIsActive );
	// Assert( renderSystem->object_count < RENDER_SYSTEM__MAX_OBJECT_COUNT );
	
	// vertexBuffer->taskIsActive = true;
   
   // int32 vertex_index = getTempIndex( vertexBuffer );
	
	// RENDER_OBJECT tempObject = {};
	// tempObject.drawType        = drawType;
	// tempObject.vertex_index    = vertex_index;
	// tempObject.textureID       = textureID;
	// tempObject.color           = modColor;
	// renderSystem->tempObject = tempObject;
   
   // VERTEX_TEXMESH * result = vertexBuffer->buffer_base + vertex_index;
   // return result;
// }

// internal void
// addQuad( RENDER_SYSTEM * renderSystem, rect vert_bound, rect tex_bound ) {
	// VERTEX_BUFFER * vertexBuffer = renderSystem->vertexBuffer;
	// Assert( vertexBuffer->taskIsActive );
	
	// VERTEX_TEXMESH vertexA = VertexTexMesh2D( vert_bound.left,  vert_bound.bottom,    tex_bound.left,  tex_bound.bottom );
	// VERTEX_TEXMESH vertexB = VertexTexMesh2D( vert_bound.right, vert_bound.bottom,    tex_bound.right, tex_bound.bottom );
	// VERTEX_TEXMESH vertexC = VertexTexMesh2D( vert_bound.right, vert_bound.top,       tex_bound.right, tex_bound.top );
	// VERTEX_TEXMESH vertexD = VertexTexMesh2D( vert_bound.left,  vert_bound.top,       tex_bound.left,  tex_bound.top );
	
	// switch( renderSystem->tempObject.drawType ) {
		// case meshDrawType_triangles: {
			// addVertex( vertexBuffer, vertexA );
			// addVertex( vertexBuffer, vertexB );
			// addVertex( vertexBuffer, vertexC );
			
			// addVertex( vertexBuffer, vertexA );
			// addVertex( vertexBuffer, vertexC );
			// addVertex( vertexBuffer, vertexD );
			
			// renderSystem->tempObject.vertex_count += 6;
		// } break;
		
		// case meshDrawType_lineLoop:
		// case meshDrawType_triangleFan: {
			// addVertex( vertexBuffer, vertexA );
			// addVertex( vertexBuffer, vertexB );
			// addVertex( vertexBuffer, vertexC );
			// addVertex( vertexBuffer, vertexD );
			
			// renderSystem->tempObject.vertex_count += 4;
		// } break;
		
		// default: {
			// InvalidCodePath;
		// };
	// }
// }

// internal void
// endTask( RENDER_SYSTEM * renderSystem, int32 vertex_count ) {
   // Assert( ( renderSystem->vertexBuffer->temp_count + vertex_count ) < renderSystem->vertexBuffer->temp_maxCount );
   
	// VERTEX_BUFFER * vertexBuffer = renderSystem->vertexBuffer;
	// Assert( vertexBuffer->taskIsActive );
   // if( vertex_count > 0 ) {
      // renderSystem->vertexBuffer->temp_count += vertex_count;
      // renderSystem->tempObject.vertex_count  += vertex_count;
      
		// renderSystem->object[ renderSystem->object_count++ ] = renderSystem->tempObject;
	// }
   // vertexBuffer->taskIsActive = false;
// }

//----------
// vertex task functions
//----------

// internal VERTEX_TASK *
// beginTask( RENDER_SYSTEM * renderSystem, MESH_DRAW_TYPE drawType, ASSET_ID textureID, vec4 modColor, flo32 ignore ) {
	// VERTEX_BUFFER * vertexBuffer = renderSystem->vertexBuffer;
	
	// Assert( !vertexBuffer->taskIsActive );
	// Assert( renderSystem->object_count < RENDER_SYSTEM__MAX_OBJECT_COUNT );
	
	// vertexBuffer->taskIsActive = true;
   
   // int32 vertex_index = getTempIndex( vertexBuffer );
	
	// RENDER_OBJECT tempObject = {};
	// tempObject.drawType     = drawType;
	// tempObject.vertex_index = vertex_index;
	// tempObject.textureID    = textureID;
	// tempObject.color        = modColor;
	// renderSystem->tempObject = tempObject;
   
   // renderSystem->vertexTask.vertex = vertexBuffer->buffer_base + vertex_index;
   // renderSystem->vertexTask.count  = 0;
   // VERTEX_TASK * result = &renderSystem->vertexTask;
   // return result;
// }

// internal void
// endTask( RENDER_SYSTEM * renderSystem, VERTEX_TASK * vertexTask ) {
   // Assert( ( renderSystem->vertexBuffer->temp_count + vertexTask->count ) < renderSystem->vertexBuffer->temp_maxCount );
   
	// VERTEX_BUFFER * vertexBuffer = renderSystem->vertexBuffer;
	// Assert( vertexBuffer->taskIsActive );
   // if( vertexTask->count > 0 ) {
      // renderSystem->vertexBuffer->temp_count += vertexTask->count;
      // renderSystem->tempObject.vertex_count  += vertexTask->count;
      
		// renderSystem->object[ renderSystem->object_count++ ] = renderSystem->tempObject;
	// }
   // vertexBuffer->taskIsActive = false;
// }

#endif 	// STD_INCLUDE_FUNC