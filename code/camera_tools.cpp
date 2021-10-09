
internal mat4
genTransform( vec3 position, vec3 scale, quat orientation ) {
    mat4 result = mat4_translate( position ) * mat4_toRotation( orientation ) * mat4_scale( scale );
    return result;
}

internal void
playAudio( AUDIO_SYSTEM * audioSystem, ASSET_ID audioID, flo32 volume ) {
	AUDIO_DATA audio = getAudio( audioSystem->assetSystem, audioID );
    playAudio( audioSystem, audio, volume );
}

internal void
playMusic( MUSIC_SYSTEM * musicSystem, ASSET_ID musicID, flo32 volume ) {
	AUDIO_DATA music = getAudio( musicSystem->assetSystem, musicID );
    playMusic( musicSystem, music, volume );
}

internal void
drawCylinder( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position, flo32 height, flo32 radius, vec4 color ) {
    int32 segment_count = 16;
    
    flo32 sin_lookup[ 16 + 1 ] = {};
    flo32 cos_lookup[ 16 + 1 ] = {};
    for( int32 segment_index = 0; segment_index <= segment_count; segment_index++ ) {
        flo32 radians = ( ( flo32 )segment_index / ( flo32 )segment_count ) * PI * 2.0f;
        sin_lookup[ segment_index ] = sinf( radians ) * radius;
        cos_lookup[ segment_index ] = cosf( radians ) * radius;
    }
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    
    for( int32 segment_index = 0; segment_index < segment_count; segment_index++ ) {
        vec3 pos[ 4 ] = {
            position + Vec3( cos_lookup[ segment_index     ], 0.0f,   -sin_lookup[ segment_index     ] ),
            position + Vec3( cos_lookup[ segment_index + 1 ], 0.0f,   -sin_lookup[ segment_index + 1 ] ),
            position + Vec3( cos_lookup[ segment_index     ], height, -sin_lookup[ segment_index     ] ),
            position + Vec3( cos_lookup[ segment_index + 1 ], height, -sin_lookup[ segment_index + 1 ] ),
        };
        
        vec3 normal = getNormal( cross( pos[ 1 ] - pos[ 0 ], pos[ 2 ] - pos[ 0 ] ) );
        addQuad( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ], normal );
    }
    
    vec3 base_position = position + Vec3( 0.0f, height, 0.0f );
    for( int32 segment_index = 0; segment_index < segment_count; segment_index++ ) {
        vec3 pos[ 3 ] = {
            base_position,
            base_position + Vec3( cos_lookup[ segment_index     ], 0.0f, -sin_lookup[ segment_index     ] ),
            base_position + Vec3( cos_lookup[ segment_index + 1 ], 0.0f, -sin_lookup[ segment_index + 1 ] ),
        };
        vec3 normal = getNormal( cross( pos[ 1 ] - pos[ 0 ], pos[ 2 ] - pos[ 0 ] ) );
        addTri( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], normal );
    }
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_boxTexture, color ) );
}

internal void
drawShadow( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position, flo32 radius, vec4 color ) {
    int32 segment_count = 16;
    
    flo32 sin_lookup[ 16 + 1 ] = {};
    flo32 cos_lookup[ 16 + 1 ] = {};
    for( int32 segment_index = 0; segment_index <= segment_count; segment_index++ ) {
        flo32 radians = ( ( flo32 )segment_index / ( flo32 )segment_count ) * PI * 2.0f;
        sin_lookup[ segment_index ] = sinf( radians ) * radius;
        cos_lookup[ segment_index ] = cosf( radians ) * radius;
    }
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    for( int32 segment_index = 0; segment_index < segment_count; segment_index++ ) {
        vec3 pos[ 3 ] = {
            position,
            position + Vec3( cos_lookup[ segment_index     ], 0.0f, -sin_lookup[ segment_index     ] ),
            position + Vec3( cos_lookup[ segment_index + 1 ], 0.0f, -sin_lookup[ segment_index + 1 ] ),
        };
        addTri( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], Vec3( 0.0f, 1.0f, 0.0f ) );
    }
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
addTri( VERTEX1_TASK * vertexTask, vec3 * pos ) {
    vec3 normal = getNormal( cross( pos[ 1 ] - pos[ 0 ], pos[ 2 ] - pos[ 0 ] ) );
    addTri( vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], normal );
}

internal void
drawQuad( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position, vec3 xAxis, vec3 yAxis, vec4 color ) {
    vec3 pos[ 4 ] = {
        position,
        position + xAxis,
        position + yAxis,
        position + xAxis + yAxis,
    };
    vec3 normal = getNormal( cross( xAxis, yAxis ) );
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addQuad( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ], normal );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
addQuad( COLLISION_SYSTEM * collisionSystem, vec3 position, vec3 xAxis, vec3 yAxis ) {
    vec3 pos[ 4 ] = {
        position,
        position + xAxis,
        position + yAxis,
        position + xAxis + yAxis,
    };
    
    addTri( collisionSystem, pos[ 0 ], pos[ 1 ], pos[ 2 ] );
    addTri( collisionSystem, pos[ 3 ], pos[ 2 ], pos[ 1 ] );
}

internal void
addQuad( COLLISION_SYSTEM * collisionSystem, vec3 point0, vec3 point1, vec3 point2, vec3 point3 ) {
    addTri( collisionSystem, point0, point1, point2 );
    addTri( collisionSystem, point3, point2, point1 );
}

internal boo32
isInLedgeBound( COLLISION_LEDGE ledge, vec3 player_position) {
    boo32 isInBound0 = true;
    boo32 isInBound1 = false;
    { // test bound 0
        flo32 dotV = dot( ledge.point1 - ledge.point0, Vec3( 0.0f, 1.0f, 0.0f ) ) / getLengthSq( ledge.point1 - ledge.point0 );
        if( fabs( dotV ) >= 0.000001f ) { 
            flo32 theta = ( PI * 0.5f ) - acosf( dotV );
            flo32 e = PLAYER__HEIGHT_STAND * sinf( theta );
            flo32 o = PLAYER__HEIGHT_STAND * cosf( theta );
            flo32 ledge_length = getLength( ledge.point1 - ledge.point0 ) + e;
            
            vec3 origin = ledge.point0 + Vec3( 0.0f, -COLLISION__LEDGE_HEIGHT + PLAYER__HEIGHT_STAND, 0.0f );
            vec3 xAxis  = getNormal( ledge.point1 - ledge.point0 ) * ledge_length;
            vec3 zAxis  = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), xAxis ) ) * COLLISION__LEDGE_DEPTH;
            vec3 yAxis  = getNormal( cross( zAxis, xAxis ) ) * o;
            vec3 zAxis_offset = getNormal( zAxis ) * COLLISION__LEDGE_DEPTH_EPSILON;
            
            flo32 xAxis_lengthSq = getLengthSq( xAxis );
            flo32 yAxis_lengthSq = getLengthSq( yAxis );
            flo32 zAxis_lengthSq = getLengthSq( zAxis );
            
            vec3 vector = player_position + zAxis_offset + Vec3( 0.0f, PLAYER__HEIGHT_STAND, 0.0f ) - origin;
            flo32 dotX = dot( vector, xAxis ) / xAxis_lengthSq;
            flo32 dotY = dot( vector, yAxis ) / yAxis_lengthSq;
            flo32 dotZ = dot( vector, zAxis ) / zAxis_lengthSq;
            isInBound0 = ( ( dotX >= 0.0f ) && ( dotX <= 1.0f ) && ( dotY >= 0.0f ) && ( dotY <= 1.0f ) && ( dotZ >= 0.0f ) && ( dotZ <= 1.0f ) );
        }
    }
    { // test bound 1
        vec3 point0 = ledge.point0 + Vec3( 0.0f, -COLLISION__LEDGE_HEIGHT + PLAYER__HEIGHT_STAND, 0.0f );
        vec3 point1 = ledge.point1 + Vec3( 0.0f, -COLLISION__LEDGE_HEIGHT,                        0.0f );
        
        vec3 origin = point0;
        vec3 xAxis  = Vec3( point1.x, point0.y, point1.z ) - origin;
        vec3 yAxis  = Vec3( point0.x, point1.y, point0.z ) - origin;
        vec3 zAxis  = getNormal( cross( xAxis, yAxis ) ) * COLLISION__LEDGE_DEPTH;
        vec3 zAxis_offset = getNormal( zAxis ) * COLLISION__LEDGE_DEPTH_EPSILON;
        
        flo32 xAxis_lengthSq = getLengthSq( xAxis );
        flo32 yAxis_lengthSq = getLengthSq( yAxis );
        flo32 zAxis_lengthSq = getLengthSq( zAxis );
        
        vec3 vector = player_position + zAxis_offset + Vec3( 0.0f, PLAYER__HEIGHT_STAND, 0.0f ) - origin;
        flo32 dotX = dot( vector, xAxis ) / xAxis_lengthSq;
        flo32 dotY = dot( vector, yAxis ) / yAxis_lengthSq;
        flo32 dotZ = dot( vector, zAxis ) / zAxis_lengthSq;
        
        isInBound1 = ( ( dotX >= 0.0f ) && ( dotX <= 1.0f ) && ( dotY >= 0.0f ) && ( dotY <= 1.0f ) && ( dotZ >= 0.0f ) && ( dotZ <= 1.0f ) );
    }
    boo32 result = ( isInBound0 ) && ( isInBound1 );
    return result;
}

internal boo32
isInLedgeBound_ledgeJump( COLLISION_LEDGE ledge, vec3 player_position, vec3 move_vector ) {
    boo32 result = true;
    if( result ) {
        vec3 xAxis  = getNormal( ledge.point1 - ledge.point0 );
        vec3 zAxis  = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), xAxis ) );
        flo32 dotZ = dot( move_vector, zAxis );
        result = ( dotZ < 0.0f );
    }
    if( result ) {
        boo32 isInBound0 = true;
        boo32 isInBound1 = false;
        { // test bound 0
            flo32 dotV = dot( ledge.point1 - ledge.point0, Vec3( 0.0f, 1.0f, 0.0f ) ) / getLengthSq( ledge.point1 - ledge.point0 );
            if( fabs( dotV ) >= 0.000001f ) { 
                flo32 theta = ( PI * 0.5f ) - acosf( dotV );
                flo32 e = ( COLLISION__LEDGE_HEIGHT + COLLISION__LEDGE_JUMP_HEIGHT ) * sinf( theta );
                flo32 o = ( COLLISION__LEDGE_HEIGHT + COLLISION__LEDGE_JUMP_HEIGHT ) * cosf( theta );
                flo32 ledge_length = getLength( ledge.point1 - ledge.point0 ) + e;
                
                vec3 origin = ledge.point0 + Vec3( 0.0f, COLLISION__LEDGE_JUMP_HEIGHT, 0.0f );
                vec3 xAxis  = getNormal( ledge.point1 - ledge.point0 ) * ledge_length;
                vec3 zAxis  = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), xAxis ) ) * COLLISION__LEDGE_DEPTH;
                vec3 yAxis  = getNormal( cross( zAxis, xAxis ) ) * o;
                vec3 zAxis_offset = getNormal( zAxis ) * COLLISION__LEDGE_DEPTH_EPSILON;
                
                flo32 xAxis_lengthSq = getLengthSq( xAxis );
                flo32 yAxis_lengthSq = getLengthSq( yAxis );
                flo32 zAxis_lengthSq = getLengthSq( zAxis );
                
                vec3 vector = player_position + zAxis_offset - origin;
                flo32 dotX = dot( vector, xAxis ) / xAxis_lengthSq;
                flo32 dotY = dot( vector, yAxis ) / yAxis_lengthSq;
                flo32 dotZ = dot( vector, zAxis ) / zAxis_lengthSq;
                isInBound0 = ( ( dotX >= 0.0f ) && ( dotX <= 1.0f ) && ( dotY >= 0.0f ) && ( dotY <= 1.0f ) && ( dotZ >= 0.0f ) && ( dotZ <= 1.0f ) );
            }
        }
        { // test bound 1
            vec3 point0 = ledge.point0 + Vec3( 0.0f,  COLLISION__LEDGE_JUMP_HEIGHT, 0.0f );
            vec3 point1 = ledge.point1 + Vec3( 0.0f, -COLLISION__LEDGE_HEIGHT,      0.0f );
            
            vec3 origin = point0;
            vec3 xAxis  = Vec3( point1.x, point0.y, point1.z ) - origin;
            vec3 yAxis  = Vec3( point0.x, point1.y, point0.z ) - origin;
            vec3 zAxis  = getNormal( cross( xAxis, yAxis ) ) * COLLISION__LEDGE_DEPTH;
            vec3 zAxis_offset = getNormal( zAxis ) * COLLISION__LEDGE_DEPTH_EPSILON;
            
            flo32 xAxis_lengthSq = getLengthSq( xAxis );
            flo32 yAxis_lengthSq = getLengthSq( yAxis );
            flo32 zAxis_lengthSq = getLengthSq( zAxis );
            
            vec3 vector = player_position + zAxis_offset - origin;
            flo32 dotX = dot( vector, xAxis ) / xAxis_lengthSq;
            flo32 dotY = dot( vector, yAxis ) / yAxis_lengthSq;
            flo32 dotZ = dot( vector, zAxis ) / zAxis_lengthSq;
            
            isInBound1 = ( ( dotX >= 0.0f ) && ( dotX <= 1.0f ) && ( dotY >= 0.0f ) && ( dotY <= 1.0f ) && ( dotZ >= 0.0f ) && ( dotZ <= 1.0f ) );
        }
        result = ( isInBound0 ) && ( isInBound1 );
    }
    return result;
}

internal boo32
isInLedgeBound_noAddHeight( COLLISION_LEDGE ledge, vec3 player_position ) {
    boo32 isInBound0 = true;
    boo32 isInBound1 = false;
    
    flo32 ledge_depth = COLLISION__LEDGE_DEPTH + COLLISION__LEDGE_DEPTH_EPSILON;
    { // test bound 0
        flo32 dotV = dot( ledge.point1 - ledge.point0, Vec3( 0.0f, 1.0f, 0.0f ) ) / getLengthSq( ledge.point1 - ledge.point0 );
        if( fabs( dotV ) >= 0.000001f ) {
            flo32 theta = ( PI * 0.5f ) - acosf( dotV );
            flo32 e = COLLISION__LEDGE_HEIGHT * sinf( theta );
            flo32 o = COLLISION__LEDGE_HEIGHT * cosf( theta );
            flo32 ledge_length = getLength( ledge.point1 - ledge.point0 ) + e;
            
            vec3 origin = ledge.point0;
            vec3 xAxis  = getNormal( ledge.point1 - ledge.point0 ) * ledge_length;
            vec3 zAxis  = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), xAxis ) ) * ledge_depth;
            vec3 yAxis  = getNormal( cross( zAxis, xAxis ) ) * o;
            vec3 zAxis_offset = getNormal( zAxis ) * COLLISION__LEDGE_DEPTH_EPSILON;
            
            flo32 xAxis_lengthSq = getLengthSq( xAxis );
            flo32 yAxis_lengthSq = getLengthSq( yAxis );
            flo32 zAxis_lengthSq = getLengthSq( zAxis );
            
            vec3 vector = player_position + zAxis_offset - origin;
            flo32 dotX = dot( vector, xAxis ) / xAxis_lengthSq;
            flo32 dotY = dot( vector, yAxis ) / yAxis_lengthSq;
            flo32 dotZ = dot( vector, zAxis ) / zAxis_lengthSq;
            isInBound0 = ( ( dotX >= 0.0f ) && ( dotX <= 1.0f ) && ( dotY >= 0.0f ) && ( dotY <= 1.0f ) && ( dotZ >= 0.0f ) && ( dotZ <= 1.0f ) );
        }
    }
    { // test bound 1
        vec3 point0 = ledge.point0;
        vec3 point1 = ledge.point1 + Vec3( 0.0f, -COLLISION__LEDGE_HEIGHT, 0.0f );
        
        vec3 origin = point0;
        vec3 xAxis  = Vec3( point1.x, point0.y, point1.z ) - origin;
        vec3 yAxis  = Vec3( point0.x, point1.y, point0.z ) - origin;
        vec3 zAxis  = getNormal( cross( xAxis, yAxis ) ) * ledge_depth;
        vec3 zAxis_offset = getNormal( zAxis ) * COLLISION__LEDGE_DEPTH_EPSILON;
        
        flo32 xAxis_lengthSq = getLengthSq( xAxis );
        flo32 yAxis_lengthSq = getLengthSq( yAxis );
        flo32 zAxis_lengthSq = getLengthSq( zAxis );
        
        vec3 vector = player_position + zAxis_offset - origin;
        flo32 dotX = dot( vector, xAxis ) / xAxis_lengthSq;
        flo32 dotY = dot( vector, yAxis ) / yAxis_lengthSq;
        flo32 dotZ = dot( vector, zAxis ) / zAxis_lengthSq;
        
        isInBound1 = ( ( dotX >= 0.0f ) && ( dotX <= 1.0f ) && ( dotY >= 0.0f ) && ( dotY <= 1.0f ) && ( dotZ >= 0.0f ) && ( dotZ <= 1.0f ) );
    }
    boo32 result = ( isInBound0 ) && ( isInBound1 );
    return result;
}

// internal RENDER_DATA
// addCube( COLLISION_SYSTEM * collisionSystem, vec3 min, vec3 max, vec4 color ) {
// RENDER_DATA result = {};
// result.vertex       = collisionSystem->tri_vertex + collisionSystem->tri_count * 3;
// result.vertex_count = 36;
// result.color = color;

// vec3 pos[ 8 ] = {
// min,
// Vec3( min.x, min.y, max.z ),
// Vec3( min.x, max.y, min.z ),
// Vec3( min.x, max.y, max.z ),
// Vec3( max.x, min.y, min.z ),
// Vec3( max.x, min.y, max.z ),
// Vec3( max.x, max.y, min.z ),
// max,
// };

// addQuad( collisionSystem, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ] );
// // addQuad( collisionSystem, pos[ 0 ], pos[ 4 ], pos[ 1 ], pos[ 5 ] );
// addQuad( collisionSystem, pos[ 4 ], pos[ 0 ], pos[ 6 ], pos[ 2 ] );
// addQuad( collisionSystem, pos[ 5 ], pos[ 4 ], pos[ 7 ], pos[ 6 ] );
// addQuad( collisionSystem, pos[ 3 ], pos[ 7 ], pos[ 2 ], pos[ 6 ] );
// addQuad( collisionSystem, pos[ 1 ], pos[ 5 ], pos[ 3 ], pos[ 7 ] );
// addEdge( collisionSystem, pos[ 0 ], pos[ 2 ] );
// addEdge( collisionSystem, pos[ 1 ], pos[ 3 ] );
// addEdge( collisionSystem, pos[ 5 ], pos[ 7 ] );
// addEdge( collisionSystem, pos[ 4 ], pos[ 6 ] );

// return result;
// }

// internal RENDER_DATA
// addCube( COLLISION_SYSTEM * collisionSystem, vec3 min, vec3 max, vec4 color, flo32 player_radius ) {
// RENDER_DATA result = {};
// result.vertex       = collisionSystem->tri_vertex + collisionSystem->tri_count * 3;
// result.vertex_count = 36;
// result.color = color;

// vec3 pos[ 8 ] = {
// min,
// Vec3( min.x, min.y, max.z ),
// Vec3( min.x, max.y, min.z ),
// Vec3( min.x, max.y, max.z ),
// Vec3( max.x, min.y, min.z ),
// Vec3( max.x, min.y, max.z ),
// Vec3( max.x, max.y, min.z ),
// max,
// };

// addQuad( collisionSystem, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ] );
// // addQuad( collisionSystem, pos[ 0 ], pos[ 4 ], pos[ 1 ], pos[ 5 ] );
// addQuad( collisionSystem, pos[ 4 ], pos[ 0 ], pos[ 6 ], pos[ 2 ] );
// addQuad( collisionSystem, pos[ 5 ], pos[ 4 ], pos[ 7 ], pos[ 6 ] );
// addQuad( collisionSystem, pos[ 3 ], pos[ 7 ], pos[ 2 ], pos[ 6 ] );
// addQuad( collisionSystem, pos[ 1 ], pos[ 5 ], pos[ 3 ], pos[ 7 ] );
// addEdge( collisionSystem, pos[ 0 ], pos[ 2 ] );
// addEdge( collisionSystem, pos[ 1 ], pos[ 3 ] );
// addEdge( collisionSystem, pos[ 5 ], pos[ 7 ] );
// addEdge( collisionSystem, pos[ 4 ], pos[ 6 ] );

// return result;
// }

internal void
debugTriOutline( vec3 point0, vec3 point1, vec3 point2, vec4 color ) {
    RENDERER * renderer = globalVar_debugSystem.renderer;
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    
    addEdge( &vertexTask, point0, point1, DIRECTION_TO_LIGHT );
    addEdge( &vertexTask, point1, point2, DIRECTION_TO_LIGHT );
    addEdge( &vertexTask, point2, point0, DIRECTION_TO_LIGHT );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, renderPassID_main_debug, RenderObject( meshDrawType_lines, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

// internal void
// updateMoveCollision( APP_STATE * appState, flo32 dt ) {
// PROFILE_FUNCTION();

// PLAYER_STATE * playerState = &appState->playerState;
// COLLISION_SYSTEM * collisionSystem = &appState->collisionSystem;

// vec3 velocity = Vec3( playerState->velocity_move.x, 0.0f, playerState->velocity_move.y );

// flo32 tMin = 1.0f;
// for( int32 iter = 0; ( tMin > 0.0f ) && ( iter < 4 ); iter++ ) {
// boo32 collision_isValid = false;
// flo32 collision_t       = tMin;
// vec3  collision_normal  = {};

// vec3 dPos = velocity * ( dt * tMin );
// if( getLength( dPos ) > 0.0001f ) {
// for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
// vec3 vertex[ 3 ] = {};
// memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
// vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );

// if( dot( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) <= 0.0f ) {
// vec3 offset = normal * ( playerState->radius + 0.01f );
// for( int32 vertex_index = 0; vertex_index < 3; vertex_index++ ) { vertex[ vertex_index ] += offset; }
// }

// vec3 ray_origin = playerState->position;
// vec3 ray_vector = dPos;

// flo32 t0 = dot( ray_vector, normal );
// if( t0 < -0.000001f ) {
// flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
// if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }

// if( ( t >= 0.0f ) && ( t < collision_t ) ) {
// vec3 point = ray_origin + ray_vector * t;

// vec3 A = vertex[ 0 ];
// vec3 B = vertex[ 1 ];
// vec3 C = vertex[ 2 ];
// vec3 P = point;

// vec3 X = B - A; // 3 sub
// vec3 Y = C - A; // 3 sub
// vec3 Z = P - A; // 3 sub

// flo32 dotXX = dot( X, X ); // 3 mul, 2 add
// flo32 dotXY = dot( X, Y ); // 3 mul, 2 add
// flo32 dotXZ = dot( X, Z ); // 3 mul, 2 add
// flo32 dotYY = dot( Y, Y ); // 3 mul, 2 add
// flo32 dotYZ = dot( Y, Z ); // 3 mul, 2 add

// flo32 detA  = dotXX * dotYY - dotXY * dotXY; // 2 mul, 1 sub
// flo32 detCv = dotXZ * dotYY - dotXY * dotYZ; // 2 mul, 1 sub
// flo32 detCw = dotXX * dotYZ - dotXZ * dotXY; // 2 mul, 1 sub

// flo32 w = detCw / detA; // 1 div
// flo32 v = detCv / detA; // 1 div
// flo32 u = 1.0f - v - w; // 2 sub
// flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
// // total : 21 mul, 14 sub, 10 add, 2 div

// if( isValid ) {
// collision_isValid = true;
// collision_t       = t;
// collision_normal  = normal;
// }
// }
// }
// }

// flo32 cylinder_radius = playerState->radius + 0.01f;
// for( int32 edge_index = 0; edge_index < collisionSystem->edge_count; edge_index++ ) {
// vec3 vertex[ 2 ] = {};
// memcpy( vertex, collisionSystem->edge_vertex + edge_index * 2, sizeof( vec3 ) * 2 );

// vec3 ray_origin = playerState->position;
// vec3 ray_vector = dPos;

// vec3 cylinder_origin = vertex[ 0 ];
// vec3 cylinder_vector = vertex[ 1 ] - vertex[ 0 ];
// vec3 cylinder_normal = getNormal( cylinder_vector );
// cylinder_origin -= cylinder_normal * 0.01f;

// flo32 s      =  0.0f;
// flo32 distSq = -1.0f;

// { // get dist sq
// vec3 U = ray_vector;
// vec3 V = cylinder_vector;
// vec3 W = ray_origin - cylinder_origin;

// flo32 a = dot( U, U );
// flo32 b = dot( U, V );
// flo32 c = dot( V, V );
// flo32 d = dot( U, W );
// flo32 e = dot( V, W );

// flo32 denom = a * c - b * b;
// if( denom != 0.0f ) {
// s = ( b * e - c * d ) / denom;
// flo32 t = ( a * e - b * d ) / denom;

// vec3 pointA = ray_origin      + ray_vector      * s;
// vec3 pointB = cylinder_origin + cylinder_vector * t;
// distSq = getLengthSq( pointB - pointA );
// }
// }

// flo32 bSq = ( cylinder_radius * cylinder_radius ) - distSq;
// if( bSq > 0.000001f ) {
// flo32 denom = 1.0f - ( squareValue( dot( ray_vector, cylinder_vector ) ) / ( getLengthSq( ray_vector ) * getLengthSq( cylinder_vector ) ) );
// if( absValue( denom ) > 0.000001f ) {
// flo32 s0 = s - sqrtf( bSq / ( denom * getLengthSq( ray_vector ) ) );
// if( ( ( s0 * getLength( ray_vector ) ) >= -0.01f ) && ( s0 < 0.0f ) ) { s0 = 0.0f; }
// if( ( s0 >= 0.0f ) && ( s0 < collision_t ) ) {
// vec3  intersect_vector = ( ray_origin + ray_vector * s0 ) - cylinder_origin;
// flo32 intersect_ratio  = ( dot( intersect_vector, cylinder_vector ) / dot( cylinder_vector, cylinder_vector ) );
// if( ( intersect_ratio >= 0.0f ) && ( intersect_ratio <= 1.0f ) ) {
// vec3 normal = getNormal( intersect_vector - cylinder_vector * intersect_ratio );
// if( dot( normal, ray_vector ) < -0.000001f ) {
// collision_isValid = true;
// collision_t       = s0;
// collision_normal  = normal;
// }
// }
// }
// }
// }
// }

// if( collision_isValid ) {
// flo32 collision_dot     = dot( collision_normal, Vec3( 0.0f, 1.0f, 0.0f ) );
// flo32 collision_radians = acosf( collision_dot );
// flo32 radians0 = ( PI * 0.25f );
// // DISPLAY_VALUE( flo32, collision_radians );
// // DISPLAY_VALUE( flo32, radians0 );
// if( collision_radians > ( PI * 0.25f ) ) {
// collision_normal = getNormal( collision_normal - Vec3( 0.0f, 1.0f, 0.0f ) * collision_dot );
// }

// velocity -= dot( collision_normal, velocity ) * collision_normal;
// }

// // check dest is valid
// // {
// // vec3 position = playerState->position + dPos * collision_t;
// // vec3 vector   = Vec3( 0.0f, -1.0f, 0.0f );

// // boo32 dest_isValid = false;
// // flo32 dest_t       = 1.0f;

// // vec3 debug_vertex[ 3 ] = {};

// // for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
// // vec3 vertex[ 3 ] = {};
// // memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
// // vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );

// // vec3 ray_origin = position;
// // vec3 ray_vector = vector;

// // flo32 t0 = dot( ray_vector, normal );
// // if( t0 < -0.000001f ) {
// // flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
// // if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }

// // if( ( t >= 0.0f ) && ( t < dest_t ) ) {
// // vec3 point = ray_origin + ray_vector * t;
// // vec3 normal0 = getNormal( cross( normal, vertex[ 1 ] - vertex[ 0 ] ) );
// // vec3 normal1 = getNormal( cross( normal, vertex[ 2 ] - vertex[ 1 ] ) );
// // vec3 normal2 = getNormal( cross( normal, vertex[ 0 ] - vertex[ 2 ] ) );

// // flo32 dot0 = dot( point - ( vertex[ 0 ] - normal0 * 0.01f ), normal0 );
// // flo32 dot1 = dot( point - ( vertex[ 1 ] - normal1 * 0.01f ), normal1 );
// // flo32 dot2 = dot( point - ( vertex[ 2 ] - normal2 * 0.01f ), normal2 );

// // boo32 isValid = ( ( dot0 >= 0.0f ) && ( dot1 >= 0.0f ) && ( dot2 >= 0.0f ) );
// // if( isValid ) {
// // dest_isValid = true;
// // dest_t       = t;

// // memcpy( debug_vertex, vertex, sizeof( vec3 ) * 3 );
// // }
// // }
// // }
// // }

// // // DISPLAY_VALUE( boo32, dest_isValid );
// // if( dest_isValid ) {
// // playerState->position  = position + vector * dest_t;
// // playerState->position += Vec3( 0.0f, 0.5f, 0.0f );

// // debugTriOutline( debug_vertex[ 0 ], debug_vertex[ 1 ], debug_vertex[ 2 ], COLOR_CYAN );
// // }
// // }

// playerState->position += dPos * collision_t;
// tMin -= collision_t;
// }
// }

// playerState->velocity_move = Vec2( velocity.x, velocity.z );
// }

internal flo32
distSq_pointToLine( vec3 point, vec3 vertex0, vec3 vertex1 ) {
    vec3  vector0 = point   - vertex0;
    vec3  vector1 = vertex1 - vertex0;
    
    flo32 t      = clamp01( dot( vector0, vector1 ) / getLengthSq( vector1 ) );
    flo32 result = getLengthSq( point - ( vertex0 + vector1 * t ) );
    return result;
}

internal flo32
dist_pointToLine( vec3 point, vec3 vertex0, vec3 vertex1 ) {
    flo32 result = sqrtf( distSq_pointToLine( point, vertex0, vertex1 ) );
    return result;
}

internal vec3
getShadowPosition( COLLISION_SYSTEM * collisionSystem, vec3 player_position, flo32 player_radius ) {
    PROFILE_FUNCTION();
    
    flo32 tMin   = 1000000.0f;
    vec3  result = Vec3( 0.0f, 1000000.0f, 0.0f );
    
    flo32 cylinder_radiusSq = ( player_radius + 0.01f ) * ( player_radius + 0.01f );
    
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        vec3 ray_origin = player_position;
        vec3 ray_vector = Vec3( 0.0f, -1000000.0f, 0.0f );
        
        flo32 t0 = dot( ray_vector, normal );
        if( t0 < -0.000001f ) {
            flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
            if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
            
            if( ( t >= 0.0f ) && ( t < tMin ) ) {
                vec3 P = ray_origin + ray_vector * t;
                vec3 A = vertex[ 0 ];
                vec3 B = vertex[ 1 ];
                vec3 C = vertex[ 2 ];
                
                vec3 X = B - A;
                vec3 Y = C - A;
                vec3 Z = P - A;
                
                flo32 dotXX = dot( X, X );
                flo32 dotXY = dot( X, Y );
                flo32 dotXZ = dot( X, Z );
                flo32 dotYY = dot( Y, Y );
                flo32 dotYZ = dot( Y, Z );
                
                flo32 detA  = dotXX * dotYY - dotXY * dotXY;
                flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
                flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
                
                flo32 w = detCw / detA;
                flo32 v = detCv / detA;
                flo32 u = 1.0f - v - w;
                flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
                if( isValid ) {
                    tMin   = t;
                    result = P + Vec3( 0.0f, 0.01f, 0.0f );
                }
            }
        }
    }
    
    return result;
}

internal boo32
canJump( COLLISION_SYSTEM * collisionSystem, vec3 player_position, flo32 player_radius ) {
    PROFILE_FUNCTION();
    
    boo32 result = false;
    
    flo32 cylinder_radiusSq = player_radius * player_radius;
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        vec3 ray_origin = player_position;
        vec3 ray_vector = Vec3( 0.0f, -1.0f, 0.0f );
        
        flo32 t0 = dot( ray_vector, normal );
        if( t0 < -0.000001f ) {
            flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
            if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
            
            if( ( t >= 0.0f ) && ( getLength( ray_vector * t ) < 0.0001f ) ) {
                vec3 P = ray_origin + ray_vector * t;
                vec3 A = vertex[ 0 ];
                vec3 B = vertex[ 1 ];
                vec3 C = vertex[ 2 ];
                
                vec3 X = B - A;
                vec3 Y = C - A;
                vec3 Z = P - A;
                
                flo32 dotXX = dot( X, X );
                flo32 dotXY = dot( X, Y );
                flo32 dotXZ = dot( X, Z );
                flo32 dotYY = dot( Y, Y );
                flo32 dotYZ = dot( Y, Z );
                
                flo32 detA  = dotXX * dotYY - dotXY * dotXY;
                flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
                flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
                
                flo32 w = detCw / detA;
                flo32 v = detCv / detA;
                flo32 u = 1.0f - v - w;
                flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
                if( isValid ) {
                    result = true;
                }
            }
        }
    }
    
    return result;
}

internal COLLISION_TRIANGLE
getFirstTriangle( COLLISION_SYSTEM * collisionSystem, vec3 ray_origin, vec3 ray_vector ) {
    PROFILE_FUNCTION();
    
    COLLISION_TRIANGLE result = {};
    flo32 tMin   = 1000000.0f;
    
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        flo32 t0 = dot( ray_vector, normal );
        if( t0 < -0.000001f ) {
            flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
            if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
            
            if( ( t >= 0.0f ) && ( t < tMin ) ) {
                vec3 P = ray_origin + ray_vector * t;
                vec3 A = vertex[ 0 ];
                vec3 B = vertex[ 1 ];
                vec3 C = vertex[ 2 ];
                
                vec3 X = B - A;
                vec3 Y = C - A;
                vec3 Z = P - A;
                
                flo32 dotXX = dot( X, X );
                flo32 dotXY = dot( X, Y );
                flo32 dotXZ = dot( X, Z );
                flo32 dotYY = dot( Y, Y );
                flo32 dotYZ = dot( Y, Z );
                
                flo32 detA  = dotXX * dotYY - dotXY * dotXY;
                flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
                flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
                
                flo32 w = detCw / detA;
                flo32 v = detCv / detA;
                flo32 u = 1.0f - v - w;
                flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
                
                if( !isValid ) {
                    flo32 dist_u = dist_pointToLine( P, B, C );
                    flo32 dist_v = dist_pointToLine( P, C, A );
                    flo32 dist_w = dist_pointToLine( P, A, B );
                    
                    if( ( dist_u <= COLLISION__BORDER_EPSILON ) || ( dist_v <= COLLISION__BORDER_EPSILON ) || ( dist_w <= COLLISION__BORDER_EPSILON ) ) {
                        isValid = true;
                    }
                }
                
                if( isValid ) {
                    tMin = t;
                    result.index            = tri_index;
                    result.isValid          = true;
                    result.vertex           = collisionSystem->tri_vertex + tri_index * 3;
                    result.collision_point  = P;
                    result.collision_normal = normal;
                    result.collision_t      = t;
                    result.collision_dist   = getLength( ray_vector ) * t;
                    
                    flo32 radians = acosf( dot( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) );
                    flo32 degrees = radians / ( 2.0f * PI ) * 360.0f;
                    // // DISPLAY_VALUE( flo32, degrees );
                    if( degrees <= 25.0f ) {
                        result.isWalkable = true;
                    } else {
                        result.isWalkable = false;
                    }
                }
            }
        }
    }
    return result;
}

internal COLLISION_TRIANGLE
getSecondaryTriangle( COLLISION_SYSTEM * collisionSystem, vec3 ray_origin, vec3 ray_vector, flo32 height ) {
    // PROFILE_FUNCTION();
    
    COLLISION_TRIANGLE result = {};
    flo32 tMin   = 1000000.0f;
    
    // DISPLAY_VALUE( uint32, collisionSystem->overhang_count );
    for( uint32 overhang_index = 0; overhang_index < collisionSystem->overhang_count; overhang_index++ ) {
        COLLISION_OVERHANG overhang = collisionSystem->overhang[ overhang_index ];
        
        vec3 vert0 = overhang.point0;
        vec3 vert1 = overhang.point1;
        vec3 vert2 = vert0 + Vec3( 0.0f, -height, 0.0f );
        vec3 vert3 = vert1 + Vec3( 0.0f, -height, 0.0f );
        
        vec3 v0[ 6 ] = {};
        v0[ 0 ] = vert2;
        v0[ 1 ] = vert1;
        v0[ 2 ] = vert0;
        
        v0[ 3 ] = vert1;
        v0[ 4 ] = vert2;
        v0[ 5 ] = vert3;
        
        for( int32 tri_index = 0; tri_index < 2; tri_index++ ) {
            vec3 * vertex = v0 + ( tri_index * 3 );
            vec3 normal = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
            
            flo32 t0 = dot( ray_vector, normal );
            if( t0 < -0.000001f ) {
                flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
                if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
                
                if( ( t >= 0.0f ) && ( t < tMin ) ) {
                    vec3 P = ray_origin + ray_vector * t;
                    vec3 A = vertex[ 0 ];
                    vec3 B = vertex[ 1 ];
                    vec3 C = vertex[ 2 ];
                    
                    vec3 X = B - A;
                    vec3 Y = C - A;
                    vec3 Z = P - A;
                    
                    flo32 dotXX = dot( X, X );
                    flo32 dotXY = dot( X, Y );
                    flo32 dotXZ = dot( X, Z );
                    flo32 dotYY = dot( Y, Y );
                    flo32 dotYZ = dot( Y, Z );
                    
                    flo32 detA  = dotXX * dotYY - dotXY * dotXY;
                    flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
                    flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
                    
                    flo32 w = detCw / detA;
                    flo32 v = detCv / detA;
                    flo32 u = 1.0f - v - w;
                    flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
                    
                    if( !isValid ) {
                        flo32 dist_u = dist_pointToLine( P, B, C );
                        flo32 dist_v = dist_pointToLine( P, C, A );
                        flo32 dist_w = dist_pointToLine( P, A, B );
                        
                        if( ( dist_u <= 0.0005 ) || ( dist_v <= 0.0005 ) || ( dist_w <= 0.0005 ) ) {
                            isValid = true;
                        }
                    }
                    
                    if( isValid ) {
                        tMin = t;
                        result.isValid          = true;
                        result.collision_point  = P;
                        result.collision_normal = normal;
                        result.collision_t      = t;
                        result.collision_dist   = getLength( ray_vector ) * t;
                    }
                }
            }
        }
    }
    return result;
}

internal COLLISION_TRIANGLE
getTertiaryTriangle( COLLISION_SYSTEM * collisionSystem, vec3 ray_origin, vec3 ray_vector ) {
    PROFILE_FUNCTION();
    
    COLLISION_TRIANGLE result = {};
    flo32 tMin   = 1000000.0f;
    
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        flo32 dot0 = dot( normal, Vec3( 0.0f, -1.0f, 0.0f ) );
        if( dot0 > 0.000001f ) {
            flo32 t0 = dot( ray_vector, normal );
            if( t0 < -0.000001f ) {
                flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
                if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
                
                if( ( t >= 0.0f ) && ( t < tMin ) ) {
                    vec3 P = ray_origin + ray_vector * t;
                    vec3 A = vertex[ 0 ];
                    vec3 B = vertex[ 1 ];
                    vec3 C = vertex[ 2 ];
                    
                    vec3 X = B - A;
                    vec3 Y = C - A;
                    vec3 Z = P - A;
                    
                    flo32 dotXX = dot( X, X );
                    flo32 dotXY = dot( X, Y );
                    flo32 dotXZ = dot( X, Z );
                    flo32 dotYY = dot( Y, Y );
                    flo32 dotYZ = dot( Y, Z );
                    
                    flo32 detA  = dotXX * dotYY - dotXY * dotXY;
                    flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
                    flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
                    
                    flo32 w = detCw / detA;
                    flo32 v = detCv / detA;
                    flo32 u = 1.0f - v - w;
                    flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
                    
                    if( !isValid ) {
                        flo32 dist_u = dist_pointToLine( P, B, C );
                        flo32 dist_v = dist_pointToLine( P, C, A );
                        flo32 dist_w = dist_pointToLine( P, A, B );
                        
                        if( ( dist_u <= 0.0005 ) || ( dist_v <= 0.0005 ) || ( dist_w <= 0.0005 ) ) {
                            isValid = true;
                        }
                    }
                    
                    if( isValid ) {
                        tMin = t;
                        result.index            = tri_index;
                        result.isValid          = true;
                        result.vertex           = collisionSystem->tri_vertex + tri_index * 3;
                        result.collision_point  = P;
                        result.collision_normal = normal;
                        result.collision_t      = t;
                        result.collision_dist   = getLength( ray_vector ) * t;
                    }
                }
            }
        }
    }
    return result;
}

internal COLLISION_TRIANGLE
getWallTriangle( COLLISION_SYSTEM * collisionSystem, vec3 position, vec3 move_vector ) {
    PROFILE_FUNCTION();
    
    uint32             triangle_count = 0;
    COLLISION_TRIANGLE triangle[ 6 ]  = {};
    
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        flo32 d    = dot( normal, vertex[ 0 ] );
        flo32 dist = dot( normal, position ) - d;
        if( ( dist >= -0.0001f ) && ( dist < 0.0f ) ) { dist = 0.0f; }
        
        if( ( dist >= 0.0f ) && ( dist <= COLLISION__WALL_JUMP_THRESHOLD ) ) {
            vec3 P = position - normal * dist;
            vec3 A = vertex[ 0 ];
            vec3 B = vertex[ 1 ];
            vec3 C = vertex[ 2 ];
            
            vec3 X = B - A;
            vec3 Y = C - A;
            vec3 Z = P - A;
            
            flo32 dotXX = dot( X, X );
            flo32 dotXY = dot( X, Y );
            flo32 dotXZ = dot( X, Z );
            flo32 dotYY = dot( Y, Y );
            flo32 dotYZ = dot( Y, Z );
            
            flo32 detA  = dotXX * dotYY - dotXY * dotXY;
            flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
            flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
            
            flo32 w = detCw / detA;
            flo32 v = detCv / detA;
            flo32 u = 1.0f - v - w;
            flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
            
            if( isValid ) {
                flo32 radians = acosf( dot( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) );
                flo32 degrees = radians / ( 2.0f * PI ) * 360.0f;
                boo32 isWalkable = ( degrees <= 25.0f );
                if( !isWalkable ) {
                    COLLISION_TRIANGLE tri = {};
                    
                    tri.index            = tri_index;
                    tri.isValid          = true;
                    tri.vertex           = collisionSystem->tri_vertex + tri_index * 3;
                    tri.collision_point  = P;
                    tri.collision_normal = normal;
                    tri.collision_dist   = dist;
                    tri.isWalkable       = isWalkable;
                    
                    Assert( triangle_count < 6 );
                    triangle[ triangle_count++ ] = tri;
                }
            }
        }
    }
    
    COLLISION_TRIANGLE result = {};
    
    vec3 direction = getNormal( move_vector );
    flo32 dMax = -1000000.0f;
    for( uint32 iTriangle = 0; iTriangle < triangle_count; iTriangle++ ) {
        COLLISION_TRIANGLE tri = triangle[ iTriangle ];
        
        flo32 d = fabsf( dot( direction, tri.collision_normal ) );
        if( d > dMax ) {
            dMax   = d;
            result = tri;
        }
    }
    
    return result;
}

internal void
loadLevel( APP_STATE * appState, MEMORY * permMemory, MEMORY * tempMemory ) {
    READ_FILE_RESULT file_level = readFile( tempMemory, "../../art/level/level.lvl" );
    Assert( file_level.contents );
    
    FILE_PARSER  _parser = FileParser( file_level.contents, file_level.size );
    FILE_PARSER * parser = &_parser;
    
    eatWhiteSpace( parser );
    char * heading = parseToken( parser );
    if( !matchString( heading, "[LEVEL_VERS01]" ) ) {
        OutputDebugString( "ERROR! File is not valid level file!\n" );
    }
    
    // remove comments
    char * start = ( char * )file_level.contents;
    char * at    = start;
    boo32 eatComment = false;
    while( ( at - start ) < file_level.size ) {
        if( eatComment ) {
            if( at[ 0 ] == '\n' ) {
                eatComment = false;
            } else {
                at[ 0 ] = ' ';
            }
        } else {
            if( at[ 0 ] == '/' ) {
                if( ( ( at - start ) < ( file_level.size - 1 ) ) && ( at[ 1 ] == '/' ) ) {
                    eatComment = true;
                    at[ 0 ] = ' ';
                }
            }
        }
        at++;
    }
    
    // TODO: better error checking (like, actually check for errors)
    while( hasTextRemaining( parser ) ) {
        char * token0 = parseToken( parser );
        char * token1 = parseToken( parser );
        char * token2 = parseToken( parser );
        if( token2[ 0 ] != '{' ) {
            OutputDebugString( "ERROR! File is not valid level file!\n" );
        }
        
        if( token0[ 0 ] == '-' ) {
            boo32 skipEntity = true;
            while( skipEntity ) {
                char * token = parseToken( parser );
                if( matchString( token, "}" ) ) {
                    skipEntity = false;
                }
            }
        } else {
            if( matchString( token0, "[ENTITY]" ) ) {
                uint32 entityID  = parseU32( token1 );
                mat4   transform_translate = mat4_identity();
                mat4   transform_scale     = mat4_identity();
                mat4   transform_rotation  = mat4_identity();
                vec4   modColor  = COLOR_WHITE;
                
                boo32 enableCollision = true;
                
                boo32 parseEntity = true;
                while( parseEntity ) {
                    token0 = parseToken( parser );
                    if( token0[ 0 ] == '-' ) {
                        parseLine( parser );
                    } else {
                        if( matchString( token0, "[POSITION]" ) ) {
                            flo32 x = parseF32( parser );
                            flo32 y = parseF32( parser );
                            flo32 z = parseF32( parser );
                            transform_translate = mat4_translate( x, y, z );
                        } else if( matchString( token0, "[SCALE]" ) ) {
                            flo32 x = parseF32( parser );
                            flo32 y = parseF32( parser );
                            flo32 z = parseF32( parser );
                            transform_scale = mat4_scale( x, y, z );
                        } else if( matchString( token0, "[ROTATION]" ) ) {
                            flo32 xDegrees = parseF32( parser );
                            flo32 yDegrees = parseF32( parser );
                            flo32 zDegrees = parseF32( parser );
                            
                            quat xRot = Quat( Vec3( 1.0f, 0.0f, 0.0f ), toRadians( xDegrees ) );
                            quat yRot = Quat( Vec3( 0.0f, 1.0f, 0.0f ), toRadians( yDegrees ) );
                            quat zRot = Quat( Vec3( 0.0f, 0.0f, 1.0f ), toRadians( zDegrees ) );
                            quat orientation = xRot * yRot * zRot;
                            transform_rotation = mat4_toRotation( orientation );
                        } else if( matchString( token0, "[COLOR]" ) ) {
                            flo32 r = parseF32( parser );
                            flo32 g = parseF32( parser );
                            flo32 b = parseF32( parser );
                            modColor = Vec4( r, g, b, 1.0f );
                        } else if( matchString( token0, "[COLLISION]" ) ) {
                            token0 = parseToken( parser );
                            
                            if( matchString( token0, "NONE" ) ) {
                                enableCollision = false;
                            } else {
                                InvalidCodePath;
                            }
                        } else if( matchString( token0, "}" ) ) {
                            mat4 transform = transform_translate * transform_rotation * transform_scale;
                            loadEntity( appState, permMemory, tempMemory, entityID, transform, modColor, enableCollision );
                            parseEntity = false;
                        } else {
                            InvalidCodePath;
                        }
                    }
                }
            } else if( matchString( token0, "[PLAYER]" ) ) {
                boo32 parseEntity = true;
                while( parseEntity ) {
                    token0 = parseToken( parser );
                    if( token0[ 0 ] == '-' ) {
                        parseLine( parser );
                    } else {
                        if( matchString( token0, "[POSITION]" ) ) {
                            flo32 x = parseF32( parser );
                            flo32 y = parseF32( parser );
                            flo32 z = parseF32( parser );
                            appState->player_resetPosition = Vec3( x, y, z );
                        } else if( matchString( token0, "}" ) ) {
                            parseEntity = false;
                        } else {
                            InvalidCodePath;
                        }
                    }
                }
            } else {
                InvalidCodePath;
            }
        }
    }
    
    _popSize( tempMemory, file_level.size );
}

internal flo32
getCameraFOVAngle( flo32 heightOnScreen, flo32 cameraDist ) {
    flo32 result = 2.0f * atanf( ( 0.5f * PLAYER__HEIGHT_STAND / heightOnScreen ) / cameraDist );
    return result;
}

internal void
genTerrain( TERRAIN_GENERATION * terrain ) {
    terrain->position_start = terrain->position_end;
    
    uint32 vertexPerTerrain = 30;
    
    { // copy last terrain into first position
        memcpy( terrain->collision_vertex, terrain->collision_vertex + ( vertexPerTerrain * ( terrain->terrain_count - 1 ) ), sizeof( vec3    ) * vertexPerTerrain );
        memcpy( terrain->render_vertex,    terrain->render_vertex    + ( vertexPerTerrain * ( terrain->terrain_count - 1 ) ), sizeof( VERTEX1 ) * vertexPerTerrain );
    }
    
    vec3 atPos = terrain->position_start;
    
    flo32 prev_width = 2.0f;
    flo32 prev_depth = 2.0f;
    
    flo32 dir_minRadians = -( PI * 0.25f );
    flo32 dir_maxRadians =  ( PI * 0.25f );
    // CONSOLE_STRING( "NEW FRAME" );
    for( uint32 terrain_index = 1; terrain_index < terrain->terrain_count; terrain_index++ ) {
        flo32 direction = ( Random4() * ( dir_maxRadians - dir_minRadians ) ) + dir_minRadians;
        
        flo32 offset_distance = ( Random4() * 3.5f ) + 2.0f;
        flo32 terrain_width   = ( Random4() * 1.5f ) + 0.5f;
        flo32 terrain_depth   = ( Random4() * 1.5f ) + 0.5f;
        
        flo32 maxOffsetY = lerp(  2.0f, ( offset_distance - 2.0f ) / 3.5f, 0.0f );
        flo32 minOffsetY = -2.0f;
        flo32 offsetY = ( Random4() * ( maxOffsetY - minOffsetY ) ) + minOffsetY;
        
        // CONSOLE_VALUE( uint32, terrain_index );
        // CONSOLE_VALUE( flo32, offset_distance );
        // CONSOLE_VALUE( flo32, maxOffsetY );
        // CONSOLE_VALUE( flo32, minOffsetY );
        // CONSOLE_VALUE( flo32, offsetY );
        
        vec3 offset   = Vec3( cosf( direction ), 0.0f, -sinf( direction ) ) * ( offset_distance + terrain_width + prev_width );
        offset.y = offsetY;
        prev_width = terrain_width;
        
        atPos += offset;
        
        vec3 xAxis = getNormal( Vec3( offset.x, 0.0f, offset.z ) ) * terrain_width;
        vec3 zAxis = getNormal( cross( xAxis, Vec3( 0.0f, 1.0f, 0.0f ) ) ) * terrain_depth;
        
        vec3 yAxis = Vec3( 0.0f, -10.0f, 0.0f );
        
        vec3 point[ 8 ] = {};
        point[ 0 ] = atPos - xAxis + zAxis;
        point[ 1 ] = atPos + xAxis + zAxis;
        point[ 2 ] = atPos - xAxis - zAxis;
        point[ 3 ] = atPos + xAxis - zAxis;
        point[ 4 ] = point[ 0 ] + yAxis;
        point[ 5 ] = point[ 1 ] + yAxis;
        point[ 6 ] = point[ 2 ] + yAxis;
        point[ 7 ] = point[ 3 ] + yAxis;
        
        vec2 tex[ 4 ] = {};
        tex[ 0 ] = Vec2( 0.0f, 0.0f );
        tex[ 1 ] = Vec2( 1.0f, 0.0f );
        tex[ 2 ] = Vec2( 0.0f, 1.0f );
        tex[ 3 ] = Vec2( 1.0f, 1.0f );
        
        vec3 normal[ 5 ] = {};
        normal[ 0 ] = getNormal( cross( point[ 1 ] - point[ 0 ], point[ 2 ] - point[ 0 ] ) );
        normal[ 1 ] = getNormal( cross( point[ 4 ] - point[ 6 ], point[ 2 ] - point[ 6 ] ) );
        normal[ 2 ] = getNormal( cross( point[ 5 ] - point[ 4 ], point[ 0 ] - point[ 4 ] ) );
        normal[ 3 ] = getNormal( cross( point[ 7 ] - point[ 5 ], point[ 1 ] - point[ 5 ] ) );
        normal[ 4 ] = getNormal( cross( point[ 6 ] - point[ 7 ], point[ 3 ] - point[ 7 ] ) );
        
        uint32 collision_index = 0;
        vec3    * collision_vertex = terrain->collision_vertex + ( terrain_index * vertexPerTerrain );
        collision_vertex[ collision_index++ ] = point[ 0 ];
        collision_vertex[ collision_index++ ] = point[ 1 ];
        collision_vertex[ collision_index++ ] = point[ 2 ];
        collision_vertex[ collision_index++ ] = point[ 3 ];
        collision_vertex[ collision_index++ ] = point[ 2 ];
        collision_vertex[ collision_index++ ] = point[ 1 ];
        
        collision_vertex[ collision_index++ ] = point[ 6 ];
        collision_vertex[ collision_index++ ] = point[ 4 ];
        collision_vertex[ collision_index++ ] = point[ 2 ];
        collision_vertex[ collision_index++ ] = point[ 0 ];
        collision_vertex[ collision_index++ ] = point[ 2 ];
        collision_vertex[ collision_index++ ] = point[ 4 ];
        
        collision_vertex[ collision_index++ ] = point[ 4 ];
        collision_vertex[ collision_index++ ] = point[ 5 ];
        collision_vertex[ collision_index++ ] = point[ 0 ];
        collision_vertex[ collision_index++ ] = point[ 1 ];
        collision_vertex[ collision_index++ ] = point[ 0 ];
        collision_vertex[ collision_index++ ] = point[ 5 ];
        
        collision_vertex[ collision_index++ ] = point[ 5 ];
        collision_vertex[ collision_index++ ] = point[ 7 ];
        collision_vertex[ collision_index++ ] = point[ 1 ];
        collision_vertex[ collision_index++ ] = point[ 3 ];
        collision_vertex[ collision_index++ ] = point[ 1 ];
        collision_vertex[ collision_index++ ] = point[ 7 ];
        
        collision_vertex[ collision_index++ ] = point[ 7 ];
        collision_vertex[ collision_index++ ] = point[ 6 ];
        collision_vertex[ collision_index++ ] = point[ 3 ];
        collision_vertex[ collision_index++ ] = point[ 2 ];
        collision_vertex[ collision_index++ ] = point[ 3 ];
        collision_vertex[ collision_index++ ] = point[ 6 ];
        
        VERTEX1 * render_vertex = terrain->render_vertex + ( terrain_index * vertexPerTerrain );
        uint32 render_index = 0;
        render_vertex[ render_index++ ] = Vertex1( point[ 0 ], normal[ 0 ], tex[ 0 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 1 ], normal[ 0 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 2 ], normal[ 0 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 3 ], normal[ 0 ], tex[ 3 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 2 ], normal[ 0 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 1 ], normal[ 0 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 6 ], normal[ 1 ], tex[ 0 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 4 ], normal[ 1 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 2 ], normal[ 1 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 0 ], normal[ 1 ], tex[ 3 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 2 ], normal[ 1 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 4 ], normal[ 1 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 4 ], normal[ 2 ], tex[ 0 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 5 ], normal[ 2 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 0 ], normal[ 2 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 1 ], normal[ 2 ], tex[ 3 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 0 ], normal[ 2 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 5 ], normal[ 2 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 5 ], normal[ 3 ], tex[ 0 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 7 ], normal[ 3 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 1 ], normal[ 3 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 3 ], normal[ 3 ], tex[ 3 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 1 ], normal[ 3 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 7 ], normal[ 3 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 7 ], normal[ 4 ], tex[ 0 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 6 ], normal[ 4 ], tex[ 1 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 3 ], normal[ 4 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 2 ], normal[ 4 ], tex[ 3 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 3 ], normal[ 4 ], tex[ 2 ] );
        render_vertex[ render_index++ ] = Vertex1( point[ 6 ], normal[ 4 ], tex[ 1 ] );
    }
    terrain->position_end = atPos;
}

internal void
resetLevel( APP_STATE * appState, MEMORY * permMemory, MEMORY * tempMemory ) {
    COLLISION_SYSTEM * collisionSystem = &appState->collisionSystem;
    collisionSystem->tri_count  = 0;
    collisionSystem->edge_count = 0;
    collisionSystem->overhang_count = 0;
    collisionSystem->ledge_count = 0;
    collisionSystem->pole_count = 0;
    
    for( int32 renderData_index = 0; renderData_index < appState->renderData_count; renderData_index++ ) {
        RENDER_DATA renderData = appState->renderData[ renderData_index ];
        _popArray( permMemory, VERTEX1, renderData.vertex_count );
    }
    appState->renderData_count  = 0;
    
#if ENABLE_TERRAIN_GENERATION
    TERRAIN_GENERATION * terrain = &appState->terrain_generation;
    terrain->terrain_count      = TERRAIN_GENERATION__TERRAIN_COUNT;
    terrain->vertex_count       = ( terrain->terrain_count * TERRAIN_GENERATION__VERTEX_PER_TERRAIN );
    terrain->render_vertex      = _pushArray_clear( permMemory, VERTEX1, terrain->vertex_count );
    terrain->collision_vertex   = collisionSystem->tri_vertex;
    collisionSystem->tri_count += terrain->vertex_count;
    
    terrain->position_start = Vec3( 0.0f, 0.0f, 0.0f );
    terrain->position_end   = Vec3( 0.0f, 0.0f, 0.0f );
    genTerrain( terrain );
    
    RENDER_DATA renderData = {};
    renderData.vertex       = terrain->render_vertex;
    renderData.vertex_count = terrain->vertex_count;
    renderData.modColor     = COLOR_WHITE;
    appState->renderData[ appState->renderData_count++ ] = renderData;
#endif // ENABLE_TERRAIN_GENERATION
    
    appState->secureDoorList.nSecureDoor = 0;
    appState->pixieList.nPixie           = 0;
    appState->killZoneList.nKillZone     = 0;
    appState->checkpointList.nCheckpoint = 0;
    appState->viewList.nView             = 0;
    
    loadLevel( appState, permMemory, tempMemory );
    
    uint32 breakHere = 10;
}

internal vec3
getClosestIntersection( COLLISION_SYSTEM * collisionSystem, vec3 ray_origin, vec3 ray_vector ) {
    boo32 tMin_isValid = false;
    flo32 tMin         = 1000000.0f;
    
    vec3 debug_vertex[ 3 ] = {};
    
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        flo32 t0 = dot( ray_vector, normal );
        if( t0 < -0.000001f ) {
            flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
            if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
            
            if( ( t >= 0.0f ) && ( t < tMin ) ) {
                vec3 point = ray_origin + ray_vector * t;
                vec3 normal0 = getNormal( cross( normal, vertex[ 1 ] - vertex[ 0 ] ) );
                vec3 normal1 = getNormal( cross( normal, vertex[ 2 ] - vertex[ 1 ] ) );
                vec3 normal2 = getNormal( cross( normal, vertex[ 0 ] - vertex[ 2 ] ) );
                
                flo32 dot0 = dot( point - ( vertex[ 0 ] - normal0 * 0.01f ), normal0 );
                flo32 dot1 = dot( point - ( vertex[ 1 ] - normal1 * 0.01f ), normal1 );
                flo32 dot2 = dot( point - ( vertex[ 2 ] - normal2 * 0.01f ), normal2 );
                
                boo32 isValid = ( ( dot0 >= 0.0f ) && ( dot1 >= 0.0f ) && ( dot2 >= 0.0f ) );
                if( isValid ) {
                    tMin_isValid = true;
                    tMin         = t;
                    
                    memcpy( debug_vertex, vertex, sizeof( vec3 ) * 3 );
                }
            }
        }
    }
    
    debugTriOutline( debug_vertex[ 0 ], debug_vertex[ 1 ], debug_vertex[ 2 ], COLOR_CYAN );
    // DISPLAY_VALUE( vec3, debug_vertex[ 0 ] );
    // DISPLAY_VALUE( vec3, debug_vertex[ 1 ] );
    // DISPLAY_VALUE( vec3, debug_vertex[ 2 ] );
    
    vec3 result = ray_origin;
    if( tMin_isValid ) {
        result = ray_origin + ray_vector * tMin;
    }
    return result;
}

internal COLLISION_TRIANGLE
getDoorCollision( SECURE_DOOR_LIST * s, vec3 pos, vec3 dPos ) {
    COLLISION_TRIANGLE result = {};
    flo32 tMin   = 1000000.0f;
    
    for( uint32 iDoor = 0; iDoor < s->nSecureDoor; iDoor++ ) {
        SECURE_DOOR door = s->secureDoor[ iDoor ];
        if( door.collision_isActive ) {
            vec3 ray_origin = pos;
            vec3 ray_vector = dPos;
            vec3 normal     = door.collision_bound.zAxis;
            
            flo32 t0 = dot( ray_vector, normal );
            if( ( t0 < -0.000001f ) || ( t0 > 0.000001f ) ) {
                if( t0 > 0.000001f ) {
                    normal = -normal;
                    t0 = -t0;
                }
                
                vec3 Q = door.collision_bound.center + normal * door.collision_bound.halfDim.z;
                flo32 t = -( dot( ray_origin, normal ) + dot( -Q, normal ) ) / t0;
                if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
                
                if( ( t >= 0.0f ) && ( t < tMin ) ) {
                    vec3 P = ray_origin + ray_vector * t;
                    vec3 V = P - door.collision_bound.center;
                    
                    flo32 dotX = fabsf( dot( V, door.collision_bound.xAxis ) );
                    flo32 dotY = fabsf( dot( V, door.collision_bound.yAxis ) );
                    boo32 isValid = ( dotX <= door.collision_bound.halfDim.x ) && ( dotY <= door.collision_bound.halfDim.y );
                    
                    if( isValid ) {
                        tMin = t;
                        result.isValid          = true;
                        result.collision_point  = P;
                        result.collision_normal = normal;
                        result.collision_t      = t;
                        result.collision_dist   = getLength( ray_vector ) * t;
                        result.isWalkable       = false;
                    }
                }
            }
        }
    }
    return result;
}

internal void
updateCollision( APP_STATE * appState, COLLISION_SYSTEM * collisionSystem, PLAYER_STATE * playerState, flo32 dt ) {
    PROFILE_FUNCTION();
    boo32 hadFloorCollision   = false;
    boo32 hadWallCollision    = false;
    
    flo32 dtLeft = dt;
    
    vec3 dPos0    = playerState->velocity * dt;
    vec3 vel_hor0 = dPos0 - dot( dPos0, Vec3( 0.0f, 1.0f, 0.0f ) ) * Vec3( 0.0f, 1.0f, 0.0f );
    vec2 vel_hor  = Vec2( vel_hor0.x, vel_hor0.z );
    flo32 hor_distSq = getLengthSq( vel_hor );
    if( hor_distSq < 0.00000001f ) {
        playerState->velocity.x = 0.0f;
        playerState->velocity.z = 0.0f;
    }
    
    for( int32 iter = 0; ( dtLeft > 0.0f ) && ( iter < 4 ); iter++ ) {
        flo32 dtUsed = dtLeft;
        
        vec3  pos = playerState->position;
        vec3 dPos = playerState->velocity * dtUsed;
        if( getLengthSq( dPos ) > 0.00000001f ) {
            COLLISION_TRIANGLE collision_triangle  = getFirstTriangle( collisionSystem, pos, dPos );
            COLLISION_TRIANGLE collision_gen       = getDoorCollision( &appState->secureDoorList, pos, dPos );
            if( ( collision_gen.isValid ) && ( ( !collision_triangle.isValid ) || ( collision_triangle.collision_t > collision_gen.collision_t ) ) ) {
                collision_triangle = collision_gen;
            }
            COLLISION_TRIANGLE collision_secondary = getSecondaryTriangle( collisionSystem, pos, dPos, playerState->height );
            COLLISION_TRIANGLE collision_tertiary  = getTertiaryTriangle( collisionSystem, pos + Vec3( 0.0f, playerState->height, 0.0f ), dPos );
            
            if( ( collision_secondary.isValid ) && ( ( !collision_triangle.isValid ) || ( collision_triangle.collision_t > collision_secondary.collision_t ) ) ) {
                collision_triangle = collision_secondary;
            }
            if( ( collision_tertiary.isValid ) && ( ( !collision_triangle.isValid ) || ( collision_triangle.collision_t > collision_tertiary.collision_t ) ) ) {
                collision_triangle = collision_tertiary;
                collision_triangle.collision_point += Vec3( 0.0f, -playerState->height, 0.0f );
            }
            if( ( collision_triangle.isValid ) && ( collision_triangle.collision_t < 1.0f ) ) {
                dtUsed *= collision_triangle.collision_t;
                
                flo32 offset_dist     = minValue( collision_triangle.collision_dist, COLLISION__OFFSET_FROM_COLLISION_PLANE );
                vec3  collision_point = collision_triangle.collision_point + collision_triangle.collision_normal * offset_dist;
                
                playerState->position  = collision_point;
                playerState->velocity -= dot( collision_triangle.collision_normal, playerState->velocity ) * collision_triangle.collision_normal;
                
                if( collision_triangle.isWalkable ) {
                    hadFloorCollision = true;
                }
                
                if( !collision_triangle.isWalkable ) {
                    hadWallCollision = true;
                    playerState->isSliding_normal = collision_triangle.collision_normal;
                }
            } else {
                playerState->position += dPos;
            }
        }
        dtLeft -= dtUsed;
    }
    
    if( ( hadFloorCollision ) && ( hadWallCollision ) ) {
        playerState->isWalking = true;
        playerState->isSliding = false;
    } else if( hadFloorCollision ) {
        playerState->isWalking = true;
    } else if( ( hadWallCollision ) && ( !playerState->isWalking ) ) {
        if( !isOrthogonalTo( playerState->isSliding_normal, Vec3( 0.0f, 1.0f, 0.0f ) ) ) {
            playerState->isSliding        = true;
        }
    }
}

internal DEBUG_COLLISION_QUERY
getCollisionQuery( uint32 tri_index, COLLISION_SYSTEM * collisionSystem, vec3 ray_origin, vec3 ray_vector ) {
    DEBUG_COLLISION_QUERY result = {};
    flo32 tMin   = 1000000.0f;
    
    vec3 vertex[ 3 ] = {};
    memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
    vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
    
    // result.vertex[ 0 ] = vertex[ 0 ];
    // result.vertex[ 1 ] = vertex[ 1 ];
    // result.vertex[ 2 ] = vertex[ 2 ];
    // result.normal      = normal;
    
    flo32 t0 = dot( ray_vector, normal );
    result.t0 = t0;
    if( t0 < -0.000001f ) {
        flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
        result.t_ = t;
        result.t_length = t * getLength( ray_vector );
        if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
        result.t = t;
        
        if( ( t >= 0.0f ) && ( t < tMin ) ) {
            vec3 P = ray_origin + ray_vector * t;
            vec3 A = vertex[ 0 ];
            vec3 B = vertex[ 1 ];
            vec3 C = vertex[ 2 ];
            
            vec3 X = B - A;
            vec3 Y = C - A;
            vec3 Z = P - A;
            
            flo32 dotXX = dot( X, X );
            flo32 dotXY = dot( X, Y );
            flo32 dotXZ = dot( X, Z );
            flo32 dotYY = dot( Y, Y );
            flo32 dotYZ = dot( Y, Z );
            
            flo32 detA  = dotXX * dotYY - dotXY * dotXY;
            flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
            flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
            
            flo32 w = detCw / detA;
            flo32 v = detCv / detA;
            flo32 u = 1.0f - v - w;
            result.u = u;
            result.v = v;
            result.w = w;
            
            flo32 dist_u = dist_pointToLine( P, B, C );
            flo32 dist_v = dist_pointToLine( P, C, A );
            flo32 dist_w = dist_pointToLine( P, A, B );
            
            result.dist_u = dist_u;
            result.dist_v = dist_v;
            result.dist_w = dist_w;
            // result.point = P;
            // flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
            
            // if( isValid ) {
            // tMin = t;
            // result.isValid          = true;
            // result.vertex           = collisionSystem->tri_vertex + print_index * 3;
            // result.collision_point  = P;
            // result.collision_normal = normal;
            // result.collision_t      = t;
            // result.collision_dist   = getLength( ray_vector ) * t;
            
            // flo32 radians = acosf( dot( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) );
            // flo32 degrees = radians / ( 2.0f * PI ) * 360.0f;
            // // DISPLAY_VALUE( flo32, degrees );
            // if( degrees <= 25.0f ) {
            // result.isWalkable = true;
            // }
            // }
        }
    }
    return result;
}

internal COLLISION_TRIANGLE
debugClosestTriangle( COLLISION_SYSTEM * collisionSystem, vec3 ray_origin, vec3 ray_vector ) {
    PROFILE_FUNCTION();
    
    COLLISION_TRIANGLE result = {};
    flo32 tMin   = 1000000.0f;
    
    for( int32 tri_index = 0; tri_index < collisionSystem->tri_count; tri_index++ ) {
        vec3 vertex[ 3 ] = {};
        memcpy( vertex, collisionSystem->tri_vertex + tri_index * 3, sizeof( vec3 ) * 3 );
        vec3 normal      = getNormal( cross( vertex[ 1 ] - vertex[ 0 ], vertex[ 2 ] - vertex[ 0 ] ) );
        
        flo32 t0 = dot( ray_vector, normal );
        if( t0 < -0.000001f ) {
            flo32 t = -( dot( ray_origin, normal ) + dot( -vertex[ 0 ], normal ) ) / t0;
            if( ( ( t * getLength( ray_vector ) ) >= -0.01f ) && ( t < 0.0f ) ) { t = 0.0f; }
            
            if( ( t >= 0.0f ) && ( t < tMin ) ) {
                vec3 P = ray_origin + ray_vector * t;
                vec3 A = vertex[ 0 ];
                vec3 B = vertex[ 1 ];
                vec3 C = vertex[ 2 ];
                
                vec3 X = B - A;
                vec3 Y = C - A;
                vec3 Z = P - A;
                
                flo32 dotXX = dot( X, X );
                flo32 dotXY = dot( X, Y );
                flo32 dotXZ = dot( X, Z );
                flo32 dotYY = dot( Y, Y );
                flo32 dotYZ = dot( Y, Z );
                
                flo32 detA  = dotXX * dotYY - dotXY * dotXY;
                flo32 detCv = dotXZ * dotYY - dotXY * dotYZ;
                flo32 detCw = dotXX * dotYZ - dotXZ * dotXY;
                
                flo32 w = detCw / detA;
                flo32 v = detCv / detA;
                flo32 u = 1.0f - v - w;
                flo32 isValid = ( ( u >= 0.0f ) && ( u <= 1.0f ) && ( v >= 0.0f ) && ( v <= 1.0f ) && ( w >= 0.0f ) && ( w <= 1.0f ) );
                
                if( !isValid ) {
                    flo32 dist_u = dist_pointToLine( P, B, C );
                    flo32 dist_v = dist_pointToLine( P, C, A );
                    flo32 dist_w = dist_pointToLine( P, A, B );
                    
                    if( ( dist_u <= 0.0005 ) || ( dist_v <= 0.0005 ) || ( dist_w <= 0.0005 ) ) {
                        isValid = true;
                    }
                }
                
                if( isValid ) {
                    tMin = t;
                    result.index            = tri_index;
                    result.isValid          = true;
                    result.vertex           = collisionSystem->tri_vertex + tri_index * 3;
                    result.collision_point  = P;
                    result.collision_normal = normal;
                    result.collision_t      = t;
                    result.collision_dist   = getLength( ray_vector ) * t;
                    
                    flo32 radians = acosf( dot( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) );
                    flo32 degrees = radians / ( 2.0f * PI ) * 360.0f;
                    // // DISPLAY_VALUE( flo32, degrees );
                    if( degrees <= 25.0f ) {
                        result.isWalkable = true;
                    }
                }
            }
        }
    }
    return result;
}

#define DEBUG_COLLISION__OUTPUT__NAME_WIDTH    ( 14 )
#define DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH  ( 56 )
#define DEBUG_COLLISION__QUERY__NAME_WIDTH     ( 10 )
#define DEBUG_COLLISION__QUERY__COLUMN_WIDTH   ( 28 )

internal void
debugCollision_output_vec3( MEMORY * output_memory, vec3 value, const char * name ) {
    writeSegment( output_memory, "\n" );
    uint32 start = ( uint32 )output_memory->used;
    writeSegment( output_memory, "  " );
    writeSegment( output_memory, name );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__OUTPUT__NAME_WIDTH );
    uint32 space_count = DEBUG_COLLISION__OUTPUT__NAME_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
    writeSegment( output_memory, ": " );
    writeV3  ( output_memory, value );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH );
    space_count = DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
}

internal void
debugCollision_output_flo32( MEMORY * output_memory, flo32 value, const char * name ) {
    writeSegment( output_memory, "\n" );
    uint32 start = ( uint32 )output_memory->used;
    writeSegment( output_memory, "  " );
    writeSegment( output_memory, name );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__OUTPUT__NAME_WIDTH );
    uint32 space_count = DEBUG_COLLISION__OUTPUT__NAME_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
    writeSegment( output_memory, ": " );
    writeF32  ( output_memory, value );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH );
    space_count = DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
}

internal void
debugCollision_output_boo32( MEMORY * output_memory, boo32 value, const char * name ) {
    writeSegment( output_memory, "\n" );
    uint32 start = ( uint32 )output_memory->used;
    writeSegment( output_memory, "  " );
    writeSegment( output_memory, name );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__OUTPUT__NAME_WIDTH );
    uint32 space_count = DEBUG_COLLISION__OUTPUT__NAME_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
    writeSegment( output_memory, ": " );
    write_boo32  ( output_memory, value );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH );
    space_count = DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
}

internal void
debugCollision_query_flo32( MEMORY * output_memory, flo32 value, const char * name ) {
    uint32 start = ( uint32 )output_memory->used;
    writeSegment( output_memory, name );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__QUERY__NAME_WIDTH );
    uint32 space_count = DEBUG_COLLISION__QUERY__NAME_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
    writeSegment( output_memory, ": " );
    writeF32 ( output_memory, value );
    Assert( ( ( uint32 )output_memory->used - start ) <= DEBUG_COLLISION__QUERY__COLUMN_WIDTH );
    space_count = DEBUG_COLLISION__QUERY__COLUMN_WIDTH - ( ( uint32 )output_memory->used - start );
    for( uint32 iter = 0; iter < space_count; iter++ ) { writeSegment( output_memory, " " ); }
}

internal void
outputDebugCollision( MEMORY * tempMemory, DEBUG_COLLISION_SYSTEM * debugCollisionSystem ) {
    uint32   output_memory_size = _MB( 6 );
    MEMORY  _output_memory = subArena( tempMemory, output_memory_size );
    MEMORY * output_memory = &_output_memory;
    
    uint32 query_count = 4;
    
    uint32 prevIter = 5;
    for( uint32 data_index = 0; data_index < DEBUG_COLLISION_SYSTEM__DATA_COUNT; data_index++ ) {
        uint32 index = ( ( debugCollisionSystem->index + data_index ) % DEBUG_COLLISION_SYSTEM__DATA_COUNT );
        DEBUG_COLLISION_DATA * data = debugCollisionSystem->data + index;
        
        if( data->iter != ( prevIter + 1 ) ) {
            writeSegment( output_memory, "\n\n***----------*** NEW FRAME ***----------***" );
        }
        
        writeSegment( output_memory, "\niter: " );
        writeS32    ( output_memory, data->iter );
        writeSegment( output_memory, "\npos : " );
        writeV3  ( output_memory, data->pos );
        writeSegment( output_memory, "\ndPos: " );
        writeV3  ( output_memory, data->dPos );
        writeSegment( output_memory, data->isWalking ? "\nisWalking: YES" : "\nisWalking: NO" );
        
        if( data->isValid ) {
            writeSegment( output_memory, "\n  tri_index : " );
            writeU32( output_memory, data->tri_index );
#define _output( type, name )  { debugCollision_output_##type( output_memory, data->name, #name ); }
#define _query(  type, name )  { for( uint32 query_index = 0; query_index < query_count; query_index++ ) {\
DEBUG_COLLISION_QUERY * query = debugCollisionSystem->query[ query_index ] + index;\
debugCollision_query_##type( output_memory, query->name, #name ); } }
            
            _output( vec3,  vertex[ 0 ] );   _query( flo32, t0 );
            _output( vec3,  vertex[ 1 ] );   _query( flo32, t_ );
            _output( vec3,  vertex[ 2 ] );   _query( flo32, t_length );
            _output( flo32, t           );   _query( flo32, t );
            _output( flo32, dist        );   _query( flo32, u );
            _output( vec3,  point       );   _query( flo32, v );
            _output( vec3,  normal      );   _query( flo32, w );
            _output( boo32, isWalkable  );   _query( flo32, dist_u );
            writeSegment( output_memory, "\n" ); for( uint32 iter = 0; iter < DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH; iter++ ) { writeSegment( output_memory, " " ); }   _query( flo32, dist_u );
            writeSegment( output_memory, "\n" ); for( uint32 iter = 0; iter < DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH; iter++ ) { writeSegment( output_memory, " " ); }   _query( flo32, dist_v );
            writeSegment( output_memory, "\n" ); for( uint32 iter = 0; iter < DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH; iter++ ) { writeSegment( output_memory, " " ); }   _query( flo32, dist_w );
        } else {
            writeSegment( output_memory, "\n  tri_index   : ----------                              " );
            writeSegment( output_memory, "\n  vertex[ 0 ] : ----------                              " );   _query( flo32, t0 );
            writeSegment( output_memory, "\n  vertex[ 1 ] : ----------                              " );   _query( flo32, t_ );
            writeSegment( output_memory, "\n  vertex[ 2 ] : ----------                              " );   _query( flo32, t_length );
            writeSegment( output_memory, "\n  t           : ----------                              " );   _query( flo32, t );
            writeSegment( output_memory, "\n  dist        : ----------                              " );   _query( flo32, u );
            writeSegment( output_memory, "\n  point       : ----------                              " );   _query( flo32, v );
            writeSegment( output_memory, "\n  normal      : ----------                              " );   _query( flo32, w );
            writeSegment( output_memory, "\n" ); for( uint32 iter = 0; iter < DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH; iter++ ) { writeSegment( output_memory, " " ); }   _query( flo32, dist_u );
            writeSegment( output_memory, "\n" ); for( uint32 iter = 0; iter < DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH; iter++ ) { writeSegment( output_memory, " " ); }   _query( flo32, dist_v );
            writeSegment( output_memory, "\n" ); for( uint32 iter = 0; iter < DEBUG_COLLISION__OUTPUT__COLUMN_WIDTH; iter++ ) { writeSegment( output_memory, " " ); }   _query( flo32, dist_w );
        }
        
        if( ( data->isValid ) && ( data->t < 1.0f ) ) {
            writeSegment( output_memory, "\nHAD COLLISION\n" );
        } else {
            writeSegment( output_memory, "\nNO COLLISION\n" );
        }
        
        prevIter = data->iter;
    }
    
    writeFile( "collision_log.txt", output_memory->base, ( uint32 )output_memory->used );
    _popSize( tempMemory, output_memory_size );
}

internal void
add( DEBUG_COLLISION_SYSTEM * debugCollisionSystem, uint32 iter, vec3 pos, vec3 dPos, boo32 isWalking, COLLISION_TRIANGLE collision_triangle ) {
    DEBUG_COLLISION_DATA * data = debugCollisionSystem->data + debugCollisionSystem->index;
    
    data->iter      = iter;
    data->isWalking = isWalking;
    data->pos       = pos;
    data->dPos      = dPos;
    data->tri_index = collision_triangle.index;
    data->isValid   = collision_triangle.isValid;
    if( collision_triangle.isValid ) {
        data->vertex[ 0 ] = collision_triangle.vertex[ 0 ];
        data->vertex[ 1 ] = collision_triangle.vertex[ 1 ];
        data->vertex[ 2 ] = collision_triangle.vertex[ 2 ];
    }
    data->t       = collision_triangle.collision_t;
    data->dist    = collision_triangle.collision_dist;
    data->point   = collision_triangle.collision_point;
    data->normal  = collision_triangle.collision_normal;
    data->isWalkable = collision_triangle.isWalkable;
}

internal void
add( DEBUG_COLLISION_SYSTEM * debugCollisionSystem, uint32 query_index, DEBUG_COLLISION_QUERY query ) {
    Assert( query_index < DEBUG_COLLISION_SYSTEM__QUERY_COUNT );
    debugCollisionSystem->query[ query_index ][ debugCollisionSystem->index ] = query;
}

internal vec3
debugCollision( DEBUG_COLLISION_SYSTEM * debugCollisionSystem, COLLISION_SYSTEM * collisionSystem, PLAYER_STATE playerState, flo32 dt ) {
    flo32 dtLeft = dt;
    for( int32 iter = 0; ( dtLeft > 0.0f ) && ( iter < 4 ); iter++ ) {
        flo32 dtUsed = dtLeft;
        
        if( ( playerState.isWalking ) && ( playerState.velocity.y < 0.0f ) ) {
            playerState.velocity.y = 0.0f;
        }
        
        vec3  pos = playerState.position;
        if( playerState.isWalking ) { pos.y += 0.0001f; }
        vec3 dPos = playerState.velocity * dtUsed;
        
        COLLISION_TRIANGLE collision_triangle = {};
        if( getLengthSq( dPos ) > 0.00000001f ) {
            collision_triangle = debugClosestTriangle( collisionSystem, pos, dPos );
            if( ( collision_triangle.isValid ) && ( collision_triangle.collision_t < 1.0f ) ) {
                dtUsed *= collision_triangle.collision_t;
                playerState.position  = collision_triangle.collision_point;
                playerState.velocity -= dot( collision_triangle.collision_normal, playerState.velocity ) * collision_triangle.collision_normal;
                playerState.isWalking = collision_triangle.isWalkable;
            } else {
                playerState.position += dPos;
            }
        }
        dtLeft -= dtUsed;
        
        add( debugCollisionSystem, iter, pos, dPos, playerState.isWalking, collision_triangle );
        add( debugCollisionSystem, 0, getCollisionQuery( 20,  collisionSystem, pos, dPos ) );
        add( debugCollisionSystem, 1, getCollisionQuery( 38, collisionSystem, pos, dPos ) );
        add( debugCollisionSystem, 2, getCollisionQuery( 23,  collisionSystem, pos, dPos ) );
        add( debugCollisionSystem, 3, getCollisionQuery( 41, collisionSystem, pos, dPos ) );
        debugCollisionSystem->index = ( ( debugCollisionSystem->index + 1 ) % DEBUG_COLLISION_SYSTEM__DATA_COUNT );
    }
    
    vec3 result = playerState.position;
    return result;
}

internal void
debug_drawLedges( RENDERER * renderer, COLLISION_SYSTEM * collisionSystem ) {
    PROFILE_FUNCTION();
    for( uint32 ledge_index = 0; ledge_index < collisionSystem->ledge_count; ledge_index++ ) {
        COLLISION_LEDGE ledge = collisionSystem->ledge[ ledge_index ];
        
        vec3 A = ledge.point0;
        vec3 B = ledge.point1;
        
        vec3 X = B - A;
        vec3 Y = Vec3( 0.0f, -COLLISION__LEDGE_HEIGHT, 0.0f );
        vec3 Z = getNormal( cross( X, Y ) ) * COLLISION__LEDGE_DEPTH;
        
        vec3 C = A + Z;
        vec3 D = B + Z;
        
        drawLine( renderer, renderPassID_main, A, C, COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, B, D, COLOR_MAGENTA );
        
        drawLine( renderer, renderPassID_main, A + Y, C + Y, COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, B + Y, D + Y, COLOR_MAGENTA );
        
        drawLine( renderer, renderPassID_main, C, C + Y, COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, D, D + Y, COLOR_MAGENTA );
        
        drawLine( renderer, renderPassID_main, C,     D,     COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, C + Y, D + Y, COLOR_MAGENTA );
    }
}

internal void
debug_drawPoles( RENDERER * renderer, COLLISION_SYSTEM * collisionSystem, boo32 poleGrab_isActive ) {
    PROFILE_FUNCTION();
    vec4 color = poleGrab_isActive ? COLOR_CYAN : COLOR_MAGENTA;
    for( uint32 pole_index = 0; pole_index < collisionSystem->pole_count; pole_index++ ) {
        COLLISION_POLE pole = collisionSystem->pole[ pole_index ];
        
        vec3 A = pole.point0;
        vec3 B = pole.point1;
        
        vec3 Y = B - A;
        vec3 Z = pole.normal * COLLISION__POLE_DEPTH;
        vec3 X = getNormal( cross( Y, Z ) ) * COLLISION__POLE_WIDTH;
        
        vec3 P[ 8 ] = {};
        P[ 0 ] = A - X * 0.5f;
        P[ 1 ] = P[ 0 ] + X;
        P[ 2 ] = P[ 0 ] + Y;
        P[ 3 ] = P[ 0 ] + Z;
        P[ 4 ] = P[ 1 ] + Y + Z;
        P[ 5 ] = P[ 4 ] - X;
        P[ 6 ] = P[ 4 ] - Y;
        P[ 7 ] = P[ 4 ] - Z;
        
        drawLine( renderer, renderPassID_main, P[ 0 ], P[ 3 ], color );
        drawLine( renderer, renderPassID_main, P[ 1 ], P[ 6 ], color );
        drawLine( renderer, renderPassID_main, P[ 2 ], P[ 5 ], color );
        drawLine( renderer, renderPassID_main, P[ 7 ], P[ 4 ], color );
        drawLine( renderer, renderPassID_main, P[ 3 ], P[ 6 ], color );
        drawLine( renderer, renderPassID_main, P[ 6 ], P[ 4 ], color );
        drawLine( renderer, renderPassID_main, P[ 4 ], P[ 5 ], color );
        drawLine( renderer, renderPassID_main, P[ 5 ], P[ 3 ], color );
    }
}

internal flo32
getInputSpeed( uint32 input_speed ) {
    flo32 result = 0.0f;
    switch( input_speed ) {
        case 0: {} break;
        case 1: { result = 0.1f; } break;
        case 2: { result = 0.3f; } break;
        case 3: { result = 0.5f; } break;
        case 4: { result = 1.0f; } break;
        default: { InvalidCodePath; } break;
    }
    return result;
}

internal void
drawSky( RENDERER * renderer ) {
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    
    flo32 min = -1000.0f;
    flo32 max =  1000.0f;
    vec3 point[] = {
        Vec3( min, min, min ),
        Vec3( min, min, max ),
        Vec3( min, max, min ),
        Vec3( min, max, max ),
        Vec3( max, min, min ),
        Vec3( max, min, max ),
        Vec3( max, max, min ),
        Vec3( max, max, max ),
    };
    
    addQuad( &vertexTask, point[ 1 ], point[ 0 ], point[ 3 ], point[ 2 ], Vec3( 1.0f, 0.0f, 0.0f ) );
    addQuad( &vertexTask, point[ 0 ], point[ 4 ], point[ 2 ], point[ 6 ], Vec3( 1.0f, 0.0f, 0.0f ) );
    addQuad( &vertexTask, point[ 4 ], point[ 5 ], point[ 6 ], point[ 7 ], Vec3( 1.0f, 0.0f, 0.0f ) );
    addQuad( &vertexTask, point[ 5 ], point[ 1 ], point[ 7 ], point[ 3 ], Vec3( 1.0f, 0.0f, 0.0f ) );
    addQuad( &vertexTask, point[ 2 ], point[ 6 ], point[ 3 ], point[ 7 ], Vec3( 1.0f, 0.0f, 0.0f ) );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, renderPassID_main_noLighting, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, Vec4( 0.2f, 0.8f, 0.9f, 1.0f ) ) );
}

internal boo32
isInBound( vec3 position, ORIENTED_BOX bound ) {
    vec3  V    = position - bound.center;
    flo32 dotX = fabsf( dot( V, bound.xAxis ) );
    flo32 dotY = fabsf( dot( V, bound.yAxis ) );
    flo32 dotZ = fabsf( dot( V, bound.zAxis ) );
    boo32 result = ( dotX <= bound.halfDim.x ) && ( dotY <= bound.halfDim.y ) && ( dotZ <= bound.halfDim.z );
    return result;
}

internal void
drawPixies( RENDERER * renderer, RENDER_PASS_ID passID, PIXIE_LIST * pixieList ) {
    for( uint32 pixie_index = 0; pixie_index < pixieList->nPixie; pixie_index++ ) {
        PIXIE pixie = pixieList->pixie[ pixie_index ];
        drawPixie( renderer, passID, pixie );
    }
}

internal vec3
setPlayerPosOnLedge( COLLISION_LEDGE ledge, flo32 t, flo32 height ) {
    ORIENTED_BOX bound = toOBox( ledge );
    
    vec3 result = ledge.point0;
    result += ( ledge.point1 - ledge.point0 ) * t;
    result += Vec3( 0.0f, -height, 0.0f );
    result += bound.zAxis * COLLISION__LEDGE_OFFSET_FROM_WALL;
    return result;
}

internal boo32
isPlayerInBound( vec3 pos, flo32 height, ORIENTED_BOX bound ) {
    boo32 result = isInBound( pos, bound ) || isInBound( pos + Vec3( 0.0f, height, 0.0f ), bound );
    return result;
}

internal void
drawOBox( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position, flo32 radius, vec4 color ) {
    ORIENTED_BOX b = {};
    b.center = position;
    b.xAxis  = Vec3( 1.0f, 0.0f, 0.0f );
    b.yAxis  = Vec3( 0.0f, 1.0f, 0.0f );
    b.zAxis  = Vec3( 0.0f, 0.0f, 1.0f );
    b.halfDim = Vec3( radius );
    
    drawBox( renderer, passID, b, color );
}

internal VIEW
getClosestView( VIEW_LIST * viewList, vec3 P ) {
    VIEW  result    = viewList->view[ 0 ];
    flo32 minDistSq = getLengthSq( result.P - P );
    for( uint32 iView = 1; iView < viewList->nView; iView++ ) {
        VIEW view = viewList->view[ iView ];
        
        flo32 t = getLengthSq( view.P - P );
        if( t < minDistSq ) {
            minDistSq = t;
            result = view;
        }
    }
    return result;
}

internal flo32
getCameraFOV( flo32 maxDist, flo32 sizeOnScreen ) {
    flo32 result = 2.0f * atanf( ( 0.5f * PLAYER__HEIGHT_STAND / sizeOnScreen ) / maxDist );
    return result;
}

internal VIEW
View( vec3 P, flo32 lo, flo32 hi, flo32 maxDist = 8.0f, flo32 hOffset = 0.0f, flo32 vOffset = 0.0f, flo32 fovAngle = ( PI * 0.3333f ) ) {
    VIEW result = {};
    result.type = viewType_point;
    result.P    = P;
    result.lo   = lo;
    result.hi   = hi;
    result.maxDist  = maxDist;
    result.hOffset  = hOffset;
    result.vOffset  = vOffset;
    result.fovAngle = fovAngle;
    return result;
}

internal VIEW
View( vec3 P, vec3 Q, flo32 lo, flo32 hi, flo32 maxDist = 8.0f, flo32 hOffset = 0.0f, flo32 vOffset = 0.0f, flo32 fovAngle = ( PI * 0.3333f ) ) {
    VIEW result = {};
    result.type = viewType_line;
    result.P    = P;
    result.Q    = Q;
    result.lo   = lo;
    result.hi   = hi;
    result.maxDist  = maxDist;
    result.hOffset  = hOffset;
    result.vOffset  = vOffset;
    result.fovAngle = fovAngle;
    return result;
}

internal vec3
getClosestPoint_point_lineSegment( vec3 C, vec3 A, vec3 B ) {
    vec3 AB = B - A;
    vec3 AC = C - A;
    
    flo32 t      = clamp01( dot( AC, AB ) / dot( AB, AB ) );
    // DISPLAY_VALUE( flo32, t );
    vec3  result = A + AB * t;
    return result;
}

struct SPRING_COEF {
    // P0 = a * P + b * V + dest
    // V0 = c * P + d * V;
    flo32 a;
    flo32 b;
    flo32 c;
    flo32 d;
};

internal SPRING_COEF
getSpringCoef( flo32 w, flo32 t ) {
    flo32 e = expf( -w * t );
    flo32 f = e * t;
    flo32 g = w * f;
    
    SPRING_COEF result = {};
    result.a = g + e;
    result.b = f;
    result.c = -w * g;
    result.d = -g + e;
    return result;
}