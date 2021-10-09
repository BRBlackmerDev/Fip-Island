
#ifdef	STD_INCLUDE_DECL

struct NAV_FACE {
   int32 index[ 3 ];
};

struct NAV_EDGE {
   int32 index[ 3 ];
};

#define NAV_MESH__POINT_COUNT  ( 1024 )
#define NAV_MESH__FACE_COUNT   ( 1024 )
struct NAV_MESH {
   int32    point_count;
   vec3     point[ NAV_MESH__POINT_COUNT ];
   
   int32    face_count;
   NAV_FACE face[ NAV_MESH__FACE_COUNT ];
   NAV_EDGE edge[ NAV_MESH__FACE_COUNT ];
};

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

internal int32
addPoint( NAV_MESH * navMesh, vec3 point ) {
   int32 result = -1;
   for( int32 point_index = 0; ( result == -1 ) && ( point_index < navMesh->point_count ); point_index++ ) {
      if( point == navMesh->point[ point_index ] ) { result = point_index; }
   }
   if( result == -1 ) {
      result = navMesh->point_count;
      
      Assert( navMesh->point_count < NAV_MESH__POINT_COUNT );
      navMesh->point[ navMesh->point_count++ ] = point;
   }
   
   Assert( result != -1 );
   return result;
}

internal void
addFace( NAV_MESH * navMesh, NAV_FACE face ) {
   Assert( navMesh->face_count < NAV_MESH__FACE_COUNT );
   navMesh->face[ navMesh->face_count++ ] = face;
}

internal void
addFace( NAV_MESH * navMesh, int32 index0, int32 index1, int32 index2 ) {
   NAV_FACE face = { index0, index1, index2 };
   addFace( navMesh, face );
}

internal void
addTri( NAV_MESH * navMesh, vec3 point0, vec3 point1, vec3 point2 ) {
   int32 index0 = addPoint( navMesh, point0 );
   int32 index1 = addPoint( navMesh, point1 );
   int32 index2 = addPoint( navMesh, point2 );
   addFace( navMesh, index0, index1, index2 );
}

internal vec3
getCenter( vec3 point0, vec3 point1, vec3 point2 ) {
   vec3 result = ( point0 + point1 + point2 ) / 3.0f;
   return result;
}

internal vec3
getCenter( NAV_MESH * navMesh, int32 face_index ) {
   NAV_FACE face = navMesh->face[ face_index ];
   vec3 result = getCenter( navMesh->point[ face.index[ 0 ] ], navMesh->point[ face.index[ 1 ] ], navMesh->point[ face.index[ 2 ] ] );
   return result;
}

#endif	// STD_INCLUDE_FUNC