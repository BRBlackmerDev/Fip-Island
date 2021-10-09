

internal void
outputTexFile( MEMORY * tempMemory, uint32 entityID, TEX_FILE_DATA texFile_data ) {
    uint32 memory_size = sizeof( uint32 ) + ( sizeof( vec2 ) * texFile_data.texCoord_count );
    MEMORY  _output_memory = subArena( tempMemory, memory_size );
    MEMORY * output_memory = &_output_memory;
    
    _writem( output_memory, uint32, entityID );
    vec2 * texCoord_data = _pushArray( output_memory, vec2, texFile_data.texCoord_count );
    memcpy( texCoord_data, texFile_data.texCoord, ( sizeof( vec2 ) * texFile_data.texCoord_count ) );
    
    char filename[ 128 ] = {};
    sprintf( filename, "w:/Projects/camera/art/mesh/TEX_%04d.tex", entityID );
    writeFile( filename, output_memory->base, ( uint32 )output_memory->used );
    
    _popSize( tempMemory, memory_size );
}

internal TEX_FILE_DATA
readTexFile( MEMORY * permMemory, uint32 entityID ) {
    TEX_FILE_DATA result = {};
    
    char filename[ 128 ] = {};
    sprintf( filename, "w:/Projects/camera/art/mesh/TEX_%04d.tex", entityID );
    
    READ_FILE_RESULT tex_file = readFile( permMemory, filename );
    if( tex_file.contents ) {
        result.texCoord_count = ( ( uint32 * )tex_file.contents )[ 0 ];
        result.texCoord       = ( vec2 * )( ( uint32 * )tex_file.contents + 1 );
        result.texFileExists  = true;
    }
    
    return result;
}

struct COLLISION_FACE {
    uint32 index[ 3 ];
};

internal vec3
getNormal( X_FILE_DATA xFile_data, COLLISION_FACE face ) {
    Assert( face.index[ 0 ] < xFile_data.position_count );
    Assert( face.index[ 1 ] < xFile_data.position_count );
    Assert( face.index[ 2 ] < xFile_data.position_count );
    
    vec3 point0 = xFile_data.position[ face.index[ 0 ] ];
    vec3 point1 = xFile_data.position[ face.index[ 1 ] ];
    vec3 point2 = xFile_data.position[ face.index[ 2 ] ];
    
    vec3 result = getNormal( cross( point1 - point0, point2 - point0 ) );
    return result;
}

struct EDGE {
    uint32 index0;
    uint32 index1;
    uint32 texCoord_index0;
    uint32 texCoord_index1;
};

internal EDGE
Edge( uint32 index0, uint32 index1, uint32 texCoord_index0, uint32 texCoord_index1 ) {
    EDGE result = {};
    result.index0 = index0;
    result.index1 = index1;
    result.texCoord_index0 = texCoord_index0;
    result.texCoord_index1 = texCoord_index1;
    return result;
}

#define EDGE_LIST__EDGE_COUNT  ( 8192 )
struct EDGE_LIST {
    uint32 edge_count;
    EDGE   edge[ EDGE_LIST__EDGE_COUNT ];
};

internal void
add( EDGE_LIST * edgeList, EDGE edge ) {
    Assert( edgeList->edge_count < EDGE_LIST__EDGE_COUNT );
    edgeList->edge[ edgeList->edge_count++ ] = edge;
}

internal int32
findEdge( EDGE_LIST * edgeList, EDGE find_edge ) {
    int32 atIndex = -1;
    for( uint32 edge_index = 0; ( atIndex == -1 ) && ( edge_index < edgeList->edge_count ); edge_index++ ) {
        EDGE edge = edgeList->edge[ edge_index ];
        
        if( ( find_edge.index0 == edge.index0 ) && ( find_edge.index1 == edge.index1 ) ||
           ( find_edge.index0 == edge.index1 ) && ( find_edge.index1 == edge.index0 ) ) {
            atIndex = ( int32 )edge_index;
        }
    }
    return atIndex;
}

internal void
remove( EDGE_LIST * edgeList, EDGE remove_edge ) {
    int32 atIndex = findEdge( edgeList, remove_edge );
    Assert( atIndex != -1 );
    edgeList->edge[ atIndex ] = edgeList->edge[ --edgeList->edge_count ];
}

internal void
removeIfFound_elseAdd( EDGE_LIST * edgeList, EDGE target_edge ) {
    int32 atIndex = findEdge( edgeList, target_edge );
    if( atIndex == -1 ) {
        add( edgeList, target_edge );
    } else {
        edgeList->edge[ atIndex ] = edgeList->edge[ --edgeList->edge_count ];
    }
}

internal boo32
isShared( EDGE edge0, EDGE edge1 ) {
    boo32 result = ( edge0.index0 == edge1.index1 ) && ( edge0.index1 == edge1.index0 );
    return result;
}

#define QUEUE__MAX_COUNT  ( 16384 )
struct QUEUE{
    uint32 * queue;
    uint32 start_index;
    uint32 end_index;
    boo32  isEmpty;
};

internal QUEUE
Queue( MEMORY * memory, uint32 max ) {
    QUEUE result = {};
    result.queue   = _pushArray( memory, uint32, max );
    result.isEmpty = true;
    return result;
}

internal void
enqueue( QUEUE * queue, uint32 value ) {
    Assert( ( queue->start_index != queue->end_index ) || ( queue->isEmpty ) );
    queue->queue[ queue->end_index ] = value;
    queue->end_index = ( queue->end_index + 1 ) % QUEUE__MAX_COUNT;
    queue->isEmpty = false;
}

internal uint32
dequeue( QUEUE * queue ) {
    Assert( !queue->isEmpty );
    uint32 result = queue->queue[ queue->start_index ];
    queue->start_index = ( queue->start_index + 1 ) % QUEUE__MAX_COUNT;
    queue->isEmpty = ( queue->start_index == queue->end_index );
    return result;
}

internal void
unfoldTexCoords( MEMORY * permMemory, X_FILE_DATA xFile_data, TEX_FILE_DATA * texFile_data ) {
    uint32 texCoord_count = xFile_data.face_count * 3;
    vec2 * texCoord       = _pushArray( permMemory, vec2, texCoord_count );
    
    for( uint32 face_index = 0; face_index < xFile_data.face_count; face_index++ ) {
        int32 base_index = face_index * 3;
        int32 index0 = xFile_data.face[ base_index + 0 ];
        int32 index1 = xFile_data.face[ base_index + 1 ];
        int32 index2 = xFile_data.face[ base_index + 2 ];
        
        texCoord[ base_index + 0 ] = texFile_data->texCoord[ index0 ];
        texCoord[ base_index + 1 ] = texFile_data->texCoord[ index1 ];
        texCoord[ base_index + 2 ] = texFile_data->texCoord[ index2 ];
    }
    
    texFile_data->texCoord_count = texCoord_count;
    texFile_data->texCoord       = texCoord;
}

internal void
genTexCoords( MEMORY * permMemory, MEMORY * tempMemory, X_FILE_DATA xFile_data, TEX_FILE_DATA * texFile_data ) {
    uint32 texCoord_count = xFile_data.face_count * 3;
    vec2 * texCoord       = _pushArray( permMemory, vec2, texCoord_count );
    
    int32 * neighborList   = _pushArray      ( tempMemory, int32, xFile_data.face_count * 3 );
    boo32 * isPartOfIsland = _pushArray_clear( tempMemory, boo32, xFile_data.face_count     );
    for( uint32 neighbor_index = 0; neighbor_index < ( xFile_data.face_count * 3 ); neighbor_index++ ) { neighborList[ neighbor_index ] = -1; }
    for( uint32 texCoord_index = 0; texCoord_index < texCoord_count; texCoord_index++ ) { texCoord[ texCoord_index ] = Vec2( 0.5f, 0.5f ); }
    
    
    QUEUE  _face_queue = Queue( tempMemory, QUEUE__MAX_COUNT );
    QUEUE * face_queue = &_face_queue;
    
    EDGE_LIST  _edgeList = {};
    EDGE_LIST * edgeList = &_edgeList;
    
    COLLISION_FACE * face_array  = ( COLLISION_FACE * )xFile_data.face;
    for( uint32 face_index = 0; face_index < xFile_data.face_count; face_index++ ) {
        COLLISION_FACE face  = face_array[ face_index ];
        vec3 normal0         = getNormal( xFile_data, face );
        int32 * hasNeighbor0 = neighborList + ( face_index * 3 );
        
        EDGE edge0[ 3 ] = {
            Edge( face.index[ 0 ], face.index[ 1 ], face_index * 3 + 0, face_index * 3 + 1 ),
            Edge( face.index[ 1 ], face.index[ 2 ], face_index * 3 + 1, face_index * 3 + 2 ),
            Edge( face.index[ 2 ], face.index[ 0 ], face_index * 3 + 2, face_index * 3 + 0 ),
        };
        
        for( uint32 neighbor_index = face_index + 1; neighbor_index < xFile_data.face_count; neighbor_index++ ) {
            COLLISION_FACE neighbor = face_array[ neighbor_index ];
            int32 * hasNeighbor1 = neighborList + ( neighbor_index * 3 );
            
            EDGE edge1[ 3 ] = {
                Edge( neighbor.index[ 0 ], neighbor.index[ 1 ], neighbor_index * 3 + 0, neighbor_index * 3 + 1 ),
                Edge( neighbor.index[ 1 ], neighbor.index[ 2 ], neighbor_index * 3 + 1, neighbor_index * 3 + 2 ),
                Edge( neighbor.index[ 2 ], neighbor.index[ 0 ], neighbor_index * 3 + 2, neighbor_index * 3 + 0 ),
            };
            
            for( uint32 iter0 = 0; iter0 < 3; iter0++ ) {
                for( uint32 iter1 = 0; iter1 < 3; iter1++ ) {
                    if( isShared( edge0[ iter0 ], edge1[ iter1 ] ) ) {
                        vec3 normal1 = getNormal( xFile_data, neighbor );
                        
                        flo32 dotN = dot( normal0, normal1 );
                        flo32 testValue = ( 1.0f - 0.0001f );
                        // testValue = cosf( PI / 4.0f );
                        if( dotN >= testValue ) {
                            hasNeighbor0[ iter0 ] = neighbor_index;
                            hasNeighbor1[ iter1 ] = face_index;
                        }
                    }
                }
            }
        }
    }
    
    for( uint32 face_index = 0; face_index < xFile_data.face_count; face_index++ ) {
        if( !isPartOfIsland[ face_index ] ) {
            edgeList->edge_count = 0;
            enqueue( face_queue, face_index );
            while( !face_queue->isEmpty ) {
                uint32 index = dequeue( face_queue );
                isPartOfIsland[ index ] = true;
                int32 * hasNeighbor = neighborList + ( index * 3 );
                
                COLLISION_FACE face = face_array[ index ];
                for( uint32 side_index = 0; side_index < 3; side_index++ ) {
                    int32 neighbor_index = hasNeighbor[ side_index ];
                    if( neighbor_index == -1 ) {
                        uint32 index0 =   side_index;
                        uint32 index1 = ( side_index + 1 ) % 3;
                        
                        EDGE edge = {};
                        edge.index0 = face.index[ index0 ];
                        edge.index1 = face.index[ index1 ];
                        edge.texCoord_index0 = ( index * 3 ) + index0;
                        edge.texCoord_index1 = ( index * 3 ) + index1;
                        
                        add( edgeList, edge );
                    } else if( !isPartOfIsland[ neighbor_index ] ) {
                        enqueue( face_queue, neighbor_index );
                    }
                }
            }
            
            // sort edges into continuous chain
            for( uint32 edge_index0 = 0; edge_index0 < edgeList->edge_count; edge_index0++ ) {
                boo32 linkFound = false;
                
                EDGE edge0 = edgeList->edge[ edge_index0 ];
                for( uint32 edge_index1 = edge_index0 + 1; ( !linkFound ) && ( edge_index1 < edgeList->edge_count ); edge_index1++ ) {
                    EDGE edge1 = edgeList->edge[ edge_index1 ];
                    
                    if( edge0.index1 == edge1.index0 ) {
                        linkFound = true;
                        _swap( EDGE, edgeList->edge[ edge_index0 + 1 ], edgeList->edge[ edge_index1 ] );
                    }
                }
            }
            // Assert( edgeList->edge[ 0 ].index0 == edgeList->edge[ edgeList->edge_count - 1 ].index1 );
            
            vec3 xAxis = {};
            vec3 yAxis = {};
            vec3 zAxis = {};
            
            vec3 face_normal = getNormal( xFile_data, face_array[ face_index ] );
            if( fabs( dot( face_normal, Vec3( 0.0f, 1.0f, 0.0f ) ) ) <= ( 1.0f - 0.0001f ) ) {
                zAxis = face_normal;
                xAxis = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), zAxis ) );
                yAxis = getNormal( cross( zAxis, xAxis ) );
            } else {
                xAxis = Vec3( 1.0f, 0.0f, 0.0f );
                zAxis = face_normal;
                yAxis = getNormal( cross( zAxis, xAxis ) );
            }
            
            vec2 min = Vec2(  1000000.0f,  1000000.0f );
            vec2 max = Vec2( -1000000.0f, -1000000.0f );
            for( uint32 edge_index = 0; edge_index < edgeList->edge_count; edge_index++ ) {
                EDGE edge = edgeList->edge[ edge_index ];
                vec3 position = xFile_data.position[ edge.index0 ];
                
                min.x = minValue( min.x, dot( position, xAxis ) );
                max.x = maxValue( max.x, dot( position, xAxis ) );
                min.y = minValue( min.y, dot( position, yAxis ) );
                max.y = maxValue( max.y, dot( position, yAxis ) );
            }
            
            vec3 point[ 4 ] = {};
            point[ 0 ] = min.x * xAxis + max.y * yAxis;
            point[ 1 ] = min.x * xAxis + min.y * yAxis;
            point[ 2 ] = max.x * xAxis + min.y * yAxis;
            point[ 3 ] = max.x * xAxis + max.y * yAxis;
            
            flo32 minDistSq           [ 4 ] = { 1000000.0f, 1000000.0f, 1000000.0f, 1000000.0f };
            int32 closestPositionIndex[ 4 ] = { -1, -1, -1, -1 };
            int32 startingEdgeIndex   [ 4 ] = { -1, -1, -1, -1 };
            
            for( uint32 point_index = 0; point_index < 4; point_index++ ) {
                for( uint32 iter = 0; iter < edgeList->edge_count; iter++ ) {
                    EDGE edge = edgeList->edge[ iter ];
                    vec3 position = xFile_data.position[ edge.index0 ];
                    
                    flo32 distSq = getLengthSq( position - point[ point_index ] );
                    if( distSq < minDistSq[ point_index ] ) {
                        boo32 isValid = true;
                        for( uint32 prevPoint_index = 0; prevPoint_index < point_index; prevPoint_index++ ) {
                            if( ( int32 )edge.index0 == closestPositionIndex[ prevPoint_index ] ) {
                                isValid = false;
                            }
                        }
                        if( isValid ) {
                            minDistSq           [ point_index ] = distSq;
                            closestPositionIndex[ point_index ] = ( int32 )edge.index0;
                            startingEdgeIndex   [ point_index ] = iter;
                        }
                    }
                }
            }
            
            vec2 stride_vector = Vec2( 1.0f, 0.0f );
            vec2 atTex         = Vec2( 0.0f, 0.0f );
            for( uint32 side_index = 0; side_index < 4; side_index++ ) {
                uint32 side_index0 =   side_index;
                uint32 side_index1 = ( side_index + 1 ) % 4;
                
                uint32 start_index0 = startingEdgeIndex[ side_index0 ];
                uint32 start_index1 = startingEdgeIndex[ side_index1 ];
                
                uint32 edgeOnSide   = ( start_index1 + edgeList->edge_count - start_index0 ) % edgeList->edge_count;
                flo32  stride_value = 1.0f / ( flo32 )edgeOnSide;
                vec2   stride       = stride_vector * stride_value;
                for( uint32 counter = 0; counter < edgeOnSide; counter++ ) {
                    uint32 index = ( start_index0 + counter ) % edgeList->edge_count;
                    
                    EDGE edge = edgeList->edge[ index ];
                    vec2 tex0 = Vec2( clamp01( atTex.x ), clamp01( atTex.y ) );
                    vec2 tex1 = Vec2( clamp01( atTex.x + stride.x ), clamp01( atTex.y + stride.y ) );
                    
                    texCoord[ edge.texCoord_index0 ] = tex0;
                    texCoord[ edge.texCoord_index1 ] = tex1;
                    
                    atTex += stride;
                }
                
                stride_vector = Vec2( -stride_vector.y, stride_vector.x );
            }
        }
    }
    
    texFile_data->texCoord_count = texCoord_count;
    texFile_data->texCoord       = texCoord;
}


internal void
addEdge( COLLISION_SYSTEM * collisionSystem, vec3 point0, vec3 point1 ) {
    vec3 * vertex = collisionSystem->edge_vertex + collisionSystem->edge_count * 2;
    *vertex++ = point0;
    *vertex++ = point1;
    collisionSystem->edge_count++;
    Assert( collisionSystem->edge_count < COLLISION__EDGE_COUNT );
}

internal void
addTri( COLLISION_SYSTEM * collisionSystem, vec3 point0, vec3 point1, vec3 point2 ) {
    vec3 * vertex = collisionSystem->tri_vertex + collisionSystem->tri_count * 3;
    *vertex++ = point0;
    *vertex++ = point1;
    *vertex++ = point2;
    collisionSystem->tri_count++;
    Assert( collisionSystem->tri_count < COLLISION__TRI_COUNT );
}

internal void
addOverhang( COLLISION_SYSTEM * collisionSystem, vec3 point0, vec3 point1 ) {
    COLLISION_OVERHANG overhang = CollisionOverhang( point0, point1 );
    Assert( collisionSystem->overhang_count < COLLISION__OVERHANG_COUNT );
    collisionSystem->overhang[ collisionSystem->overhang_count++ ] = overhang;
}

internal void
addLedge( COLLISION_SYSTEM * collisionSystem, vec3 point0, vec3 point1, int32 link0, int32 link1 ) {
    COLLISION_LEDGE ledge = CollisionLedge( point0, point1, link0, link1 );
    Assert( collisionSystem->ledge_count < COLLISION__LEDGE_COUNT );
    collisionSystem->ledge[ collisionSystem->ledge_count++ ] = ledge;
}

internal void
addPole( COLLISION_SYSTEM * collisionSystem, COLLISION_POLE pole ) {
    Assert( dot( getNormal( pole.point1 - pole.point0 ), Vec3( 0.0f, 1.0f, 0.0f ) ) >= ( 1.0f - DOT__COLLINEAR_EPSILON ) );
    Assert( collisionSystem->pole_count < COLLISION__POLE_COUNT );
    pole.point0.y += COLLISION__POLE_OFFSET_BOTTOM;
    collisionSystem->pole[ collisionSystem->pole_count++ ] = pole;
}

internal void
addCollisionData( COLLISION_SYSTEM * collisionSystem, X_FILE_DATA xFile_data, ENTITY_FILE_DATA entityFile_data ) {
    for( uint32 face_index = 0; face_index < xFile_data.face_count; face_index++ ) {
        int32 base_index = face_index * 3;
        vec3 pos0 = xFile_data.position[ xFile_data.face[ base_index + 0 ] ];
        vec3 pos1 = xFile_data.position[ xFile_data.face[ base_index + 1 ] ];
        vec3 pos2 = xFile_data.position[ xFile_data.face[ base_index + 2 ] ];
        addTri( collisionSystem, pos0, pos1, pos2 );
    }
    
    for( uint32 overhang_index = 0; overhang_index < entityFile_data.overhang_count; overhang_index++ ) {
        OVERHANG overhang = entityFile_data.overhang[ overhang_index ];
        addOverhang( collisionSystem, overhang.point[ 0 ], overhang.point[ 1 ] );
    }
    
    for( uint32 iLedge = 0; iLedge < entityFile_data.nLedge; iLedge++ ) {
        LEDGE ledge = entityFile_data.ledge[ iLedge ];
        addLedge( collisionSystem, ledge.point[ 0 ], ledge.point[ 1 ], -1, -1 );
    }
    
    for( uint32 pole_index = 0; pole_index < entityFile_data.pole_count; pole_index++ ) {
        POLE pole = entityFile_data.pole[ pole_index ];
        
        vec3 p0 = pole.point[ 0 ];
        vec3 p1 = pole.point[ 1 ];
        if( p1.y < p0.y ) {
            _swap( vec3, p0, p1 );
        }
        addPole( collisionSystem, CollisionPole( p0, p1, pole.normal ) );
    }
    
    if( collisionSystem->ledge_count > 1 ) {
        for( uint32 iLedge0 = 0; iLedge0 < ( collisionSystem->ledge_count - 1 ); iLedge0++ ) {
            COLLISION_LEDGE * ledge0 = collisionSystem->ledge + iLedge0;
            for( uint32 iLedge1 = ( iLedge0 + 1 ); iLedge1 < collisionSystem->ledge_count; iLedge1++ ) {
                COLLISION_LEDGE * ledge1 = collisionSystem->ledge + iLedge1;
                
                if( ledge0->point0 == ledge1->point1 ) {
                    ledge0->link0 = iLedge1;
                    ledge1->link1 = iLedge0;
                }
                if( ledge0->point1 == ledge1->point0 ) {
                    ledge0->link1 = iLedge1;
                    ledge1->link0 = iLedge0;
                }
            }
        }
    }
}