
#ifdef	STD_INCLUDE_DECL

//----------
// vertex declarations
//----------

// struct VERTEX_TEXMESH {
   // vec3 position;
   // vec2 texCoord;
   
   // int32 bone_index [ 4 ];
   // flo32 bone_weight[ 4 ];
// };

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

//----------
// vertex functions
//----------

// internal VERTEX_TEXMESH
// VertexTexMesh( vec3 pos, vec2 tex, int32 * bone_index, flo32 * bone_weight ) {
	// VERTEX_TEXMESH result = { pos.x, pos.y, pos.z, tex.x, tex.y };
   // Assert( bone_index  );
   // Assert( bone_weight );
   // for( int32 iter = 0; iter < 4; iter++ ) {
      // result.bone_index[  iter ] = bone_index[  iter ];
      // result.bone_weight[ iter ] = bone_weight[ iter ];
   // }
	// return result;
// }

// internal VERTEX_TEXMESH
// VertexTexMesh( vec3 pos, vec2 tex, int32 bone_index ) {
	// VERTEX_TEXMESH result = { pos.x, pos.y, pos.z, tex.x, tex.y };
   // result.bone_index[  0 ] = bone_index;
   // result.bone_weight[ 0 ] = 1.0f;
	// return result;
// }

// internal VERTEX_TEXMESH
// VertexTexMesh( flo32 x, flo32 y, flo32 z, flo32 u, flo32 v ) {
	// VERTEX_TEXMESH result = { x, y, z, u, v };
	// return result;
// }

// internal VERTEX_TEXMESH
// VertexTexMesh( vec3 a, flo32 u, flo32 v ) {
	// VERTEX_TEXMESH result = { a.x, a.y, a.z, u, v };
	// return result;
// }

// internal VERTEX_TEXMESH
// VertexTexMesh( flo32 x, flo32 y, flo32 z, vec2 uv ) {
	// VERTEX_TEXMESH result = { x, y, z, uv.x, uv.y };
	// return result;
// }

// internal VERTEX_TEXMESH
// VertexTexMesh( vec2 xy, flo32 z, vec2 uv ) {
   // VERTEX_TEXMESH result = { xy.x, xy.y, z, uv.x, uv.y };
   // return result;
// }

#endif	// STD_INCLUDE_FUNC