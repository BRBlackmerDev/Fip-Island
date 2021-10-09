
internal void
drawTri( RENDERER * renderer, RENDER_PASS_ID passID, vec3 pos0, vec3 pos1, vec3 pos2, vec4 color ) {
    vec3 normal = getNormal( cross( pos1 - pos0, pos2 - pos0 ) );
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addTri( &vertexTask, pos0, pos1, pos2, normal );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_boxTexture, color ) );
}

internal void
drawQuad( RENDERER * renderer, RENDER_PASS_ID passID, vec3 pos0, vec3 pos1, vec3 pos2, vec3 pos3, vec4 color ) {
    vec3 normal = getNormal( cross( pos1 - pos0, pos2 - pos0 ) );
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addQuad( &vertexTask, pos0, pos1, pos2, pos3, normal );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_boxTexture, color ) );
}

internal void
drawQuadCXY( RENDERER * renderer, RENDER_PASS_ID passID, vec3 center, vec3 xAxis, vec3 yAxis, vec4 color ) {
    vec3 normal = getNormal( cross( xAxis, yAxis ) );
    vec3 pos0 = center - xAxis - yAxis;
    vec3 pos1 = center + xAxis - yAxis;
    vec3 pos2 = center - xAxis + yAxis;
    vec3 pos3 = center + xAxis + yAxis;
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addQuad( &vertexTask, pos0, pos1, pos2, pos3, normal );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawLine( RENDERER * renderer, RENDER_PASS_ID passID, vec3 point0, vec3 point1, vec4 color ) {
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addEdge( &vertexTask, point0, point1, DIRECTION_TO_LIGHT );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lines, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawTriOutline( RENDERER * renderer, RENDER_PASS_ID passID, vec3 point0, vec3 point1, vec3 point2, vec4 color ) {
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addPoint( &vertexTask, point0, DIRECTION_TO_LIGHT );
    addPoint( &vertexTask, point1, DIRECTION_TO_LIGHT );
    addPoint( &vertexTask, point2, DIRECTION_TO_LIGHT );
    addPoint( &vertexTask, point0, DIRECTION_TO_LIGHT );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lineStrip, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawCube( RENDERER * renderer, RENDER_PASS_ID passID, vec3 min, vec3 max, vec4 color ) {
    vec3 pos[ 8 ] = {
        min,
        Vec3( min.x, min.y, max.z ),
        Vec3( min.x, max.y, min.z ),
        Vec3( min.x, max.y, max.z ),
        Vec3( max.x, min.y, min.z ),
        Vec3( max.x, min.y, max.z ),
        Vec3( max.x, max.y, min.z ),
        max,
    };
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addQuad( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ], Vec3( -1.0f,  0.0f,  0.0f ) );
    addQuad( &vertexTask, pos[ 0 ], pos[ 4 ], pos[ 1 ], pos[ 5 ], Vec3(  0.0f, -1.0f,  0.0f ) );
    addQuad( &vertexTask, pos[ 4 ], pos[ 0 ], pos[ 6 ], pos[ 2 ], Vec3(  0.0f,  0.0f, -1.0f ) );
    addQuad( &vertexTask, pos[ 5 ], pos[ 4 ], pos[ 7 ], pos[ 6 ], Vec3(  1.0f,  0.0f,  0.0f ) );
    addQuad( &vertexTask, pos[ 3 ], pos[ 7 ], pos[ 2 ], pos[ 6 ], Vec3(  0.0f,  1.0f,  0.0f ) );
    addQuad( &vertexTask, pos[ 1 ], pos[ 5 ], pos[ 3 ], pos[ 7 ], Vec3(  0.0f,  0.0f,  1.0f ) );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_boxTexture, color ) );
}

internal void
addCube( VERTEX1_TASK * vertexTask, vec3 min, vec3 max ) {
    vec3 pos[ 8 ] = {
        min,
        Vec3( min.x, min.y, max.z ),
        Vec3( min.x, max.y, min.z ),
        Vec3( min.x, max.y, max.z ),
        Vec3( max.x, min.y, min.z ),
        Vec3( max.x, min.y, max.z ),
        Vec3( max.x, max.y, min.z ),
        max,
    };
    
    addQuad( vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ], Vec3( -1.0f,  0.0f,  0.0f ) );
    addQuad( vertexTask, pos[ 0 ], pos[ 4 ], pos[ 1 ], pos[ 5 ], Vec3(  0.0f, -1.0f,  0.0f ) );
    addQuad( vertexTask, pos[ 4 ], pos[ 0 ], pos[ 6 ], pos[ 2 ], Vec3(  0.0f,  0.0f, -1.0f ) );
    addQuad( vertexTask, pos[ 5 ], pos[ 4 ], pos[ 7 ], pos[ 6 ], Vec3(  1.0f,  0.0f,  0.0f ) );
    addQuad( vertexTask, pos[ 3 ], pos[ 7 ], pos[ 2 ], pos[ 6 ], Vec3(  0.0f,  1.0f,  0.0f ) );
    addQuad( vertexTask, pos[ 1 ], pos[ 5 ], pos[ 3 ], pos[ 7 ], Vec3(  0.0f,  0.0f,  1.0f ) );
}

internal void
drawCubeOutline( RENDERER * renderer, RENDER_PASS_ID passID, cube bound, vec4 color ) {
    vec3 pos[ 8 ] = {
        bound.origin,
        bound.origin + bound.zAxis,
        bound.origin + bound.yAxis,
        bound.origin + bound.zAxis + bound.yAxis,
        bound.origin + bound.xAxis,
        bound.origin + bound.xAxis + bound.zAxis,
        bound.origin + bound.xAxis + bound.yAxis,
        bound.origin + bound.xAxis + bound.zAxis + bound.yAxis,
    };
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addQuadOutline( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 5 ], pos[ 4 ], DIRECTION_TO_LIGHT );
    addQuadOutline( &vertexTask, pos[ 2 ], pos[ 3 ], pos[ 7 ], pos[ 6 ], DIRECTION_TO_LIGHT );
    addEdge( &vertexTask, pos[ 0 ], pos[ 2 ], DIRECTION_TO_LIGHT );
    addEdge( &vertexTask, pos[ 1 ], pos[ 3 ], DIRECTION_TO_LIGHT );
    addEdge( &vertexTask, pos[ 5 ], pos[ 7 ], DIRECTION_TO_LIGHT );
    addEdge( &vertexTask, pos[ 4 ], pos[ 6 ], DIRECTION_TO_LIGHT );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lines, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal rect
takeL( rect * src_bound, flo32 take_width ) {
    flo32 width = minValue( take_width, getWidth( *src_bound ) );
    Assert( width > 0.0f );
    
    rect result = Rect( src_bound->left, src_bound->bottom, src_bound->left + width, src_bound->top );
    src_bound->left = result.right;
    
    return result;
}

internal rect
takeB( rect * src_bound, flo32 take_height ) {
    flo32 height = minValue( take_height, getHeight( *src_bound ) );
    Assert( height > 0.0f );
    
    rect result = Rect( src_bound->left, src_bound->bottom, src_bound->right, src_bound->bottom + height );
    src_bound->bottom = result.top;
    
    return result;
}

internal rect
takeR( rect * src_bound, flo32 take_width ) {
    flo32 width = minValue( take_width, getWidth( *src_bound ) );
    Assert( width > 0.0f );
    
    rect result = Rect( src_bound->right - width, src_bound->bottom, src_bound->right, src_bound->top );
    src_bound->right = result.left;
    
    return result;
}

internal rect
takeT( rect * src_bound, flo32 take_height ) {
    flo32 height = minValue( take_height, getHeight( *src_bound ) );
    Assert( height > 0.0f );
    
    rect result = Rect( src_bound->left, src_bound->top - height, src_bound->right, src_bound->top );
    src_bound->top = result.bottom;
    
    return result;
}

internal void
removeFace( EDITOR_STATE * editorState, int32 point_index ) {
    int32 face_newCount = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        FACE face = editorState->face[ face_index ];
        
        int32 index_count = 0;
        for( int32 index = 0; index < 4; index++ ) {
            if( face.index[ index ] == point_index ) { face.index[ index ] = -1; }
            if( face.index[ index ] >  point_index ) { face.index[ index ]--;    }
            if( face.index[ index ] != -1 ) { index_count++; }
        }
        
        if( index_count > 2 ) {
            editorState->face[ face_newCount++ ] = face;
        }
    }
    editorState->face_count = face_newCount;
}

internal void
removeOverhang( EDITOR_STATE * editorState, int32 point_index ) {
    int32 overhang_newCount = 0;
    for( int32 overhang_index = 0; overhang_index < editorState->overhang_count; overhang_index++ ) {
        OVERHANG overhang = editorState->overhang[ overhang_index ];
        
        boo32 shouldRemove = false;
        for( int32 index = 0; ( !shouldRemove ) && ( index < 2 ); index++ ) {
            if( overhang.index[ index ] == point_index ) { shouldRemove = true; }
            if( overhang.index[ index ] >  point_index ) { overhang.index[ index ]--; }
        }
        
        if( !shouldRemove ) {
            editorState->overhang[ overhang_newCount++ ] = overhang;
        }
    }
    editorState->overhang_count = overhang_newCount;
}

internal void
resetEditor( EDITOR_STATE * editorState ) {
    editorState->face_count   = 0;
    editorState->point_count  = 0;
}

internal void
findMandatoryVertices( int32 minIndex, int32 maxIndex, flo32 maxContourDist, boo32 * isMandatory, vec3 * region_vertex ) {
    Assert( isMandatory[ minIndex ] );
    Assert( isMandatory[ maxIndex ] );
    
    if( ( maxIndex - minIndex ) >= 2 ) {
        vec3  vertex0 = region_vertex[ minIndex ];
        vec3  v     = region_vertex[ maxIndex ] - vertex0;
        flo32 dotVV = dot( v, v );
        
        int32 maxDistSq_index = -1;
        flo32 maxDistSq       = maxContourDist * maxContourDist;
        if( dotVV == 0.0f ) {
            for( int32 vertex_index = minIndex + 1; vertex_index < maxIndex; vertex_index++ ) {
                flo32 distSq = getLengthSq( region_vertex[ vertex_index ] - vertex0 );
                
                if( distSq > maxDistSq ) {
                    maxDistSq_index = vertex_index;
                    maxDistSq       = distSq;
                }
            }
        } else {
            for( int32 vertex_index = minIndex + 1; vertex_index < maxIndex; vertex_index++ ) {
                vec3 w = region_vertex[ vertex_index ] - vertex0;
                flo32 dotWW = dot( w, w );
                flo32 dotVW = dot( v, w );
                
                flo32 distSq = dotWW - ( ( dotVW * dotVW ) / dotVV );
                if( distSq > maxDistSq ) {
                    maxDistSq_index = vertex_index;
                    maxDistSq       = distSq;
                }
            }
        }
        
        if( maxDistSq_index != -1 ) {
            isMandatory[ maxDistSq_index ] = true;
            
            findMandatoryVertices( minIndex, maxDistSq_index, maxContourDist, isMandatory, region_vertex );
            findMandatoryVertices( maxDistSq_index, maxIndex, maxContourDist, isMandatory, region_vertex );
        }
    }
}

internal void
loadEntity( EDITOR_STATE * editorState, MEMORY * permMemory, MEMORY * tempMemory, uint32 entityID, mat4 transform = mat4_identity(), vec3 translation = Vec3( 0.0f ), vec3 rotation = Vec3( 0.0f ) ) {
    READ_FILE_RESULT file_mesh = readXFile( SnapMemory( tempMemory ), entityID );
    Assert( file_mesh.contents );
    
    TEX_FILE_DATA texFile_data = {};
    X_FILE_DATA__QUAD xFile_data = parseXFile_quad( tempMemory, file_mesh, &texFile_data );
    // if( !texFile_data.texFileExists ) {
    // if( texFile_data.texDataExistsInXFile ) {
    // unfoldTexCoords( permMemory, xFile_data, &texFile_data );
    // } else {
    // genTexCoords( permMemory, tempMemory, xFile_data, &texFile_data );
    // }
    // }
    
    // editorState->renderData = addRenderData( permMemory, xFile_data, texFile_data, COLOR_WHITE );
    
    // vec3 * point = _pushArray_clear( tempMemory, vec3, xFile_data.position_count );
    // memcpy( point, xFile_data.position, sizeof( vec3 ) * xFile_data.position_count );
    // bubbleSort( point, xFile_data.position_count );
    // uint32 point_count = removeDuplicates( point, xFile_data.position_count );
    
    // for( uint32 point_index = 0; point_index < point_count; point_index++ ) {
    // addPoint( editorState, point[ point_index ] );
    // }
    uint32 bPoint = editorState->point_count;
    vec3 * pDest  = editorState->point + editorState->point_count;
    for( uint32 iPoint = 0; iPoint < xFile_data.position_count; iPoint++ ) {
        pDest[ iPoint ] = ( transform * Vec4( xFile_data.position[ iPoint ], 1.0f ) ).xyz;
    }
    editorState->point_count += xFile_data.position_count;
    
    uint32 bFace = editorState->face_count;
    for( uint32 face_index = 0; face_index < xFile_data.face_count; face_index++ ) {
        FACE face    = xFile_data.face[ face_index ];
        uint32 nElem = getElemCount( face );
        for( uint32 iElem = 0; iElem < nElem; iElem++ ) {
            face.index[ iElem ] += bPoint;
        }
        addFace( editorState, face );
    }
    popToSnap( tempMemory );
    
    LEVEL level = {};
    level.entityID = entityID;
    level.bPoint   = bPoint;
    level.nPoint   = xFile_data.position_count;
    level.bFace    = bFace;
    level.nFace    = xFile_data.face_count;
    level.translation = translation;
    level.rotation    = rotation;
    
    READ_FILE_RESULT file_entity = readEntityFile( SnapMemory( tempMemory ), entityID );
    if( file_entity.contents ) {
        ENTITY_FILE_DATA entityFile_data = parseEntityFile( tempMemory, file_entity );
        
        if( entityFile_data.color ) {
            for( uint32 iColor = 0; iColor < entityFile_data.nColor; iColor++ ) {
                uint32   color = entityFile_data.color[ iColor ];
                uint32 * color_face = entityFile_data.color_face + entityFile_data.bColor0[ iColor ];
                for( uint32 iFace = 0; iFace < entityFile_data.nColor0[ iColor ]; iFace++ ) {
                    editorState->face_color[ color_face[ iFace ] ] = color;
                }
            }
        } else {
            uint32 color = toU32Color( COLOR_GRAY( 0.75f ) );
            uint32 * face_color = editorState->face_color + bFace;
            for( uint32 iFace = 0; iFace < xFile_data.face_count; iFace++ ) {
                face_color[ iFace ] = color;
            }
        }
        
        for( uint32 overhang_index = 0; overhang_index < entityFile_data.overhang_count; overhang_index++ ) {
            OVERHANG * overhang = entityFile_data.overhang + overhang_index;
            
            flo32 minDiff0 = 1000000.0f;
            flo32 minDiff1 = 1000000.0f;
            
            int32 minIndex0 = -1;
            int32 minIndex1 = -1;
            
            for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
                vec3 point = editorState->point[ point_index ];
                
                flo32 diff0 = fabsf( point.x - overhang->point[ 0 ].x ) + fabsf( point.y - overhang->point[ 0 ].y ) + fabsf( point.z - overhang->point[ 0 ].z );
                flo32 diff1 = fabsf( point.x - overhang->point[ 1 ].x ) + fabsf( point.y - overhang->point[ 1 ].y ) + fabsf( point.z - overhang->point[ 1 ].z );
                
                if( diff0 < minDiff0 ) {
                    minDiff0 = diff0;
                    minIndex0 = point_index;
                }
                if( diff1 < minDiff1 ) {
                    minDiff1 = diff1;
                    minIndex1 = point_index;
                }
            }
            Assert( minIndex0 != -1 );
            Assert( minIndex1 != -1 );
            
            overhang->index[ 0 ] = minIndex0;
            overhang->index[ 1 ] = minIndex1;
        }
        memcpy( editorState->overhang, entityFile_data.overhang, sizeof( OVERHANG ) * entityFile_data.overhang_count );
        editorState->overhang_count = entityFile_data.overhang_count;
        
        for( uint32 iLedge = 0; iLedge < entityFile_data.nLedge; iLedge++ ) {
            LEDGE ledge = entityFile_data.ledge[ iLedge ];
            EDITOR__LEDGE eLedge = toEditorLedge( editorState, ledge, transform );
            addLedge( editorState, eLedge );
        }
        
        for( uint32 pole_index = 0; pole_index < entityFile_data.pole_count; pole_index++ ) {
            POLE * pole = entityFile_data.pole + pole_index;
            
            flo32 minDiff0 = 1000000.0f;
            flo32 minDiff1 = 1000000.0f;
            
            int32 minIndex0 = -1;
            int32 minIndex1 = -1;
            
            for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
                vec3 point = editorState->point[ point_index ];
                
                flo32 diff0 = fabsf( point.x - pole->point[ 0 ].x ) + fabsf( point.y - pole->point[ 0 ].y ) + fabsf( point.z - pole->point[ 0 ].z );
                flo32 diff1 = fabsf( point.x - pole->point[ 1 ].x ) + fabsf( point.y - pole->point[ 1 ].y ) + fabsf( point.z - pole->point[ 1 ].z );
                
                if( diff0 < minDiff0 ) {
                    minDiff0 = diff0;
                    minIndex0 = point_index;
                }
                if( diff1 < minDiff1 ) {
                    minDiff1 = diff1;
                    minIndex1 = point_index;
                }
            }
            Assert( minIndex0 != -1 );
            Assert( minIndex1 != -1 );
            
            pole->index[ 0 ] = minIndex0;
            pole->index[ 1 ] = minIndex1;
        }
        memcpy( editorState->pole, entityFile_data.pole, sizeof( POLE ) * entityFile_data.pole_count );
        editorState->pole_count = entityFile_data.pole_count;
        
        level.bDoor = editorState->nSecureDoor;
        level.nDoor = entityFile_data.nSecureDoor;
        for( uint32 iDoor = 0; iDoor < entityFile_data.nSecureDoor; iDoor++ ) {
            SECURE_DOOR          door = entityFile_data.secureDoor[ iDoor ];
            EDITOR__SECURE_DOOR eDoor = toEditorSecureDoor( editorState, door, transform );
            addSecureDoor( editorState, eDoor );
        }
        
        for( uint32 iPixie = 0; iPixie < entityFile_data.nPixie; iPixie++ ) {
            PIXIE          pixie = entityFile_data.pixie[ iPixie ];
            EDITOR__PIXIE ePixie = toEditorPixie( editorState, pixie, transform );
            addPixie( editorState, ePixie );
        }
        
        level.bKillZone = editorState->nKillZone;
        level.nKillZone = entityFile_data.nKillZone;
        for( uint32 iKillZone = 0; iKillZone < entityFile_data.nKillZone; iKillZone++ ) {
            KILL_ZONE          killZone = entityFile_data.killZone[ iKillZone ];
            EDITOR__KILL_ZONE eKillZone = toEditorKillZone( editorState, killZone, transform );
            addKillZone( editorState, eKillZone );
        }
        
        level.bCheckpoint = editorState->nCheckpoint;
        level.nCheckpoint = entityFile_data.nCheckpoint;
        for( uint32 iCheckpoint = 0; iCheckpoint < entityFile_data.nCheckpoint; iCheckpoint++ ) {
            CHECKPOINT          checkpoint = entityFile_data.checkpoint[ iCheckpoint ];
            EDITOR__CHECKPOINT eCheckpoint = toEditorCheckpoint( editorState, checkpoint, transform );
            addCheckpoint( editorState, eCheckpoint );
        }
        
        for( uint32 iView = 0; iView < entityFile_data.nView; iView++ ) {
            VIEW          view = entityFile_data.view[ iView ];
            EDITOR__VIEW eView = toEditorView( editorState, view, transform );
            addView( editorState, eView );
        }
        
        popToSnap( tempMemory );
    }
    addLevel( &editorState->levelPanel, level );
    
    // outputEntityFile( tempMemory, xFile_data, entityID );
    // _popSize( tempMemory, getDataSize( xFile_data ) );
    // _popSize( tempMemory, file_mesh.size );
    
    
    // READ_FILE_RESULT file_entity = readEntityFile( tempMemory, entityID );
    // if( !file_entity.contents ) {
    // READ_FILE_RESULT file_mesh = readXFile( tempMemory, entityID );
    // Assert( file_mesh.contents );
    
    // file_entity = readEntityFile( tempMemory, entityID );
    // }
    
    // ENTITY_FILE_DATA entityFile_data = parseEntityFile( tempMemory, file_entity );
    
    editorState->entityID = entityID;
}

internal void
saveLevel( LEVEL_PANEL * l, MEMORY * memory ) {
    writeSegment( memory, "[LEVEL_VERS01]" );
    writeSegment( memory, "\n\n" );
    
    for( uint32 iLevel = 0; iLevel < l->nLevel; iLevel++ ) {
        LEVEL level = l->level[ iLevel ];
        writeSegment( memory, "[ENTITY] " );
        writeU32    ( memory, level.entityID );
        writeSegment( memory, " {\n" );
        
        writeSegment( memory, "[POSITION] ", 1, false );
        writeV3     ( memory, level.translation, 0, true );
        
        vec3 r = level.rotation;
        for( uint32 iter = 0; iter < 3; iter++ ) { r.elem[ iter ] = toDegrees( r.elem[ iter ] ); }
        
        writeSegment( memory, "[ROTATION] ", 1, false );
        writeV3     ( memory, r, 0, true );
        writeSegment( memory, "}\n\n" );
    }
    
    char entity_filePath[ 128 ] = {};
    sprintf( entity_filePath, "../../art/level/level_.lvl" );
    writeFile( entity_filePath, getSnapBase( memory ), ( uint32 )getSnapUsed( memory ) );
    
    popToSnap( memory );
}

internal void
loadLevel( EDITOR_STATE * e, MEMORY * permMemory, MEMORY * tempMemory ) {
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
                
                vec3 translation = {};
                vec3 rotation    = {};
                
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
                            translation = Vec3( x, y, z );
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
                            
                            rotation = Vec3( toRadians( xDegrees ), toRadians( yDegrees ), toRadians( zDegrees ) );
                            
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
                            loadEntity( e, permMemory, tempMemory, entityID, transform, translation, rotation );
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

internal void
loadEntity( APP_STATE * appState, MEMORY * permMemory, MEMORY * tempMemory, uint32 entityID, mat4 transform, vec4 modColor, boo32 enableCollision ) {
    X_FILE_DATA xFile_data = {};
    ENTITY_FILE_DATA entityFile_data = {};
    
    TEX_FILE_DATA texFile_data = readTexFile( permMemory, entityID );
    
    SnapMemory( tempMemory );
    READ_FILE_RESULT file_mesh = readXFile( tempMemory, entityID );
    Assert( file_mesh.contents );
    
    xFile_data = parseXFile( tempMemory, file_mesh, &texFile_data );
    if( !texFile_data.texFileExists ) {
        if( texFile_data.texDataExistsInXFile ) {
            unfoldTexCoords( permMemory, xFile_data, &texFile_data );
        } else {
            genTexCoords( permMemory, tempMemory, xFile_data, &texFile_data );
        }
        // outputTexFile( tempMemory, entityID, texFile_data );
    }
    for( uint32 position_index = 0; position_index < xFile_data.position_count; position_index++ ) {
        vec3 position = xFile_data.position[ position_index ];
        xFile_data.position[ position_index ] = ( transform * Vec4( position, 1.0f ) ).xyz;
    }
    
    READ_FILE_RESULT file_entity = readEntityFile( tempMemory, entityID );
    if( file_entity.contents ) {
        entityFile_data = parseEntityFile( tempMemory, file_entity );
        for( uint32 overhang_index = 0; overhang_index < entityFile_data.overhang_count; overhang_index++ ) {
            OVERHANG overhang = entityFile_data.overhang[ overhang_index ];
            overhang.point[ 0 ] = ( transform * Vec4( overhang.point[ 0 ], 1.0f ) ).xyz;
            overhang.point[ 1 ] = ( transform * Vec4( overhang.point[ 1 ], 1.0f ) ).xyz;
            entityFile_data.overhang[ overhang_index ] = overhang;
        }
        for( uint32 iLedge = 0; iLedge < entityFile_data.nLedge; iLedge++ ) {
            LEDGE ledge = entityFile_data.ledge[ iLedge ];
            ledge.point[ 0 ] = ( transform * Vec4( ledge.point[ 0 ], 1.0f ) ).xyz;
            ledge.point[ 1 ] = ( transform * Vec4( ledge.point[ 1 ], 1.0f ) ).xyz;
            entityFile_data.ledge[ iLedge ] = ledge;
        }
        for( uint32 pole_index = 0; pole_index < entityFile_data.pole_count; pole_index++ ) {
            POLE pole = entityFile_data.pole[ pole_index ];
            pole.point[ 0 ] = ( transform * Vec4( pole.point[ 0 ], 1.0f ) ).xyz;
            pole.point[ 1 ] = ( transform * Vec4( pole.point[ 1 ], 1.0f ) ).xyz;
            pole.normal     = ( transform * Vec4( pole.normal, 0.0f ) ).xyz;
            entityFile_data.pole[ pole_index ] = pole;
        }
        
        SECURE_DOOR_LIST * s = &appState->secureDoorList;
        for( uint32 iDoor = 0; iDoor < entityFile_data.nSecureDoor; iDoor++ ) {
            SECURE_DOOR door = entityFile_data.secureDoor[ iDoor ];
            door = transformSecureDoor( transform, door );
            addSecureDoor( s, door );
        }
        
        PIXIE_LIST * p = &appState->pixieList;
        for( uint32 iPixie = 0; iPixie < entityFile_data.nPixie; iPixie++ ) {
            PIXIE pixie = entityFile_data.pixie[ iPixie ];
            pixie.pickUp_bound = transformOBox( transform, pixie.pickUp_bound );
            pixie.position     = pixie.pickUp_bound.center;
            add( p, pixie );
        }
        
        KILL_ZONE_LIST * k = &appState->killZoneList;
        for( uint32 iKillZone = 0; iKillZone < entityFile_data.nKillZone; iKillZone++ ) {
            KILL_ZONE killZone = entityFile_data.killZone[ iKillZone ];
            killZone.bound     = transformOBox( transform, killZone.bound );
            add( k, killZone );
        }
        
        CHECKPOINT_LIST * c = &appState->checkpointList;
        for( uint32 iCheckpoint = 0; iCheckpoint < entityFile_data.nCheckpoint; iCheckpoint++ ) {
            CHECKPOINT checkpoint = entityFile_data.checkpoint[ iCheckpoint ];
            checkpoint.bound      = transformOBox( transform, checkpoint.bound );
            checkpoint.respawnPos = ( transform * Vec4( checkpoint.respawnPos, 1.0f ) ).xyz;
            add( c, checkpoint );
        }
        
        VIEW_LIST * v = &appState->viewList;
        for( uint32 iView = 0; iView < entityFile_data.nView; iView++ ) {
            VIEW view = entityFile_data.view[ iView ];
            view.P    = ( transform * Vec4( view.P, 1.0f ) ).xyz;
            view.Q    = ( transform * Vec4( view.Q, 1.0f ) ).xyz;
            view.b    = transformOBox( transform, view.b );
            add( v, view );
        }
        
    }
    
    addRenderData( appState, permMemory, xFile_data, texFile_data, entityFile_data, modColor );
    if( enableCollision ) {
        addCollisionData( &appState->collisionSystem, xFile_data, entityFile_data );
    }
    
    popToSnap( tempMemory );
}

internal void
drawPoles( RENDERER * renderer, EDITOR_STATE * editorState ) {
    for( int32 pole_index = 0; pole_index < editorState->pole_count; pole_index++ ) {
        POLE pole = editorState->pole[ pole_index ];
        
        vec3 A = pole.point[ 0 ];
        vec3 B = pole.point[ 1 ];
        
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
        
        drawLine( renderer, renderPassID_main, P[ 0 ], P[ 3 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 1 ], P[ 6 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 2 ], P[ 5 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 7 ], P[ 4 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 3 ], P[ 6 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 6 ], P[ 4 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 4 ], P[ 5 ], COLOR_MAGENTA );
        drawLine( renderer, renderPassID_main, P[ 5 ], P[ 3 ], COLOR_MAGENTA );
    }
}

internal vec3
getNormal( EDITOR_STATE * editorState, int32 index0, int32 index1 ) {
    vec3 result = {};
    
    vec3   normal[ 2 ]  = {};
    uint32 normal_count = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        FACE face = editorState->face[ face_index ];
        
        boo32 isFound0 = false;
        boo32 isFound1 = false;
        for( int32 iter = 0; iter < 4; iter++ ) { if( face.index[ iter ] == index0 ) { isFound0 = true; } }
        for( int32 iter = 0; iter < 4; iter++ ) { if( face.index[ iter ] == index1 ) { isFound1 = true; } }
        
        if( isFound0 && isFound1 ) {
            Assert( normal_count < 2 );
            normal[ normal_count++ ] = getNormal( editorState, face );
        }
    }
    
    if( normal_count == 2 ) {
        flo32 dotNN = dot( normal[ 0 ], normal[ 1 ] );
        Assert( dotNN >= ( 1.0f - DOT__COLLINEAR_EPSILON ) );
        result = normal[ 0 ];
    }
    if( normal_count == 1 ) {
        result = normal[ 0 ];
    }
    
    return result;
}

internal void
updateEditor( EDITOR_STATE * editorState, PLATFORM * platform ) {
    PROFILE_FUNCTION();
    
    RENDERER * renderer = &platform->renderer;
    
    MOUSE_STATE    * mouse    = &platform->mouse;
    KEYBOARD_STATE * keyboard = &platform->keyboard;
    
    MEMORY * permMemory = &platform->permMemory;
    MEMORY * tempMemory = &platform->tempMemory;
    
    vec2 app_dim     = editorState->app_dim;
    vec2 app_halfDim = editorState->app_halfDim;
    rect app_bound   = editorState->app_bound;
    
    UNDO_SYSTEM * undoSystem = &editorState->undoSystem;
    
    rect window_bound = app_bound;
    rect toolbar_bound     = takeB( &window_bound, 20.0f );
    rect toolbar_takeBound = toolbar_bound;
	
	if( !editorState->isInitialized ) {
		editorState->isInitialized = true;
        
        editorState->entityID = -1;
        
        CAMERA * c = &editorState->camera;
        c->fovAngle  = PI * 0.3333f;
        c->nearPlane = 0.001f;
        c->farPlane  = 10000.0f;
        
        c->ortho_dim = Vec2( 4.0f * ( app_dim.x / app_dim.y ), 4.0f );
        
        c->dist      = 10.0f;
        c->vRadians  = 0.5236f;
        c->hRadians  = 0.7854f;
        
        c->vRadians = 0.0f;
        c->hRadians = 0.0f;
        c->position = Vec3( 0.0f, 1.0f, 0.0f );
        c->xAxis = Vec3( 1.0f, 0.0f, 0.0f );
        c->yAxis = Vec3( 0.0f, 1.0f, 0.0f );
        c->zAxis = Vec3( 0.0f, 0.0f, 1.0f );
        
        vec2 panel_dim  = Vec2( 240.0f, 120.0f );
        rect take_bound = rectBRD( getBR( app_bound ), Vec2( panel_dim.x, app_dim.y ) );
        editorState->toolWindow_bound = take_bound;
        
        editorState->displayWindow_bound = Rect( app_bound.left, toolbar_bound.top, editorState->toolWindow_bound.left, app_bound.top );
        editorState->textBox_bound      = addRadius( takeT( &take_bound, 20.0f ), -2.0f );
        {
            UTILITY_PANEL * u = &editorState->utilityPanel;
            u->bound = addRadius( takeT( &take_bound, 400.0f ), -2.0f );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            u->text_charWidth = font->alphabet[ 0 ].advanceWidth;
            
            rect sub_bound = u->bound;
            takeT( &sub_bound, font->advanceHeight + 4.0f );
            
            flo32 textBox_height = font->advanceHeight + 12.0f;
            
            flo32 * cElem0 = u->centroid.position.elem;
            for( uint32 iter = 0; iter < 3; iter++ ) {
                rect bound = addOffset( addRadius( takeT( &sub_bound, textBox_height ), -2.0f ), Vec2( -2.0f, 0.0f ) );
                bound.left += 40.0f;
                
                uint32 index = ( uint32 )( utilityPanel_textBoxID_centroidX + iter );
                u->textBox[ index ] = TextBox( bound, cElem0 + iter );
            }
            
            // grid
            takeT( &sub_bound, font->advanceHeight + 4.0f );
            
            u->grid_isActive_bound = addRadius( rectTLD( getTL( sub_bound ) + Vec2( 8.0f, -8.0f ), Vec2( textBox_height, textBox_height ) ), -2.0f );
            
            rect bound0  = addOffset( addRadius( takeT( &sub_bound, textBox_height ), -2.0f ), Vec2( -2.0f, 0.0f ) );
            bound0.left += 100.0f;
            u->textBox[ utilityPanel_textBoxID_scale ] = TextBox( bound0, &u->scale );
            
            rect bound1  = addOffset( addRadius( takeT( &sub_bound, textBox_height ), -2.0f ), Vec2( -2.0f, 0.0f ) );
            bound1.left += 100.0f;
            u->textBox[ utilityPanel_textBoxID_degrees ] = TextBox( bound1, &u->degrees );
            
            // pivot
            takeT( &sub_bound, font->advanceHeight + 4.0f );
            
            u->pivot_isActive_bound = addRadius( rectTLD( getTL( sub_bound ) + Vec2( 8.0f, -8.0f ), Vec2( textBox_height, textBox_height ) ), -2.0f );
            u->pivot_setPivot_bound = addOffset( u->pivot_isActive_bound, Vec2( 0.0f, -( textBox_height + 4.0f ) ) );
            
            flo32 * cElem1 = u->pivot.elem;
            for( uint32 iElem = 0; iElem < 3; iElem++ ) {
                rect bound  = addOffset( addRadius( takeT( &sub_bound, textBox_height ), -2.0f ), Vec2( -2.0f, 0.0f ) );
                bound.left += 60.0f;
                
                uint32 index = ( uint32 )( utilityPanel_textBoxID_pivotX + iElem );
                u->textBox[ index ] = TextBox( bound, cElem1 + iElem );
            }
            
            // direction
            takeT( &sub_bound, font->advanceHeight + 4.0f );
            
            u->direction_setDirection_bound = addRadius( rectTLD( getTL( sub_bound ) + Vec2( 8.0f, -8.0f ), Vec2( textBox_height, textBox_height ) ), -2.0f );         
            flo32 * cElem2 = u->direction.elem;
            for( uint32 iElem = 0; iElem < 3; iElem++ ) {
                rect bound  = addOffset( addRadius( takeT( &sub_bound, textBox_height ), -2.0f ), Vec2( -2.0f, 0.0f ) );
                bound.left += 60.0f;
                
                uint32 index = ( uint32 )( utilityPanel_textBoxID_directionX + iElem );
                u->textBox[ index ] = TextBox( bound, cElem2 + iElem );
            }
            
            
            u->grid_isActive = true;
            u->scale         = 0.1f;
            u->degrees       = 1.0f;
            u->direction     = Vec3( 1.0f, 0.0f, 0.0f );
        }
        {
            ENTITY_PANEL * e = &editorState->entityPanel;
            e->bound = addRadius( takeT( &take_bound, 200.0f ), -2.0f );
            
            e->hover  = -1;
            e->active = -1;
            
            addButton( e ); // spawn point
            addButton( e ); // ledge
            addButton( e ); // secure door
            addButton( e ); // pixie
            addButton( e ); // kill zone
            addButton( e ); // checkpoint
            addButton( e ); // view
        }
        {
            editorState->doorPanel_bound = addRadius( takeT( &take_bound, getWidth( take_bound ) * 0.25f ), -2.0f );
        }
        {
            LEVEL_PANEL * l = &editorState->levelPanel;
            l->bound         = addRadius( takeT( &take_bound, 200.0f ), -2.0f );
            l->addButton_dim = Vec2( 48.0f, 24.0f );
            l->entry_dim     = Vec2( getWidth( l->bound ), 24.0f );
        }
        
        strcpy( editorState->entityDir, "../../art/entity" );
        
        addPoint( editorState, Vec3( -1.0f, 0.0f,  1.0f ) );
        addPoint( editorState, Vec3(  1.0f, 0.0f,  1.0f ) );
        addPoint( editorState, Vec3( -1.0f, 0.0f, -1.0f ) );
        addPoint( editorState, Vec3(  1.0f, 0.0f, -1.0f ) );
        addFace ( editorState, Face( 0, 1, 2, 3 ) );
        
        if( !undoSystem->isInitialized ) {
            undoSystem->isInitialized = true;
            undoSystem->memory_base = ( uint8 * )_pushSize( permMemory, UNDO_SYSTEM__MEMORY_SIZE );
        }
        
        computeFaceCentroids( editorState );
        computeFaceNormals  ( editorState );
        
        computeEdges        ( editorState );
        computeEdgeCentroids( editorState );
        
        editorState->drawFaces     = true;
        editorState->entity_select = -1;
	}
    
    { // count geometry
        uint32 selectedVertex_count = getSelectedCount_vertex( editorState );
        uint32 selectedEdge_count   = getSelectedCount_edge  ( editorState );
        uint32 selectedFace_count   = getSelectedCount_face  ( editorState );
        
        char debug_string[ 128 ] = {};
        sprintf( debug_string, "Vert: %u(%u), Edge: %u(%u), Face: %u(%u)", editorState->point_count, selectedVertex_count, editorState->edge_count, selectedEdge_count, editorState->face_count, selectedFace_count );
        // DISPLAY_STRING( debug_string );
    }
    
    // DISPLAY_VALUE( int32, editorState->entityFile_count );
    // DISPLAY_VALUE( int32, editorState->entityID );
    
    CAMERA * camera = &editorState->camera;
    updateCameraInput( editorState, camera, keyboard, getDim( editorState->displayWindow_bound ), app_dim, tempMemory );
    if( ( editorState->editorMode == editorMode_default ) ||
       ( editorState->editorMode == editorMode_placeEntity ) ) {
        flo32 dt = 1.0f / APP_targetFPS;
        updateCameraMovement( editorState, camera, keyboard, mouse, app_dim, dt );
    }
    
    // DISPLAY_VALUE( vec3, camera->position );
    // DISPLAY_VALUE( vec3, camera->xAxis );
    // DISPLAY_VALUE( vec3, camera->yAxis );
    // DISPLAY_VALUE( flo32, camera->dist );
    
    mat4 main_transform   = genCameraTransform( camera, app_dim );
    
    vec2 toolbarButton_dim = Vec2( 20.0f, 20.0f );
    rect toolbarButton_vertexSelect_bound = takeL( &toolbar_takeBound, toolbarButton_dim.x );
    rect toolbarButton_edgeSelect_bound   = takeL( &toolbar_takeBound, toolbarButton_dim.x );
    rect toolbarButton_faceSelect_bound   = takeL( &toolbar_takeBound, toolbarButton_dim.x );
    
    if( wasPressed( mouse, mouseButton_left ) && isInBound( mouse->position, toolbarButton_vertexSelect_bound ) ) { editorState->selectMode = selectMode_vertex; }
    if( wasPressed( mouse, mouseButton_left ) && isInBound( mouse->position, toolbarButton_edgeSelect_bound   ) ) { editorState->selectMode = selectMode_edge;   }
    if( wasPressed( mouse, mouseButton_left ) && isInBound( mouse->position, toolbarButton_faceSelect_bound   ) ) { editorState->selectMode = selectMode_face;   }
    
    drawRect( renderer, renderPassID_overlay, toolbar_bound, COLOR_GRAY( 0.25f ) );
    drawToolbarButton( renderer, renderPassID_overlay, toolbarButton_vertexSelect_bound, "V", editorState->selectMode == selectMode_vertex );
    drawToolbarButton( renderer, renderPassID_overlay, toolbarButton_edgeSelect_bound,   "E", editorState->selectMode == selectMode_edge );
    drawToolbarButton( renderer, renderPassID_overlay, toolbarButton_faceSelect_bound,   "F", editorState->selectMode == selectMode_face );
    
    {
        UTILITY_PANEL * u = &editorState->utilityPanel;
        u->centroid = getCentroidOfSelection( editorState );
        
        TEXT_BOX_F32 * t = u->textBox + utilityPanel_textBoxID_centroidX;
        if( u->centroid.isValid ) {
            flo32 * v = u->centroid.position.elem;
            for( uint32 iElem = 0; iElem < 3; iElem++ ) {
                t[ iElem ].output = v + iElem;
            }
        } else {
            for( uint32 iElem = 0; iElem < 3; iElem++ ) {
                t[ iElem ].output = 0;
            }
        }
    }
    
    boo32 saveEntity = false;
    boo32 openEntity = false;
    switch( editorState->editorMode ) {
        case editorMode_default: {
            if( wasPressed( keyboard, keyCode_z ) ) {
                editorState->drawFaces = !editorState->drawFaces;
            }
            if( wasPressed( keyboard, keyCode_z, KEYBOARD_FLAGS__CONTROL ) ) {
                undo( undoSystem, editorState, tempMemory );
            }
            if( wasPressed( keyboard, keyCode_y, KEYBOARD_FLAGS__CONTROL ) ) {
                redo( undoSystem, editorState, tempMemory );
            }
            if( wasPressed( keyboard, keyCode_a ) ) {
                uint32 total_count = editorState->point_count + editorState->edge_count + editorState->face_count;
                
                uint32 isSelected_count = 0;
                isSelected_count += getSelectedCount_vertex( editorState );
                isSelected_count += getSelectedCount_edge  ( editorState );
                isSelected_count += getSelectedCount_face  ( editorState );
                if( isSelected_count == total_count ) {
                    memset( editorState->point_isSelected, 0, sizeof( boo32 ) * editorState->point_count );
                    memset( editorState->edge_isSelected,  0, sizeof( boo32 ) * editorState->edge_count  );
                    memset( editorState->face_isSelected,  0, sizeof( boo32 ) * editorState->face_count  );
                } else {
                    memset( editorState->point_isSelected, 1, sizeof( boo32 ) * editorState->point_count );
                    memset( editorState->edge_isSelected,  1, sizeof( boo32 ) * editorState->edge_count  );
                    memset( editorState->face_isSelected,  1, sizeof( boo32 ) * editorState->face_count  );
                }
            }
            if( wasPressed( keyboard, keyCode_d, KEYBOARD_FLAGS__CONTROL ) ) {
                DIRECTION D = getDirectionOfSelection( editorState );
                if( D.isValid ) {
                    UTILITY_PANEL * u = &editorState->utilityPanel;
                    u->direction = D.direction;
                }
            }
            if( wasPressed( mouse, mouseButton_left ) ) {
                if( isInBound( mouse->position, editorState->displayWindow_bound ) ) {
                    editorState->boxSelect_isEnabled = true;
                    editorState->mouse_prevPos       = mouse->position;
                    
                    // TODO : handle selection of duplicated points
                    if( !( keyboard->flags & KEYBOARD_FLAGS__SHIFT ) ) {
                        memset( editorState->point_isSelected, 0, sizeof( boo32 ) * editorState->point_count );
                        memset( editorState->edge_isSelected,  0, sizeof( boo32 ) * editorState->edge_count  );
                        memset( editorState->face_isSelected,  0, sizeof( boo32 ) * editorState->face_count  );
                    }
                    
                    flo32 range   = 5.0f;
                    flo32 rangeSq = range * range;
                    
                    vec3  * point_array      = editorState->point;
                    uint32  point_count      = ( uint32 )editorState->point_count;
                    boo32 * point_isSelected = editorState->point_isSelected;
                    
                    if( editorState->selectMode == selectMode_edge ) {
                        point_array      = editorState->edge_centroid;
                        point_count      = ( uint32 )editorState->edge_count;
                        point_isSelected = editorState->edge_isSelected;
                    }
                    if( editorState->selectMode == selectMode_face ) {
                        point_array      = editorState->face_centroid;
                        point_count      = ( uint32 )editorState->face_count;
                        point_isSelected = editorState->face_isSelected;
                    }
                    
                    int32 closestIndex = -1;
                    flo32 maxDistSq    = rangeSq;
                    for( uint32 point_index = 0; point_index < point_count; point_index++ ) {
                        vec2 screenPos = toScreenCoord( main_transform, point_array[ point_index ] ) * app_dim;
                        flo32 distSq   = getLengthSq( mouse->position - screenPos );
                        if( distSq < maxDistSq ) {
                            closestIndex = point_index;
                            maxDistSq    = distSq;
                        }
                    }
                    
                    if( closestIndex != -1 ) {
                        point_isSelected[ closestIndex ] = !point_isSelected[ closestIndex ];
                        updateSelectedVertices( editorState );
                    }
                } else if( isInBound( mouse->position, editorState->utilityPanel.bound ) ) {
                    UTILITY_PANEL * u = &editorState->utilityPanel;
                    CENTROID p = getCentroidOfSelection( editorState );
                    for( uint32 iTextBox = 0; iTextBox < utilityPanel_textBoxID_count; iTextBox++ ) {
                        TEXT_BOX_F32 * t = u->textBox + iTextBox;
                        if( t->output && isInBound( mouse->position, t->bound ) ) {
                            INPUT_STRING * str = &editorState->input_string;
                            init( &editorState->input_string, t, u, mouse, 16 );                 
                            editorState->editorMode = editorMode_utilityPanel_textBox;
                        }
                    }
                    if( isInBound( mouse->position, u->grid_isActive_bound ) ) {
                        u->grid_isActive = !u->grid_isActive;
                    }
                    if( isInBound( mouse->position, u->pivot_isActive_bound ) ) {
                        u->pivot_isActive = !u->pivot_isActive;
                    }
                    if( isInBound( mouse->position, u->pivot_setPivot_bound ) ) {
                        CENTROID P = getCentroidOfSelection( editorState );
                        if( P.isValid ) {
                            u->pivot = P.position;
                        }
                    }
                    if( isInBound( mouse->position, u->direction_setDirection_bound ) ) {
                        DIRECTION D = getDirectionOfSelection( editorState );
                        if( D.isValid ) {
                            u->direction = D.direction;
                        }
                    }
                }
            }
            if( ( editorState->boxSelect_isEnabled ) && ( isDown( mouse, mouseButton_left ) ) &&
               ( isInBound( mouse->position, editorState->displayWindow_bound ) ) && ( getLengthSq( mouse->position - editorState->mouse_prevPos ) > 64.0f ) ) {
                editorState->boxSelect_isEnabled = false;
                editorState->boxSelect_isActive  = true;
                editorState->boxSelect_position  = editorState->mouse_prevPos;
            }
            if( wasReleased( mouse, mouseButton_left ) ) {
                if( editorState->boxSelect_isActive ) {
                    vec3  * point_array      = editorState->point;
                    uint32  point_count      = ( uint32 )editorState->point_count;
                    boo32 * point_isSelected = editorState->point_isSelected;
                    
                    if( editorState->selectMode == selectMode_edge ) {
                        point_array      = editorState->edge_centroid;
                        point_count      = ( uint32 )editorState->edge_count;
                        point_isSelected = editorState->edge_isSelected;
                    }
                    if( editorState->selectMode == selectMode_face ) {
                        point_array      = editorState->face_centroid;
                        point_count      = ( uint32 )editorState->face_count;
                        point_isSelected = editorState->face_isSelected;
                    }
                    
                    for( uint32 point_index = 0; point_index < point_count; point_index++ ) {
                        vec3 P = point_array[ point_index ];
                        boo32 isValid = isInFrontOfCamera( camera, P );
                        if( isValid ) {
                            vec2 screenPos = toScreenCoord( main_transform, P ) * app_dim;
                            if( isInBound( screenPos, editorState->boxSelect_bound ) ) {
                                point_isSelected[ point_index ] = true;
                            }
                        }
                    }
                    
                    updateSelectedVertices( editorState );
                }
                editorState->boxSelect_isEnabled = false;
                editorState->boxSelect_isActive = false;
            }
            if( editorState->boxSelect_isActive ) {
                editorState->boxSelect_bound = rectCompare( editorState->boxSelect_position, mouse->position );
            }
            
            if( wasPressed( keyboard, keyCode_f ) ) {
                uint32 selectedVertex_count = getSelectedCount_vertex( editorState );
                uint32 selectedEdge_count   = getSelectedCount_edge  ( editorState );
                
                boo32 addNewEdge = ( editorState->selectMode == selectMode_vertex ) && ( selectedVertex_count == 2 );
                boo32 addNewFace = ( ( editorState->selectMode == selectMode_vertex ) || ( editorState->selectMode == selectMode_edge ) ) &&
                    ( ( selectedVertex_count == 3 ) || ( selectedVertex_count == 4 ) );
                
                if( addNewEdge ) {
                    uint32 index[ 2 ] = {};
                    getSelected_vertex( editorState, index, selectedVertex_count );
                    EDGE__ edge = Edge__( index[ 0 ], index[ 1 ] );
                    addEdge( editorState, edge, true );
                    add_newEdge( undoSystem, edge, SnapMemory( tempMemory ) );
                }
                if( addNewFace ) {
                    uint32 index[ 4 ] = {};
                    getSelected_vertex( editorState, index, selectedVertex_count );
                    
                    if( selectedVertex_count == 4 ) {
                        // NOTE: assumes points are coplanar
                        // NOTE: assumes no 3 points are collinear
                        // NOTE: assumes area is not degenerate
                        
                        vec3 A = editorState->point[ index[ 0 ] ];
                        vec3 B = editorState->point[ index[ 1 ] ];
                        vec3 C = editorState->point[ index[ 2 ] ];
                        vec3 D = editorState->point[ index[ 3 ] ];
                        
                        vec3 P = ( A + B + C + D ) * 0.25f;
                        vec3 V = getNormal( A - P );
                        vec3 X = getNormal( B - P );
                        vec3 Y = getNormal( C - P );
                        vec3 Z = getNormal( D - P );
                        
                        if( dot( V, X ) < dot( V, Y ) ) {
                            _swap( uint32, index[ 1 ], index[ 2 ] );
                            _swap( vec3, X, Y );
                        }
                        if( dot( V, Z ) < dot( V, Y ) ) {
                            _swap( uint32, index[ 2 ], index[ 3 ] );
                            _swap( vec3, Y, Z );
                        }
                        _swap( uint32, index[ 2 ], index[ 3 ] );
                    }
                    
                    FACE face = Face( index[ 0 ], index[ 1 ], index[ 2 ], ( selectedVertex_count == 4 ) ? index[ 3 ] : -1 );
                    addFace( editorState, face, true );
                    
                    
                    if( selectedVertex_count == 4 ) {
                        // NOTE: The standard order for face indices is 0, 1, 3, 2 CCW along the outer edge. (This is for drawing purposes)
                        // This swap is to reorder the indices to create the correct edges using just a for loop.
                        _swap( uint32, index[ 2 ], index[ 3 ] );
                    }
                    
                    uint32 oldEdge_count = 0;
                    uint32 newEdge_count = 0;
                    uint32 oldEdge[ 4 ] = {};
                    EDGE__ newEdge[ 4 ] = {};
                    
                    for( uint32 vert_index = 0; vert_index < selectedVertex_count; vert_index++ ) {
                        uint32 index0 = index[ vert_index ];
                        uint32 index1 = index[ ( vert_index + 1 ) % selectedVertex_count ];
                        
                        EDGE__ edge         = Edge__( index0, index1 );
                        int32  edge_atIndex = getEdgeIndex( editorState, edge );
                        if( edge_atIndex == -1 ) {
                            newEdge[ newEdge_count++ ] = edge;
                            addEdge( editorState, edge, true );
                        } else {
                            oldEdge[ oldEdge_count++ ] = edge_atIndex;
                            editorState->edge_isSelected[ edge_atIndex ] = true;
                        }
                    }
                    
                    add_newFace( undoSystem, face, oldEdge, oldEdge_count, newEdge, newEdge_count, SnapMemory( tempMemory ) );
                }
            }
            if( wasPressed( keyboard, keyCode_d, KEYBOARD_FLAGS__SHIFT ) ) {
                if( editorState->selectMode == selectMode_vertex ) {
                    uint32 isSelected_count = getSelectedCount_vertex( editorState );
                    if( isSelected_count > 0 ) {
                        uint32   duplicated_count   = 0;
                        uint32 * duplicated_indices = _pushArray( tempMemory, uint32, isSelected_count );
                        
                        int32 point_count = editorState->point_count;
                        for( int32 point_index = 0; point_index < point_count; point_index++ ) {
                            if( editorState->point_isSelected[ point_index ] ) {
                                duplicated_indices[ duplicated_count++ ] = point_index;
                                
                                addPoint( editorState, editorState->point[ point_index ], true );
                                editorState->point_isSelected[ point_index ] = false;
                                editorState->entity_hasUnsavedChanges        = true;
                            }
                        }
                        
                        Assert( duplicated_count == isSelected_count );
                        add_duplicateVertex( undoSystem, duplicated_indices, duplicated_count, editorState, SnapMemory( tempMemory ) );
                        
                        _popArray( tempMemory, uint32, isSelected_count );
                        
                        initVertexEdit( editorState, tempMemory, editorMode_translateSelection );
                    }
                }
                if( editorState->selectMode == selectMode_edge ) {
                    uint32 selectedEdge_count  = getSelectedCount_edge( editorState );
                    if( selectedEdge_count > 0 ) {
                        uint32 selectedVertex_count = getSelectedCount_vertex( editorState );
                        
                        uint32 dupPoint_count = 0;
                        uint32 dupEdge_count  = 0;
                        
                        uint32 * curIndex = _pushArray( tempMemory, uint32, selectedVertex_count );
                        uint32 * newIndex = _pushArray( tempMemory, uint32, selectedVertex_count );
                        uint32 * dup_edge = _pushArray( tempMemory, uint32, selectedEdge_count );
                        
                        int32 point_count = editorState->point_count;
                        for( int32 point_index = 0; point_index < point_count; point_index++ ) {
                            if( editorState->point_isSelected[ point_index ] ) {
                                curIndex[ dupPoint_count ] = point_index;
                                newIndex[ dupPoint_count ] = addPoint( editorState, editorState->point[ point_index ], true );
                                editorState->point_isSelected[ point_index ] = false;
                                dupPoint_count++;
                                
                                editorState->entity_hasUnsavedChanges        = true;
                            }
                        }
                        Assert( dupPoint_count == selectedVertex_count );
                        
                        int32 edge_count = editorState->edge_count;
                        for( int32 edge_index = 0; edge_index < edge_count; edge_index++ ) {
                            if( editorState->edge_isSelected[ edge_index ] ) {
                                dup_edge[ dupEdge_count++ ] = edge_index;
                                
                                EDGE__ oldEdge = editorState->edge[ edge_index ];
                                EDGE__ newEdge = updateToNewEdge( oldEdge, curIndex, newIndex, dupPoint_count );
                                
                                addEdge( editorState, newEdge, true );
                                editorState->edge_isSelected[ edge_index ] = false;
                                
                                editorState->entity_hasUnsavedChanges      = true;
                            }
                        }
                        
                        add_duplicateEdge( undoSystem, dup_edge, selectedEdge_count, curIndex, selectedVertex_count, editorState, SnapMemory( tempMemory ) );
                        
                        _popArray( tempMemory, uint32, selectedVertex_count );
                        _popArray( tempMemory, uint32, selectedVertex_count );
                        _popArray( tempMemory, uint32, selectedEdge_count );
                        
                        initVertexEdit( editorState, tempMemory, editorMode_translateSelection );
                    }
                }
                if( editorState->selectMode == selectMode_face ) {
                    uint32 selectedFace_count = getSelectedCount_face( editorState );
                    if( selectedFace_count > 0 ) {
                        uint32 selectedVertex_count = getSelectedCount_vertex( editorState );
                        uint32 selectedEdge_count   = getSelectedCount_edge  ( editorState );
                        Assert( selectedEdge_count > 0 );
                        
                        uint32 dupPoint_count = 0;
                        uint32 dupEdge_count  = 0;
                        uint32 dupFace_count  = 0;
                        
                        uint32 * curIndex = _pushArray( tempMemory, uint32, selectedVertex_count );
                        uint32 * newIndex = _pushArray( tempMemory, uint32, selectedVertex_count );
                        uint32 * dup_edge = _pushArray( tempMemory, uint32, selectedEdge_count );
                        uint32 * dup_face = _pushArray( tempMemory, uint32, selectedFace_count );
                        
                        int32 point_count = editorState->point_count;
                        for( int32 point_index = 0; point_index < point_count; point_index++ ) {
                            if( editorState->point_isSelected[ point_index ] ) {
                                curIndex[ dupPoint_count ] = point_index;
                                newIndex[ dupPoint_count ] = addPoint( editorState, editorState->point[ point_index ], true );
                                editorState->point_isSelected[ point_index ] = false;
                                dupPoint_count++;
                                
                                editorState->entity_hasUnsavedChanges        = true;
                            }
                        }
                        Assert( dupPoint_count == selectedVertex_count );
                        
                        int32 edge_count = editorState->edge_count;
                        for( int32 edge_index = 0; edge_index < edge_count; edge_index++ ) {
                            if( editorState->edge_isSelected[ edge_index ] ) {
                                dup_edge[ dupEdge_count++ ] = edge_index;
                                
                                EDGE__ oldEdge = editorState->edge[ edge_index ];
                                EDGE__ newEdge = updateToNewEdge( oldEdge, curIndex, newIndex, dupPoint_count );
                                
                                addEdge( editorState, newEdge, true );
                                editorState->edge_isSelected[ edge_index ] = false;
                                
                                editorState->entity_hasUnsavedChanges      = true;
                            }
                        }
                        
                        int32 face_count = editorState->face_count;
                        for( int32 face_index = 0; face_index < face_count; face_index++ ) {
                            if( editorState->face_isSelected[ face_index ] ) {
                                dup_face[ dupFace_count++ ] = face_index;
                                
                                FACE oldFace = editorState->face[ face_index ];
                                FACE newFace = updateToNewFace( oldFace, curIndex, newIndex, dupPoint_count );
                                
                                addFace( editorState, newFace, true );
                                editorState->face_isSelected[ face_index ] = false;
                                
                                editorState->entity_hasUnsavedChanges      = true;
                            }
                        }
                        
                        add_duplicateFace( undoSystem, dup_face, selectedFace_count, dup_edge, selectedEdge_count, curIndex, selectedVertex_count, editorState, SnapMemory( tempMemory ) );
                        
                        _popArray( tempMemory, uint32, selectedVertex_count );
                        _popArray( tempMemory, uint32, selectedVertex_count );
                        _popArray( tempMemory, uint32, selectedEdge_count );
                        _popArray( tempMemory, uint32, selectedFace_count );
                        
                        initVertexEdit( editorState, tempMemory, editorMode_translateSelection );
                    }
                }
            }
            if( wasPressed( keyboard, keyCode_e ) ) {
                if( editorState->selectMode == selectMode_vertex ) {
                    uint32 nVertex = getSelectedCount( editorState );
                    
                    if( nVertex > 0 ) {
                        uint32 * vert = _pushArray( tempMemory, uint32, nVertex );
                        getSelected_vertex( editorState, vert, nVertex );
                        
                        clearSelectedPoints( editorState );
                        for( uint32 iVert = 0; iVert < nVertex; iVert++ ) {
                            uint32 index0 = vert[ iVert ];
                            uint32 index1 = addPoint( editorState, editorState->point[ index0 ], true );
                            
                            EDGE__ e = Edge__( index0, index1 );
                            addEdge( editorState, e, false );
                        }
                        
                        add_extrudeVertex( undoSystem, vert, nVertex, SnapMemory( tempMemory ) );
                        _popArray( tempMemory, uint32, nVertex );
                        
                        initVertexEdit( editorState, tempMemory, editorMode_translateSelection );
                    }
                }
                if( editorState->selectMode == selectMode_edge ) {
                    uint32 nEdge = getSelectedCount( editorState );
                    
                    if( nEdge > 0 ) {
                        INDEX_LIST__ selectEdge = IndexList( tempMemory, nEdge );
                        getSelected_edge( editorState, &selectEdge );
                        
                        EDGE_LIST__  newEdge  = EdgeList ( tempMemory, nEdge * 3 );
                        INDEX_LIST__ newPoint = IndexList( tempMemory, nEdge * 2 );
                        
                        FACE_LIST__ newFace = {};
                        newFace.list     = editorState->face + editorState->face_count;
                        newFace.count    = nEdge;
                        newFace.maxCount = nEdge;
                        
                        uint32 bPoint = editorState->point_count;
                        for( uint32 iEdge = 0; iEdge < nEdge; iEdge++ ) {
                            EDGE__ e = editorState->edge[ selectEdge.list[ iEdge ] ];
                            
                            uint32 p0 = bPoint + add_noDup( &newPoint, e.index[ 0 ] );
                            uint32 p1 = bPoint + add_noDup( &newPoint, e.index[ 1 ] );
                            
                            EDGE__ e0 = Edge__( e.index[ 0 ], p0 );
                            EDGE__ e1 = Edge__( e.index[ 1 ], p1 );
                            EDGE__ e2 = Edge__( p0, p1 );
                            
                            add_noDup( &newEdge, e0 );
                            add_noDup( &newEdge, e1 );
                            add_noDup( &newEdge, e2 );
                            
                            FACE f = Face( e.index[ 0 ], p0, e.index[ 1 ], p1 );
                            addFace( editorState, f, false );
                        }
                        
                        clearSelectedPoints( editorState );
                        clearSelectedEdges ( editorState );
                        
                        for( uint32 iPoint = 0; iPoint < newPoint.count; iPoint++ ) {
                            uint32 p0 = newPoint.list[ iPoint ];
                            addPoint( editorState, editorState->point[ p0 ], true );
                        }
                        for( uint32 iEdge = 0; iEdge < newEdge.count; iEdge++ ) {
                            EDGE__ e = newEdge.list[ iEdge ];
                            
                            boo32 isSelected0 = editorState->point_isSelected[ e.index [ 0 ] ];
                            boo32 isSelected1 = editorState->point_isSelected[ e.index [ 1 ] ];
                            addEdge( editorState, e, ( isSelected0 && isSelected1 ) );
                        }
                        
                        add_extrudeEdge( undoSystem, editorState, selectEdge, newPoint, newEdge, newFace, SnapMemory( tempMemory ) );
                        
                        pop( tempMemory, newEdge );
                        pop( tempMemory, newPoint );
                        pop( tempMemory, selectEdge );
                        
                        initVertexEdit( editorState, tempMemory, editorMode_translateSelection );
                    }
                }
            }
            if( wasPressed( keyboard, keyCode_m ) ) {
                uint32 nSelect = getSelectedCount_vertex( editorState );
                if( nSelect > 0 ) {
                    UTILITY_PANEL * u = &editorState->utilityPanel;
                    
                    vec3 O = getMeanOfSelection( editorState );
                    vec3 N = u->direction;
                    
                    mat4 transform = getReflectTransform( O, N );
                    
                    uint64 used = tempMemory->used;
                    uint32 * selected = _pushArray( tempMemory, uint32, nSelect );
                    vec3   * oldPos   = _pushArray( tempMemory, vec3,   nSelect );
                    vec3   * newPos   = _pushArray( tempMemory, vec3,   nSelect );
                    
                    getSelected_vertex( editorState, selected, nSelect );
                    for( uint32 iPoint = 0; iPoint < nSelect; iPoint++ ) {
                        uint32 index = selected[ iPoint ];
                        vec3 P  = editorState->point[ index ];
                        vec3 P0 = ( transform * Vec4( P, 1.0f ) ).xyz;
                        
                        editorState->point[ index ] = P0;
                        
                        oldPos[ iPoint ] = P;
                        newPos[ iPoint ] = P0;
                    }
                    
                    add_modifyVertex( undoSystem, nSelect, selected, oldPos, newPos, SnapMemory( tempMemory ) );
                    
                    tempMemory->used = used;
                }
            }
            if( wasPressed( keyboard, keyCode_n ) ) {
                if( editorState->selectMode == selectMode_face ) {
                    uint32 selectedFace_count = getSelectedCount_face( editorState );
                    if( selectedFace_count > 0 ) {
                        uint32 * index = _pushArray( tempMemory, uint32, selectedFace_count );
                        getSelected_face( editorState, index, selectedFace_count );
                        
                        for( uint32 face_index = 0; face_index < selectedFace_count; face_index++ ) {
                            flipNormal( editorState->face + index[ face_index ] );
                        }
                        
                        add_flipNormal( undoSystem, index, selectedFace_count, SnapMemory( tempMemory ) );
                        
                        _popArray( tempMemory, uint32, selectedFace_count );
                    }
                }
                
            }
            
            if( wasPressed( keyboard, keyCode_g ) ) {
                uint32 isSelected_count = getSelectedCount_vertex( editorState );
                if( isSelected_count > 0 ) {
                    initVertexEdit( editorState, tempMemory, editorMode_translateSelection );
                }
            }
            if( wasPressed( keyboard, keyCode_r ) ) {
                uint32 nSelect = getSelectedCount_vertex( editorState );
                if( nSelect > 0 ) {
                    initVertexEdit( editorState, tempMemory, editorMode_rotateSelection );
                }
            }
            if( wasPressed( keyboard, keyCode_s ) ) {
                uint32 nSelect = getSelectedCount_vertex( editorState );
                if( nSelect > 0 ) {
                    initVertexEdit( editorState, tempMemory, editorMode_scaleSelection );
                }
            }
            if( wasPressed( keyboard, keyCode_x ) ) {
                // TODO : Hack!! Using permMemory because tempMemory is too tangled in the undo system. Maybe I could do like a stack for SnapMemory to pop multiple levels of memory.
                EDITOR__ENTITY_POSITION entityPos = {};
                CaptureEntityPositionForDelete( editorState, &entityPos, SnapMemory( permMemory ) );
                
                if( editorState->selectMode == selectMode_vertex ) {
                    uint32 nSelect = getSelectedCount( editorState );
                    if( nSelect > 0 ) {
                        add_deleteSelection( undoSystem, editorState, SnapMemory( tempMemory ) );
                        deleteSelected( editorState, tempMemory );
                    }
                }
                if( editorState->selectMode == selectMode_edge ) {
                    uint32 selectedEdge_count = getSelectedCount_edge( editorState );
                    if( selectedEdge_count > 0 ) {
                        uint32 * edge_array = _pushArray( tempMemory, uint32, selectedEdge_count );
                        getSelected_edge( editorState, edge_array, selectedEdge_count );
                        
                        for( uint32 iEdge = 0; iEdge < selectedEdge_count; iEdge++ ) {
                            EDGE__ e = editorState->edge[ edge_array[ iEdge ] ];
                            EDGE__CONNECTED_FACE c = findConnectedFace( editorState, e );
                            if( c.face[ 0 ] != -1 ) {
                                editorState->face_isSelected[ c.face[ 0 ] ] = true;
                            }
                            if( c.face[ 1 ] != -1 ) {
                                editorState->face_isSelected[ c.face[ 1 ] ] = true;
                            }
                        }
                        
                        uint32 selectedFace_count = getSelectedCount_face( editorState );
                        uint32 * face_array = _pushArray( tempMemory, uint32, selectedFace_count );
                        getSelected_face( editorState, face_array, selectedFace_count );
                        
                        add_deleteEdges( undoSystem, editorState, edge_array, selectedEdge_count, face_array, selectedFace_count, SnapMemory( tempMemory ) );
                        
                        deleteSelectedEdges( editorState );
                        deleteSelectedFaces( editorState );
                        
                        _popArray( tempMemory, uint32, selectedEdge_count );
                        _popArray( tempMemory, uint32, selectedFace_count );
                    }
                }
                if( editorState->selectMode == selectMode_face ) {
                    uint32 selectedFace_count = getSelectedCount_face( editorState );
                    if( selectedFace_count > 0 ) {
                        uint32 * face_array = _pushArray( tempMemory, uint32, selectedFace_count );
                        getSelected_face( editorState, face_array, selectedFace_count );
                        
                        add_deleteFaces( undoSystem, editorState, face_array, selectedFace_count, SnapMemory( tempMemory ) );
                        
                        deleteSelectedFaces( editorState );
                        
                        _popArray( tempMemory, uint32, selectedFace_count );
                    }
                }
                
                FindPointIndicesForEntityPositions( editorState, &entityPos );
                popToSnap( permMemory );
            }
            updateEntityPanel( editorState, mouse );
            updateLevelPanel ( editorState, keyboard, mouse );
            
            if( wasPressed( keyboard, keyCode_s, KEYBOARD_FLAGS__CONTROL ) ) {
                if( editorState->levelEditor_isActive ) {
                    saveLevel( &editorState->levelPanel, SnapMemory( tempMemory ) );
                } else {
                    if( editorState->entityID == -1 ) {
                        editorState->editorMode = editorMode_setEntityID;
                        init( &editorState->input_string, 4 );
                    } else {
                        saveEntity = true;
                    }
                }
            }
            if( wasPressed( keyboard, keyCode_o, KEYBOARD_FLAGS__CONTROL ) ) {
                editorState->editorMode = editorMode_openEntity;
                init( &editorState->input_string, 4 );
            }
            
            if( wasPressed( keyboard, keyCode_a, KEYBOARD_FLAGS__CONTROL ) ) {
                selectAllConnectedToSelection( editorState, tempMemory );
            }
            
            if( wasPressed( keyboard, keyCode_l, KEYBOARD_FLAGS__CONTROL ) ) {
                editorState->levelEditor_isActive = true;
                resetEditorState( editorState );
                loadLevel( editorState, permMemory, tempMemory );
            }
        } break;
        
        case editorMode_placeEntity: {
            updateEntityPanel( editorState, mouse );
            switch( editorState->entityMode ) {
                case entityMode_spawn: {
                    if( isInBound( mouse->position, editorState->displayWindow_bound ) ) {
                        boo32 isValid     = false;
                        vec3  spawn_point = {};
                        
                        RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                        
                        RAY_INTERSECT intersect = doesIntersectFirstWalkableFace( editorState, select );
                        if( intersect.isValid ) {
                            isValid = true;
                            spawn_point = intersect.P;
                        }
                        
                        if( isValid ) {
                            if( wasPressed( mouse, mouseButton_left ) ) {
                                editorState->spawn_useSpawnPoint = true;
                                editorState->spawn_spawnPoint    = spawn_point;
                            }
                            if( wasPressed( mouse, mouseButton_right ) ) {
                                editorState->spawn_useSpawnPoint = false;
                            }
                            drawLine( renderer, renderPassID_main_debug, spawn_point, spawn_point + Vec3( 0.0f, 1.0f, 0.0f ), COLOR_MAGENTA );
                        }
                        if( editorState->spawn_useSpawnPoint ) {
                            drawLine( renderer, renderPassID_main_debug, editorState->spawn_spawnPoint, editorState->spawn_spawnPoint + Vec3( 0.0f, 1.0f, 0.0f ), COLOR_MAGENTA );
                        }
                    }
                } break;
                
                case entityMode_ledge: {
                    if( isInBound( mouse->position, editorState->displayWindow_bound ) ) {
                        EDITOR__LEDGE ledge   = {};
                        boo32         isValid = false;
                        
                        RAY   select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                        int32 bEdge  = getBestEdge ( editorState, select );
                        int32 bLedge = getBestLedge( editorState, select );
                        
                        if( bEdge != -1 ) {
                            EDGE__ edge = editorState->edge[ bEdge ];
                            if( isValidLedge( editorState, edge ) ) {
                                isValid = true;
                                ledge   = toLedge( edge );
                                
                                // TODO : This code aligns a ledge to a face normal if a face is selected. I can add this code later if its too much of a hassle to keep manually flipping ledges to the correct facing direction.
                                //if( bFace != -1 ) {
                                //vec3 normal = getNormal( editorState, face );
                                //if( isOrthogonalTo( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) ) {
                                //flo32 d = dot( cross( ledge.point[ 1 ] - ledge.point[ 0 ], normal ), Vec3( 0.0f, 1.0f, 0.0f ) );
                                //if( d < 0.0f ) {
                                //flip( &ledge );
                                //}
                                //}
                                //}
                            }
                        }
                        
                        
                        if( bLedge != -1 ) {
                            if( wasPressed( mouse, mouseButton_left ) ) {
                                flip( editorState->ledge + bLedge );
                            }
                            if( wasPressed( mouse, mouseButton_right ) ) {
                                deleteLedge( editorState, bLedge );
                            }
                        } else if( isValid ) {
                            drawLedge( renderer, renderPassID_main, editorState, ledge );
                            if( wasPressed( mouse, mouseButton_left ) ) {
                                addLedge( editorState, ledge );
                            }
                        }
                    }
                } break;
                
                case entityMode_door: {
                    if( isInBound( mouse->position, editorState->displayWindow_bound ) ) {
                        RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                        
                        int32 bDoor  = getBestDoor( editorState, select );
                        int32 bPoint = getBestPoint( editorState, select.origin, select.vector, 0.15f );
                        
                        if( wasPressed( mouse, mouseButton_left ) ) {
                            if( bDoor != -1 ) {
                                editorState->entity_select = bDoor;
                            } else if( bPoint != -1 ) {
                                EDITOR__SECURE_DOOR door = {};
                                door.point_index = bPoint;
                                door.height      = 4.0f;
                                editorState->entity_select = addSecureDoor( editorState, door );
                            } else {
                                editorState->entity_select = -1;
                            }
                        }
                        
                        if( ( bDoor == -1 ) && ( bPoint != -1 ) ) {
                            EDITOR__SECURE_DOOR d = {};
                            d.point_index = bPoint;
                            d.height      = 4.0f;
                            
                            SECURE_DOOR door = toSecureDoor( editorState, d );
                            drawBoxOutline( renderer, renderPassID_main, door.activate_bound, COLOR_CYAN );
                            drawBoxOutline( renderer, renderPassID_main, door.render_bound,   COLOR_MAGENTA );
                        }
                        
                        if( editorState->entity_select != -1 ) {
                            if( wasPressed( keyboard, keyCode_r ) ) {
                                editorState->secureDoor_temp = editorState->secureDoor[ editorState->entity_select ];
                                editorState->editorMode = editorMode_rotateDoor;
                                init( &editorState->input_string );
                            }
                            if( wasPressed( keyboard, keyCode_s ) ) {
                                editorState->secureDoor_temp = editorState->secureDoor[ editorState->entity_select ];
                                editorState->editorMode = editorMode_sizeDoor;
                                init( &editorState->input_string );
                            }
                        }
                    }
                    if( isInBound( mouse->position, editorState->doorPanel_bound ) ) {
                        if( editorState->entity_select != -1 ) {
                            EDITOR__SECURE_DOOR * sDoor = editorState->secureDoor + editorState->entity_select;
                            
                            rect doorPanel_bound  = editorState->doorPanel_bound;
                            flo32 dim = getHeight( doorPanel_bound );
                            rect pBound[ 4 ] = {};
                            for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                                pBound[ iPixie ] = addRadius( rectTLD( getTL( doorPanel_bound ) + Vec2( dim * ( flo32 )iPixie, 0.0f ), Vec2( dim, dim ) ), -2.0f );
                            }
                            
                            if( mouse->wheelClicks != 0 ) {
                                int32 bBound = -1;
                                for( uint32 iBound = 0; iBound < 4; iBound++ ) {
                                    if( isInBound( mouse->position, pBound[ iBound ] ) ) {
                                        // DISPLAY_VALUE( rect, pBound[ iBound ] );
                                        bBound = iBound;
                                    }
                                }
                                if( bBound != -1 ) {
                                    sDoor->pixie_cost[ bBound ] = ( sDoor->pixie_cost[ bBound ] + mouse->wheelClicks + SECURE_DOOR__MAX_PIXIE_COST ) % SECURE_DOOR__MAX_PIXIE_COST;
                                }
                            }
                        }
                    }
                } break;
                
                case entityMode_pixie: {
                    RAY   select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                    int32 bPoint = getBestPoint ( editorState, select );
                    int32 pixie_index = getFirstPixie( editorState, select );
                    vec3  P = editorState->point[ bPoint ];
                    
                    if( ( pixie_index == -1 ) && ( bPoint != -1 ) ) {
                        drawPixie( renderer, renderPassID_main, P );
                    }
                    
                    if( wasPressed( mouse, mouseButton_left ) ) {
                        if( pixie_index != -1 ) {
                            editorState->entity_select = pixie_index;
                        } else if( bPoint != -1 ) {
                            editorState->entity_select = addPixie( editorState, bPoint );
                        }
                    }
                    if( wasPressed( mouse, mouseButton_right ) ) {
                        if( pixie_index != -1 ) {
                            deletePixie( editorState, pixie_index );
                        }
                    }
                    if( isInBound( mouse->position, editorState->doorPanel_bound ) ) {
                        if( editorState->entity_select != -1 ) {
                            EDITOR__PIXIE * sPixie = editorState->pixie + editorState->entity_select;
                            
                            rect doorPanel_bound  = editorState->doorPanel_bound;
                            flo32 dim = getHeight( doorPanel_bound );
                            rect pBound[ 4 ] = {};
                            for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                                pBound[ iPixie ] = addRadius( rectTLD( getTL( doorPanel_bound ) + Vec2( dim * ( flo32 )iPixie, 0.0f ), Vec2( dim, dim ) ), -2.0f );
                            }
                            
                            if( wasPressed( mouse, mouseButton_left ) ) {
                                int32 bBound = -1;
                                for( uint32 iBound = 0; iBound < 4; iBound++ ) {
                                    if( isInBound( mouse->position, pBound[ iBound ] ) ) {
                                        bBound = iBound;
                                    }
                                }
                                if( bBound != -1 ) {
                                    sPixie->typeID = ( PIXIE_TYPE_ID )bBound;
                                }
                            }
                        }
                    }
                } break;
                
                case entityMode_killZone: {
                    RAY   select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                    int32 bPoint    = getBestPoint ( editorState, select );
                    int32 bKillZone = getFirstKillZone( editorState, select );
                    vec3  P = editorState->point[ bPoint ];
                    
                    if( ( bKillZone == -1 ) && ( bPoint != -1 ) ) {
                        drawKillZone( renderer, renderPassID_main, P );
                    }
                    
                    if( wasPressed( mouse, mouseButton_left ) ) {
                        if( bKillZone != -1 ) {
                            editorState->entity_select = bKillZone;
                        } else if( bPoint != -1 ) {
                            editorState->entity_select = addKillZone( editorState, bPoint );
                        }
                    }
                    if( wasPressed( mouse, mouseButton_right ) ) {
                        if( bKillZone != -1 ) {
                            deleteKillZone( editorState, bKillZone );
                        }
                    }
                    if( editorState->entity_select != -1 ) {
                        if( wasPressed( keyboard, keyCode_r ) ) {
                            editorState->killZone_temp = editorState->killZone[ editorState->entity_select ];
                            editorState->editorMode = editorMode_rotateKillZone;
                            init( &editorState->input_string );
                        }
                        if( wasPressed( keyboard, keyCode_s ) ) {
                            editorState->killZone_temp = editorState->killZone[ editorState->entity_select ];
                            editorState->editorMode = editorMode_sizeKillZone;
                            init( &editorState->input_string );
                        }
                    }
                } break;
                
                case entityMode_checkpoint: {
                    RAY   select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                    int32 bPoint    = getBestPoint ( editorState, select );
                    int32 bCheckpoint = getFirstCheckpoint( editorState, select );
                    vec3  P = editorState->point[ bPoint ];
                    
                    if( ( bCheckpoint == -1 ) && ( bPoint != -1 ) ) {
                        drawCheckpoint( renderer, renderPassID_main, P );
                    }
                    
                    if( wasPressed( mouse, mouseButton_left ) ) {
                        if( bCheckpoint != -1 ) {
                            editorState->entity_select = bCheckpoint;
                        } else if( bPoint != -1 ) {
                            editorState->entity_select = addCheckpoint( editorState, bPoint );
                        }
                    }
                    if( wasPressed( mouse, mouseButton_right ) ) {
                        if( bCheckpoint != -1 ) {
                            deleteCheckpoint( editorState, bCheckpoint );
                        }
                    }
                    if( editorState->entity_select != -1 ) {
                        if( wasPressed( keyboard, keyCode_r ) ) {
                            editorState->checkpoint_temp = editorState->checkpoint[ editorState->entity_select ];
                            editorState->editorMode = editorMode_rotateCheckpoint;
                            init( &editorState->input_string );
                        }
                        if( wasPressed( keyboard, keyCode_s ) ) {
                            editorState->checkpoint_temp = editorState->checkpoint[ editorState->entity_select ];
                            editorState->editorMode = editorMode_sizeCheckpoint;
                            init( &editorState->input_string );
                        }
                    }
                } break;
                
                case entityMode_view: {
                    RAY   select = getSelectRay( camera, main_transform, mouse->position, app_dim );
                    int32 bPoint = getBestPoint( editorState, select );
                    int32 bView  = getFirstView( editorState, select );
                    vec3  P = editorState->point[ bPoint ];
                    
                    // DISPLAY_VALUE( int32, bView );
                    
                    boo32 shift_isDown = ( keyboard->flags & KEYBOARD_FLAGS__SHIFT );
                    
                    if( ( bView == -1 ) && ( bPoint != -1 ) ) {
                        drawView( renderer, renderPassID_main, P );
                    }
                    
                    if( wasPressed( mouse, mouseButton_left ) ) {
                        if( ( bView != -1 ) && ( !shift_isDown ) ) {
                            editorState->entity_select = bView;
                        } else if( bPoint != -1 ) {
                            if( shift_isDown ) {
                                if( editorState->entity_select != -1 ) {
                                    EDITOR__VIEW * view = editorState->view + editorState->entity_select;
                                    
                                    if( bPoint != ( int32 )view->pIndex ) {
                                        view->type   = viewType_line;
                                        view->qIndex = bPoint;
                                    }
                                }
                            } else {
                                editorState->entity_select = addView( editorState, bPoint );
                            }
                        }
                    }
                    if( wasPressed( mouse, mouseButton_right ) ) {
                        if( bView != -1 ) {
                            deleteView( editorState, bView );
                        }
                    }
                    
                    if( editorState->entity_select != -1 ) {
                        EDITOR__VIEW * view = editorState->view + editorState->entity_select;
                        if( wasPressed( keyboard, keyCode_c ) ) {
                            view->maxDist  = editorState->modCamera_maxDist;
                            view->hOffset  = editorState->modCamera_hOffset;
                            view->vOffset  = editorState->modCamera_vOffset;
                            view->fovAngle = editorState->modCamera_fovAngle;
                        }
                        if( wasPressed( keyboard, keyCode_h ) ) {
                            editorState->view_temp  = editorState->view[ editorState->entity_select ];
                            editorState->editorMode = editorMode_sizeView_hi;
                            init( &editorState->input_string );
                        }
                        if( wasPressed( keyboard, keyCode_l ) ) {
                            editorState->view_temp  = editorState->view[ editorState->entity_select ];
                            editorState->editorMode = editorMode_sizeView_lo;
                            init( &editorState->input_string );
                        }
                        
                        // DISPLAY_VALUE( flo32, view->maxDist );
                        // DISPLAY_VALUE( flo32, view->hOffset );
                        // DISPLAY_VALUE( flo32, view->vOffset );
                        // DISPLAY_VALUE( flo32, view->fovAngle );
                    }
                } break;
                
                default: {
                    InvalidCodePath;
                } break;
            }
            if( wasPressed( keyboard, keyCode_escape ) ) {
                editorState->editorMode = editorMode_default;
                
                ENTITY_PANEL * e = &editorState->entityPanel;
                e->active = -1;
            }
        } break;
        
        case editorMode_translateSelection: {
            INPUT_STRING * input_string = &editorState->input_string;
            for( uint32 numericalCode_index = 0; numericalCode_index < 10; numericalCode_index++ ) {
                if( wasPressed( keyboard, ( KEY_STATE_CODE )( keyCode_0 + numericalCode_index ) ) ) {
                    addChar( input_string, '0' + ( char )numericalCode_index );
                }
            }
            if( wasPressed( keyboard, keyCode_period ) ) {
                addChar( input_string, '.' );
            }
            if( wasPressed( keyboard, keyCode_backspace ) ) {
                deleteChar_backspace( input_string );
            }
            if( wasPressed( keyboard, keyCode_x ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = Vec3( 1.0f, 0.0f, 0.0f );
            }
            if( wasPressed( keyboard, keyCode_y ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = Vec3( 0.0f, 1.0f, 0.0f );
            }
            if( wasPressed( keyboard, keyCode_z ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = Vec3( 0.0f, 0.0f, 1.0f );
            }
            if( wasPressed( keyboard, keyCode_x, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->translateMode  = translateMode_twoAxis;
                editorState->translate_axis = Vec3( 1.0f, 0.0f, 0.0f );
                editorState->translate_xAxis = Vec3( 0.0f, 0.0f, -1.0f );
                editorState->translate_yAxis = Vec3( 0.0f, 1.0f,  0.0f );
            }
            if( wasPressed( keyboard, keyCode_y, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->translateMode   = translateMode_twoAxis;
                editorState->translate_axis  = Vec3( 0.0f, 1.0f,  0.0f );
                editorState->translate_xAxis = Vec3( 1.0f, 0.0f,  0.0f );
                editorState->translate_yAxis = Vec3( 0.0f, 0.0f, -1.0f );
            }
            if( wasPressed( keyboard, keyCode_z, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->translateMode   = translateMode_twoAxis;
                editorState->translate_axis  = Vec3( 0.0f, 0.0f, 1.0f );
                editorState->translate_xAxis = Vec3( 1.0f, 0.0f, 0.0f );
                editorState->translate_yAxis = Vec3( 0.0f, 1.0f, 0.0f );
            }
            if( wasPressed( keyboard, keyCode_d ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = editorState->utilityPanel.direction;
            }
            if( wasPressed( keyboard, keyCode_d, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->translateMode  = translateMode_twoAxis;
                editorState->translate_axis = editorState->utilityPanel.direction;
                
                vec3 N = Vec3( 0.0f, 1.0f, 0.0f );
                vec3 Z = editorState->utilityPanel.direction;
                if( isCollinearTo( N, Z ) ) {
                    N = Vec3( 1.0f, 0.0f, 0.0f );
                }
                
                editorState->translate_xAxis = getNormal( cross( N, Z ) );
                editorState->translate_yAxis = getNormal( cross( Z, editorState->translate_xAxis ) );
            }
            
            flo32 length_value = strtof( input_string->string, 0 );
            
            RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
            vec3 translate_vector = {};
            switch( editorState->translateMode ) {
                case translateMode_twoAxis: {
                    UTILITY_PANEL * u = &editorState->utilityPanel;
                    if( u->pivot_isActive ) {
                        int32 bIndex = getBestPoint( editorState, select );
                        if( bIndex != -1 ) {
                            vec3 P = editorState->point[ bIndex ];
                            vec3 Q = u->pivot;
                            
                            translate_vector = P - Q;
                        }
                    } else {
                        vec3 plane_normal = editorState->translate_axis;
                        vec3 plane_point  = editorState->translate_origin;
                        
                        flo32 denom = dot( select.vector, plane_normal );
                        if( denom != 0.0f ) {
                            flo32 t = -( dot( select.origin, plane_normal ) + dot( -plane_point, plane_normal ) ) / denom;
                            if( t > 0.0f ) {
                                if( u->grid_isActive ) {
                                    vec3 intersect = select.origin + select.vector * t;
                                    vec3 direction = intersect - plane_point;
                                    flo32 tX = roundTo( dot( direction, editorState->translate_xAxis ), u->scale );
                                    flo32 tY = roundTo( dot( direction, editorState->translate_yAxis ), u->scale );
                                    
                                    translate_vector = editorState->translate_xAxis * tX + editorState->translate_yAxis * tY;
                                } else {
                                    translate_vector = select.origin + select.vector * t - plane_point;
                                }
                            }
                        }
                    }
                } break;
                
                case translateMode_oneAxis: {
                    vec3 P1 = editorState->translate_origin;
                    vec3 P2 = select.origin;
                    
                    vec3 U = editorState->translate_axis;
                    vec3 V = select.vector;
                    vec3 W = P1 - P2;
                    
                    flo32 dotUU = dot( U, U );
                    flo32 dotUV = dot( U, V );
                    flo32 dotUW = dot( U, W );
                    flo32 dotVV = dot( V, V );
                    flo32 dotVW = dot( V, W );
                    
                    flo32 denom = dotUU * dotVV - dotUV * dotUV;
                    Assert( denom != 0.0f );
                    
                    flo32 s = ( dotUV * dotVW - dotVV * dotUW ) / denom;
                    flo32 t = ( dotUU * dotVW - dotUV * dotUW ) / denom;
                    if( t > 0.0f ) {
                        RAY ray = Ray( P1, U * s );
                        RAY_INTERSECT i = doesIntersectNextFace( editorState, ray );
                        
                        UTILITY_PANEL * u = &editorState->utilityPanel;
                        if( u->grid_isActive ) {
                            s = roundTo( s, u->scale );
                        } else if( i.isValid ) {
                            U = ray.vector;
                            s = i.t;
                        }
                        translate_vector = U * s;
                    }
                } break;
                
                default: {
                    InvalidCodePath;
                }
            }
            if( length_value != 0.0f ) {
                translate_vector = getNormal( translate_vector ) * length_value;
            }
            for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
                if( editorState->point_isSelected[ point_index ] ) {
                    editorState->point[ point_index ] = editorState->translate_tempPos[ point_index ] + translate_vector;
                }
            }
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                confirmVertexEdit( editorState, tempMemory );
                add_translate( undoSystem, translate_vector, editorState, SnapMemory( tempMemory ) );
                
                LEVEL_PANEL * l = &editorState->levelPanel;
                if( editorState->levelEditor_isActive ) {
                    for( uint32 iLevel = 0; iLevel < l->nLevel; iLevel++ ) {
                        if( l->level_isSelected[ iLevel ] ) {
                            l->level[ iLevel ].translation += translate_vector;
                        }
                    }
                }
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                cancelVertexEdit( editorState, tempMemory );
            }
        } break;
        
        case editorMode_rotateSelection: {
            INPUT_STRING * i = &editorState->input_string;
            processFloatInput( keyboard, i );
            
            if( wasPressed( keyboard, keyCode_x ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = Vec3( 1.0f, 0.0f, 0.0f );
                editorState->transform_basePos = editorState->translate_origin + Vec3( 0.0f, 0.0f, -1.0f );
            }
            if( wasPressed( keyboard, keyCode_y ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = Vec3( 0.0f, 1.0f, 0.0f );
                editorState->transform_basePos = editorState->translate_origin + Vec3( 1.0f, 0.0f, 0.0f );
            }
            if( wasPressed( keyboard, keyCode_z ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = Vec3( 0.0f, 0.0f, 1.0f );
                editorState->transform_basePos = editorState->translate_origin + Vec3( 1.0f, 0.0f, 0.0f );
            }
            if( wasPressed( keyboard, keyCode_d ) ) {
                editorState->translateMode  = translateMode_oneAxis;
                editorState->translate_axis = editorState->utilityPanel.direction;
                
                vec3 X = getNormal( cross( Vec3( 0.0f, 1.0f, 0.0f ), editorState->translate_axis ) );
                editorState->transform_basePos = editorState->translate_origin + X;
            }
            
            vec3 O = editorState->translate_origin;
            vec3 A = editorState->translate_axis;
            vec3 B = editorState->transform_basePos;
            vec3 C = editorState->transform_basePos;
            
            RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
            
            PLANE plane = Plane( editorState->translate_axis, editorState->translate_origin );
            RAY_INTERSECT intersect = doesIntersectPlane( select.origin, select.vector, plane );
            if( intersect.isValid ) {
                C = intersect.P;
            }
            
            UTILITY_PANEL * u = &editorState->utilityPanel;
            
            flo32 radians = getRadiansBetween( B - O, C - O, editorState->translate_axis );
            radians = checkInputOrGridForRadians( i, u, radians );
            
            mat4  transform = getRotationTransform( O, radians, editorState->translate_axis );
            // DISPLAY_VALUE( flo32, radians );
            
            uint32 nSelect = getSelectedCount_vertex( editorState );
            uint32 * selectList = _pushArray( tempMemory, uint32, nSelect );
            vec3   * oldPos     = _pushArray( tempMemory, vec3,   nSelect );
            vec3   * newPos     = _pushArray( tempMemory, vec3,   nSelect );
            
            getSelected_vertex( editorState, selectList, nSelect );
            for( uint32 iPoint = 0; iPoint < nSelect; iPoint++ ) {
                uint32 index = selectList[ iPoint ];
                
                vec3 P = editorState->translate_tempPos[ index ];
                vec3 R = ( transform * Vec4( P, 1.0f ) ).xyz;
                editorState->point[ index ] = R;
                
                oldPos[ iPoint ] = P;
                newPos[ iPoint ] = R;
            }
            
            drawLine( renderer, renderPassID_main_debug, O, O + A, COLOR_MAGENTA );
            drawLine( renderer, renderPassID_main_debug, O, B,     COLOR_CYAN    );
            drawLine( renderer, renderPassID_main_debug, O, C,     COLOR_BLACK   );
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                add_modifyVertex( undoSystem, nSelect, selectList, oldPos, newPos, SnapMemory( tempMemory ) );
                confirmVertexEdit( editorState, tempMemory );
                
                if( editorState->levelEditor_isActive ) {
                    LEVEL_PANEL * l = &editorState->levelPanel;
                    for( uint32 iLevel = 0; iLevel < l->nLevel; iLevel++ ) {
                        if( l->level_isSelected[ iLevel ] ) {
                            LEVEL * level = l->level + iLevel;
                            level->rotation.y += radians;
                            level->translation = ( transform * Vec4( level->translation, 1.0f ) ).xyz;
                            
                            EDITOR__SECURE_DOOR * secureDoor = editorState->secureDoor + level->bDoor;
                            for( uint32 iDoor = 0; iDoor < level->nDoor; iDoor++ ) {
                                secureDoor[ iDoor ].radians += radians;
                            }
                            
                            EDITOR__KILL_ZONE * killZone = editorState->killZone + level->bKillZone;
                            for( uint32 iKillZone = 0; iKillZone < level->nKillZone; iKillZone++ ) {
                                killZone[ iKillZone ].radians += radians;
                            }
                            
                            EDITOR__CHECKPOINT * checkpoint = editorState->checkpoint + level->bCheckpoint;
                            for( uint32 iCheckpoint = 0; iCheckpoint < level->nCheckpoint; iCheckpoint++ ) {
                                checkpoint[ iCheckpoint ].radians += radians;
                            }
                        }
                    }
                }
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                cancelVertexEdit( editorState, tempMemory );
            }
            
            _popArray( tempMemory, uint32, nSelect );
            _popArray( tempMemory, vec3,   nSelect );
            _popArray( tempMemory, vec3,   nSelect );
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", toDegrees( radians ) );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_scaleSelection: {
            // if input_string.count > 0, use tValue
            // else {
            // select basePoint  or  use  basePoint
            INPUT_STRING * i = &editorState->input_string;
            processFloatInput( keyboard, i );
            
            if( wasPressed( keyboard, keyCode_x ) ) {
                editorState->scale_axisIsEnabled[ 0 ] = true;
                editorState->scale_axisIsEnabled[ 1 ] = false;
                editorState->scale_axisIsEnabled[ 2 ] = false;
            }
            if( wasPressed( keyboard, keyCode_y ) ) {
                editorState->scale_axisIsEnabled[ 0 ] = false;
                editorState->scale_axisIsEnabled[ 1 ] = true;
                editorState->scale_axisIsEnabled[ 2 ] = false;
            }
            if( wasPressed( keyboard, keyCode_z ) ) {
                editorState->scale_axisIsEnabled[ 0 ] = false;
                editorState->scale_axisIsEnabled[ 1 ] = false;
                editorState->scale_axisIsEnabled[ 2 ] = true;
            }
            if( wasPressed( keyboard, keyCode_x, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->scale_axisIsEnabled[ 0 ] = false;
                editorState->scale_axisIsEnabled[ 1 ] = true;
                editorState->scale_axisIsEnabled[ 2 ] = true;
            }
            if( wasPressed( keyboard, keyCode_y, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->scale_axisIsEnabled[ 0 ] = true;
                editorState->scale_axisIsEnabled[ 1 ] = false;
                editorState->scale_axisIsEnabled[ 2 ] = true;
            }
            if( wasPressed( keyboard, keyCode_z, KEYBOARD_FLAGS__SHIFT ) ) {
                editorState->scale_axisIsEnabled[ 0 ] = true;
                editorState->scale_axisIsEnabled[ 1 ] = true;
                editorState->scale_axisIsEnabled[ 2 ] = false;
            }
            
            vec3 scale = Vec3( 1.0f, 1.0f, 1.0f );
            if( i->count > 0 ) {
                flo32 value = strtof( i->string, 0 );
                scale = Vec3( value, value, value );
            }
            
            for( uint32 iAxis = 0; iAxis < 3; iAxis++ ) {
                if( !editorState->scale_axisIsEnabled[ iAxis ] ) {
                    scale.elem[ iAxis ] = 1.0f;
                }
            }
            
            boo32 clickMeansConfirmEdit = true;
            RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
            
            UTILITY_PANEL * u = &editorState->utilityPanel;
            //       if( editorState->scale_selectBasePoint ) {
            //          flo32 range = 0.15f;
            //          // TODO: modify range to adjust based on a point's distance from the camera
            //          int32 index = getBestPoint( editorState, select.origin, select.vector, range );
            //         // DISPLAY_VALUE( int32, index );
            
            //         if( wasPressed( mouse, mouseButton_left ) && ( index != -1 ) ) {
            //            editorState->scale_selectBasePoint = false;
            //            editorState->scale_basePoint       = editorState->point[ index ];
            //            clickMeansConfirmEdit = false;
            //         }
            //      } else {
            //         vec3 O = editorState->translate_origin;
            //         vec3 A = editorState->translate_axis;
            //         vec3 B = editorState->scale_basePoint;
            //         vec3 C = editorState->scale_basePoint;
            //         
            //         PLANE plane = Plane( editorState->translate_axis, editorState->translate_origin );
            //         RAY_INTERSECT intersect = doesIntersectPlane( select.origin, select.vector, plane );
            //         if( intersect.isValid ) {
            //            C = intersect.P;
            //            drawLine( renderer, renderPassID_main_debug, C, C + Vec3( 0.0f, 1.0f, 0.0f ), COLOR_MAGENTA );
            //         }
            //         
            //         flo32 dist0 = getLength( B - O );
            //         flo32 dist1 = getLength( C - O );
            //         if( u->grid_isActive ) {
            //            dist1 = roundTo( dist1, u->scale );
            //         }
            //         // DISPLAY_VALUE( flo32, dist0 );
            //         // DISPLAY_VALUE( flo32, dist1 );
            //         if( dist0 != 0.0f ) {
            //            flo32 value = dist1 / dist0;
            //            // DISPLAY_VALUE( flo32, value );
            //            scale = Vec3( value, value, value );
            //         }
            //      }
            
            // what about backspace to input_string.count == 0?
            //      if( u->scale_selectBasePoint ) {
            // input_string : select point  or  enter tValue
            //      }
            
            
            //      if( uelse {
            
            //      }
            // select basePoint or enter tValue
            // select basePoint : click single vertex
            // baseDist = dist from basePoint to origin
            // find select.vector intersect with scale plane
            // find dist from intersect to translate_origin
            // scale by dist / baseDist
            
            vec3  O = editorState->translate_origin;
            if( u->pivot_isActive ) {
                O = u->pivot;
            }
            mat4  transform = getScaleTransform( O, scale );
            // DISPLAY_VALUE( vec3, scale );
            
            uint32 nSelect = getSelectedCount_vertex( editorState );
            uint32 * selectList = _pushArray( tempMemory, uint32, nSelect );
            vec3   * oldPos     = _pushArray( tempMemory, vec3,   nSelect );
            vec3   * newPos     = _pushArray( tempMemory, vec3,   nSelect );
            
            getSelected_vertex( editorState, selectList, nSelect );
            for( uint32 iPoint = 0; iPoint < nSelect; iPoint++ ) {
                uint32 index = selectList[ iPoint ];
                
                vec3 P = editorState->translate_tempPos[ index ];
                vec3 R = ( transform * Vec4( P, 1.0f ) ).xyz;
                editorState->point[ index ] = R;
                
                oldPos[ iPoint ] = P;
                newPos[ iPoint ] = R;
            }
            
            //      drawLine( renderer, renderPassID_main_debug, O, O + A, COLOR_MAGENTA );
            //      drawLine( renderer, renderPassID_main_debug, O, B,     COLOR_CYAN    );
            //      drawLine( renderer, renderPassID_main_debug, O, C,     COLOR_BLACK   );
            
            if( wasPressed( keyboard, keyCode_enter ) || ( wasPressed( mouse, mouseButton_left ) && ( clickMeansConfirmEdit ) ) ) {
                add_modifyVertex( undoSystem, nSelect, selectList, oldPos, newPos, SnapMemory( tempMemory ) );
                confirmVertexEdit( editorState, tempMemory );
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                cancelVertexEdit( editorState, tempMemory );
            }
            
            _popArray( tempMemory, uint32, nSelect );
            _popArray( tempMemory, vec3,   nSelect );
            _popArray( tempMemory, vec3,   nSelect );
            
            //      char str[ 128 ] = {};
            //      sprintf( str, "%.02f", toDegrees( radians ) );
            
            //      FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            //      vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            //      rect bound = rectTRD( mouse->position, dim );
            //      drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            //      drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_rotateDoor: {
            Assert( editorState->entity_select != -1 );
            
            INPUT_STRING  * i = &editorState->input_string;
            UTILITY_PANEL * u = &editorState->utilityPanel;
            processFloatInput( keyboard, i );
            
            EDITOR__SECURE_DOOR * door = editorState->secureDoor + editorState->entity_select;
            vec3 O = editorState->point[ door->point_index ];
            vec3 A = Vec3( 0.0f, 1.0f, 0.0f );
            vec3 B = O + Vec3( 1.0f, 0.0f, 0.0f );
            vec3 C = B;
            
            RAY select  = getSelectRay( camera, main_transform, mouse->position, app_dim );      
            PLANE plane = Plane( A, O );
            RAY_INTERSECT intersect = doesIntersectPlane( select.origin, select.vector, plane );
            if( intersect.isValid ) {
                C = intersect.P;
            }
            
            flo32 radians = getRadiansBetween( B - O, C - O, A );
            door->radians = checkInputOrGridForRadians( i, u, radians );
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->secureDoor[ editorState->entity_select ] = editorState->secureDoor_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            drawLine( renderer, renderPassID_main_debug, O, O + A, COLOR_MAGENTA );
            drawLine( renderer, renderPassID_main_debug, O, B,     COLOR_CYAN    );
            drawLine( renderer, renderPassID_main_debug, O, C,     COLOR_BLACK   );
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", toDegrees( door->radians ) );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_sizeDoor: {
            Assert( editorState->entity_select != -1 );
            
            INPUT_STRING  * i = &editorState->input_string;
            UTILITY_PANEL * u = &editorState->utilityPanel;
            processFloatInput( keyboard, i );
            
            EDITOR__SECURE_DOOR * door = editorState->secureDoor + editorState->entity_select;
            
            vec3 O = editorState->point[ door->point_index ];
            vec3 A = Vec3( 0.0f, 1.0f, 0.0f );
            
            RAY            select  = getSelectRay( camera, main_transform, mouse->position, app_dim );
            CLOSEST_POINTS closest = getClosest_ray_ray( select.origin, select.vector, O, A );
            
            flo32 height = checkInputOrGridForScale( i, u, closest.t );
            door->height = height;         
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->secureDoor[ editorState->entity_select ] = editorState->secureDoor_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", height );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_rotateKillZone: {
            Assert( editorState->entity_select != -1 );
            
            INPUT_STRING  * i = &editorState->input_string;
            UTILITY_PANEL * u = &editorState->utilityPanel;
            processFloatInput( keyboard, i );
            
            EDITOR__KILL_ZONE * killZone = editorState->killZone + editorState->entity_select;
            vec3 O = editorState->point[ killZone->point_index ];
            vec3 A = Vec3( 0.0f, 1.0f, 0.0f );
            vec3 B = O + Vec3( 1.0f, 0.0f, 0.0f );
            vec3 C = B;
            
            RAY select  = getSelectRay( camera, main_transform, mouse->position, app_dim );      
            PLANE plane = Plane( A, O );
            RAY_INTERSECT intersect = doesIntersectPlane( select.origin, select.vector, plane );
            if( intersect.isValid ) {
                C = intersect.P;
            }
            
            flo32 radians = getRadiansBetween( B - O, C - O, A );
            killZone->radians = checkInputOrGridForRadians( i, u, radians );
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->killZone[ editorState->entity_select ] = editorState->killZone_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            drawLine( renderer, renderPassID_main_debug, O, O + A, COLOR_MAGENTA );
            drawLine( renderer, renderPassID_main_debug, O, B,     COLOR_CYAN    );
            drawLine( renderer, renderPassID_main_debug, O, C,     COLOR_BLACK   );
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", toDegrees( killZone->radians ) );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_sizeKillZone: {
            Assert( editorState->entity_select != -1 );
            
            INPUT_STRING  * i = &editorState->input_string;
            UTILITY_PANEL * u = &editorState->utilityPanel;
            processFloatInput( keyboard, i );
            
            EDITOR__KILL_ZONE * killZone = editorState->killZone + editorState->entity_select;
            *killZone = editorState->killZone_temp;
            
            if( wasPressed( keyboard, keyCode_x ) ) { editorState->sizeKillZone_axis = 0; }
            if( wasPressed( keyboard, keyCode_y ) ) { editorState->sizeKillZone_axis = 1; }
            if( wasPressed( keyboard, keyCode_z ) ) { editorState->sizeKillZone_axis = 2; }
            
            vec3 O = editorState->point[ killZone->point_index ];
            
            vec3 A = Vec3( 0.0f, 0.0f, 0.0f );
            if( editorState->sizeKillZone_axis == 0 ) {
                A = Vec3( cosf( killZone->radians ), 0.0f, -sinf( killZone->radians ) );
            }
            if( editorState->sizeKillZone_axis == 1 ) {
                A = Vec3( 0.0f, 1.0f, 0.0f );
            }
            if( editorState->sizeKillZone_axis == 2 ) {
                A = Vec3( sinf( killZone->radians ), 0.0f, cosf( killZone->radians ) );
            }
            
            RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
            
            CLOSEST_POINTS closest = getClosest_ray_ray( select.origin, select.vector, O, A );
            
            flo32 d = checkInputOrGridForScale( i, u, closest.t );
            killZone->halfDim.elem[ editorState->sizeKillZone_axis ] = d;
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->killZone[ editorState->entity_select ] = editorState->killZone_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", d );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_rotateCheckpoint: {
            Assert( editorState->entity_select != -1 );
            
            INPUT_STRING  * i = &editorState->input_string;
            UTILITY_PANEL * u = &editorState->utilityPanel;
            processFloatInput( keyboard, i );
            
            EDITOR__CHECKPOINT * checkpoint = editorState->checkpoint + editorState->entity_select;
            vec3 O = editorState->point[ checkpoint->point_index ];
            vec3 A = Vec3( 0.0f, 1.0f, 0.0f );
            vec3 B = O + Vec3( 1.0f, 0.0f, 0.0f );
            vec3 C = B;
            
            RAY select  = getSelectRay( camera, main_transform, mouse->position, app_dim );      
            PLANE plane = Plane( A, O );
            RAY_INTERSECT intersect = doesIntersectPlane( select.origin, select.vector, plane );
            if( intersect.isValid ) {
                C = intersect.P;
            }
            
            flo32 radians = getRadiansBetween( B - O, C - O, A );
            checkpoint->radians = checkInputOrGridForRadians( i, u, radians );
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->checkpoint[ editorState->entity_select ] = editorState->checkpoint_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            drawLine( renderer, renderPassID_main_debug, O, O + A, COLOR_MAGENTA );
            drawLine( renderer, renderPassID_main_debug, O, B,     COLOR_CYAN    );
            drawLine( renderer, renderPassID_main_debug, O, C,     COLOR_BLACK   );
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", toDegrees( checkpoint->radians ) );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_sizeCheckpoint: {
            Assert( editorState->entity_select != -1 );
            
            INPUT_STRING  * i = &editorState->input_string;
            UTILITY_PANEL * u = &editorState->utilityPanel;
            processFloatInput( keyboard, i );
            
            EDITOR__CHECKPOINT * checkpoint = editorState->checkpoint + editorState->entity_select;
            *checkpoint = editorState->checkpoint_temp;
            
            if( wasPressed( keyboard, keyCode_x ) ) { editorState->sizeCheckpoint_axis = 0; }
            if( wasPressed( keyboard, keyCode_y ) ) { editorState->sizeCheckpoint_axis = 1; }
            if( wasPressed( keyboard, keyCode_z ) ) { editorState->sizeCheckpoint_axis = 2; }
            
            vec3 O = editorState->point[ checkpoint->point_index ];
            
            vec3 A = Vec3( 0.0f, 0.0f, 0.0f );
            if( editorState->sizeCheckpoint_axis == 0 ) {
                A = Vec3( cosf( checkpoint->radians ), 0.0f, -sinf( checkpoint->radians ) );
            }
            if( editorState->sizeCheckpoint_axis == 1 ) {
                A = Vec3( 0.0f, 1.0f, 0.0f );
            }
            if( editorState->sizeCheckpoint_axis == 2 ) {
                A = Vec3( sinf( checkpoint->radians ), 0.0f, cosf( checkpoint->radians ) );
            }
            
            RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
            
            CLOSEST_POINTS closest = getClosest_ray_ray( select.origin, select.vector, O, A );
            
            flo32 d = checkInputOrGridForScale( i, u, closest.t );
            checkpoint->halfDim.elem[ editorState->sizeCheckpoint_axis ] = d;
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->checkpoint[ editorState->entity_select ] = editorState->checkpoint_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", d );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_sizeView_lo:
        case editorMode_sizeView_hi: {
            Assert( editorState->entity_select != -1 );
            
            EDITOR__VIEW * view = editorState->view + editorState->entity_select;
            vec3  P = editorState->point[ view->pIndex ];
            
            vec3  N = editorState->camera.zAxis;
            flo32 d = dot( N, P );
            
            RAY select  = getSelectRay( camera, main_transform, mouse->position, app_dim );      
            PLANE plane = Plane( N, d );
            RAY_INTERSECT intersect = doesIntersectPlane( select.origin, select.vector, plane );
            
            flo32 dist = 0.0f;
            if( intersect.isValid ) {
                dist = roundTo( getLength( P - intersect.P ), 0.1f );
            }
            
            if( editorState->editorMode == editorMode_sizeView_hi ) {
                view->hi = dist;
            } else {
                view->lo = dist;
            }
            
            if( wasPressed( keyboard, keyCode_enter ) || wasPressed( mouse, mouseButton_left ) ) {
                editorState->editorMode = editorMode_placeEntity;
            }
            if( wasPressed( keyboard, keyCode_escape ) || wasPressed( mouse, mouseButton_right ) ) {
                editorState->view[ editorState->entity_select ] = editorState->view_temp;
                editorState->editorMode = editorMode_placeEntity;
            }
            
            char str[ 128 ] = {};
            sprintf( str, "%.02f", dist );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight );
            rect bound = rectTRD( mouse->position, dim );
            drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, str, getTL( bound ), ALIGN_TOPLEFT, COLOR_WHITE );
        } break;
        
        case editorMode_utilityPanel_textBox: {
            UTILITY_PANEL * u   = &editorState->utilityPanel;
            INPUT_STRING  * str = &editorState->input_string;
            
            if( wasPressed( mouse, mouseButton_left ) ) {
                if( u->active ) {
                    TEXT_BOX_F32 * t = u->active;
                    if( isInBound( mouse->position, t->bound ) ) {
                        setCursor( str, mouse->position, t->bound, u->text_charWidth );
                    } else {
                        for( uint32 iTextBox = 0; iTextBox < utilityPanel_textBoxID_count; iTextBox++ ) {
                            t = u->textBox + iTextBox;
                            if( t->output && isInBound( mouse->position, t->bound ) ) {
                                init( str, t, u, mouse, 16 );
                            }
                        }
                    }
                }
            }
            for( uint32 iter = 0; iter < 10; iter++ ) {
                if( wasPressed( keyboard, ( KEY_STATE_CODE )( keyCode_0 + iter ) ) ) {
                    addChar( str, '0' + ( char )iter );
                }
            }
            if( wasPressed( keyboard, keyCode_dash ) ) {
                addChar( str, '-' );
            }
            if( wasPressed( keyboard, keyCode_period ) ) {
                addChar( str, '.' );
            }
            if( wasPressed( keyboard, keyCode_left ) ) {
                setCursor( str, str->cursor - 1 );
            }
            if( wasPressed( keyboard, keyCode_right ) ) {
                setCursor( str, str->cursor + 1 );
            }
            if( wasPressed( keyboard, keyCode_backspace ) ) {
                deleteChar_backspace( str );
            }
            if( wasPressed( keyboard, keyCode_delete ) ) {
                deleteChar_delete( str );
            }
            if( wasPressed( keyboard, keyCode_escape ) ) {
                editorState->editorMode = editorMode_default;
                
                Assert( u->active );
                u->active->input = 0;
                u->active        = 0;
            }
            if( wasPressed( keyboard, keyCode_enter ) ) {
                Assert( u->active );
                uint32 textBoxID = ( uint32 )( u->active - u->textBox );
                switch( textBoxID ) {
                    case utilityPanel_textBoxID_centroidX:
                    case utilityPanel_textBoxID_centroidY:
                    case utilityPanel_textBoxID_centroidZ: {
                        uint32 iElem = ( uint32 )( textBoxID - utilityPanel_textBoxID_centroidX );
                        
                        CENTROID p    = getCentroidOfSelection( editorState );
                        vec3     dest = p.position;
                        dest.elem[ iElem ] = strtof( str->string, 0 );
                        
                        vec3 translate_vector = dest - p.position;
                        for( int32 iter = 0; iter < editorState->point_count; iter++ ) {
                            if( editorState->point_isSelected[ iter ] ) {
                                editorState->point[ iter ] += translate_vector;
                            }
                        }
                        add_translate( undoSystem, translate_vector, editorState, SnapMemory( tempMemory ) );
                    } break;
                    
                    case utilityPanel_textBoxID_scale:
                    case utilityPanel_textBoxID_degrees:
                    case utilityPanel_textBoxID_pivotX:
                    case utilityPanel_textBoxID_pivotY:
                    case utilityPanel_textBoxID_pivotZ: {
                        *( u->active->output ) = strtof( str->string, 0 );
                    } break;
                    
                    default: {
                        InvalidCodePath;
                    } break;
                }
                
                editorState->editorMode = editorMode_default;
                
                u->active->input = 0;
                u->active        = 0;
            }
        } break;
        
        case editorMode_setEntityID: {
            INPUT_STRING * i = &editorState->input_string;
            
            processUIntInput( keyboard, i );
            
            uint32 input_value   = strtoul( i->string, 0, 0 );
            boo32  input_isValid = ( input_value > 0 );
            
            vec2 panel_dim = {};
            panel_dim.x = app_dim.x * 0.6f;
            panel_dim.y = app_dim.y * 0.6f;
            
            rect panel_bound = rectCD( app_halfDim, panel_dim );
            
            drawRect       ( renderer, renderPassID_overlay, panel_bound, COLOR_GRAY( 0.5f ) );
            drawRectOutline( renderer, renderPassID_overlay, panel_bound, COLOR_BLACK        );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, "Set Entity ID:", getTL( panel_bound ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            vec2 textBox_offset = Vec2( 6.0f, 32.0f );
            rect textBox_bound = Rect( panel_bound.left + textBox_offset.x, panel_bound.top - textBox_offset.y * 2.0f, panel_bound.right - textBox_offset.x, panel_bound.top - textBox_offset.y );
            
            drawRect( renderer, renderPassID_overlay, textBox_bound, COLOR_WHITE );
            drawString( renderer, renderPassID_overlay, assetID_font_default, i->string, getTL( textBox_bound ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            rect file_bound = panel_bound;
            file_bound.top  = textBox_bound.bottom;
            file_bound      = addRadius( file_bound, -6.0f );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            
            SnapMemory( tempMemory );
            STRING_BUFFER filename = StringBuffer( tempMemory, 256, _KB( 4 ) );
            getListOfMeshFiles( &filename );
            
            uint32 * filter = _pushArray( tempMemory, uint32, filename.nStr );
            uint32  nFilter = filterStringsByInput( &filename, i, filter );
            
            drawRect( renderer, renderPassID_overlay, file_bound, COLOR_WHITE );
            
            uint32 nFile = nFilter;
            vec2 atPos = getTL( file_bound ) + Vec2( 2.0f, 0.0f );
            for( uint32 iFile = 0; iFile < nFile; iFile++ ) {
                drawString( renderer, renderPassID_overlay, assetID_font_default, filename.str[ filter[ iFile ] ], atPos, ALIGN_TOPLEFT, COLOR_BLACK );
                atPos.y -= ( font->advanceHeight + 1.0f );
            }
            
            if( wasPressed( keyboard, keyCode_enter ) && ( nFilter == 0 ) && ( input_isValid ) ) {
                // TODO: verify on overwrite entity
                editorState->entityID = input_value;
                saveEntity = true;
                
                editorState->editorMode = editorMode_default;
            }
            if( wasPressed( keyboard, keyCode_escape ) ) {
                editorState->editorMode = editorMode_default;
            }
        } break;
        
        case editorMode_openEntity: {
            INPUT_STRING * input_string = &editorState->input_string;
            for( uint32 numericalCode_index = 0; numericalCode_index < 10; numericalCode_index++ ) {
                if( wasPressed( keyboard, ( KEY_STATE_CODE )( keyCode_0 + numericalCode_index ) ) ) {
                    addChar( input_string, '0' + ( char )numericalCode_index );
                }
            }
            if( wasPressed( keyboard, keyCode_backspace ) ) {
                deleteChar_backspace( input_string );
            }
            
            uint32 input_value   = strtoul( input_string->string, 0, 0 );
            
            vec2 panel_dim = {};
            panel_dim.x = app_dim.x * 0.6f;
            panel_dim.y = app_dim.y * 0.6f;
            
            rect panel_bound = rectCD( app_halfDim, panel_dim );
            
            drawRect       ( renderer, renderPassID_overlay, panel_bound, COLOR_GRAY( 0.5f ) );
            drawRectOutline( renderer, renderPassID_overlay, panel_bound, COLOR_BLACK        );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, "Entity ID to open:", getTL( panel_bound ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            vec2 textBox_offset = Vec2( 6.0f, 32.0f );
            rect textBox_bound = Rect( panel_bound.left + textBox_offset.x, panel_bound.top - textBox_offset.y * 2.0f, panel_bound.right - textBox_offset.x, panel_bound.top - textBox_offset.y );
            
            drawRect( renderer, renderPassID_overlay, textBox_bound, COLOR_WHITE );
            drawString( renderer, renderPassID_overlay, assetID_font_default, input_string->string, getTL( textBox_bound ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            rect file_bound = panel_bound;
            file_bound.top  = textBox_bound.bottom;
            file_bound      = addRadius( file_bound, -6.0f );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            
            SnapMemory( tempMemory );
            STRING_BUFFER filename = StringBuffer( tempMemory, 256, _KB( 4 ) );
            getListOfMeshFiles( &filename );
            
            uint32 * filter = _pushArray( tempMemory, uint32, filename.nStr );
            uint32  nFilter = filterStringsByInput( &filename, input_string, filter );
            
            drawRect( renderer, renderPassID_overlay, file_bound, COLOR_WHITE );
            
            uint32 nFile = nFilter;
            vec2 atPos = getTL( file_bound ) + Vec2( 2.0f, 0.0f );
            for( uint32 iFile = 0; iFile < nFile; iFile++ ) {
                drawString( renderer, renderPassID_overlay, assetID_font_default, filename.str[ filter[ iFile ] ], atPos, ALIGN_TOPLEFT, COLOR_BLACK );
                atPos.y -= ( font->advanceHeight + 1.0f );
            }
            
            if( ( wasPressed( keyboard, keyCode_enter ) ) && ( nFilter > 0 ) && ( input_string->count > 0 ) ) {
                
                editorState->entityID = input_value;
                openEntity = true;
                
                editorState->editorMode = editorMode_default;
            }
            if( wasPressed( keyboard, keyCode_escape ) ) {
                editorState->editorMode = editorMode_default;
            }
            
            popToSnap( tempMemory );
        } break;
        
        case editorMode_addLevel: {
            INPUT_STRING * i = &editorState->input_string;
            processUIntInput( keyboard, i );
            
            uint32 input_value = strtoul( i->string, 0, 0 );
            
            vec2 panel_dim = {};
            panel_dim.x = app_dim.x * 0.6f;
            panel_dim.y = app_dim.y * 0.6f;
            
            rect panel_bound = rectCD( app_halfDim, panel_dim );
            
            drawRect       ( renderer, renderPassID_overlay, panel_bound, COLOR_GRAY( 0.5f ) );
            drawRectOutline( renderer, renderPassID_overlay, panel_bound, COLOR_BLACK        );
            
            drawString( renderer, renderPassID_overlay, assetID_font_default, "Entity ID to open:", getTL( panel_bound ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            vec2 textBox_offset = Vec2( 6.0f, 32.0f );
            rect textBox_bound  = Rect( panel_bound.left + textBox_offset.x, panel_bound.top - textBox_offset.y * 2.0f, panel_bound.right - textBox_offset.x, panel_bound.top - textBox_offset.y );
            
            drawRect( renderer, renderPassID_overlay, textBox_bound, COLOR_WHITE );
            drawString( renderer, renderPassID_overlay, assetID_font_default, i->string, getTL( textBox_bound ), ALIGN_TOPLEFT, COLOR_BLACK );
            
            rect file_bound = panel_bound;
            file_bound.top  = textBox_bound.bottom;
            file_bound      = addRadius( file_bound, -6.0f );
            
            FONT * font = getFont( renderer->assetSystem, assetID_font_default );
            
            SnapMemory( tempMemory );
            STRING_BUFFER filename = StringBuffer( tempMemory, 256, _KB( 4 ) );
            getListOfMeshFiles( &filename );
            
            uint32 * filter = _pushArray( tempMemory, uint32, filename.nStr );
            uint32  nFilter = filterStringsByInput( &filename, i, filter );
            
            drawRect( renderer, renderPassID_overlay, file_bound, COLOR_WHITE );
            
            uint32 nFile = nFilter;
            vec2 atPos = getTL( file_bound ) + Vec2( 2.0f, 0.0f );
            for( uint32 iFile = 0; iFile < nFile; iFile++ ) {
                drawString( renderer, renderPassID_overlay, assetID_font_default, filename.str[ filter[ iFile ] ], atPos, ALIGN_TOPLEFT, COLOR_BLACK );
                atPos.y -= ( font->advanceHeight + 1.0f );
            }
            
            if( ( wasPressed( keyboard, keyCode_enter ) ) && ( nFilter > 0 ) && ( i->count > 0 ) ) {
                loadEntity( editorState, permMemory, tempMemory, input_value );
                editorState->editorMode = editorMode_default;
            }
            if( wasPressed( keyboard, keyCode_escape ) ) {
                editorState->editorMode = editorMode_default;
            }
            
            popToSnap( tempMemory );
        } break;
        
        default: {
            InvalidCodePath;
        } break;
    }
    
    // DISPLAY_VALUE( uint32, tempMemory->snap_index );
    // DISPLAY_VALUE( uint64, tempMemory->used );
    if( wasPressed( keyboard, keyCode_g, KEYBOARD_FLAGS__CONTROL ) ) {
        UTILITY_PANEL * u = &editorState->utilityPanel;
        u->grid_isActive = !u->grid_isActive;
    }
    if( wasPressed( keyboard, keyCode_p, KEYBOARD_FLAGS__CONTROL ) ) {
        UTILITY_PANEL * u = &editorState->utilityPanel;
        u->pivot_isActive = !u->pivot_isActive;
    }
    
    boo32 geometryWasModified = true;
    if( geometryWasModified ) {
        computeFaceCentroids( editorState );
        computeFaceNormals  ( editorState );
        
        // computeEdges        ( editorState );
        computeEdgeCentroids( editorState );
    }
    
    // BEGIN DRAW
    PROFILE_BREAK();
    for( uint32 iPixie = 0; iPixie < editorState->nPixie; iPixie++ ) {
        EDITOR__PIXIE ePixie = editorState->pixie[ iPixie ];
        PIXIE pixie = toPixie( editorState, ePixie );
        drawPixie( renderer, renderPassID_main, pixie );
    }
    PROFILE_BREAK();
    // DISPLAY_VALUE( uint32, editorState->nSecureDoor );
    for( uint32 iDoor = 0; iDoor < editorState->nSecureDoor; iDoor++ ) {
        EDITOR__SECURE_DOOR d = editorState->secureDoor[ iDoor ];
        
        SECURE_DOOR door = toSecureDoor( editorState, d );
        drawBoxOutline( renderer, renderPassID_main, door.activate_bound, COLOR_CYAN );
        drawBoxOutline( renderer, renderPassID_main, door.render_bound,   COLOR_MAGENTA );
    }
    PROFILE_BREAK();
    for( uint32 iKillZone = 0; iKillZone < editorState->nKillZone; iKillZone++ ) {
        EDITOR__KILL_ZONE k = editorState->killZone[ iKillZone ];
        
        KILL_ZONE killZone = toKillZone( editorState, k );
        drawBoxOutline( renderer, renderPassID_main, killZone.bound, COLOR_CYAN );
    }
    PROFILE_BREAK();
    for( uint32 iCheckpoint = 0; iCheckpoint < editorState->nCheckpoint; iCheckpoint++ ) {
        EDITOR__CHECKPOINT k = editorState->checkpoint[ iCheckpoint ];
        
        CHECKPOINT checkpoint = toCheckpoint( editorState, k );
        drawBoxOutline( renderer, renderPassID_main, checkpoint.bound, COLOR_RED );
    }
    for( uint32 iView = 0; iView < editorState->nView; iView++ ) {
        EDITOR__VIEW v = editorState->view[ iView ];
        VIEW view = toView( editorState, v );
        
        boo32 isSelected = ( editorState->entity_select == ( int32 )iView );
        drawView( renderer, renderPassID_main, view, isSelected );
    }
    
    
    if( ( editorState->editorMode == editorMode_translateSelection ) && ( editorState->translateMode == translateMode_oneAxis ) ) {
        vec3 P0 = editorState->translate_origin + editorState->translate_axis * 100.0f;
        vec3 P1 = editorState->translate_origin - editorState->translate_axis * 100.0f;
        drawLine( renderer, renderPassID_main_debug, P0, P1, COLOR_MAGENTA );
    }
    
    PROFILE_BREAK();
    { // draw selection points
        vec3  * point_array      = editorState->point;
        uint32  point_count      = ( uint32 )editorState->point_count;
        boo32 * point_isSelected = editorState->point_isSelected;
        
        if( editorState->selectMode == selectMode_edge ) {
            point_array      = editorState->edge_centroid;
            point_count      = ( uint32 )editorState->edge_count;
            point_isSelected = editorState->edge_isSelected;
        }
        if( editorState->selectMode == selectMode_face ) {
            point_array      = editorState->face_centroid;
            point_count      = ( uint32 )editorState->face_count;
            point_isSelected = editorState->face_isSelected;
        }
        
        vec3 origin       = getCameraPos( camera );
        vec3 point_offset = Vec3( 0.1f, 0.1f, 0.1f );
        
        if( camera->isOrthographic ) {
            flo32 dim = camera->ortho_dim.x * 0.0025f;
            for( uint32 iPoint = 0; iPoint < point_count; iPoint++ ) {
                vec3 P = point_array[ iPoint ];
                
                vec2  screenPos  = toScreenCoord( main_transform, P ) * app_dim;
                boo32 isSelected = ( point_isSelected[ iPoint ] ) ||
                    ( ( editorState->boxSelect_isActive ) && ( isInBound( screenPos, editorState->boxSelect_bound ) ) );
                vec4 color = isSelected ? Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) : COLOR_BLACK;
                
                drawQuadCXY( renderer, renderPassID_main, P, camera->xAxis * dim, camera->yAxis * dim, color );
            }
        } else {
            for( uint32 point_index = 0; point_index < point_count; point_index++ ) {
                vec3 point = point_array[ point_index ];
                
                flo32 dist = dot( origin - point, camera->zAxis );
                flo32 dim  = tanf( camera->fovAngle ) * dist * 0.0025f;
                
                vec2  screenPos  = toScreenCoord( main_transform, point ) * app_dim;
                boo32 isSelected = ( point_isSelected[ point_index ] ) ||
                    ( ( editorState->boxSelect_isActive ) && ( isInBound( screenPos, editorState->boxSelect_bound ) ) );
                vec4 color = isSelected ? Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) : COLOR_BLACK;
                
                drawQuadCXY( renderer, renderPassID_main, point, camera->xAxis * dim, camera->yAxis * dim, color );
            }
        }
    }
    
    PROFILE_BREAK();
    for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
        EDGE__ edge = editorState->edge[ edge_index ];
        
        vec3 P0 = editorState->point[ edge.index[ 0 ] ];
        vec3 P1 = editorState->point[ edge.index[ 1 ] ];
        
        vec4 color = editorState->edge_isSelected[ edge_index ] ? Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) : COLOR_BLACK;
        
        drawLine( renderer, renderPassID_main, P0, P1, color );
    }
    
    if( editorState->selectMode == selectMode_face ) {
        for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
            vec3 point  = editorState->face_centroid[ face_index ];
            vec3 normal = editorState->face_normal  [ face_index ];
            
            drawLine   ( renderer, renderPassID_main, point, point + normal, COLOR_CYAN );
        }
    }
    
    { // median point of selection
        vec3 atPos = {};
        uint32 selectedPoint_count = 0;
        for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
            if( editorState->point_isSelected[ point_index ] ) {
                selectedPoint_count++;
                atPos += editorState->point[ point_index ];
            }
        }
        
        vec3 median = {};
        if( selectedPoint_count > 0 ) {
            median = atPos / ( flo32 )selectedPoint_count;
        }
        // DISPLAY_VALUE( vec3, median );
    }
    // // DISPLAY_VALUE( int32, editorState->face_count );
    if( editorState->drawFaces ) {
        for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
            FACE face  = editorState->face[ face_index ];
            vec4 color = editorState->face_isSelected[ face_index ] ? Vec4( 1.0f, 0.75f, 0.75f, 1.0f ) : toV4Color( editorState->face_color[ face_index ] );
            
            vec3 pos0 = editorState->point[ face.index[ 0 ] ];
            vec3 pos1 = editorState->point[ face.index[ 1 ] ];
            vec3 pos2 = editorState->point[ face.index[ 2 ] ];
            if( face.index[ 3 ] == -1 ) {
                drawTri ( renderer, renderPassID_main, pos0, pos1, pos2,                                        color );
            } else {
                drawQuad( renderer, renderPassID_main, pos0, pos1, pos2, editorState->point[ face.index[ 3 ] ], color );
            }
        }
    }
    
    // drawRenderData( renderer, renderPassID_main, editorState->renderData );
    
    drawLine( renderer, renderPassID_main, Vec3( 0.0f, 0.0f, 0.0f ), Vec3( 10.0f,  0.0f,  0.0f ), COLOR_RED   );
    drawLine( renderer, renderPassID_main, Vec3( 0.0f, 0.0f, 0.0f ), Vec3(  0.0f, 10.0f,  0.0f ), COLOR_GREEN );
    drawLine( renderer, renderPassID_main, Vec3( 0.0f, 0.0f, 0.0f ), Vec3(  0.0f,  0.0f, 10.0f ), COLOR_BLUE  );
    if( editorState->boxSelect_isActive ) {
        drawRectOutline( renderer, renderPassID_overlay, editorState->boxSelect_bound, COLOR_BLACK );
    }
    
    for( int32 overhang_index = 0; overhang_index < editorState->overhang_count; overhang_index++ ) {
        OVERHANG overhang = editorState->overhang[ overhang_index ];
        
        vec3 A = editorState->point[ overhang.index[ 0 ] ];
        vec3 B = editorState->point[ overhang.index[ 1 ] ];
        vec3 C = A + Vec3( 0.0f, -1.0f, 0.0f );
        vec3 N = getNormal( cross( C - A, B - A ) );
        
        vec3 point0 = ( A + B ) * 0.5f;
        vec3 point1 = point0 + N;
        drawLine( renderer, renderPassID_main, point0, point1, COLOR_CYAN );
    }
    
    for( uint32 iLedge = 0; iLedge < editorState->nLedge; iLedge++ ) {
        drawLedge( renderer, renderPassID_main, editorState, editorState->ledge[ iLedge ] );
    }
    
    drawPoles( renderer, editorState );
    
    
    drawRect       ( renderer, renderPassID_overlay, editorState->toolWindow_bound, editorState->entity_hasUnsavedChanges ? COLOR_GRAY( 0.4f ) : COLOR_WHITE );
    drawRectOutline( renderer, renderPassID_overlay, editorState->toolWindow_bound, COLOR_BLACK );
    
    drawRect       ( renderer, renderPassID_overlay, editorState->textBox_bound, COLOR_GRAY( 0.98f ) );
    drawRectOutline( renderer, renderPassID_overlay, editorState->textBox_bound, COLOR_BLACK );
    
    if( editorState->input_string.count > 0 ) {
        drawString( renderer, renderPassID_overlay, assetID_font_default, editorState->input_string.string, getBL( editorState->textBox_bound ) + Vec2( 2.0f, 0.0f ), ALIGN_BOTTOMLEFT, COLOR_BLACK );
    }
    
    // { // draw text box
    // if( editorState->inputMode > inputMode_none ) {
    // char input_string[ 128 ] = {};
    // switch( editorState->inputMode ) {
    // case inputMode_xTranslation: { strcpy ( input_string, "X: " ); } break;
    // case inputMode_yTranslation: { strcpy ( input_string, "Y: " ); } break;
    // case inputMode_zTranslation: { strcpy ( input_string, "Z: " ); } break;
    // case inputMode_setFaceIndex: { sprintf( input_string, "SetFace %d[%d]:", editorState->facePanel_faceSelect, editorState->facePanel_indexSelect ); } break;
    // case inputMode_importEntity: { strcpy ( input_string, "Import: " ); } break;
    // case inputMode_openEntity:   { strcpy ( input_string, "Open: " ); } break;
    // default: { InvalidCodePath; } break;
    // }
    // strcat( input_string, editorState->textBox_string );
    // drawString( renderer, renderPassID_overlay, assetID_font_default, input_string, getBL( editorState->textBox_bound ) + Vec2( 2.0f, 0.0f ), ALIGN_BOTTOMLEFT, COLOR_BLACK );
    // }      
    // }
    
    { // draw utility panel
        UTILITY_PANEL * u   = &editorState->utilityPanel;
        INPUT_STRING  * str = &editorState->input_string;
        
        FONT * font = getFont( renderer->assetSystem, assetID_font_default );
        
        drawBox( renderer, renderPassID_overlay, u->bound, COLOR_GRAY( 0.98f ) );
        
        vec2 atPos = getTL( u->bound ) + Vec2( 2.0f, -2.0f );
        drawString( renderer, renderPassID_overlay, assetID_font_default, "Selection:", atPos, ALIGN_TOPLEFT, COLOR_BLACK );
        atPos.y -= ( font->advanceHeight + 2.0f );
        
        vec2 atPos1 = getTL( u->textBox[ utilityPanel_textBoxID_scale ].bound ) + Vec2( -100.0f + 2.0f, font->advanceHeight + 4.0f );
        drawString( renderer, renderPassID_overlay, assetID_font_default, "Snap:", atPos1, ALIGN_TOPLEFT, COLOR_BLACK );
        
        vec2 atPos2 = getTL( u->textBox[ utilityPanel_textBoxID_pivotX ].bound ) + Vec2( -60.0f + 2.0f, font->advanceHeight + 4.0f );
        drawString( renderer, renderPassID_overlay, assetID_font_default, "Pivot:", atPos2, ALIGN_TOPLEFT, COLOR_BLACK );
        
        char * label[ utilityPanel_textBoxID_count ] = {
            "X:", "Y:", "Z:",
            "Scale:",
            "Degrees:",
            "X:", "Y:", "Z:",
            "X:", "Y:", "Z:",
        };
        for( uint32 iTextBox = 0; iTextBox < utilityPanel_textBoxID_count; iTextBox++ ) {
            drawTextBox( renderer, renderPassID_overlay, u->textBox[ iTextBox ], label[ iTextBox ] );
        }
        
        CENTROID P = getCentroidOfSelection( editorState );
        
        drawToggleButton( renderer, renderPassID_overlay, u->grid_isActive_bound,  u->grid_isActive  );
        drawToggleButton( renderer, renderPassID_overlay, u->pivot_isActive_bound, u->pivot_isActive );
        drawPushButton  ( renderer, renderPassID_overlay, u->pivot_setPivot_bound, mouse );
        
        vec2 atPos3 = getTL( u->textBox[ utilityPanel_textBoxID_directionX ].bound ) + Vec2( -60.0f + 2.0f, font->advanceHeight + 4.0f );
        drawString( renderer, renderPassID_overlay, assetID_font_default, "Direction:", atPos3, ALIGN_TOPLEFT, COLOR_BLACK );
        drawPushButton( renderer, renderPassID_overlay, u->direction_setDirection_bound, mouse );
        
        if( editorState->editorMode == editorMode_utilityPanel_textBox ) {
            Assert( u->active );
            vec2 cursor_pos = getBL( u->active->bound ) + Vec2( ( flo32 )str->cursor * u->text_charWidth, 0.0f );
            rect cursor_bound = rectBCD( cursor_pos + Vec2( 4.0f, 4.0f ), Vec2( 2.0f, font->advanceHeight ) );
            drawRect( renderer, renderPassID_overlay, cursor_bound, COLOR_BLACK );
        }
    }
    
    { // draw entity panel
        ENTITY_PANEL * e = &editorState->entityPanel;
        drawBox( renderer, renderPassID_overlay, e->bound, COLOR_GRAY( 0.98f ) );
        
        entityPanel_drawButton( renderer, e, 0, "SPWN" );
        entityPanel_drawButton( renderer, e, 1, "LDGE" );
        entityPanel_drawButton( renderer, e, 2, "DOOR" );
        entityPanel_drawButton( renderer, e, 3, "PXIE" );
        entityPanel_drawButton( renderer, e, 4, "KILL" );
        entityPanel_drawButton( renderer, e, 5, "CHCK" );
        entityPanel_drawButton( renderer, e, 6, "VIEW" );
    }
    
    if( editorState->levelEditor_isActive ) {
        LEVEL_PANEL * l = &editorState->levelPanel;
        drawBox( renderer, renderPassID_overlay, l->bound, COLOR_GRAY( 0.98f ) );
        
        vec2 atPos = getTL( l->bound );
        rect addButton_bound = addRadius( rectTLD( atPos, l->addButton_dim ), -2.0f );
        drawPushButton( renderer, renderPassID_overlay, addButton_bound, mouse );
        drawString( renderer, renderPassID_overlay, assetID_font_default, "ADD", getCenter( addButton_bound ), ALIGN_CENTER, COLOR_BLACK );
        
        atPos.y -= l->addButton_dim.y;
        for( uint32 iLevel = 0; iLevel < l->nLevel; iLevel++ ) {
            LEVEL level = l->level[ iLevel ];
            
            rect bound = addRadius( rectTLD( atPos, l->entry_dim ), -1.0f );
            drawBox( renderer, renderPassID_overlay, bound, COLOR_WHITE );
            drawU32( renderer, renderPassID_overlay, level.entityID, atPos + Vec2( 2.0f, -2.0f ), ALIGN_TOPLEFT, COLOR_BLACK );
            atPos.y -= l->entry_dim.y;
        }
        
    }
    
    if( ( editorState->editorMode == editorMode_placeEntity ) && ( editorState->entity_select != -1 ) ) {
        switch( editorState->entityMode ) {
            case entityMode_door: {
                EDITOR__SECURE_DOOR * sDoor = editorState->secureDoor + editorState->entity_select;
                
                rect doorPanel_bound  = editorState->doorPanel_bound;
                flo32 dim = getHeight( doorPanel_bound );
                rect pBound[ 4 ] = {};
                for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                    pBound[ iPixie ] = addRadius( rectTLD( getTL( doorPanel_bound ) + Vec2( dim * ( flo32 )iPixie, 0.0f ), Vec2( dim, dim ) ), -2.0f );
                }
                
                vec4 pColor[ 4 ] = PIXIE__COLOR_HI__ARRAY;
                
                drawBox( renderer, renderPassID_overlay, doorPanel_bound, COLOR_WHITE );
                for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                    drawNumBox( renderer, renderPassID_overlay, pBound[ iPixie ], sDoor->pixie_cost[ iPixie ], pColor[ iPixie ] );
                }
            } break;
            
            case entityMode_pixie: {
                rect doorPanel_bound  = editorState->doorPanel_bound;
                flo32 dim = getHeight( doorPanel_bound );
                rect pBound[ 4 ] = {};
                for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                    pBound[ iPixie ] = addRadius( rectTLD( getTL( doorPanel_bound ) + Vec2( dim * ( flo32 )iPixie, 0.0f ), Vec2( dim, dim ) ), -2.0f );
                }
                
                vec4 pColor_hi[ 4 ] = PIXIE__COLOR_HI__ARRAY;
                vec4 pColor_lo[ 4 ] = PIXIE__COLOR_LO__ARRAY;
                
                EDITOR__PIXIE ePixie = editorState->pixie[ editorState->entity_select ];
                
                drawBox( renderer, renderPassID_overlay, doorPanel_bound, COLOR_WHITE );
                for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                    vec4 color = ( ( uint32 )ePixie.typeID == iPixie ) ? pColor_hi[ iPixie ] : pColor_lo[ iPixie ];
                    drawBox( renderer, renderPassID_overlay, pBound[ iPixie ], color );
                }            
            } break;
            
            case entityMode_view:
            case entityMode_checkpoint:
            case entityMode_killZone: {
                // do nothing
            } break;
            
            default: {
                InvalidCodePath;
            } break;
        }
    }
    
    if( openEntity ) {
        editorState->levelEditor_isActive = false;
        
        resetEditorState( editorState );
        loadEntity( editorState, permMemory, tempMemory, editorState->entityID );
        
        computeEdges( editorState );
        
        editorState->entity_hasUnsavedChanges = false;
    }
    if( saveEntity ) {
        outputXFile( tempMemory, editorState->entityID, editorState->point, editorState->point_count, editorState->face, editorState->face_count );
        
        outputEntity_begin     ( SnapMemory( tempMemory ) );
        outputEntity_overhang  ( tempMemory, editorState );
        outputEntity_ledge     ( tempMemory, editorState );
        outputEntity_pole      ( tempMemory, editorState );
        outputEntity_secureDoor( tempMemory, editorState );
        outputEntity_pixie     ( tempMemory, editorState );
        outputEntity_killZone  ( tempMemory, editorState );
        outputEntity_checkpoint( tempMemory, editorState );
        outputEntity_view      ( tempMemory, editorState );
        outputEntity_color     ( tempMemory, editorState );
        outputEntity_end       ( tempMemory, editorState->entityID );
        
        editorState->entity_hasUnsavedChanges         = false;
        editorState->entity_showUnsavedChangesMessage = false;
    }
    
    if( editorState->entity_showUnsavedChangesMessage ) {
        rect bound = rectCD( app_halfDim, Vec2( 400.0f, 200.0f ) );
        drawRect       ( renderer, renderPassID_overlay, bound, COLOR_WHITE );
        drawRectOutline( renderer, renderPassID_overlay, bound, COLOR_BLACK );
        drawString     ( renderer, renderPassID_overlay, assetID_font_default, "The entity contains unsaved changes!!", app_halfDim, ALIGN_CENTER, COLOR_BLACK );
    }
    
    if( editorState->editorMode == editorMode_translateSelection ) {
        RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
        
        switch( editorState->translateMode ) {
            case translateMode_twoAxis: {
                vec3 N = editorState->translate_axis;
                vec3 O = editorState->translate_origin;
                
                flo32 denom = dot( select.vector, N );
                if( denom != 0.0f ) {
                    flo32 t = -( dot( select.origin, N ) + dot( -O, N ) ) / denom;
                    if( t > 0.0f ) {
                        UTILITY_PANEL * u = &editorState->utilityPanel;
                        
                        vec3 intersect = select.origin + select.vector * t;
                        vec3 direction = intersect - O;
                        flo32 tX = dot( direction, editorState->translate_xAxis );
                        flo32 tY = dot( direction, editorState->translate_yAxis );
                        if( u->grid_isActive ) {
                            tX = roundTo( tX, u->scale );
                            tY = roundTo( tY, u->scale );
                        }
                        vec3 Q = O + editorState->translate_xAxis * tX;
                        vec3 P = Q + editorState->translate_yAxis * tY;
                        
                        vec2 A = toScreenCoord( main_transform, O ) * app_dim;
                        vec2 B = toScreenCoord( main_transform, P ) * app_dim;
                        vec2 C = toScreenCoord( main_transform, Q ) * app_dim;
                        
                        vec3  V  = P - O;
                        flo32 d  = getLength( V );
                        flo32 dX = dot( V, Vec3( 1.0f, 0.0f, 0.0f ) );
                        flo32 dY = dot( V, Vec3( 0.0f, 1.0f, 0.0f ) );
                        flo32 dZ = dot( V, Vec3( 0.0f, 0.0f, 1.0f ) );
                        
                        drawLine( renderer, renderPassID_overlay, A, B, COLOR_MAGENTA );
                        drawLine( renderer, renderPassID_overlay, B, C, COLOR_CYAN );
                        drawLine( renderer, renderPassID_overlay, C, A, COLOR_CYAN );
                        
                        char str[ 128 ] = {};
                        sprintf( str, "%.02f, %.02f, %.02f", dX, dY, dZ );
                        
                        FONT * font = getFont( renderer->assetSystem, assetID_font_default );
                        vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight * 3.0f );
                        rect bound = rectTRD( B, dim );
                        drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
                        
                        vec2 atPos = getTL( bound );
                        sprintf( str, "%.02f", d );
                        drawString( renderer, renderPassID_overlay, assetID_font_default, str, atPos, ALIGN_TOPLEFT, Vec4( 1.0f, 0.25f, 1.0f, 1.0f ) );
                        atPos.y -= font->advanceHeight;
                        
                        sprintf( str, "%.02f, %.02f", tX, tY );
                        drawString( renderer, renderPassID_overlay, assetID_font_default, str, atPos, ALIGN_TOPLEFT, Vec4( 0.25f, 1.0f, 1.0f, 1.0f ) );
                        atPos.y -= font->advanceHeight;
                        
                        sprintf( str, "%.02f, %.02f, %.02f", dX, dY, dZ );
                        drawString( renderer, renderPassID_overlay, assetID_font_default, str, atPos, ALIGN_TOPLEFT, COLOR_WHITE );
                    }
                }
            } break;
            
            case translateMode_oneAxis: {
                vec3 P1 = editorState->translate_origin;
                vec3 P2 = select.origin;
                
                vec3 U = editorState->translate_axis;
                vec3 V = select.vector;
                vec3 W = P1 - P2;
                
                flo32 dotUU = dot( U, U );
                flo32 dotUV = dot( U, V );
                flo32 dotUW = dot( U, W );
                flo32 dotVV = dot( V, V );
                flo32 dotVW = dot( V, W );
                
                flo32 denom = dotUU * dotVV - dotUV * dotUV;
                Assert( denom != 0.0f );
                
                flo32 s = ( dotUV * dotVW - dotVV * dotUW ) / denom;
                flo32 t = ( dotUU * dotVW - dotUV * dotUW ) / denom;
                if( t > 0.0f ) {
                    UTILITY_PANEL * u = &editorState->utilityPanel;
                    if( u->grid_isActive ) {
                        s = roundTo( s, u->scale );
                    }
                    
                    vec3 O = P1;
                    vec3 P = O + editorState->translate_axis * s;
                    
                    vec2 A = toScreenCoord( main_transform, O ) * app_dim;
                    vec2 B = toScreenCoord( main_transform, P ) * app_dim;
                    drawLine( renderer, renderPassID_overlay, A, B, COLOR_MAGENTA );
                    
                    vec3  V0  = P - O;
                    flo32 d  = getLength( V0 );
                    flo32 dX = dot( V0, Vec3( 1.0f, 0.0f, 0.0f ) );
                    flo32 dY = dot( V0, Vec3( 0.0f, 1.0f, 0.0f ) );
                    flo32 dZ = dot( V0, Vec3( 0.0f, 0.0f, 1.0f ) );
                    
                    // DISPLAY_VALUE( vec3, O );
                    // DISPLAY_VALUE( vec3, P );
                    
                    char str[ 128 ] = {};
                    sprintf( str, "%.02f, %.02f, %.02f", dX, dY, dZ );
                    
                    FONT * font = getFont( renderer->assetSystem, assetID_font_default );
                    vec2 dim   = Vec2( font->alphabet[ 0 ].advanceWidth * ( flo32 )strlen( str ), font->advanceHeight * 2.0f );
                    rect bound = rectTRD( B, dim );
                    drawRect( renderer, renderPassID_overlay, bound, Vec4( 0.0f, 0.0f, 0.0f, 0.75f ) );
                    
                    vec2 atPos = getTL( bound );
                    sprintf( str, "%.02f", d );
                    drawString( renderer, renderPassID_overlay, assetID_font_default, str, atPos, ALIGN_TOPLEFT, Vec4( 1.0f, 0.25f, 1.0f, 1.0f ) );
                    atPos.y -= font->advanceHeight;
                    
                    sprintf( str, "%.02f, %.02f, %.02f", dX, dY, dZ );
                    drawString( renderer, renderPassID_overlay, assetID_font_default, str, atPos, ALIGN_TOPLEFT, COLOR_WHITE );
                }
            } break;
            
            default: {
                InvalidCodePath;
            }
        }
    }
    
    //RAY select = getSelectRay( camera, main_transform, mouse->position, app_dim );
    //int32 bPoint = getBestPoint( editorState, select );
    //int32 bCheckpoint = getFirstCheckpoint( editorState, select );
    //// DISPLAY_VALUE( int32, bPoint );
    
    //if( bCheckpoint != -1 ) {
    //EDITOR__CHECKPOINT checkpoint = editorState->checkpoint[ bCheckpoint ];
    //flo32 degrees = toDegrees( checkpoint.radians );
    //// DISPLAY_VALUE( flo32, degrees );
    //}
}