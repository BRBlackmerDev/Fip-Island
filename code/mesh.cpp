
#ifdef	STD_INCLUDE_DECL

enum MESH_DRAW_TYPE {
	meshDrawType_null,
	
	meshDrawType_triangles,
	meshDrawType_triangleStrip,
	meshDrawType_lines,
	meshDrawType_lineLoop,
   meshDrawType_lineStrip,
	
	meshDrawType_count,
};

struct MESH {
   MESH_DRAW_TYPE   drawType;
   // VERTEX_TEXMESH * vertex;
   int32            vertex_count;
};

#define MODEL__MAX_MESH_COUNT  (   8 )
#define MODEL__MAX_BONE_COUNT  ( 128 )
struct MODEL {
   int32  mesh_count;
   MESH   mesh[ MODEL__MAX_MESH_COUNT ];
   
   int32  bone_count;
   vec3   bone_position   [ MODEL__MAX_BONE_COUNT ];
   int32  bone_parent     [ MODEL__MAX_BONE_COUNT ];
   quat   bone_orientation[ MODEL__MAX_BONE_COUNT ];
   char * bone_name       [ MODEL__MAX_BONE_COUNT ];
   vec3   bone_finalPos   [ MODEL__MAX_BONE_COUNT ];
   mat4   bone_transform  [ MODEL__MAX_BONE_COUNT ];
};

#endif	// STD_INCLUDE_DECL

#ifdef	STD_INCLUDE_FUNC

// internal MESH
// Mesh( MESH_DRAW_TYPE drawType, VERTEX_TEXMESH * vertex, int32 vertex_count ) {
   // MESH result = { drawType, vertex, vertex_count };
   // return result;
// }

#endif	// STD_INCLUDE_FUNC