
internal vec4
toV4Color( uint32 color ) {
    uint8 r = ( uint8 )( ( color >> 0  ) & 0xFF );
    uint8 g = ( uint8 )( ( color >> 8  ) & 0xFF );
    uint8 b = ( uint8 )( ( color >> 16 ) & 0xFF );
    uint8 a = ( uint8 )( ( color >> 24 ) & 0xFF );
    
    vec4 result = {};
    result.r = ( flo32 )r / 255.0f;
    result.g = ( flo32 )g / 255.0f;
    result.b = ( flo32 )b / 255.0f;
    result.a = ( flo32 )a / 255.0f;
    return result;
}

internal uint32
toU32Color( vec4 color ) {
    uint8 r = ( uint8 )( color.r * 255.0f );
    uint8 g = ( uint8 )( color.g * 255.0f );
    uint8 b = ( uint8 )( color.b * 255.0f );
    uint8 a = ( uint8 )( color.a * 255.0f );
    
    uint32 result = ( a << 24 ) | ( b << 16 ) | ( g << 8 ) | r;
    return result;
}

internal void
addRenderData( APP_STATE * appState, MEMORY * memory, X_FILE_DATA xFile_data, TEX_FILE_DATA texFile_data, ENTITY_FILE_DATA entityFile_data, vec4 modColor ) {
    //RENDER_DATA result = {};
    //result.vertex       = _pushArray_clear( memory, VERTEX1, xFile_data.face_count * 3 );
    //result.vertex_count = xFile_data.face_count * 3;
    //for( uint32 face_index = 0; face_index < xFile_data.face_count; face_index++ ) {
    //int32 base_index = face_index * 3;
    //int32 index0 = xFile_data.face[ base_index + 0 ];
    //int32 index1 = xFile_data.face[ base_index + 1 ];
    //int32 index2 = xFile_data.face[ base_index + 2 ];
    //
    //vec3 pos0 = xFile_data.position[ index0 ];
    //vec3 pos1 = xFile_data.position[ index1 ];
    //vec3 pos2 = xFile_data.position[ index2 ];
    //
    //vec3 normal = getNormal( cross( pos1 - pos0, pos2 - pos0 ) );
    //
    //vec2 tex0 = texFile_data.texCoord[ face_index * 3 + 0 ];
    //vec2 tex1 = texFile_data.texCoord[ face_index * 3 + 1 ];
    //vec2 tex2 = texFile_data.texCoord[ face_index * 3 + 2 ];
    //
    //result.vertex[ base_index + 0 ] = Vertex1( pos0, normal, tex0 );
    //result.vertex[ base_index + 1 ] = Vertex1( pos1, normal, tex1 );
    //result.vertex[ base_index + 2 ] = Vertex1( pos2, normal, tex2 );
    //}
    //result.modColor = modColor;
    //return result;
    
    for( uint32 iColor = 0; iColor < entityFile_data.nColor; iColor++ ) {
        uint32 nFace = entityFile_data.nColor0[ iColor ];
        
        RENDER_DATA renderData = {};
        renderData.vertex       = _pushArray( memory, VERTEX1, nFace * 3 );
        renderData.vertex_count = nFace * 3;
        renderData.modColor     = toV4Color( entityFile_data.color[ iColor ] );
        
        uint32 * color_face = entityFile_data.color_face + entityFile_data.bColor0[ iColor ];
        for( uint32 iter = 0; iter < nFace; iter++ ) {
            uint32 face_index = color_face[ iter ];
            
            int32 base_index = face_index * 3;
            int32 index0 = xFile_data.face[ base_index + 0 ];
            int32 index1 = xFile_data.face[ base_index + 1 ];
            int32 index2 = xFile_data.face[ base_index + 2 ];
            
            vec3 pos0 = xFile_data.position[ index0 ];
            vec3 pos1 = xFile_data.position[ index1 ];
            vec3 pos2 = xFile_data.position[ index2 ];
            
            vec3 normal = getNormal( cross( pos1 - pos0, pos2 - pos0 ) );
            
            vec2 tex0 = texFile_data.texCoord[ face_index * 3 + 0 ];
            vec2 tex1 = texFile_data.texCoord[ face_index * 3 + 1 ];
            vec2 tex2 = texFile_data.texCoord[ face_index * 3 + 2 ];
            
            renderData.vertex[ base_index + 0 ] = Vertex1( pos0, normal, tex0 );
            renderData.vertex[ base_index + 1 ] = Vertex1( pos1, normal, tex1 );
            renderData.vertex[ base_index + 2 ] = Vertex1( pos2, normal, tex2 );
        }
        
        Assert( appState->renderData_count < COLLISION__RENDER_DATA_COUNT );
        appState->renderData[ appState->renderData_count++ ] = renderData;
    }
}

internal void
clearSelectedPoints( EDITOR_STATE * editorState ) {
    memset( editorState->point_isSelected, 0, sizeof( boo32 ) * editorState->point_count );
}

internal void
clearSelectedEdges( EDITOR_STATE * editorState ) {
    memset( editorState->edge_isSelected, 0, sizeof( boo32 ) * editorState->edge_count );
}

internal void
clearSelectedFaces( EDITOR_STATE * editorState ) {
    memset( editorState->face_isSelected, 0, sizeof( boo32 ) * editorState->face_count );
}

internal void
clearAllSelected( EDITOR_STATE * editorState ) {
    clearSelectedPoints( editorState );
    clearSelectedEdges ( editorState );
    clearSelectedFaces ( editorState );
}

struct INDEX_LIST__ {
    uint32 * list;
    uint32   count;
    uint32   maxCount;
};

internal INDEX_LIST__
IndexList( MEMORY * memory, uint32 maxCount ) {
    INDEX_LIST__ result = {};
    result.list     = _pushArray( memory, uint32, maxCount );
    result.maxCount = maxCount;
    return result;
}

internal uint32
add( INDEX_LIST__ * indexList, uint32 i ) {
    Assert( indexList->count < indexList->maxCount );
    uint32 result = indexList->count;
    indexList->list[ indexList->count++ ] = i;
    return result;
}

internal uint32
add_noDup( INDEX_LIST__ * indexList, uint32 i ) {
    boo32  wasFound = false;
    uint32 result   = 0;
    for( uint32 iEdge = 0; ( !wasFound ) && ( iEdge < indexList->count ); iEdge++ ) {
        uint32 e0 = i;
        uint32 e1 = indexList->list[ iEdge ];
        
        if( e0 == e1 ) {
            wasFound = true;
            result   = iEdge;
        }
    }
    if( !wasFound ) {
        result = add( indexList, i );
    }
    return result;
}

internal void
copyTo( MEMORY * memory, INDEX_LIST__ indexList ) {
    uint32 * dest = _pushArray( memory, uint32, indexList.count );
    memcpy( dest, indexList.list, sizeof( uint32 ) * indexList.count );
}

internal void
pop( MEMORY * memory, INDEX_LIST__ indexList ) {
    _popArray( memory, uint32, indexList.maxCount );
}

struct EDGE_LIST__ {
    EDGE__ * list;
    uint32   count;
    uint32   maxCount;
};

internal EDGE_LIST__
EdgeList( MEMORY * memory, uint32 maxCount ) {
    EDGE_LIST__ result = {};
    result.list     = _pushArray( memory, EDGE__, maxCount );
    result.maxCount = maxCount;
    return result;
}

internal uint32
add( EDGE_LIST__ * edgeList, EDGE__ e ) {
    Assert( edgeList->count < edgeList->maxCount );
    uint32 result = edgeList->count;
    edgeList->list[ edgeList->count++ ] = e;
    return result;
}

internal boo32
operator==( EDGE__ e, EDGE__ f ) {
    boo32 result = ( ( e.index[ 0 ] == f.index[ 0 ] ) && ( e.index[ 1 ] == f.index[ 1 ] ) );
    return result;
}

internal uint32
add_noDup( EDGE_LIST__ * edgeList, EDGE__ e ) {
    boo32  wasFound = false;
    uint32 result   = 0;
    for( uint32 iEdge = 0; ( !wasFound ) && ( iEdge < edgeList->count ); iEdge++ ) {
        EDGE__ e0 = e;
        EDGE__ e1 = edgeList->list[ iEdge ];
        
        if( e0 == e1 ) {
            wasFound = true;
            result   = iEdge;
        }
    }
    if( !wasFound ) {
        result = add( edgeList, e );
    }
    return result;
}

internal void
copyTo( MEMORY * memory, EDGE_LIST__ edgeList ) {
    EDGE__ * dest = _pushArray( memory, EDGE__, edgeList.count );
    memcpy( dest, edgeList.list, sizeof( EDGE__ ) * edgeList.count );
}

internal void
pop( MEMORY * memory, EDGE_LIST__ edgeList ) {
    _popArray( memory, EDGE__, edgeList.maxCount );
}

struct FACE_LIST__ {
    FACE   * list;
    uint32   count;
    uint32   maxCount;
};

internal FACE_LIST__
FaceList( MEMORY * memory, uint32 maxCount ) {
    FACE_LIST__ result = {};
    result.list     = _pushArray( memory, FACE, maxCount );
    result.maxCount = maxCount;
    return result;
}

internal uint32
add( FACE_LIST__ * faceList, FACE f ) {
    Assert( faceList->count < faceList->maxCount );
    uint32 result = faceList->count;
    faceList->list[ faceList->count++ ] = f;
    return result;
}

internal boo32
operator==( FACE f, FACE g ) {
    boo32 result = true;
    for( uint32 iElem = 0; iElem < 4; iElem++ ) {
        if( f.index[ iElem ] != g.index[ iElem ] ) {
            result = false;
        }
    }
}

internal uint32
add_noDup( FACE_LIST__ * faceList, FACE f ) {
    boo32  wasFound = false;
    uint32 result   = 0;
    for( uint32 iFace = 0; ( !wasFound ) && ( iFace < faceList->count ); iFace++ ) {
        FACE f0 = f;
        FACE f1 = faceList->list[ iFace ];
        
        if( f0 == f1 ) {
            wasFound = true;
            result   = iFace;
        }
    }
    if( !wasFound ) {
        result = add( faceList, f );
    }
    return result;
}

internal void
copyTo( MEMORY * memory, FACE_LIST__ faceList ) {
    FACE * dest = _pushArray( memory, FACE, faceList.count );
    memcpy( dest, faceList.list, sizeof( FACE ) * faceList.count );
}

internal void
pop( MEMORY * memory, FACE_LIST__ faceList ) {
    _popArray( memory, uint32, faceList.maxCount );
}

internal flo32
mat3_cofactor( mat3 A, uint32 row_index, uint32 column_index ) {
    // cofactor = -1^i+j * det( minor )
    flo32  mat2_elem[ 4 ] = {};
    uint32 dest_index = 0;
    for( uint32 j = 0; j < 3; j++ ) {
        if( j != column_index ) {
            for( uint32 i = 0; i < 3; i++ ) {
                if( i != row_index ) {
                    mat2_elem[ dest_index++ ] = A.elem[ j * 3 + i ];
                }
            }
        }
    }
    Assert( dest_index == 4 );
    
    flo32 result = mat2_elem[ 0 ] * mat2_elem[ 3 ] - mat2_elem[ 1 ] * mat2_elem[ 2 ];
    if( ( ( row_index + column_index ) % 2 ) == 1 ) {
        result = -result;
    }
    return result;
}

internal flo32
mat3_determinant( mat3 A ) {
    // det = sum of element * cofactor
    flo32 result = 0.0f;
    for( uint32 iter = 0; iter < 3; iter++ ) {
        vec3 column = A.column[ iter ];
        
        flo32 element  = column.z;
        flo32 cofactor = mat3_cofactor( A, 2, iter );
        
        result += ( element * cofactor );
    }
    return result;
}

internal flo32
mat4_cofactor( mat4 A, uint32 row_index, uint32 column_index ) {
    // cofactor = -1^i+j * det( minor )
    mat3   minor = {};
    uint32 dest_index = 0;
    for( uint32 j = 0; j < 4; j++ ) {
        if( j != column_index ) {
            for( uint32 i = 0; i < 4; i++ ) {
                if( i != row_index ) {
                    minor.elem[ dest_index++ ] = A.elem[ j * 4 + i ];
                }
            }
        }
    }
    Assert( dest_index == 9 );
    
    flo32 result = mat3_determinant( minor );
    if( ( ( row_index + column_index ) % 2 ) == 1 ) {
        result = -result;
    }
    return result;
}

internal flo32
mat4_determinant( mat4 A ) {
    // sum of element * cofactor
    flo32 result = 0.0f;
    for( uint32 iter = 0; iter < 4; iter++ ) {
        vec4 column = A.column[ iter ];
        
        flo32 element  = column.w;
        flo32 cofactor = mat4_cofactor( A, 3, iter );
        
        result += ( element * cofactor );
    }
    return result;
}

internal mat4
mat4_cofactor( mat4 A ) {
    mat4 result = {};
    for( uint32 j = 0; j < 4; j++ ) {
        for( uint32 i = 0; i < 4; i++ ) {
            result.elem[ j * 4 + i ] = mat4_cofactor( A, i, j );
        }
    }
    return result;
}

internal mat4
mat4_adjugate( mat4 A ) {
    mat4 cofactor = mat4_cofactor( A );
    mat4 result   = mat4_transpose( cofactor );
    return result;
}

internal mat4
mat4_inverse( mat4 A ) {
    PROFILE_FUNCTION();
    // 1 / |A| * M
    flo32 det = mat4_determinant( A );
    mat4  M   = mat4_adjugate   ( A );
    
    Assert( det != 0.0f );
    mat4 result = M * ( 1.0f / det );
    return result;
}

internal vec2
toScreenCoord( mat4 camera_transform, vec3 point ) {
    vec4 position = camera_transform * Vec4( point, 1.0f );
    vec2 result   = ( ( Vec2( position.x, position.y ) / position.w ) * 0.5f + Vec2( 0.5f, 0.5f ) );
    return result;
}

internal vec3
screenCoordToWorld( vec2 pos, vec2 dim, mat4 camera_transform, vec3 camera_position ) {
    flo32 tX = ( pos.x / dim.x ) * 2.0f - 1.0f;
    flo32 tY = ( pos.y / dim.y ) * 2.0f - 1.0f;
    
    mat4 inverse_transform = mat4_inverse( camera_transform );
    vec4 nearPoint         = inverse_transform * Vec4( tX, tY, -1.0f, 1.0f );
    nearPoint.xyz    /= nearPoint.w;
    
    vec3 result = getNormal( nearPoint.xyz - camera_position );
    return result;
}

internal vec3
getCameraPos( CAMERA * camera ) {
    vec3 result = camera->position + camera->zAxis * camera->dist;
    return result;
}

internal void
drawLine( RENDERER * renderer, RENDER_PASS_ID passID, vec2 A, vec2 B, vec4 color ) {
    int32 vertex_index    = renderer->vertex_count[ 0 ];
    int32 vertex_count    = 2;
	DEBUG_VERTEX * vertex = ( ( DEBUG_VERTEX * )renderer->vertex_ptr[ 0 ] ) + vertex_index;
    *vertex++ = DebugVertex( A, Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( B, Vec2( 0.0f, 0.0f ) );
    
    addVertex( renderer, 0, vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lines, vertex_index, vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawRect( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, vec4 color ) {
    int32 vertex_index    = renderer->vertex_count[ 0 ];
    int32 vertex_count    = 4;
	DEBUG_VERTEX * vertex = ( ( DEBUG_VERTEX * )renderer->vertex_ptr[ 0 ] ) + vertex_index;
    
    vec2 pos[ 4 ] = {
        Vec2( bound.left,  bound.bottom ),
        Vec2( bound.right, bound.bottom ),
        Vec2( bound.left,  bound.top ),
        Vec2( bound.right, bound.top ),
    };
    
    *vertex++ = DebugVertex( pos[ 0 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 1 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 2 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 3 ], Vec2( 0.0f, 0.0f ) );
    
    addVertex( renderer, 0, vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangleStrip, vertex_index, vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawRectOutline( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, vec4 color ) {
    int32 vertex_index    = renderer->vertex_count[ 0 ];
    int32 vertex_count    = 5;
	DEBUG_VERTEX * vertex = ( ( DEBUG_VERTEX * )renderer->vertex_ptr[ 0 ] ) + vertex_index;
    
    vec2 pos[ 4 ] = {
        Vec2( bound.left,  bound.bottom ),
        Vec2( bound.right, bound.bottom ),
        Vec2( bound.left,  bound.top ),
        Vec2( bound.right, bound.top ),
    };
    
    *vertex++ = DebugVertex( pos[ 0 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 1 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 3 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 2 ], Vec2( 0.0f, 0.0f ) );
    *vertex++ = DebugVertex( pos[ 0 ], Vec2( 0.0f, 0.0f ) );
    
    addVertex( renderer, 0, vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lineStrip, vertex_index, vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawToolbarButton( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, char * text, boo32 isSelected ) {
    vec4 background_color = isSelected ? COLOR_GRAY( 0.5f ) : COLOR_GRAY( 0.25f );
    vec4 outline_color    = COLOR_BLACK;
    
    rect inner_bound = addRadius( bound, -1.0f );
    drawRect       ( renderer, passID, inner_bound, background_color );
    drawRectOutline( renderer, passID, inner_bound, outline_color );
    drawString     ( renderer, passID, assetID_font_default, text, getCenter( bound ), ALIGN_CENTER, COLOR_BLACK );
}

internal void
computeFaceCentroids( EDITOR_STATE * editorState ) {
    PROFILE_FUNCTION();
    
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        FACE face = editorState->face[ face_index ];
        
        vec3 centroid = {};
        centroid += editorState->point[ face.index[ 0 ] ];
        centroid += editorState->point[ face.index[ 1 ] ];
        centroid += editorState->point[ face.index[ 2 ] ];
        if( face.index[ 3 ] == -1 ) {
            centroid /= 3.0f;
        } else {
            centroid += editorState->point[ face.index[ 3 ] ];
            centroid /= 4.0f;
        }
        
        editorState->face_centroid[ face_index ] = centroid;
    }
}

internal void
computeFaceNormals( EDITOR_STATE * editorState ) {
    PROFILE_FUNCTION();
    
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        FACE face = editorState->face[ face_index ];
        
        vec3 P0 = editorState->point[ face.index[ 0 ] ];
        vec3 P1 = editorState->point[ face.index[ 1 ] ];
        vec3 P2 = editorState->point[ face.index[ 2 ] ];
        vec3 normal = getNormal( cross( P1 - P0, P2 - P0 ) );
        
        editorState->face_normal[ face_index ] = normal;
    }
}

internal int32
addPoint( EDITOR_STATE * editorState, vec3 point, boo32 isSelected = false ) {
    int32 result = editorState->point_count;
    editorState->point           [ editorState->point_count ] = point;
    editorState->point_isSelected[ editorState->point_count ] = isSelected;
    editorState->point_count++;
    Assert( editorState->point_count < EDITOR__POINT_COUNT );
    
    return result;
}

internal void
addEdge( EDITOR_STATE * editorState, EDGE__ edge, boo32 isSelected = false ) {
    Assert( editorState->edge_count < EDITOR__EDGE_COUNT );
    editorState->edge           [ editorState->edge_count ] = edge;
    editorState->edge_isSelected[ editorState->edge_count ] = isSelected;
    editorState->edge_count++;
}

internal void
addFace( EDITOR_STATE * editorState, FACE face, boo32 isSelected = false ) {
    Assert( editorState->face_count < EDITOR__FACE_COUNT );
    editorState->face           [ editorState->face_count ] = face;
    editorState->face_isSelected[ editorState->face_count ] = isSelected;
    editorState->face_count++;
}

internal void
addOverhang( EDITOR_STATE * editorState, OVERHANG overhang, boo32 isSelected = false ) {
    editorState->overhang           [ editorState->overhang_count ] = overhang;
    editorState->overhang_isSelected[ editorState->overhang_count ] = isSelected;
    editorState->overhang_count++;
    Assert( editorState->overhang_count < EDITOR__OVERHANG_COUNT );
}

internal void
addLedge( EDITOR_STATE * editorState, EDITOR__LEDGE ledge ) {
    Assert( editorState->nLedge < EDITOR__LEDGE_COUNT );
    editorState->ledge[ editorState->nLedge++ ] = ledge;
}

internal EDITOR__LEDGE
toLedge( EDGE__ edge ) {
    EDITOR__LEDGE result = {};
    result.point_index[ 0 ] = edge.index[ 0 ];
    result.point_index[ 1 ] = edge.index[ 1 ];
    return result;
}

internal EDGE__
toEdge( EDITOR__LEDGE ledge ) {
    EDGE__ result = {};
    result.index[ 0 ] = ledge.point_index[ 0 ];
    result.index[ 1 ] = ledge.point_index[ 1 ];
    return result;
}

internal LEDGE
toLedge( EDITOR_STATE * e, EDITOR__LEDGE ledge ) {
    LEDGE result = {};
    result.point[ 0 ] = e->point[ ledge.point_index[ 0 ] ];
    result.point[ 1 ] = e->point[ ledge.point_index[ 1 ] ];
    return result;
}

internal void
addPole( EDITOR_STATE * editorState, POLE pole, boo32 isSelected = false ) {
    editorState->pole           [ editorState->pole_count ] = pole;
    editorState->pole_isSelected[ editorState->pole_count ] = isSelected;
    editorState->pole_count++;
    Assert( editorState->pole_count < EDITOR__POLE_COUNT );
}

internal void
addEdge_noDup( EDITOR_STATE * e, EDGE__ edge0 ) {
    int32 index = -1;
    for( int32 iEdge = 0; ( index == -1 ) && ( iEdge < e->edge_count ); iEdge++ ) {
        EDGE__ edge1 = e->edge[ iEdge ];
        
        if( edge0 == edge1 ) {
            index = iEdge;
        }
    }
    if( index == -1 ) {
        addEdge( e, edge0 );
    }
}

internal void
computeEdges( EDITOR_STATE * editorState ) {
    PROFILE_FUNCTION();
    
    editorState->edge_count = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        FACE face = editorState->face[ face_index ];
        
        if( face.index[ 3 ] != -1 ) {
            addEdge_noDup( editorState, Edge__( face.index[ 0 ], face.index[ 1 ] ) );
            addEdge_noDup( editorState, Edge__( face.index[ 1 ], face.index[ 3 ] ) );
            addEdge_noDup( editorState, Edge__( face.index[ 3 ], face.index[ 2 ] ) );
            addEdge_noDup( editorState, Edge__( face.index[ 2 ], face.index[ 0 ] ) );
        } else {
            addEdge_noDup( editorState, Edge__( face.index[ 0 ], face.index[ 1 ] ) );
            addEdge_noDup( editorState, Edge__( face.index[ 1 ], face.index[ 2 ] ) );
            addEdge_noDup( editorState, Edge__( face.index[ 2 ], face.index[ 0 ] ) );
        }
    }
    
    for( uint32 iLedge = 0; iLedge < editorState->nLedge; iLedge++ ) {
        EDITOR__LEDGE ledge = editorState->ledge[ iLedge ];
        EDGE__ edge = toEdge( ledge );
        addEdge_noDup( editorState, edge );
    }
    
    // sort edges
    // remove duplicates
}

internal void
computeEdgeCentroids( EDITOR_STATE * editorState ) {
    PROFILE_FUNCTION();
    
    for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
        EDGE__ edge = editorState->edge[ edge_index ];
        
        vec3 P0 = editorState->point[ edge.index[ 0 ] ];
        vec3 P1 = editorState->point[ edge.index[ 1 ] ];
        vec3 centroid = ( P0 + P1 ) * 0.5f;
        editorState->edge_centroid[ edge_index ] = centroid;
    }
}

internal void
updateSelectedVertices( EDITOR_STATE * editorState ) {
    switch( editorState->selectMode ) {
        case selectMode_vertex: {
            // do nothing
        } break;
        
        case selectMode_edge: {
            memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
            for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
                if( editorState->edge_isSelected[ edge_index ] ) {
                    EDGE__ edge = editorState->edge[ edge_index ];
                    
                    editorState->point_isSelected[ edge.index[ 0 ] ] = true;
                    editorState->point_isSelected[ edge.index[ 1 ] ] = true;
                }
            }
        } break;
        
        case selectMode_face: {
            memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
            memset( editorState->edge_isSelected,  0, sizeof( uint32 ) * editorState->edge_count  );
            for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
                if( editorState->face_isSelected[ face_index ] ) {
                    FACE face = editorState->face[ face_index ];
                    
                    editorState->point_isSelected[ face.index[ 0 ] ] = true;
                    editorState->point_isSelected[ face.index[ 1 ] ] = true;
                    editorState->point_isSelected[ face.index[ 2 ] ] = true;
                    if( face.index[ 3 ] != -1 ) {
                        editorState->point_isSelected[ face.index[ 3 ] ] = true;
                    }
                }
            }
            
            for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
                EDGE__ edge = editorState->edge[ edge_index ];
                if( ( editorState->point_isSelected[ edge.index[ 0 ] ] ) && ( editorState->point_isSelected[ edge.index[ 1 ] ] ) ) {
                    editorState->edge_isSelected[ edge_index ] = true;
                }
            }
        } break;
        
        default: { InvalidCodePath; }
    }
}

internal EDGE__
updateToNewEdge( EDGE__ oldEdge, uint32 * oldVert, uint32 * newVert, uint32 vert_count ) {
    int32 index[ 2 ] = { -1, -1 };
    for( uint32 vert_index = 0; ( ( index[ 0 ] == -1 ) || ( index[ 1 ] == -1 ) ) && ( vert_index < vert_count ); vert_index++ ) {
        if( oldEdge.index[ 0 ] == ( int32 )oldVert[ vert_index ] ) {
            index[ 0 ] = ( int32 )newVert[ vert_index ];
        }
        if( oldEdge.index[ 1 ] == ( int32 )oldVert[ vert_index ] ) {
            index[ 1 ] = ( int32 )newVert[ vert_index ];
        }
    }
    Assert( ( index[ 0 ] != -1 ) && ( index[ 1 ] != -1 ) );
    
    EDGE__ result = Edge__( index[ 0 ], index[ 1 ] );
    return result;
}

internal FACE
updateToNewFace( FACE oldFace, uint32 * oldVert, uint32 * newVert, uint32 vert_count ) {
    FACE result = { -1, -1, -1, -1 };
    
    uint32 elem_count = ( oldFace.index[ 3 ] == -1 ) ? 3 : 4;
    for( uint32 elem_index = 0; elem_index < 4; elem_index++ ) { result.index[ elem_index ] = -1; }
    
    boo32 continueFind = true;
    for( uint32 vert_index = 0; continueFind && ( vert_index < vert_count ); vert_index++ ) {
        for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
            if( oldFace.index[ elem_index ] == ( int32 )oldVert[ vert_index ] ) {
                result.index[ elem_index ] = ( int32 )newVert[ vert_index ];
            }
        }
        
        continueFind = false;
        for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
            if( result.index[ elem_index ] == -1 ) {
                continueFind = true;
            }
        }
    }
    
    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
        Assert( result.index[ elem_index ] != -1 );
    }
    
    return result;
}

internal void
add( UNDO_SYSTEM * undoSystem, UNDO_EVENT event, MEMORY * tempMemory ) {
    Assert( getSnapUsed( tempMemory ) == event.data_size );
    
    uint32 memory_left = ( uint32 )( UNDO_SYSTEM__MEMORY_SIZE - undoSystem->memory_at );
    if( ( undoSystem->event_atIndex >= UNDO_SYSTEM__EVENT_COUNT ) || ( event.data_size > memory_left ) ) {
        Assert( undoSystem->event_atIndex > UNDO_SYSTEM__BUMP_COUNT );
        
        uint32 bump_count = UNDO_SYSTEM__BUMP_COUNT;
        uint8 * ptr_src  = undoSystem->event[ UNDO_SYSTEM__BUMP_COUNT ].data;
        uint8 * ptr_dest = undoSystem->memory_base;
        
        uint32 bump_size   = ( uint32 )( ptr_src - ptr_dest );
        Assert( ( bump_size % 32 ) == 0 );
        uint32 mem_left = undoSystem->memory_at - bump_size;
        
        while( mem_left > 0 ) {
            uint32 block_size = minValue( bump_size, mem_left );
            memcpy( ptr_dest, ptr_src, block_size );
            
            ptr_src  += block_size;
            ptr_dest += block_size;
            memory_left -= block_size;
        }
        undoSystem->memory_at -= bump_size;
        
        for( uint32 event_index = UNDO_SYSTEM__BUMP_COUNT; event_index < undoSystem->event_atIndex; event_index++ ) {
            UNDO_EVENT e = undoSystem->event[ event_index ];
            e.data -= bump_size;
            
            Assert( event.data >= undoSystem->memory_base );
            
            undoSystem->event[ event_index - UNDO_SYSTEM__BUMP_COUNT ] = e;
        }
        undoSystem->event_atIndex -= UNDO_SYSTEM__BUMP_COUNT;
    }
    
    event.data = undoSystem->memory_base + undoSystem->memory_at;
    memcpy( event.data, getSnapBase( tempMemory ), event.data_size );
    undoSystem->memory_at += event.data_size;
    
    popToSnap( tempMemory );
    
    undoSystem->event[ undoSystem->event_atIndex++ ] = event;
    undoSystem->event_maxIndex = undoSystem->event_atIndex;
}

// TRANSLATE SELECTION

internal void
undo_translate( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__TRANSLATE * translate   = _addr ( data_at, UNDO_EVENT__TRANSLATE );
    uint32                * index_array = _addra( data_at, uint32, translate->index_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    for( uint32 point_index = 0; point_index < translate->index_count; point_index++ ) {
        uint32 index = index_array[ point_index ];
        
        editorState->point           [ index ] -= translate->translate_vector;
        editorState->point_isSelected[ index ] = true;
    }
}

internal void
redo_translate( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__TRANSLATE * translate   = _addr ( data_at, UNDO_EVENT__TRANSLATE );
    uint32                * index_array = _addra( data_at, uint32, translate->index_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    for( uint32 point_index = 0; point_index < translate->index_count; point_index++ ) {
        uint32 index = index_array[ point_index ];
        
        editorState->point           [ index ] += translate->translate_vector;
        editorState->point_isSelected[ index ] = true;
    }
}

internal void
add_translate( UNDO_SYSTEM * undoSystem, vec3 translate_vector, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    UNDO_EVENT__TRANSLATE * translate = _pushType( tempMemory, UNDO_EVENT__TRANSLATE );
    translate->translate_vector = translate_vector;
    
    uint32 isSelected_count = 0;
    for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
        if( editorState->point_isSelected[ point_index ] ) {
            isSelected_count++;
        }
    }
    
    translate->index_count = isSelected_count;
    uint32 * isSelected_index = _pushArray( tempMemory, uint32, isSelected_count );
    uint32 index = 0;
    for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
        if( editorState->point_isSelected[ point_index ] ) {
            isSelected_index[ index++ ] = point_index;
        }
    }
    Assert( index == isSelected_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_translate;
    event.redo_callback = redo_translate;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// MODIFY VERTEX
//  called by any command that does not add or delete geometry
//  ROTATE SELECTION
//  SCALE SELECTION

internal void
undo_modifyVertex( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__ROTATE * rotate  = _addr ( data_at, UNDO_EVENT__ROTATE );
    uint32             * index   = _addra( data_at, uint32, rotate->nSelect );
    vec3               * oldPos  = _addra( data_at, vec3,   rotate->nSelect );
    vec3               * newPos  = _addra( data_at, vec3,   rotate->nSelect );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    clearSelectedPoints( editorState );
    
    for( uint32 iPoint = 0; iPoint < rotate->nSelect; iPoint++ ) {
        uint32 i = index[ iPoint ];
        
        editorState->point_isSelected[ i ] = true;
        editorState->point           [ i ] = oldPos[ iPoint ];
    }
}

internal void
redo_modifyVertex( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__ROTATE * rotate  = _addr ( data_at, UNDO_EVENT__ROTATE );
    uint32             * index   = _addra( data_at, uint32, rotate->nSelect );
    vec3               * oldPos  = _addra( data_at, vec3,   rotate->nSelect );
    vec3               * newPos  = _addra( data_at, vec3,   rotate->nSelect );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    clearSelectedPoints( editorState );
    
    for( uint32 iPoint = 0; iPoint < rotate->nSelect; iPoint++ ) {
        uint32 i = index[ iPoint ];
        
        editorState->point_isSelected[ i ] = true;
        editorState->point           [ i ] = newPos[ iPoint ];
    }
}

internal void
add_modifyVertex( UNDO_SYSTEM * undoSystem, uint32 nSelect, uint32 * index, vec3 * oldPos, vec3 * newPos, MEMORY * tempMemory ) {
    UNDO_EVENT__ROTATE * rotate = _pushType( tempMemory, UNDO_EVENT__ROTATE );
    rotate->nSelect = nSelect;
    
    uint32 * dest0 = _pushArray( tempMemory, uint32, nSelect );
    vec3   * dest1 = _pushArray( tempMemory, vec3,   nSelect );
    vec3   * dest2 = _pushArray( tempMemory, vec3,   nSelect );
    memcpy( dest0, index,  sizeof( uint32 ) * nSelect );
    memcpy( dest1, oldPos, sizeof( vec3   ) * nSelect );
    memcpy( dest2, newPos, sizeof( vec3   ) * nSelect );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_modifyVertex;
    event.redo_callback = redo_modifyVertex;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// DUPLICATE VERTEX

internal void
undo_duplicateVertex( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DUPLICATE_VERTEX * duplicate   = _addr ( data_at, UNDO_EVENT__DUPLICATE_VERTEX );
    uint32                       * index_array = _addra( data_at, uint32, duplicate->vertex_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    editorState->point_count -= duplicate->vertex_count;
    
    for( uint32 point_index = 0; point_index < duplicate->vertex_count; point_index++ ) {
        editorState->point_isSelected[ index_array[ point_index ] ] = true;
    }
}

internal void
redo_duplicateVertex( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DUPLICATE_VERTEX * duplicate   = _addr ( data_at, UNDO_EVENT__DUPLICATE_VERTEX );
    uint32                       * index_array = _addra( data_at, uint32, duplicate->vertex_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    int32 point_count = editorState->point_count;
    for( uint32 point_index = 0; point_index < duplicate->vertex_count; point_index++ ) {
        uint32 index = index_array[ point_index ];
        
        addPoint( editorState, editorState->point[ index ], true );
        editorState->point_isSelected[ index ] = false;
    }
}

internal void
add_duplicateVertex( UNDO_SYSTEM * undoSystem, uint32 * duplicated_indices, uint32 duplicated_count, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    UNDO_EVENT__DUPLICATE_VERTEX * duplicate = _pushType( tempMemory, UNDO_EVENT__DUPLICATE_VERTEX );
    duplicate->vertex_count = duplicated_count;
    
    uint32 * dup_indices = _pushArray( tempMemory, uint32, duplicated_count );
    memcpy( dup_indices, duplicated_indices, sizeof( uint32 ) * duplicated_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_duplicateVertex;
    event.redo_callback = redo_duplicateVertex;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// DUPLICATE EDGE

internal void
undo_duplicateEdge( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DUPLICATE_EDGE * duplicate    = _addr ( data_at, UNDO_EVENT__DUPLICATE_EDGE );
    uint32                     * vertex_array = _addra( data_at, uint32, duplicate->vertex_count );
    uint32                     * edge_array   = _addra( data_at, uint32, duplicate->edge_count   );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    memset( editorState->edge_isSelected,  0, sizeof( uint32 ) * editorState->edge_count  );
    
    editorState->point_count -= duplicate->vertex_count;
    editorState->edge_count  -= duplicate->edge_count;
    
    for( uint32 point_index = 0; point_index < duplicate->vertex_count; point_index++ ) {
        editorState->point_isSelected[ vertex_array[ point_index ] ] = true;
    }
    
    for( uint32 edge_index = 0; edge_index < duplicate->edge_count; edge_index++ ) {
        editorState->edge_isSelected[ edge_array[ edge_index ] ] = true;
    }
}

internal void
redo_duplicateEdge( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DUPLICATE_EDGE * duplicate    = _addr ( data_at, UNDO_EVENT__DUPLICATE_EDGE );
    uint32                     * vertex_array = _addra( data_at, uint32, duplicate->vertex_count );
    uint32                     * edge_array   = _addra( data_at, uint32, duplicate->edge_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    memset( editorState->edge_isSelected,  0, sizeof( uint32 ) * editorState->edge_count  );
    
    uint32 * newIndex = _pushArray( tempMemory, uint32, duplicate->vertex_count );
    
    int32 point_count = editorState->point_count;
    for( uint32 point_index = 0; point_index < duplicate->vertex_count; point_index++ ) {
        uint32 index = vertex_array[ point_index ];
        
        newIndex[ point_index ] = addPoint( editorState, editorState->point[ index ], true );
    }
    
    int32 edge_count = editorState->edge_count;
    for( uint32 edge_index = 0; edge_index < duplicate->edge_count; edge_index++ ) {
        EDGE__ oldEdge = editorState->edge[ edge_array[ edge_index ] ];
        EDGE__ newEdge = updateToNewEdge( oldEdge, vertex_array, newIndex, duplicate->vertex_count );
        addEdge( editorState, newEdge, true );
    }
    
    _popArray( tempMemory, uint32, duplicate->vertex_count );
}

internal void
add_duplicateEdge( UNDO_SYSTEM * undoSystem, uint32 * dup_edge, uint32 edge_count, uint32 * dup_vertex, uint32 vertex_count, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    UNDO_EVENT__DUPLICATE_EDGE * dup = _pushType( tempMemory, UNDO_EVENT__DUPLICATE_EDGE );
    dup->vertex_count = vertex_count;
    dup->edge_count   = edge_count;
    
    uint32 * dup0 = _pushArray( tempMemory, uint32, vertex_count );
    memcpy( dup0, dup_vertex, sizeof( uint32 ) * vertex_count );
    
    uint32 * dup1 = _pushArray( tempMemory, uint32, edge_count );
    memcpy( dup1, dup_edge, sizeof( uint32 ) * edge_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_duplicateEdge;
    event.redo_callback = redo_duplicateEdge;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// DUPLICATE FACE

internal void
undo_duplicateFace( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DUPLICATE_FACE * duplicate    = _addr ( data_at, UNDO_EVENT__DUPLICATE_FACE );
    uint32                     * vertex_array = _addra( data_at, uint32, duplicate->vertex_count );
    uint32                     * edge_array   = _addra( data_at, uint32, duplicate->edge_count   );
    uint32                     * face_array   = _addra( data_at, uint32, duplicate->face_count   );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    memset( editorState->edge_isSelected,  0, sizeof( uint32 ) * editorState->edge_count  );
    memset( editorState->face_isSelected,  0, sizeof( uint32 ) * editorState->face_count  );
    
    editorState->point_count -= duplicate->vertex_count;
    editorState->edge_count  -= duplicate->edge_count;
    editorState->face_count  -= duplicate->face_count;
    
    for( uint32 point_index = 0; point_index < duplicate->vertex_count; point_index++ ) {
        editorState->point_isSelected[ vertex_array[ point_index ] ] = true;
    }
    
    for( uint32 edge_index = 0; edge_index < duplicate->edge_count; edge_index++ ) {
        editorState->edge_isSelected[ edge_array[ edge_index ] ] = true;
    }
    
    for( uint32 face_index = 0; face_index < duplicate->face_count; face_index++ ) {
        editorState->face_isSelected[ face_array[ face_index ] ] = true;
    }
}

internal void
redo_duplicateFace( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DUPLICATE_FACE * duplicate    = _addr ( data_at, UNDO_EVENT__DUPLICATE_FACE );
    uint32                     * vertex_array = _addra( data_at, uint32, duplicate->vertex_count );
    uint32                     * edge_array   = _addra( data_at, uint32, duplicate->edge_count   );
    uint32                     * face_array   = _addra( data_at, uint32, duplicate->face_count   );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    memset( editorState->edge_isSelected,  0, sizeof( uint32 ) * editorState->edge_count  );
    memset( editorState->face_isSelected,  0, sizeof( uint32 ) * editorState->face_count  );
    
    uint32 * newIndex = _pushArray( tempMemory, uint32, duplicate->vertex_count );
    
    int32 point_count = editorState->point_count;
    for( uint32 point_index = 0; point_index < duplicate->vertex_count; point_index++ ) {
        uint32 index = vertex_array[ point_index ];
        
        newIndex[ point_index ] = addPoint( editorState, editorState->point[ index ], true );
    }
    
    int32 edge_count = editorState->edge_count;
    for( uint32 edge_index = 0; edge_index < duplicate->edge_count; edge_index++ ) {
        EDGE__ oldEdge = editorState->edge[ edge_array[ edge_index ] ];
        EDGE__ newEdge = updateToNewEdge( oldEdge, vertex_array, newIndex, duplicate->vertex_count );
        addEdge( editorState, newEdge, true );
    }
    
    int32 face_count = editorState->face_count;
    for( uint32 face_index = 0; face_index < duplicate->face_count; face_index++ ) {
        FACE oldFace = editorState->face[ face_array[ face_index ] ];
        FACE newFace = updateToNewFace( oldFace, vertex_array, newIndex, duplicate->vertex_count );
        
        addFace( editorState, newFace, true );
        editorState->face_isSelected[ face_index ] = false;
    }
    
    _popArray( tempMemory, uint32, duplicate->vertex_count );
}

internal void
add_duplicateFace( UNDO_SYSTEM * undoSystem, uint32 * dup_face, uint32 face_count, uint32 * dup_edge, uint32 edge_count, uint32 * dup_vertex, uint32 vertex_count, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    UNDO_EVENT__DUPLICATE_FACE * dup = _pushType( tempMemory, UNDO_EVENT__DUPLICATE_FACE );
    dup->vertex_count = vertex_count;
    dup->edge_count   = edge_count;
    dup->face_count   = face_count;
    
    uint32 * dup0 = _pushArray( tempMemory, uint32, vertex_count );
    memcpy( dup0, dup_vertex, sizeof( uint32 ) * vertex_count );
    
    uint32 * dup1 = _pushArray( tempMemory, uint32, edge_count );
    memcpy( dup1, dup_edge, sizeof( uint32 ) * edge_count );
    
    uint32 * dup2 = _pushArray( tempMemory, uint32, face_count );
    memcpy( dup2, dup_face, sizeof( uint32 ) * face_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_duplicateFace;
    event.redo_callback = redo_duplicateFace;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// NEW EDGE

internal void
undo_newEdge( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__NEW_EDGE * newEdge = _addr ( data_at, UNDO_EVENT__NEW_EDGE );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( uint32 ) * editorState->point_count );
    editorState->edge_count--;
    
    editorState->point_isSelected[ newEdge->edge.index[ 0 ] ] = true;
    editorState->point_isSelected[ newEdge->edge.index[ 1 ] ] = true;
}

internal void
redo_newEdge( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__NEW_EDGE * newEdge = _addr ( data_at, UNDO_EVENT__NEW_EDGE );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    addEdge( editorState, newEdge->edge, true );
}

internal void
add_newEdge( UNDO_SYSTEM * undoSystem, EDGE__ edge, MEMORY * tempMemory ) {
    UNDO_EVENT__NEW_EDGE * newEdge = _pushType( tempMemory, UNDO_EVENT__NEW_EDGE );
    newEdge->edge = edge;
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_newEdge;
    event.redo_callback = redo_newEdge;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// NEW FACE

internal void
undo_newFace( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__NEW_FACE * newFace = _addr ( data_at, UNDO_EVENT__NEW_FACE );
    uint32               * oldEdge = _addra( data_at, uint32, newFace->oldEdge_count );
    EDGE__               * newEdge = _addra( data_at, EDGE__, newFace->newEdge_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( boo32 ) * editorState->point_count );
    memset( editorState->edge_isSelected,  0, sizeof( boo32 ) * editorState->edge_count  );
    memset( editorState->face_isSelected,  0, sizeof( boo32 ) * editorState->face_count  );
    editorState->face_count--;
    
    editorState->point_isSelected[ newFace->face.index[ 0 ] ] = true;
    editorState->point_isSelected[ newFace->face.index[ 1 ] ] = true;
    editorState->point_isSelected[ newFace->face.index[ 2 ] ] = true;
    if( newFace->face.index[ 3 ] != -1 ) {
        editorState->point_isSelected[ newFace->face.index[ 3 ] ] = true;
    }
    
    editorState->edge_count -= newFace->newEdge_count;
    for( uint32 oldEdge_index = 0; oldEdge_index < newFace->oldEdge_count; oldEdge_index++ ) {
        editorState->edge_isSelected[ oldEdge[ oldEdge_index ] ] = true;
    }
}

internal void
redo_newFace( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__NEW_FACE * newFace = _addr ( data_at, UNDO_EVENT__NEW_FACE );
    uint32               * oldEdge = _addra( data_at, uint32, newFace->oldEdge_count );
    EDGE__               * newEdge = _addra( data_at, EDGE__, newFace->newEdge_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->point_isSelected, 0, sizeof( boo32 ) * editorState->point_count );
    memset( editorState->edge_isSelected,  0, sizeof( boo32 ) * editorState->edge_count  );
    memset( editorState->face_isSelected,  0, sizeof( boo32 ) * editorState->face_count  );
    addFace( editorState, newFace->face, true );
    
    editorState->point_isSelected[ newFace->face.index[ 0 ] ] = true;
    editorState->point_isSelected[ newFace->face.index[ 1 ] ] = true;
    editorState->point_isSelected[ newFace->face.index[ 2 ] ] = true;
    if( newFace->face.index[ 3 ] != -1 ) {
        editorState->point_isSelected[ newFace->face.index[ 3 ] ] = true;
    }
    
    for( uint32 oldEdge_index = 0; oldEdge_index < newFace->oldEdge_count; oldEdge_index++ ) {
        editorState->edge_isSelected[ oldEdge[ oldEdge_index ] ] = true;
    }
    for( uint32 newEdge_index = 0; newEdge_index < newFace->newEdge_count; newEdge_index++ ) {
        addEdge( editorState, newEdge[ newEdge_index ], true );
    }
}

internal void
add_newFace( UNDO_SYSTEM * undoSystem, FACE face, uint32 * oldEdge, uint32 oldEdge_count, EDGE__ * newEdge, uint32 newEdge_count, MEMORY * tempMemory ) {
    UNDO_EVENT__NEW_FACE * newFace = _pushType( tempMemory, UNDO_EVENT__NEW_FACE );
    newFace->face = face;
    newFace->oldEdge_count = oldEdge_count;
    newFace->newEdge_count = newEdge_count;
    
    uint32 * old0 = _pushArray( tempMemory, uint32, oldEdge_count );
    memcpy( old0, oldEdge, sizeof( uint32 ) * oldEdge_count );
    
    EDGE__ * new0 = _pushArray( tempMemory, EDGE__, newEdge_count );
    memcpy( new0, newEdge, sizeof( EDGE__ ) * newEdge_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_newFace;
    event.redo_callback = redo_newFace;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// EXTRUDE VERTEX

internal void
undo_extrudeVertex( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__EXTRUDE_VERTEX * extrude = _addr ( data_at, UNDO_EVENT__EXTRUDE_VERTEX );
    uint32                     * vert    = _addra( data_at, uint32, extrude->nVert );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    clearSelectedPoints( editorState );
    
    editorState->point_count -= extrude->nVert;
    editorState->edge_count  -= extrude->nVert;
    for( uint32 iVert = 0; iVert < extrude->nVert; iVert++ ) {
        editorState->point_isSelected[ vert[ iVert ] ] = true;
    }
}

internal void
redo_extrudeVertex( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__EXTRUDE_VERTEX * extrude = _addr ( data_at, UNDO_EVENT__EXTRUDE_VERTEX );
    uint32                     * vert    = _addra( data_at, uint32, extrude->nVert );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    clearSelectedPoints( editorState );
    for( uint32 iVert = 0; iVert < extrude->nVert; iVert++ ) {
        uint32 index0 = vert[ iVert ];
        uint32 index1 = addPoint( editorState, editorState->point[ index0 ], true );
        
        EDGE__ edge = Edge__( index0, index1 );
        addEdge( editorState, edge, false );
    }
}

internal void
add_extrudeVertex( UNDO_SYSTEM * undoSystem, uint32 * vert, uint32 nVert, MEMORY * tempMemory ) {
    UNDO_EVENT__EXTRUDE_VERTEX * extrude = _pushType( tempMemory, UNDO_EVENT__EXTRUDE_VERTEX );
    extrude->nVert = nVert;
    uint32 * v = _pushArray( tempMemory, uint32, nVert );
    memcpy( v, vert, sizeof( uint32 ) * nVert );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_extrudeVertex;
    event.redo_callback = redo_extrudeVertex;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// EXTRUDE EDGE

internal void
undo_extrudeEdge( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__EXTRUDE_EDGE * extrude = _addr( data_at, UNDO_EVENT__EXTRUDE_EDGE );
    uint32 * sEdge  = _addra( data_at, uint32, extrude->nSelectEdge );
    uint32 * dPoint = _addra( data_at, uint32, extrude->mPoints );
    EDGE__ * nEdge  = _addra( data_at, EDGE__, extrude->mEdges );
    FACE   * nFace  = _addra( data_at, FACE,   extrude->mFaces );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    clearAllSelected( editorState );
    
    editorState->point_count -= extrude->mPoints;
    editorState->edge_count  -= extrude->mEdges;
    editorState->face_count  -= extrude->mFaces;
    
    for( uint32 iEdge = 0; iEdge < extrude->nSelectEdge; iEdge++ ) {
        uint32 index = sEdge[ iEdge ];
        EDGE__ e = editorState->edge[ index ];
        
        editorState->edge_isSelected[ index ] = true;
        editorState->point_isSelected[ e.index[ 0 ] ] = true;
        editorState->point_isSelected[ e.index[ 1 ] ] = true;
    }
}

internal void
redo_extrudeEdge( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__EXTRUDE_EDGE * extrude = _addr( data_at, UNDO_EVENT__EXTRUDE_EDGE );
    uint32 * sEdge  = _addra( data_at, uint32, extrude->nSelectEdge );
    uint32 * dPoint = _addra( data_at, uint32, extrude->mPoints );
    EDGE__ * nEdge  = _addra( data_at, EDGE__, extrude->mEdges );
    FACE   * nFace  = _addra( data_at, FACE,   extrude->mFaces );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    clearAllSelected( editorState );
    
    for( uint32 iPoint = 0; iPoint < extrude->mPoints; iPoint++ ) {
        vec3 P = editorState->point[ dPoint[ iPoint ] ];
        addPoint( editorState, P, true );
    }
    
    for( uint32 iEdge = 0; iEdge < extrude->mEdges; iEdge++ ) {
        EDGE__ e = nEdge[ iEdge ];
        boo32 isSelected0 = editorState->point_isSelected[ e.index [ 0 ] ];
        boo32 isSelected1 = editorState->point_isSelected[ e.index [ 1 ] ];
        addEdge( editorState, e, ( isSelected0 && isSelected1 ) );
    }
    
    for( uint32 iFace = 0; iFace < extrude->mFaces; iFace++ ) {
        FACE f = nFace[ iFace ];
        addFace( editorState, f, false );
    }
}

internal void
add_extrudeEdge( UNDO_SYSTEM * undoSystem, EDITOR_STATE * editorState, INDEX_LIST__ selectEdgeList, INDEX_LIST__ dupPointList, EDGE_LIST__ newEdgeList, FACE_LIST__ newFaceList, MEMORY * tempMemory ) {
    UNDO_EVENT__EXTRUDE_EDGE * extrude = _pushType( tempMemory, UNDO_EVENT__EXTRUDE_EDGE );
    extrude->nSelectEdge = selectEdgeList.count;
    
    extrude->mPoints = dupPointList.count;
    extrude->mEdges  = newEdgeList.count;
    extrude->mFaces  = newFaceList.count;
    
    copyTo( tempMemory, selectEdgeList );
    copyTo( tempMemory, dupPointList );
    copyTo( tempMemory, newEdgeList );
    copyTo( tempMemory, newFaceList );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_extrudeEdge;
    event.redo_callback = redo_extrudeEdge;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// FLIP FACE NORMAL

internal void
flipNormal( FACE * face ) {
    _swap( int32, face->index[ 1 ], face->index[ 2 ] );
}

internal void
undo_flipNormal( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__FLIP_NORMAL * flip       = _addr ( data_at, UNDO_EVENT__FLIP_NORMAL );
    uint32                  * flip_index = _addra( data_at, uint32, flip->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    for( uint32 face_index = 0; face_index < flip->face_count; face_index++ ) {
        flipNormal( editorState->face + flip_index[ face_index ] );
    }
}

internal void
redo_flipNormal( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    undo_flipNormal( editorState, data, data_size, tempMemory );
}

internal void
add_flipNormal( UNDO_SYSTEM * undoSystem, uint32 * face, uint32 face_count, MEMORY * tempMemory ) {
    UNDO_EVENT__FLIP_NORMAL * flip = _pushType( tempMemory, UNDO_EVENT__FLIP_NORMAL );
    flip->face_count = face_count;
    
    uint32 * flip0 = _pushArray( tempMemory, uint32, face_count );
    memcpy( flip0, face, sizeof( uint32 ) * face_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_flipNormal;
    event.redo_callback = redo_flipNormal;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// DELETE SELECTION

internal int32
getBestPoint( EDITOR_STATE * e, vec3 P ) {
    // TODO : should we handle snapping to a point only within a certain range?
    int32 result = -1;
    
    flo32 dSqMax = 1000000.0f;
    for( int32 iPoint = 0; iPoint < e->point_count; iPoint++ ) {
        vec3  Q   = e->point[ iPoint ];
        flo32 dSq = getLengthSq( Q - P );
        if( dSq < dSqMax ) {
            result = iPoint;
            dSqMax = dSq;
        }
    }
    Assert( result != -1 );
    
    return result;
}

struct EDITOR__ENTITY_POSITION {
    vec3 * pos_secureDoor;
    vec3 * pos_pixie;
    vec3 * pos_ledge;
    vec3 * pos_killZone;
    vec3 * pos_checkpoint;
    vec3 * pos_view;
};

internal void
CaptureEntityPositionForDelete( EDITOR_STATE * e, EDITOR__ENTITY_POSITION * p, MEMORY * memory ) {
    p->pos_secureDoor = _pushArray_clear( memory, vec3, e->nSecureDoor );
    for( uint32 iDoor = 0; iDoor < e->nSecureDoor; iDoor++ ) {
        EDITOR__SECURE_DOOR eDoor = e->secureDoor[ iDoor ];
        p->pos_secureDoor[ iDoor ] = e->point[ eDoor.point_index ];
    }
    
    p->pos_pixie = _pushArray_clear( memory, vec3, e->nPixie );
    for( uint32 iPixie = 0; iPixie < e->nPixie; iPixie++ ) {
        EDITOR__PIXIE ePixie = e->pixie[ iPixie ];
        p->pos_pixie[ iPixie ] = e->point[ ePixie.point_index ];
    }
    
    p->pos_ledge = _pushArray_clear( memory, vec3, e->nLedge * 2 );
    for( uint32 iLedge = 0; iLedge < e->nLedge; iLedge++ ) {
        EDITOR__LEDGE eLedge = e->ledge[ iLedge ];
        p->pos_ledge[ iLedge * 2 + 0 ] = e->point[ eLedge.point_index[ 0 ] ];
        p->pos_ledge[ iLedge * 2 + 1 ] = e->point[ eLedge.point_index[ 1 ] ];
    }
    
    p->pos_killZone = _pushArray_clear( memory, vec3, e->nKillZone );
    for( uint32 iKillZone = 0; iKillZone < e->nKillZone; iKillZone++ ) {
        EDITOR__KILL_ZONE eKillZone = e->killZone[ iKillZone ];
        p->pos_killZone[ iKillZone ] = e->point[ eKillZone.point_index ];
    }
    
    p->pos_checkpoint = _pushArray_clear( memory, vec3, e->nCheckpoint * 2 );
    for( uint32 iCheckpoint = 0; iCheckpoint < e->nCheckpoint; iCheckpoint++ ) {
        EDITOR__CHECKPOINT eCheckpoint = e->checkpoint[ iCheckpoint ];
        p->pos_checkpoint[ iCheckpoint * 2 + 0 ] = e->point[ eCheckpoint.point_index ];
        p->pos_checkpoint[ iCheckpoint * 2 + 1 ] = e->point[ eCheckpoint.respawn_index ];
    }
    
    p->pos_view = _pushArray_clear( memory, vec3, e->nView * 2 );
    for( uint32 iView = 0; iView < e->nView; iView++ ) {
        EDITOR__VIEW eView = e->view[ iView ];
        p->pos_view[ iView * 2 + 0 ] = e->point[ eView.pIndex ];
        p->pos_view[ iView * 2 + 1 ] = e->point[ eView.qIndex ];
    }
}

internal void
FindPointIndicesForEntityPositions( EDITOR_STATE * e, EDITOR__ENTITY_POSITION * p ) {
    for( uint32 iDoor = 0; iDoor < e->nSecureDoor; iDoor++ ) {
        EDITOR__SECURE_DOOR * eDoor = e->secureDoor + iDoor;
        eDoor->point_index = getBestPoint( e, p->pos_secureDoor[ iDoor ] );
    }
    
    for( uint32 iPixie = 0; iPixie < e->nPixie; iPixie++ ) {
        EDITOR__PIXIE * ePixie = e->pixie + iPixie;
        ePixie->point_index = getBestPoint( e, p->pos_pixie[ iPixie ] );
    }
    
    for( uint32 iLedge = 0; iLedge < e->nLedge; iLedge++ ) {
        EDITOR__LEDGE * eLedge = e->ledge + iLedge;
        vec3 P0 = p->pos_ledge[ iLedge * 2 + 0 ];
        vec3 P1 = p->pos_ledge[ iLedge * 2 + 1 ];
        eLedge->point_index[ 0 ] = getBestPoint( e, P0 );
        eLedge->point_index[ 1 ] = getBestPoint( e, P1 );
    }
    
    for( uint32 iKillZone = 0; iKillZone < e->nKillZone; iKillZone++ ) {
        EDITOR__KILL_ZONE * eKillZone = e->killZone + iKillZone;
        eKillZone->point_index = getBestPoint( e, p->pos_killZone[ iKillZone ] );
    }
    
    for( uint32 iCheckpoint = 0; iCheckpoint < e->nCheckpoint; iCheckpoint++ ) {
        EDITOR__CHECKPOINT * eCheckpoint = e->checkpoint + iCheckpoint;
        eCheckpoint->point_index   = getBestPoint( e, p->pos_checkpoint[ iCheckpoint * 2 + 0 ] );
        eCheckpoint->respawn_index = getBestPoint( e, p->pos_checkpoint[ iCheckpoint * 2 + 1 ] );
    }
    
    for( uint32 iView = 0; iView < e->nView; iView++ ) {
        EDITOR__VIEW * eView = e->view + iView;
        eView->pIndex = getBestPoint( e, p->pos_view[ iView * 2 + 0 ] );
        eView->qIndex = getBestPoint( e, p->pos_view[ iView * 2 + 1 ] );
    }
}

internal void
undo_deleteSelection( EDITOR_STATE * e, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {EDITOR__ENTITY_POSITION entityPos = {};
    CaptureEntityPositionForDelete( e, &entityPos, SnapMemory( tempMemory ) );
    
    uint8 * data_at = data;
    UNDO_EVENT__DELETE_SELECTION * deleteSelection = _addr ( data_at, UNDO_EVENT__DELETE_SELECTION );
    vec3   * point  = _addra( data_at, vec3,   deleteSelection->vertex_count );
    boo32  * point0 = _addra( data_at, boo32,  deleteSelection->vertex_count );
    EDGE__ * edge   = _addra( data_at, EDGE__, deleteSelection->edge_count );
    boo32  * edge0  = _addra( data_at, boo32,  deleteSelection->edge_count );
    FACE   * face   = _addra( data_at, FACE,   deleteSelection->face_count );
    boo32  * face0  = _addra( data_at, boo32,  deleteSelection->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    e->point_count = deleteSelection->vertex_count;
    memcpy( e->point,            point,  sizeof( vec3  ) * e->point_count );
    memcpy( e->point_isSelected, point0, sizeof( boo32 ) * e->point_count );
    
    e->edge_count = deleteSelection->edge_count;
    memcpy( e->edge,            edge,  sizeof( EDGE__  ) * e->edge_count );
    memcpy( e->edge_isSelected, edge0, sizeof( boo32   ) * e->edge_count );
    
    e->face_count = deleteSelection->face_count;
    memcpy( e->face,            face,  sizeof( FACE    ) * e->face_count );
    memcpy( e->face_isSelected, face0, sizeof( boo32   ) * e->face_count );
    
    FindPointIndicesForEntityPositions( e, &entityPos );
    popToSnap( tempMemory );
}

internal void
deleteSelected( EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    uint32 oldPoint_count = editorState->point_count;
    uint32 * newIndex = _pushArray_clear( tempMemory, uint32, oldPoint_count );
    
    uint32 newPoint_count = 0;
    for( uint32 point_index = 0; point_index < oldPoint_count; point_index++ ) {
        if( !editorState->point_isSelected[ point_index ] ) {
            newIndex[ point_index ] = newPoint_count;
            editorState->point[ newPoint_count++ ] = editorState->point[ point_index ];
        }
    }
    editorState->point_count = newPoint_count;
    
    uint32 newEdge_count = 0;
    for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
        EDGE__ edge = editorState->edge[ edge_index ];
        boo32 shouldDelete = ( editorState->point_isSelected[ edge.index[ 0 ] ] ) || ( editorState->point_isSelected[ edge.index[ 1 ] ] );
        if( !shouldDelete ) {
            edge.index[ 0 ] = newIndex[ edge.index[ 0 ] ];
            edge.index[ 1 ] = newIndex[ edge.index[ 1 ] ];
            editorState->edge[ newEdge_count++ ] = edge;
        }
    }
    editorState->edge_count = newEdge_count;
    
    uint32 newFace_count = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        FACE face = editorState->face[ face_index ];
        boo32 shouldDelete = ( editorState->point_isSelected[ face.index[ 0 ] ] ) ||
            ( editorState->point_isSelected[ face.index[ 1 ] ] ) ||
            ( editorState->point_isSelected[ face.index[ 2 ] ] ) ||
            ( ( face.index[ 3 ] != -1 ) && ( editorState->point_isSelected[ face.index[ 3 ] ] ) );
        if( !shouldDelete ) {
            face.index[ 0 ] = newIndex[ face.index[ 0 ] ];
            face.index[ 1 ] = newIndex[ face.index[ 1 ] ];
            face.index[ 2 ] = newIndex[ face.index[ 2 ] ];
            if( face.index[ 3 ] != -1 ) {
                face.index[ 3 ] = newIndex[ face.index[ 3 ] ];
            }
            editorState->face[ newFace_count++ ] = face;
        }
    }
    editorState->face_count = newFace_count;
    
    memset( editorState->point_isSelected, 0, sizeof( boo32 ) * editorState->point_count );
    
    _popArray( tempMemory, uint32, oldPoint_count );
}

internal void
redo_deleteSelection( EDITOR_STATE * e, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    EDITOR__ENTITY_POSITION entityPos = {};
    CaptureEntityPositionForDelete( e, &entityPos, SnapMemory( tempMemory ) );
    
    uint8 * data_at = data;
    UNDO_EVENT__DELETE_SELECTION * deleteSelection = _addr ( data_at, UNDO_EVENT__DELETE_SELECTION );
    vec3   * point  = _addra( data_at, vec3,   deleteSelection->vertex_count );
    boo32  * point0 = _addra( data_at, boo32,  deleteSelection->vertex_count );
    EDGE__ * edge   = _addra( data_at, EDGE__, deleteSelection->edge_count );
    boo32  * edge0  = _addra( data_at, boo32,  deleteSelection->edge_count );
    FACE   * face   = _addra( data_at, FACE,   deleteSelection->face_count );
    boo32  * face0  = _addra( data_at, boo32,  deleteSelection->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memcpy( e->point_isSelected, point0, sizeof( boo32 ) * deleteSelection->vertex_count );
    
    deleteSelected( e, tempMemory );
    
    FindPointIndicesForEntityPositions( e, &entityPos );
    popToSnap( tempMemory );
}

internal void
add_deleteSelection( UNDO_SYSTEM * undoSystem, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    UNDO_EVENT__DELETE_SELECTION * deleteSelection = _pushType( tempMemory, UNDO_EVENT__DELETE_SELECTION );
    deleteSelection->vertex_count = editorState->point_count;
    deleteSelection->edge_count   = editorState->edge_count;
    deleteSelection->face_count   = editorState->face_count;
    
    uint8 * point = ( uint8 * )_pushArray( tempMemory, vec3, editorState->point_count );
    memcpy( point, editorState->point, sizeof( vec3 ) * editorState->point_count );
    uint8 * point0 = ( uint8 * )_pushArray( tempMemory, boo32, editorState->point_count );
    memcpy( point0, editorState->point_isSelected, sizeof( boo32 ) * editorState->point_count );
    
    uint8 * edge = ( uint8 * )_pushArray( tempMemory, EDGE__, editorState->edge_count );
    memcpy( edge, editorState->edge, sizeof( EDGE__ ) * editorState->edge_count );
    uint8 * edge0 = ( uint8 * )_pushArray( tempMemory, boo32, editorState->edge_count );
    memcpy( edge0, editorState->edge_isSelected, sizeof( boo32 ) * editorState->edge_count );
    
    uint8 * face = ( uint8 * )_pushArray( tempMemory, FACE, editorState->face_count );
    memcpy( face, editorState->face, sizeof( FACE ) * editorState->face_count );
    uint8 * face0 = ( uint8 * )_pushArray( tempMemory, boo32, editorState->face_count );
    memcpy( face0, editorState->face_isSelected, sizeof( boo32 ) * editorState->face_count );
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_deleteSelection;
    event.redo_callback = redo_deleteSelection;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

internal void
deleteSelectedEdges( EDITOR_STATE * editorState ) {
    uint32 newEdge_count = 0;
    for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
        if( !editorState->edge_isSelected[ edge_index ] ) {
            editorState->edge[ newEdge_count++ ] = editorState->edge[ edge_index ];
        }
    }
    editorState->edge_count = newEdge_count;
}

internal void
deleteSelectedFaces( EDITOR_STATE * editorState ) {
    uint32 newFace_count = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        if( !editorState->face_isSelected[ face_index ] ) {
            editorState->face[ newFace_count++ ] = editorState->face[ face_index ];
        }
    }
    editorState->face_count = newFace_count;
}

// DELETE EDGES

internal void
undo_deleteEdges( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DELETE_EDGES * deleteEdges = _addr ( data_at, UNDO_EVENT__DELETE_EDGES );
    uint32 * e0 = _addra( data_at, uint32, deleteEdges->edge_count );
    EDGE__ * e1 = _addra( data_at, EDGE__, deleteEdges->edge_count );
    uint32 * f0 = _addra( data_at, uint32, deleteEdges->face_count );
    FACE   * f1 = _addra( data_at, FACE,   deleteEdges->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    int32 src0 = deleteEdges->edge_count - 1;
    int32 src1 = editorState->edge_count - 1;
    int32 dest = editorState->edge_count + deleteEdges->edge_count - 1;
    while( src0 > -1 ) {
        if( dest > ( int32 )e0[ src0 ] ) {
            editorState->edge[ dest ] = editorState->edge[ src1-- ];
        } else {
            editorState->edge[ dest ] = e1[ src0-- ];
        }
        dest--;
    }
    editorState->edge_count += deleteEdges->edge_count;
    
    src0 = deleteEdges->face_count - 1;
    src1 = editorState->face_count - 1;
    dest = editorState->face_count + deleteEdges->face_count - 1;
    while( src0 > -1 ) {
        if( dest > ( int32 )f0[ src0 ] ) {
            editorState->face[ dest ] = editorState->face[ src1-- ];
        } else {
            editorState->face[ dest ] = f1[ src0-- ];
        }
        dest--;
    }
    editorState->face_count += deleteEdges->face_count;
}

internal void
redo_deleteEdges( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DELETE_EDGES * deleteEdges = _addr ( data_at, UNDO_EVENT__DELETE_EDGES );
    uint32 * e0 = _addra( data_at, uint32, deleteEdges->edge_count );
    EDGE__ * e1 = _addra( data_at, EDGE__, deleteEdges->edge_count );
    uint32 * f0 = _addra( data_at, uint32, deleteEdges->face_count );
    FACE   * f1 = _addra( data_at, FACE,   deleteEdges->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->edge_isSelected, 0, sizeof( boo32 ) * editorState->edge_count );
    memset( editorState->face_isSelected, 0, sizeof( boo32 ) * editorState->face_count );
    
    for( uint32 iter = 0; iter < deleteEdges->edge_count; iter++ ) {
        editorState->edge_isSelected[ e0[ iter ] ] = true;
    }
    for( uint32 iter = 0; iter < deleteEdges->face_count; iter++ ) {
        editorState->face_isSelected[ f0[ iter ] ] = true;
    }
    
    deleteSelectedEdges( editorState );
    deleteSelectedFaces( editorState );
}

internal void
add_deleteEdges( UNDO_SYSTEM * undoSystem, EDITOR_STATE * editorState, uint32 * edge_array, uint32 edge_count, uint32 * face_array, uint32 face_count, MEMORY * tempMemory ) {
    UNDO_EVENT__DELETE_EDGES * deleteEdges = _pushType( tempMemory, UNDO_EVENT__DELETE_EDGES );
    deleteEdges->edge_count = edge_count;
    deleteEdges->face_count = face_count;
    
    uint32 * e0 = _pushArray( tempMemory, uint32, edge_count );
    memcpy( e0, edge_array, sizeof( uint32 ) * edge_count );
    EDGE__ * e1 = _pushArray( tempMemory, EDGE__, edge_count );
    for( uint32 iter = 0; iter < edge_count; iter++ ) {
        e1[ iter ] = editorState->edge[ edge_array[ iter ] ];
    }
    
    uint32 * f0 = _pushArray( tempMemory, uint32, face_count );
    memcpy( f0, face_array, sizeof( uint32 ) * face_count );
    FACE   * f1 = _pushArray( tempMemory, FACE, face_count );
    for( uint32 iter = 0; iter < face_count; iter++ ) {
        f1[ iter ] = editorState->face[ face_array[ iter ] ];
    }
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_deleteEdges;
    event.redo_callback = redo_deleteEdges;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

// DELETE FACES

internal void
undo_deleteFaces( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DELETE_FACES * deleteFaces = _addr ( data_at, UNDO_EVENT__DELETE_FACES );
    uint32 * f0 = _addra( data_at, uint32, deleteFaces->face_count );
    FACE   * f1 = _addra( data_at, FACE,   deleteFaces->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    int32 src0 = deleteFaces->face_count - 1;
    int32 src1 = editorState->face_count - 1;
    int32 dest = editorState->face_count + deleteFaces->face_count - 1;
    while( src0 > -1 ) {
        if( dest > ( int32 )f0[ src0 ] ) {
            editorState->face[ dest ] = editorState->face[ src1-- ];
        } else {
            editorState->face[ dest ] = f1[ src0-- ];
        }
        dest--;
    }
    editorState->face_count += deleteFaces->face_count;
}

internal void
redo_deleteFaces( EDITOR_STATE * editorState, uint8 * data, uint32 data_size, MEMORY * tempMemory ) {
    uint8 * data_at = data;
    UNDO_EVENT__DELETE_FACES * deleteFaces = _addr ( data_at, UNDO_EVENT__DELETE_FACES );
    uint32 * f0 = _addra( data_at, uint32, deleteFaces->face_count );
    FACE   * f1 = _addra( data_at, FACE,   deleteFaces->face_count );
    Assert( ( uint32 )( data_at - data ) == data_size );
    
    memset( editorState->face_isSelected, 0, sizeof( boo32 ) * editorState->face_count );
    
    for( uint32 iter = 0; iter < deleteFaces->face_count; iter++ ) {
        editorState->face_isSelected[ f0[ iter ] ] = true;
    }
    
    deleteSelectedFaces( editorState );
}

internal void
add_deleteFaces( UNDO_SYSTEM * undoSystem, EDITOR_STATE * editorState, uint32 * face_array, uint32 face_count, MEMORY * tempMemory ) {
    UNDO_EVENT__DELETE_FACES * deleteFaces = _pushType( tempMemory, UNDO_EVENT__DELETE_FACES );
    deleteFaces->face_count = face_count;
    
    uint32 * f0 = _pushArray( tempMemory, uint32, face_count );
    memcpy( f0, face_array, sizeof( uint32 ) * face_count );
    FACE   * f1 = _pushArray( tempMemory, FACE, face_count );
    for( uint32 iter = 0; iter < face_count; iter++ ) {
        f1[ iter ] = editorState->face[ face_array[ iter ] ];
    }
    
    UNDO_EVENT event = {};
    event.undo_callback = undo_deleteFaces;
    event.redo_callback = redo_deleteFaces;
    event.data_size     = getSnapUsed( tempMemory );
    
    add( undoSystem, event, tempMemory );
}

//----------

internal void
undo( UNDO_SYSTEM * undoSystem, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    if( undoSystem->event_atIndex > 0 ) {
        UNDO_EVENT event = undoSystem->event[ --undoSystem->event_atIndex ];
        undoSystem->memory_at -= event.data_size;
        
        event.undo_callback( editorState, event.data, event.data_size, tempMemory );
    }
}

internal void
redo( UNDO_SYSTEM * undoSystem, EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    if( undoSystem->event_atIndex < undoSystem->event_maxIndex ) {
        UNDO_EVENT event = undoSystem->event[ undoSystem->event_atIndex++ ];
        undoSystem->memory_at += event.data_size;
        
        event.redo_callback( editorState, event.data, event.data_size, tempMemory );
    }
}

internal void
getSelected_vertex( EDITOR_STATE * editorState, uint32 * out_vertex, uint32 maxCount ) {
    uint32 out_count = 0;
    for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
        if( editorState->point_isSelected[ point_index ] ) {
            Assert( out_count < maxCount );
            out_vertex[ out_count++ ] = point_index;
        }
    }
}

internal uint32
getSelectedCount_vertex( EDITOR_STATE * editorState ) {
    uint32 result = 0;
    for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
        if( editorState->point_isSelected[ point_index ] ) {
            result++;
        }
    }
    return result;
}

internal void
getSelected_edge( EDITOR_STATE * editorState, uint32 * out_edge, uint32 maxCount ) {
    uint32 out_count = 0;
    for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
        if( editorState->edge_isSelected[ edge_index ] ) {
            Assert( out_count < maxCount );
            out_edge[ out_count++ ] = edge_index;
        }
    }
}

internal void
getSelected_edge( EDITOR_STATE * editorState, INDEX_LIST__ * indexList ) {
    for( int32 iEdge = 0; iEdge < editorState->edge_count; iEdge++ ) {
        if( editorState->edge_isSelected[ iEdge ] ) {
            add( indexList, iEdge );
        }
    }
}

internal uint32
getSelectedCount_edge( EDITOR_STATE * editorState ) {
    uint32 result = 0;
    for( int32 edge_index = 0; edge_index < editorState->edge_count; edge_index++ ) {
        if( editorState->edge_isSelected[ edge_index ] ) {
            result++;
        }
    }
    return result;
}

internal int32
getEdgeIndex( EDITOR_STATE * editorState, EDGE__ edgeToCheck ) {
    int32 result = -1;
    for( int32 edge_index = 0; ( result == -1 ) && ( edge_index < editorState->edge_count ); edge_index++ ) {
        EDGE__ edge = editorState->edge[ edge_index ];
        if( ( edgeToCheck.index[ 0 ] == edge.index[ 0 ] ) && ( edgeToCheck.index[ 1 ] == edge.index[ 1 ] ) ) {
            result = edge_index;
        }
    }
    return result;
}

internal boo32
doesEdgeExist( EDITOR_STATE * editorState, EDGE__ edgeToCheck ) {
    int32 atIndex = getEdgeIndex( editorState, edgeToCheck );
    boo32 result  = ( atIndex != -1 );
    return result;
}

internal void
getSelected_face( EDITOR_STATE * editorState, uint32 * out_face, uint32 maxCount ) {
    uint32 out_count = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        if( editorState->face_isSelected[ face_index ] ) {
            Assert( out_count < maxCount );
            out_face[ out_count++ ] = face_index;
        }
    }
}

internal uint32
getSelectedCount_face( EDITOR_STATE * editorState ) {
    uint32 result = 0;
    for( int32 face_index = 0; face_index < editorState->face_count; face_index++ ) {
        if( editorState->face_isSelected[ face_index ] ) {
            result++;
        }
    }
    return result;
}

internal vec3
getMeanOfSelection( EDITOR_STATE * editorState ) {
    vec3   result = {};
    uint32 count  = 0;
    for( int32 point_index = 0; point_index < editorState->point_count; point_index++ ) {
        if( editorState->point_isSelected[ point_index ] ) {
            count++;
            result += editorState->point[ point_index ];
        }
    }
    
    if( count > 0 ) {
        result /= ( flo32 )count;
    }
    return result;
}

internal uint32
getSelectedCount( EDITOR_STATE * editorState ) {
    boo32 * isSelected       = 0;
    uint32  isSelected_count = 0;
    switch( editorState->selectMode ) {
        case selectMode_vertex: {
            isSelected       = editorState->point_isSelected;
            isSelected_count = editorState->point_count;
        } break;
        
        case selectMode_edge: {
            isSelected       = editorState->edge_isSelected;
            isSelected_count = editorState->edge_count;
        } break;
        
        case selectMode_face: {
            isSelected       = editorState->face_isSelected;
            isSelected_count = editorState->face_count;
        } break;
        
        default: {
            InvalidCodePath;
        } break;
    }
    
    uint32 result = 0;
    for( uint32 iter = 0; iter < isSelected_count; iter++ ) {
        if( isSelected[ iter ] ) {
            result++;
        }
    }
    return result;
}

internal CENTROID
getCentroidOfSelection( EDITOR_STATE * editorState ) {
    vec3  * point_array      = 0;
    boo32 * point_isSelected = 0;
    uint32  point_count      = 0;
    
    switch( editorState->selectMode ) {
        case selectMode_vertex: {
            point_array      = editorState->point;
            point_isSelected = editorState->point_isSelected;
            point_count      = ( uint32 )editorState->point_count;
        } break;
        
        case selectMode_edge: {
            point_array      = editorState->edge_centroid;
            point_isSelected = editorState->edge_isSelected;
            point_count      = ( uint32 )editorState->edge_count;
        } break;
        
        case selectMode_face: {
            point_array      = editorState->face_centroid;
            point_isSelected = editorState->face_isSelected;
            point_count      = ( uint32 )editorState->face_count;
        } break;
        
        default: {
            InvalidCodePath;
        } break;
    }
    
    uint32 isSelected_count = 0;
    vec3   isSelected_atPos = {};
    for( uint32 iter = 0; iter < point_count; iter++ ) {
        if( point_isSelected[ iter ] ) {
            isSelected_count++;
            isSelected_atPos += point_array[ iter ];
        }
    }
    
    CENTROID result = {};
    if( isSelected_count > 0 ) {
        result.isValid  = true;
        result.position = ( isSelected_atPos / ( flo32 )isSelected_count );
    }
    return result;
}

internal boo32
isDegenerate( vec3 V ) {
    flo32 dotVV  = dot( V, V );
    boo32 result = ( dotVV <= DOT__DEGENERATE_EPSILON );
    return result;
}

internal boo32
isOrthogonalTo( vec3 U, vec3 V ) {
    flo32 dotUV = dot( U, V );
    dotUV = fabsf( dotUV );
    boo32 result = ( dotUV <= DOT__ORTHOGONAL_EPSILON );
    return result;
}

internal boo32
isCollinearTo( vec3 U, vec3 V ) {
    flo32 dotUV = dot( U, V );
    boo32 result = ( fabsf( dotUV ) >= ( 1.0f - DOT__COLLINEAR_EPSILON ) );
    return result;
}

internal boo32
isOppositeTo( vec3 U, vec3 V ) {
    flo32 dotUV = dot( U, V );
    boo32 result = ( dotUV < -DOT__ORTHOGONAL_EPSILON );
    return result;
}

internal vec3
getNormal( EDITOR_STATE * editorState, FACE face ) {
    vec3 A = editorState->point[ face.index[ 0 ] ];
    vec3 B = editorState->point[ face.index[ 1 ] ];
    vec3 C = editorState->point[ face.index[ 2 ] ];
    vec3 result = getNormal( cross( B - A, C - A ) );
    return result;
}

internal vec3
getNormal( EDITOR_STATE * editorState, int32 face_index ) {
    vec3 result = getNormal( editorState, editorState->face[ face_index ] );
    return result;
}

internal vec3
toVec3( flo32 radians_theta ) {
    vec3 result = {};
    result.x =  cosf( radians_theta );
    result.z = -sinf( radians_theta );
    return result;
}

internal vec3
toVec3( EDITOR_STATE * e, EDGE__ edge ) {
    vec3 P0 = e->point[ edge.index[ 0 ] ];
    vec3 P1 = e->point[ edge.index[ 1 ] ];
    
    vec3 result = P1 - P0;
    return result;
}

internal DIRECTION
getDirectionOfSelection( EDITOR_STATE * e ) {
    vec3 vector = Vec3( 0.0f, 0.0f, 0.0f );
    if( e->selectMode == selectMode_face ) {
        for( int32 iFace = 0; iFace < e->face_count; iFace++ ) {
            if( e->face_isSelected[ iFace ] ) {
                FACE face = e->face[ iFace ];
                vec3 N = getNormal( e, face );
                
                vector += N;
            }
        }
    }
    if( e->selectMode == selectMode_edge ) {
        uint32 nSelect = getSelectedCount( e );
        if( nSelect == 1 ) {
            for( int32 iEdge = 0; iEdge < e->edge_count; iEdge++ ) {
                if( e->edge_isSelected[ iEdge ] ) {
                    vector = toVec3( e, e->edge[ iEdge ] );
                }
            }
        }
    }
    
    DIRECTION result = {};
    if( !isDegenerate( vector ) ) {
        result.isValid   = true;
        result.direction = getNormal( vector );
    }
    return result;
}

struct RAY_INTERSECT {
    boo32 isValid;
    flo32 t;
    vec3  P;
};

struct PLANE {
    vec3  normal;
    flo32 distFromOrigin;
};

internal PLANE
Plane( vec3 normal, flo32 distFromOrigin ) {
    PLANE result = {};
    result.normal         = normal;
    result.distFromOrigin = distFromOrigin;
    return result;
}

internal PLANE
Plane( vec3 normal, vec3 pointOnPlane ) {
    PLANE result = {};
    result.normal         = normal;
    result.distFromOrigin = dot( normal, pointOnPlane );
    return result;
}

#define _IS_ORTHOGONAL( value )  ( fabsf( value ) <= DOT__ORTHOGONAL_EPSILON )

internal RAY_INTERSECT
doesIntersectPlane( vec3 ray_origin, vec3 ray_vector, PLANE plane ) {
    RAY_INTERSECT result = {};
    
    flo32 denom = dot( plane.normal, ray_vector );
    if( !_IS_ORTHOGONAL( denom ) ) {
        flo32 t = ( plane.distFromOrigin - dot( plane.normal, ray_origin ) ) / denom;
        result.isValid = true;
        result.t       = t;
        result.P       = ray_origin + ray_vector * t;
    }
    return result;
}

internal void
initVertexEdit( EDITOR_STATE * editorState, MEMORY * tempMemory, EDITOR_MODE editorMode ) {
    UTILITY_PANEL * u = &editorState->utilityPanel;
    CAMERA        * c = &editorState->camera;
    editorState->editorMode        = editorMode;
    editorState->translateMode     = translateMode_twoAxis;
    editorState->translate_axis    = c->zAxis;
    editorState->translate_xAxis   = c->xAxis;
    editorState->translate_yAxis   = c->yAxis;
    editorState->translate_origin  = getMeanOfSelection( editorState );
    editorState->transform_basePos = editorState->translate_origin + editorState->translate_xAxis;
    if( ( editorMode == editorMode_rotateSelection ) && ( u->pivot_isActive ) ) {
        editorState->translate_origin = u->pivot;
        editorState->transform_basePos = editorState->translate_origin + editorState->translate_xAxis;
    }
    editorState->scale_selectBasePoint = true;
    editorState->scale_axisIsEnabled[ 0 ] = true;
    editorState->scale_axisIsEnabled[ 1 ] = true;
    editorState->scale_axisIsEnabled[ 2 ] = true;
    editorState->translate_tempPos = _pushArray_clear( tempMemory, vec3, editorState->point_count );
    memcpy( editorState->translate_tempPos, editorState->point, sizeof( vec3 ) * editorState->point_count );
    init( &editorState->input_string );
}

internal void
confirmVertexEdit( EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    editorState->editorMode = editorMode_default;
    _popArray( tempMemory, vec3, editorState->point_count );
}

internal void
cancelVertexEdit( EDITOR_STATE * editorState, MEMORY * tempMemory ) {
    editorState->editorMode = editorMode_default;
    memcpy( editorState->point, editorState->translate_tempPos, sizeof( vec3 ) * editorState->point_count );
    _popArray( tempMemory, vec3, editorState->point_count );
}

internal flo32
getRadiansBetween( vec3 a, vec3 b, vec3 z ) {
    a = getNormal( a );
    b = getNormal( b );
    flo32 result = acosf( dot( a, b ) );
    if( dot( cross( a, b ), z ) < 0.0f ) {
        result = -result;
    }
    return result;
}

internal mat4
getRotationTransform( vec3 basePos, flo32 radians, vec3 axis ) {
    mat4 A = mat4_translate( -basePos );
    mat4 B = mat4_rotate( axis, radians );
    mat4 C = mat4_translate(  basePos );
    mat4 result = C * B * A;
    return result;
}

internal mat4
getScaleTransform( vec3 basePos, vec3 scale ) {
    mat4 A = mat4_translate( -basePos );
    mat4 B = mat4_scale( scale );
    mat4 C = mat4_translate(  basePos );
    mat4 result = C * B * A;
    return result;
}

internal mat4
getReflectTransform( vec3 basePos, vec3 normal ) {
    mat4 A = mat4_translate( -basePos );
    mat4 B = mat4_reflect  ( normal );
    mat4 C = mat4_translate(  basePos );
    mat4 result = C * B * A;
    return result;
}

internal void
drawBox( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, vec4 background_color ) {
    drawRect       ( renderer, passID, bound, background_color );
    drawRectOutline( renderer, passID, bound, COLOR_BLACK );
}

struct CLOSEST_POINTS {
    boo32 isValid;
    flo32 s;
    flo32 t;
    vec3  P0;
    vec3  P1;
};

internal CLOSEST_POINTS
getClosest_ray_lineSegment( vec3 ray_origin, vec3 ray_vector, vec3 lineSegment_pointA, vec3 lineSegment_pointB ) {
    CLOSEST_POINTS result = {};
    
    vec3 P1 = ray_origin;
    vec3 Q1 = ray_origin + ray_vector;
    vec3 P2 = lineSegment_pointA;
    vec3 Q2 = lineSegment_pointB;
    
    vec3 U = Q1 - P1;
    vec3 V = Q2 - P2;
    vec3 W = P1 - P2;
    
    flo32 dotUU = dot( U, U );
    flo32 dotUV = dot( U, V );
    flo32 dotUW = dot( U, W );
    flo32 dotVV = dot( V, V );
    flo32 dotVW = dot( V, W );
    
    flo32 denom = dotUU * dotVV - dotUV * dotUV;
    if( denom != 0.0f ) {
        
        flo32 s = ( dotUV * dotVW - dotVV * dotUW ) / denom;
        flo32 t = ( dotUU * dotVW - dotUV * dotUW ) / denom;
        if( ( t >= 0.0f ) && ( t <= 1.0f ) ) {
            result.isValid = true;
            result.s  = s;
            result.t  = t;
            result.P0 = P1 + U * s;
            result.P1 = P2 + V * t;
        }
    }
    return result;
}

internal CLOSEST_POINTS
getClosest_ray_ray( vec3 P1, vec3 V1, vec3 P2, vec3 V2 ) {
    CLOSEST_POINTS result = {};
    
    vec3 U = V1;
    vec3 V = V2;
    vec3 W = P1 - P2;
    
    flo32 dotUU = dot( U, U );
    flo32 dotUV = dot( U, V );
    flo32 dotUW = dot( U, W );
    flo32 dotVV = dot( V, V );
    flo32 dotVW = dot( V, W );
    
    flo32 denom = dotUU * dotVV - dotUV * dotUV;
    if( denom != 0.0f ) {
        flo32 s = ( dotUV * dotVW - dotVV * dotUW ) / denom;
        flo32 t = ( dotUU * dotVW - dotUV * dotUW ) / denom;
        
        if( ( s >= 0.0f ) && ( t >= 0.0f ) ) {
            result.isValid = true;
            result.s  = s;
            result.t  = t;
            result.P0 = P1 + U * s;
            result.P1 = P2 + V * t;
        }
    }
    return result;
}

struct RAY {
    vec3 origin;
    vec3 vector;
};

internal RAY
Ray( vec3 origin, vec3 vector ) {
    RAY result = {};
    result.origin = origin;
    result.vector = vector;
    return result;
}

internal int32
getBestEdge( EDITOR_STATE * editorState, RAY select ) {
    flo32 range   = 0.20f;
    flo32 rangeSq = range * range;
    
    int32 result  = -1;
    flo32  sCurr  = 1000000.0f;
    for( int32 iEdge = 0; iEdge < editorState->edge_count; iEdge++ ) {
        EDGE__ edge = editorState->edge[ iEdge ];
        
        vec3 A  = editorState->point[ edge.index[ 0 ] ];
        vec3 B  = editorState->point[ edge.index[ 1 ] ];
        
        CLOSEST_POINTS closest = getClosest_ray_lineSegment( select.origin, select.vector, A, B );
        if( ( closest.isValid ) && ( closest.s >= 0.0f ) ) {
            flo32 distSq = getLengthSq( closest.P0 - closest.P1 );
            if( ( distSq <= rangeSq ) && ( closest.s < sCurr ) ) {
                result = iEdge;
                sCurr  = closest.s;
            }
        }
    }
    return result;
}

internal void
entityPanel_drawButton( RENDERER * renderer, ENTITY_PANEL * e, int32 iButton, char * text ) {
    rect bound = e->button_bound[ iButton ];
    vec4 background = COLOR_GRAY( 0.92f );
    if( e->hover == iButton ) {
        background = COLOR_GRAY( 0.5f );
    }
    if( e->active == iButton ) {
        background = COLOR_RED;
    }
    drawRect       ( renderer, renderPassID_overlay, bound, background );
    drawRectOutline( renderer, renderPassID_overlay, bound, COLOR_BLACK );
    drawString( renderer, renderPassID_overlay, assetID_font_default, text, getCenter( bound ), ALIGN_CENTER, COLOR_BLACK );
}

internal void
addButton( ENTITY_PANEL * e ) {
    Assert( e->nButton < ENTITY_PANEL__BUTTON_COUNT );
    
    int32 buttonPerRow = 4;
    vec2  dim    = getDim( e->bound );
    dim.x /= ( flo32 )buttonPerRow;
    dim.y  = dim.x * 0.5f;
    
    int32 iButton = e->nButton++;
    int32 iRow = iButton / buttonPerRow;
    int32 iCol = iButton % buttonPerRow;
    rect bound = addRadius( rectTLD( getTL( e->bound ) + Vec2( dim.x * ( flo32 )iCol, -dim.y * ( flo32 )iRow ), dim ), -2.0f );
    e->button_bound[ iButton ] = bound;
}

internal boo32
isInFrontOfCamera( CAMERA * camera, vec3 P ) {
    vec3 O = getCameraPos( camera );
    vec3 U = -camera->zAxis;
    vec3 V = P - O;
    flo32 dotUV = dot( U, V );
    boo32 result = ( dotUV >= 0.0f );
    return result;
}

internal boo32
isGood( EDGE__ edge ) {
    boo32 result = ( edge.index[ 0 ] != -1 ) && ( edge.index[ 1 ] != -1 );
    return result;
}

internal boo32
isGood( FACE face ) {
    boo32 result = ( face.index[ 0 ] != -1 ) && ( face.index[ 1 ] != -1 ) && ( face.index[ 2 ] != -1 );
    return result;
}

internal boo32
isTri( FACE face ) {
    Assert( isGood( face ) );
    boo32 result = ( face.index[ 3 ] == -1 );
    return result;
}

internal boo32
isQuad( FACE face ) {
    Assert( isGood( face ) );
    boo32 result = ( face.index[ 3 ] != -1 );
    return result;
}

internal uint32
getElemCount( FACE face ) {
    uint32 result = isQuad( face ) ? 4 : 3;
    return result;
}

struct QUAD {
    vec3 P[ 4 ];
};

struct TRI {
    vec3 P[ 3 ];
};

internal vec3
getNormal( TRI tri ) {
    vec3 result = getNormal( cross( tri.P[ 1 ] - tri.P[ 0 ], tri.P[ 2 ] - tri.P[ 0 ] ) );
    return result;
}

internal RAY_INTERSECT
doesIntersectTri( RAY ray, TRI tri ) {
    RAY_INTERSECT result = {};
    
    vec3 A      = tri.P[ 0 ];
    vec3 B      = tri.P[ 1 ];
    vec3 C      = tri.P[ 2 ];
    vec3 normal = getNormal( tri );
    
    if( isOppositeTo( ray.vector, normal ) ) {
        flo32 t = -( dot( ray.origin, normal ) + dot( -A, normal ) ) / dot( ray.vector, normal );
        
        if( t >= 0.0f ) {
            vec3 P = ray.origin + ray.vector * t;
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
                result.isValid = true;
                result.t       = t;
                result.P       = P;
            }
        }
    }
    return result;
}

internal RAY_INTERSECT
doesIntersectQuad( RAY ray, QUAD quad ) {
    TRI t0 = { quad.P[ 0 ], quad.P[ 1 ], quad.P[ 2 ] };
    TRI t1 = { quad.P[ 0 ], quad.P[ 2 ], quad.P[ 3 ] };
    
    RAY_INTERSECT result = doesIntersectTri( ray, t0 );
    if( !result.isValid ) {
        result = doesIntersectTri( ray, t1 );
    }
    return result;
}

internal QUAD
toQuad( EDITOR_STATE * editorState, FACE face ) {
    Assert( isQuad( face ) );
    
    QUAD result = {};
    result.P[ 0 ] = editorState->point[ face.index[ 0 ] ];
    result.P[ 1 ] = editorState->point[ face.index[ 1 ] ];
    result.P[ 2 ] = editorState->point[ face.index[ 3 ] ];
    result.P[ 3 ] = editorState->point[ face.index[ 2 ] ];
    // NOTE THE ORDERING OF P[ 2 ] and P[ 3 ]
    return result;
}

internal TRI
toTri( EDITOR_STATE * e, FACE face ) {
    Assert( isTri( face ) );
    
    TRI result = {};
    result.P[ 0 ] = e->point[ face.index[ 0 ] ];
    result.P[ 1 ] = e->point[ face.index[ 1 ] ];
    result.P[ 2 ] = e->point[ face.index[ 2 ] ];
    
    return result;
}

internal RAY_INTERSECT
doesIntersectAnyFace( EDITOR_STATE * e, RAY ray ) {
    RAY_INTERSECT result = {};
    result.t = 1000000.0f;
    
    for( int32 iFace = 0; iFace < e->face_count; iFace++ ) {
        FACE face = e->face[ iFace ];
        
        RAY_INTERSECT i = {};
        if( isQuad( face ) ) {
            i = doesIntersectQuad( ray, toQuad( e, face ) );
        } else {
            Assert( isGood( face ) );
            i = doesIntersectTri ( ray, toTri ( e, face ) );
        }
        
        if( ( i.isValid ) && ( i.t < result.t ) ) {
            result.isValid = true;
            result.t       = i.t;
            result.P       = ray.origin + ray.vector * i.t;
        }
    }
    return result;
}

internal RAY
getSelectRay( CAMERA * camera, mat4 camera_transform, vec2 mouse_position, vec2 window_dim ) {
    flo32 mouse_tX = ( mouse_position.x / window_dim.x ) * 2.0f - 1.0f;
    flo32 mouse_tY = ( mouse_position.y / window_dim.y ) * 2.0f - 1.0f;
    
    mat4 inverse_transform = mat4_inverse( camera_transform );
    
    flo32 z = ( camera->isOrthographic ? 0.0f : -1.0f );
    vec4 nearPoint         = inverse_transform * Vec4( mouse_tX, mouse_tY, z, 1.0f );
    nearPoint.xyz /= nearPoint.w;
    
    RAY result = {};
    result.origin = getCameraPos( camera );
    result.vector = getNormal( nearPoint.xyz - result.origin );
    
    if( camera->isOrthographic ) {
        result.origin = nearPoint.xyz;
        result.vector = -camera->zAxis;
    }
    
    return result;
}

internal int32
getFirstFace( EDITOR_STATE * editorState, RAY ray ) {
    int32 result = -1;
    flo32 tMax  = 1000000.0f;
    for( int32 iFace = 0; iFace < editorState->face_count; iFace++ ) {
        FACE face = editorState->face[ iFace ];
        
        RAY_INTERSECT intersect = {};
        if( isQuad( face ) ) {
            intersect = doesIntersectQuad( ray, toQuad( editorState, face ) );
        } else {
            intersect = doesIntersectTri ( ray, toTri ( editorState, face ) );
        }
        
        if( ( intersect.isValid ) && ( intersect.t < tMax ) ) {
            result = iFace;
            tMax   = intersect.t;
        }
    }
    return result;
}

internal int32
getNextFace( EDITOR_STATE * e, RAY ray ) {
    int32 result = -1;
    flo32 tMax   = 1000000.0f;
    for( int32 iFace = 0; iFace < e->face_count; iFace++ ) {
        FACE face = e->face[ iFace ];
        
        RAY_INTERSECT intersect = {};
        if( isQuad( face ) ) {
            intersect = doesIntersectQuad( ray, toQuad( e, face ) );
        } else {
            intersect = doesIntersectTri ( ray, toTri ( e, face ) );
        }
        
        if( ( intersect.isValid ) && ( intersect.t > 1.0f ) && ( intersect.t < tMax ) ) {
            result = iFace;
            tMax   = intersect.t;
        }
    }
    return result;
}

internal RAY_INTERSECT
doesIntersectFace( EDITOR_STATE * e, RAY ray, FACE face ) {
    RAY_INTERSECT result = {};
    Assert( isGood( face ) );
    
    if( isQuad( face ) ) {
        result = doesIntersectQuad( ray, toQuad( e, face ) );
    } else {
        result = doesIntersectTri ( ray, toTri ( e, face ) );
    }
    return result;
}

internal RAY_INTERSECT
doesIntersectFirstWalkableFace( EDITOR_STATE * e, RAY ray ) {
    RAY_INTERSECT result = {};
    
    int32 face_index = getFirstFace( e, ray );
    if( face_index != -1 ) {
        FACE face = e->face[ face_index ];
        vec3 normal = getNormal( e, face );
        if( testIsWalkable( normal ) ) {
            result = doesIntersectFace( e, ray, face );
        }
    }
    return result;
}

internal RAY_INTERSECT
doesIntersectNextFace( EDITOR_STATE * e, RAY ray ) {
    RAY_INTERSECT result = {};
    int32 face_index = getNextFace( e, ray );
    if( face_index != -1 ) {
        FACE face = e->face[ face_index ];
        Assert( isGood( face ) );
        
        if( isQuad( face ) ) {
            result = doesIntersectQuad( ray, toQuad( e, face ) );
        } else {
            result = doesIntersectTri ( ray, toTri ( e, face ) );
        }
    }
    return result;
}

internal int32
getBestPoint( EDITOR_STATE * editorState, vec3 select_origin, vec3 select_vector, flo32 range ) {
    int32 result  = -1;
    flo32 maxDist = 1000000.0f;
    flo32 rangeSq = range * range;
    for( int32 iPoint = 0; iPoint < editorState->point_count; iPoint++ ) {
        vec3 P = editorState->point[ iPoint ];
        vec3 V = P - select_origin;
        
        flo32 dotUV = dot( select_vector, V );
        if( ( dotUV >= 0.0f ) && ( dotUV < maxDist ) ) {
            flo32 bSq = dot( V, V ) - ( dotUV * dotUV );
            if( bSq <= rangeSq ) {
                result  = iPoint;
                maxDist = dotUV;
            }
        }
    }
    return result;
}

internal int32
getBestPoint( EDITOR_STATE * editorState, RAY select ) {
    int32 result = getBestPoint( editorState, select.origin, select.vector, RAY_SELECT__RANGE );
    return result;
}

internal void
flip( EDITOR__LEDGE * ledge ) {
    _swap( uint32, ledge->point_index[ 0 ], ledge->point_index[ 1 ] );
}

struct INTERVAL {
    flo32 lo;
    flo32 hi;
};

internal INTERVAL
Interval( flo32 a, flo32 b ) {
    if( a > b ) {
        _swap( flo32, a, b );
    }
    INTERVAL result = { a, b };
    return result;
    
}

internal boo32
doOverlap( INTERVAL a, INTERVAL b ) {
    boo32 result = !( ( a.lo > b.hi ) || ( a.hi < b.lo ) );
    return result;
}

internal RAY_INTERSECT
doesIntersectBox( vec3 ray_origin, vec3 ray_vector, ORIENTED_BOX box ) {
    RAY_INTERSECT result = {};
    
    flo32 dotX = dot( box.center, box.xAxis );
    flo32 dotY = dot( box.center, box.yAxis );
    flo32 dotZ = dot( box.center, box.zAxis );
    
    PLANE x0 = Plane( box.xAxis, dotX + box.halfDim.x );
    PLANE x1 = Plane( box.xAxis, dotX - box.halfDim.x );
    PLANE y0 = Plane( box.yAxis, dotY + box.halfDim.y );
    PLANE y1 = Plane( box.yAxis, dotY - box.halfDim.y );
    PLANE z0 = Plane( box.zAxis, dotZ + box.halfDim.z );
    PLANE z1 = Plane( box.zAxis, dotZ - box.halfDim.z );
    
    RAY_INTERSECT i0 = doesIntersectPlane( ray_origin, ray_vector, x0 );
    RAY_INTERSECT i1 = doesIntersectPlane( ray_origin, ray_vector, x1 );
    RAY_INTERSECT i2 = doesIntersectPlane( ray_origin, ray_vector, y0 );
    RAY_INTERSECT i3 = doesIntersectPlane( ray_origin, ray_vector, y1 );
    RAY_INTERSECT i4 = doesIntersectPlane( ray_origin, ray_vector, z0 );
    RAY_INTERSECT i5 = doesIntersectPlane( ray_origin, ray_vector, z1 );
    
    if( ( i0.isValid ) && ( i1.isValid ) && ( i2.isValid ) && ( i3.isValid ) && ( i4.isValid ) && ( i5.isValid ) ) {
        INTERVAL v0 = Interval( i0.t, i1.t );
        INTERVAL v1 = Interval( i2.t, i3.t );
        INTERVAL v2 = Interval( i4.t, i5.t );
        
        // // DISPLAY_VALUE( flo32, v0.lo );
        // // DISPLAY_VALUE( flo32, v0.hi );
        // // DISPLAY_VALUE( flo32, v1.lo );
        // // DISPLAY_VALUE( flo32, v1.hi );
        // // DISPLAY_VALUE( flo32, v2.lo );
        // // DISPLAY_VALUE( flo32, v2.hi );
        
        flo32 tMax = maxValue( maxValue( v0.lo, v1.lo ), v2.lo );
        flo32 tMin = minValue( minValue( v0.hi, v1.hi ), v2.hi );
        
        // // DISPLAY_VALUE( flo32, tMax );
        // // DISPLAY_VALUE( flo32, tMin );
        
        if( tMax <= tMin ) {
            result.isValid = true;
            result.t       = tMax;
            result.P       = ray_origin + ray_vector * tMax;
        }
    }
    return result;
}

internal RAY_INTERSECT
doesIntersectBox( ORIENTED_BOX box, RAY select ) {
    RAY_INTERSECT result = doesIntersectBox( select.origin, select.vector, box );
    return result;
}

internal RAY_INTERSECT
doesIntersectSphere( SPHERE sphere, RAY select ) {
    RAY_INTERSECT result = {};
    
    vec3 U = select.vector;
    vec3 V = select.origin - sphere.center;
    
    flo32 dotUV = dot( U, V );
    
    flo32 discriminant = ( dotUV * dotUV ) + ( sphere.radius * sphere.radius ) - dot( V, V );
    if( discriminant >= 0.0f ) {
        flo32 t = -dotUV - sqrtf( discriminant );
        //NOTE: only checks if the near side of the sphere is valid
        //NOTE: t = -dotUV + sqrtf( disc ) would check the far side of the sphere
        if( t >= 0.0f ) {
            result.isValid = true;
            result.t       = t;
            result.P       = select.origin + select.vector * t;
        }
    }
    
    return result;
}

internal int32
getBestLedge( EDITOR_STATE * e, RAY select ) {
    int32 result = -1;
    flo32 tMax   = 1000000.0f;
    for( uint32 iLedge = 0; iLedge < e->nLedge; iLedge++ ) {
        EDITOR__LEDGE ledge = e->ledge[ iLedge ];
        ORIENTED_BOX bound  = toOBox( e, ledge );
        
        RAY_INTERSECT intersect = doesIntersectBox( bound, select );
        if( ( intersect.isValid ) && ( intersect.t < tMax ) ) {
            result = iLedge;
            tMax   = intersect.t;
        }
    }
    return result;
}

internal void
deleteLedge( EDITOR_STATE * e, uint32 ledge_index ) {
    Assert( ( ledge_index >= 0 ) && ( ledge_index < e->nLedge ) );
    
    for( uint32 iLedge = ledge_index; iLedge < ( e->nLedge - 1 ); iLedge++ ) {
        e->ledge[ iLedge ] = e->ledge[ iLedge + 1 ];
    }
    e->nLedge--;
}

internal void
drawBoxOutline( RENDERER * renderer, RENDER_PASS_ID passID, ORIENTED_BOX box, vec4 color ) {
    vec3 C = box.center;
    vec3 X = box.xAxis * box.halfDim.x;
    vec3 Y = box.yAxis * box.halfDim.y;
    vec3 Z = box.zAxis * box.halfDim.z;
    
    vec3 pos[ 8 ] = {};
    pos[ 0 ] = C - X - Y + Z;
    pos[ 1 ] = C + X - Y + Z;
    pos[ 2 ] = C - X + Y + Z;
    pos[ 3 ] = C + X + Y + Z;
    pos[ 4 ] = C + X - Y - Z;
    pos[ 5 ] = C - X - Y - Z;
    pos[ 6 ] = C + X + Y - Z;
    pos[ 7 ] = C - X + Y - Z;
    
    vec3 normal = DIRECTION_TO_LIGHT;
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    
    addEdge( &vertexTask, pos[ 0 ], pos[ 1 ], normal );
    addEdge( &vertexTask, pos[ 1 ], pos[ 3 ], normal );
    addEdge( &vertexTask, pos[ 3 ], pos[ 2 ], normal );
    addEdge( &vertexTask, pos[ 2 ], pos[ 0 ], normal );
    
    addEdge( &vertexTask, pos[ 4 ], pos[ 5 ], normal );
    addEdge( &vertexTask, pos[ 5 ], pos[ 7 ], normal );
    addEdge( &vertexTask, pos[ 7 ], pos[ 6 ], normal );
    addEdge( &vertexTask, pos[ 6 ], pos[ 4 ], normal );
    
    addEdge( &vertexTask, pos[ 0 ], pos[ 5 ], normal );
    addEdge( &vertexTask, pos[ 1 ], pos[ 4 ], normal );
    addEdge( &vertexTask, pos[ 3 ], pos[ 6 ], normal );
    addEdge( &vertexTask, pos[ 2 ], pos[ 7 ], normal );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lines, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawBox( RENDERER * renderer, RENDER_PASS_ID passID, ORIENTED_BOX box, vec4 color ) {
    vec3 C = box.center;
    vec3 X = box.xAxis * box.halfDim.x;
    vec3 Y = box.yAxis * box.halfDim.y;
    vec3 Z = box.zAxis * box.halfDim.z;
    
    vec3 pos[ 8 ] = {};
    pos[ 0 ] = C - X - Y + Z;
    pos[ 1 ] = C + X - Y + Z;
    pos[ 2 ] = C - X + Y + Z;
    pos[ 3 ] = C + X + Y + Z;
    pos[ 4 ] = C + X - Y - Z;
    pos[ 5 ] = C - X - Y - Z;
    pos[ 6 ] = C + X + Y - Z;
    pos[ 7 ] = C - X + Y - Z;
    
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    addQuad( &vertexTask, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ],  box.zAxis );
    addQuad( &vertexTask, pos[ 4 ], pos[ 5 ], pos[ 6 ], pos[ 7 ], -box.zAxis );
    addQuad( &vertexTask, pos[ 2 ], pos[ 3 ], pos[ 7 ], pos[ 6 ],  box.yAxis );
    addQuad( &vertexTask, pos[ 5 ], pos[ 4 ], pos[ 0 ], pos[ 1 ], -box.yAxis );
    addQuad( &vertexTask, pos[ 1 ], pos[ 4 ], pos[ 3 ], pos[ 6 ],  box.xAxis );
    addQuad( &vertexTask, pos[ 5 ], pos[ 0 ], pos[ 7 ], pos[ 2 ], -box.xAxis );
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_boxTexture, color ) );
}

internal void
drawBox( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position, flo32 radius, vec4 color ) {
    ORIENTED_BOX b = {};
    b.center = position;
    b.xAxis  = Vec3( 1.0f, 0.0f, 0.0f );
    b.yAxis  = Vec3( 0.0f, 1.0f, 0.0f );
    b.zAxis  = Vec3( 0.0f, 0.0f, 1.0f );
    b.halfDim = Vec3( radius );
    
    drawBox( renderer, passID, b, color );
}

internal void
drawSphereOutline( RENDERER * renderer, RENDER_PASS_ID passID, SPHERE sphere, vec4 color ) {
    vec3 C = sphere.center;
    vec3 X = Vec3( sphere.radius, 0.0f, 0.0f );
    vec3 Y = Vec3( 0.0f, sphere.radius, 0.0f );
    vec3 Z = Vec3( 0.0f, 0.0f, sphere.radius );
    
#define SPHERE__VER_SEGMENT_COUNT  (  8 )
#define SPHERE__HOR_SEGMENT_COUNT  ( 16 )
    
    flo32 cosPhi[ SPHERE__VER_SEGMENT_COUNT + 1 ] = {};
    flo32 sinPhi[ SPHERE__VER_SEGMENT_COUNT + 1 ] = {};
    flo32 cosThe[ SPHERE__HOR_SEGMENT_COUNT + 1 ] = {};
    flo32 sinThe[ SPHERE__HOR_SEGMENT_COUNT + 1 ] = {};
    for( uint32 iV = 0; iV < ( SPHERE__VER_SEGMENT_COUNT + 1 ); iV++ ) {
        flo32 radians = ( ( flo32 )iV / ( flo32 )SPHERE__VER_SEGMENT_COUNT ) * PI - PI * 0.5f;
        cosPhi[ iV ] = cosf( radians );
        sinPhi[ iV ] = sinf( radians );
    }
    for( uint32 iH = 0; iH < ( SPHERE__HOR_SEGMENT_COUNT + 1 ); iH++ ) {
        flo32 radians = ( ( flo32 )iH / ( flo32 )SPHERE__HOR_SEGMENT_COUNT ) * PI * 2.0f;
        cosThe[ iH ] = cosf( radians );
        sinThe[ iH ] = sinf( radians );
    }
    
    vec3 normal = DIRECTION_TO_LIGHT;
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    
    for( uint32 iV = 0; iV < SPHERE__VER_SEGMENT_COUNT; iV++ ) {
        flo32 y0 = sinPhi[ iV ];
        flo32 y1 = sinPhi[ iV + 1 ];
        
        flo32 cPhi0 = cosPhi[ iV ];
        flo32 cPhi1 = cosPhi[ iV + 1 ];
        for( uint32 iH = 0; iH < SPHERE__HOR_SEGMENT_COUNT; iH++ ) {
            flo32 x00 = cPhi0 * cosThe[ iH ];
            flo32 z00 = cPhi0 * sinThe[ iH ];
            
            flo32 x01 = cPhi0 * cosThe[ iH + 1 ];
            flo32 z01 = cPhi0 * sinThe[ iH + 1 ];
            
            flo32 x10 = cPhi1 * cosThe[ iH ];
            flo32 z10 = cPhi1 * sinThe[ iH ];
            
            vec3 P0 = Vec3( x00, y0, z00 ) * sphere.radius + sphere.center;
            vec3 P1 = Vec3( x01, y0, z01 ) * sphere.radius + sphere.center;
            vec3 P2 = Vec3( x10, y1, z10 ) * sphere.radius + sphere.center;
            
            addEdge( &vertexTask, P0, P1, normal );
            addEdge( &vertexTask, P0, P2, normal );
        }
    }
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lines, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal void
drawCapsuleOutline( RENDERER * renderer, RENDER_PASS_ID passID, CAPSULE capsule, vec4 color ) {
    vec3 P = capsule.P;
    vec3 Q = capsule.Q;
    
    vec3 Y0 = Vec3( 0.0f, 1.0f, 0.0f );
    vec3 X  = Vec3( 1.0f, 0.0f, 0.0f );
    vec3 Y  = getNormal( Q - P );
    if( !isCollinearTo( Y, Y0 ) ) {
        X = getNormal( cross( Y0, Y ) );
    }
    vec3 Z = getNormal( cross( X, Y ) );
    
#define CAPSULE__VER_SEGMENT_COUNT  ( 4 )
#define CAPSULE__HOR_SEGMENT_COUNT  ( 4 )
    
    flo32 cosPhi[ CAPSULE__VER_SEGMENT_COUNT + 1 ] = {};
    flo32 sinPhi[ CAPSULE__VER_SEGMENT_COUNT + 1 ] = {};
    flo32 cosThe[ CAPSULE__HOR_SEGMENT_COUNT + 1 ] = {};
    flo32 sinThe[ CAPSULE__HOR_SEGMENT_COUNT + 1 ] = {};
    for( uint32 iV = 0; iV < ( CAPSULE__VER_SEGMENT_COUNT + 1 ); iV++ ) {
        flo32 radians = ( ( flo32 )iV / ( flo32 )CAPSULE__VER_SEGMENT_COUNT ) * PI * 0.5f;
        cosPhi[ iV ] = cosf( radians );
        sinPhi[ iV ] = sinf( radians );
    }
    for( uint32 iH = 0; iH < ( CAPSULE__HOR_SEGMENT_COUNT + 1 ); iH++ ) {
        flo32 radians = ( ( flo32 )iH / ( flo32 )CAPSULE__HOR_SEGMENT_COUNT ) * PI * 0.5f;
        cosThe[ iH ] = cosf( radians );
        sinThe[ iH ] = sinf( radians );
    }
    
    vec3 normal = DIRECTION_TO_LIGHT;
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    
    vec3 V = capsule.Q - capsule.P;
    for( uint32 iH = 0; iH < CAPSULE__HOR_SEGMENT_COUNT; iH++ ) {
        flo32 c0 = cosThe[ iH     ] * capsule.radius;
        flo32 s0 = sinThe[ iH     ] * capsule.radius;
        flo32 c1 = cosThe[ iH + 1 ] * capsule.radius;
        flo32 s1 = sinThe[ iH + 1 ] * capsule.radius;
        
        vec3 P0 = capsule.P + X * c0 + Z * s0;
        vec3 P1 = capsule.P - X * s0 + Z * c0;
        vec3 P2 = capsule.P - X * c0 - Z * s0;
        vec3 P3 = capsule.P + X * s0 - Z * c0;
        
        vec3 Q0 = capsule.P + X * c1 + Z * s1;
        vec3 Q1 = capsule.P - X * s1 + Z * c1;
        vec3 Q2 = capsule.P - X * c1 - Z * s1;
        vec3 Q3 = capsule.P + X * s1 - Z * c1;
        
        vec3 R0 = P0 + V;
        vec3 R1 = P1 + V;
        vec3 R2 = P2 + V;
        vec3 R3 = P3 + V;
        
        vec3 S0 = Q0 + V;
        vec3 S1 = Q1 + V;
        vec3 S2 = Q2 + V;
        vec3 S3 = Q3 + V;
        
        addEdge( &vertexTask, P0, Q0, normal );
        addEdge( &vertexTask, P1, Q1, normal );
        addEdge( &vertexTask, P2, Q2, normal );
        addEdge( &vertexTask, P3, Q3, normal );
        
        addEdge( &vertexTask, P0, R0, normal );
        addEdge( &vertexTask, P1, R1, normal );
        addEdge( &vertexTask, P2, R2, normal );
        addEdge( &vertexTask, P3, R3, normal );
        
        addEdge( &vertexTask, R0, S0, normal );
        addEdge( &vertexTask, R1, S1, normal );
        addEdge( &vertexTask, R2, S2, normal );
        addEdge( &vertexTask, R3, S3, normal );
    }
    
    for( uint32 iV = 1; iV < CAPSULE__VER_SEGMENT_COUNT; iV++ ) {
        vec3 A = capsule.P - Y * ( sinPhi[ iV     ] * capsule.radius );
        vec3 B = capsule.P - Y * ( sinPhi[ iV - 1 ] * capsule.radius );
        vec3 C = capsule.Q + Y * ( sinPhi[ iV     ] * capsule.radius );
        vec3 D = capsule.Q + Y * ( sinPhi[ iV - 1 ] * capsule.radius );
        
        flo32 m = cosPhi[ iV     ];
        flo32 n = cosPhi[ iV - 1 ];
        
        for( uint32 iH = 0; iH < CAPSULE__HOR_SEGMENT_COUNT; iH++ ) {
            flo32 c0 = cosThe[ iH     ] * m * capsule.radius;
            flo32 s0 = sinThe[ iH     ] * m * capsule.radius;
            flo32 c1 = cosThe[ iH + 1 ] * m * capsule.radius;
            flo32 s1 = sinThe[ iH + 1 ] * m * capsule.radius;
            flo32 c2 = cosThe[ iH     ] * n * capsule.radius;
            flo32 s2 = sinThe[ iH     ] * n * capsule.radius;
            
            vec3 P0 = A + X * c0 + Z * s0;
            vec3 P1 = A - X * s0 + Z * c0;
            vec3 P2 = A - X * c0 - Z * s0;
            vec3 P3 = A + X * s0 - Z * c0;
            
            vec3 Q0 = A + X * c1 + Z * s1;
            vec3 Q1 = A - X * s1 + Z * c1;
            vec3 Q2 = A - X * c1 - Z * s1;
            vec3 Q3 = A + X * s1 - Z * c1;
            
            vec3 R0 = B + X * c2 + Z * s2;
            vec3 R1 = B - X * s2 + Z * c2;
            vec3 R2 = B - X * c2 - Z * s2;
            vec3 R3 = B + X * s2 - Z * c2;
            
            addEdge( &vertexTask, P0, Q0, normal );
            addEdge( &vertexTask, P1, Q1, normal );
            addEdge( &vertexTask, P2, Q2, normal );
            addEdge( &vertexTask, P3, Q3, normal );
            
            addEdge( &vertexTask, P0, R0, normal );
            addEdge( &vertexTask, P1, R1, normal );
            addEdge( &vertexTask, P2, R2, normal );
            addEdge( &vertexTask, P3, R3, normal );
            
            vec3 T0 = C + X * c0 + Z * s0;
            vec3 T1 = C - X * s0 + Z * c0;
            vec3 T2 = C - X * c0 - Z * s0;
            vec3 T3 = C + X * s0 - Z * c0;
            
            vec3 U0 = C + X * c1 + Z * s1;
            vec3 U1 = C - X * s1 + Z * c1;
            vec3 U2 = C - X * c1 - Z * s1;
            vec3 U3 = C + X * s1 - Z * c1;
            
            vec3 V0 = D + X * c2 + Z * s2;
            vec3 V1 = D - X * s2 + Z * c2;
            vec3 V2 = D - X * c2 - Z * s2;
            vec3 V3 = D + X * s2 - Z * c2;
            
            addEdge( &vertexTask, T0, U0, normal );
            addEdge( &vertexTask, T1, U1, normal );
            addEdge( &vertexTask, T2, U2, normal );
            addEdge( &vertexTask, T3, U3, normal );
            
            addEdge( &vertexTask, T0, V0, normal );
            addEdge( &vertexTask, T1, V1, normal );
            addEdge( &vertexTask, T2, V2, normal );
            addEdge( &vertexTask, T3, V3, normal );
        }
    }
    
    addVertex( renderer, 1, vertexTask.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_lines, vertexTask.vertex_index, vertexTask.vertex_count, assetID_texture_whiteTexture, color ) );
}

internal boo32
isVertical( EDITOR_STATE * editorState, EDGE__ edge ) {
    boo32 result = false;
    
    vec3 P = editorState->point[ edge.index[ 0 ] ];
    vec3 Q = editorState->point[ edge.index[ 1 ] ];
    vec3 U = Vec3( 0.0f, 1.0f, 0.0f );
    vec3 V = getNormal( Q - P );
    flo32 dotUV = dot( U, V );
    if( fabsf( dotUV ) > ( 1.0f - DOT__COLLINEAR_EPSILON ) ) {
        result = true;
    }
    return result;
}

internal void
drawTextBox( RENDERER * renderer, RENDER_PASS_ID passID, TEXT_BOX_F32 t, char * label ) {
    drawBox( renderer, passID, t.bound, COLOR_WHITE );
    
    char text[ 16 + 1 ] = {};
    if( t.input ) {
        strcpy ( text, t.input->string );
    } else if( t.output ) {
        sprintf( text, "%f", *( t.output ) );
    } else {
        strcpy ( text, "------------" );
    }
    drawString( renderer, passID, assetID_font_default, text, getTL( t.bound ) + Vec2( 4.0f, -4.0f ), ALIGN_TOPLEFT, COLOR_BLACK );
    drawString( renderer, passID, assetID_font_default, label, getTL( t.bound ) + Vec2( -2.0f, -2.0f ), ALIGN_TOPRIGHT, COLOR_BLACK );
    
}

internal void
drawToggleButton( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, boo32 isActive ) {
    drawRect       ( renderer, passID, bound, COLOR_WHITE );
    drawRectOutline( renderer, passID, bound, COLOR_BLACK );
    if( isActive ) {
        drawRect( renderer, passID, addRadius( bound, -1.0f ), COLOR_RED );
    }
}

internal void
drawPushButton( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, MOUSE_STATE * mouse ) {
    boo32 isHover = isInBound( mouse->position, bound );
    vec4  color   = isHover ? COLOR_GRAY( 0.75f ) : COLOR_WHITE;
    drawRect       ( renderer, passID, bound, color );
    drawRectOutline( renderer, passID, bound, COLOR_BLACK );
}

struct EDGE__CONNECTED_FACE {
    int32 nFace;
    int32 face[ 2 ];
};

internal boo32
hasEdge( FACE face, EDGE__ edge ) {
    uint32 nPoints = 0;
    uint32 nElem   = getElemCount( face );
    for( uint32 iElem = 0; iElem < nElem; iElem++ ) {
        if( ( edge.index[ 0 ] == face.index[ iElem ] ) || ( edge.index[ 1 ] == face.index[ iElem ] ) ) {
            nPoints++;
        }
    }
    Assert( nPoints <= 2 );
    boo32 result = ( nPoints == 2 );
    return result;
}

internal EDGE__CONNECTED_FACE
findConnectedFace( EDITOR_STATE * editorState, EDGE__ edge ) {
    EDGE__CONNECTED_FACE result = { 0, -1, -1 };
    for( int32 iFace = 0; iFace < editorState->face_count; iFace++ ) {
        FACE face = editorState->face[ iFace ];
        if( hasEdge( face, edge ) ) {
            Assert( result.nFace < 2 );
            result.face[ result.nFace++ ] = iFace;
        }
    }
    return result;
}

internal mat4
genCameraTransform( vec3 position, vec3 xAxis, vec3 yAxis, vec3 zAxis ) {
	mat3 rotMatrix   = mat3_byRow( xAxis, yAxis, zAxis );
	vec3 adjPosition = -( rotMatrix * position );
	
	mat4 result = {};
	result.elem[  0 ] = rotMatrix.elem[ 0 ];
	result.elem[  1 ] = rotMatrix.elem[ 1 ];
	result.elem[  2 ] = rotMatrix.elem[ 2 ];
	result.elem[  3 ] = 0.0f;
	result.elem[  4 ] = rotMatrix.elem[ 3 ];
	result.elem[  5 ] = rotMatrix.elem[ 4 ];
	result.elem[  6 ] = rotMatrix.elem[ 5 ];
	result.elem[  7 ] = 0.0f;
	result.elem[  8 ] = rotMatrix.elem[ 6 ];
	result.elem[  9 ] = rotMatrix.elem[ 7 ];
	result.elem[ 10 ] = rotMatrix.elem[ 8 ];
	result.elem[ 11 ] = 0.0f;
	result.elem[ 12 ] = adjPosition.x;
	result.elem[ 13 ] = adjPosition.y;
	result.elem[ 14 ] = adjPosition.z;
	result.elem[ 15 ] = 1.0f;
	return result;
}

internal void
updateCameraMovement( EDITOR_STATE * editorState, CAMERA * c, KEYBOARD_STATE * keyboard, MOUSE_STATE * mouse, vec2 app_dim, flo32 dt ) {
    flo32 camera_hRadians = c->hRadians;
    vec3  camera_xAxis    = c->xAxis;
    vec3  camera_yAxis    = c->yAxis;
    vec3  camera_zAxis    = c->zAxis;
    
    // camera movement
    if( editorState->mouseMiddle_wasPressed ) {
        editorState->mouse_prevPos = mouse->position;
        if( keyboard->flags == KEYBOARD_FLAGS__SHIFT ) {
            c->mouseMove_isActive = true;
        } else {
            c->mouseOrbit_isActive = true;
        }
    }
    if( c->mouseMove_isActive ) {
        vec2 move_vector = ( mouse->position - editorState->mouse_prevPos ) * -0.005f;
        editorState->mouse_prevPos = mouse->position;
        c->position += ( camera_xAxis * ( move_vector.x ) + camera_yAxis * ( move_vector.y ) );
    }
    if( c->mouseOrbit_isActive ) {
        vec2 radians = ( mouse->position - editorState->mouse_prevPos ) * 0.005f;
        editorState->mouse_prevPos = mouse->position;
        c->hRadians -= radians.x;
        c->vRadians -= radians.y;
    }
    if( editorState->mouseMiddle_wasReleased ) {
        c->mouseMove_isActive  = false;
        c->mouseOrbit_isActive = false;
    }
    //if( wasPressed( keyboard, keyCode_num1 ) ) {
    //c->hRadians = 0.0f;
    //c->vRadians = 0.0f;
    //}
    //if( wasPressed( keyboard, keyCode_num3 ) ) {
    //c->hRadians = PI * 0.5f;
    //c->vRadians = 0.0f;
    //}
    //if( wasPressed( keyboard, keyCode_num7 ) ) {
    //c->hRadians = 0.0f;
    //c->vRadians = PI * 0.5f;
    //}
    
    flo32 hCos = cosf( c->hRadians );
    flo32 hSin = sinf( c->hRadians );
    flo32 vCos = cosf( c->vRadians );
    flo32 vSin = sinf( c->vRadians );
    
    c->zAxis = Vec3( hSin * vCos, vSin,  hCos * vCos );
    c->xAxis = Vec3( hCos,        0.0f, -hSin        );
    c->yAxis = getNormal( cross( c->zAxis, c->xAxis ) );
    
    flo32 speed = 12.5f;
    if( c->moveSlow_isActive ) { speed = 5.0f; }
    if( c->move_left     ) { c->position -= c->xAxis * ( speed * dt ); }
    if( c->move_right    ) { c->position += c->xAxis * ( speed * dt ); }
    if( c->move_down     ) { c->position -= c->yAxis * ( speed * dt ); }
    if( c->move_up       ) { c->position += c->yAxis * ( speed * dt ); }
    if( c->isOrthographic ) {
        flo32 rate = 0.02f;
        if( c->moveSlow_isActive ) { rate = 0.01f; }
        if( c->move_forward  ) { c->ortho_dim *= ( 1.0f - rate ); }
        if( c->move_backward ) { c->ortho_dim *= ( 1.0f + rate ); }
    } else {
        if( c->move_forward  ) { c->position -= c->zAxis * ( speed * dt ); }
        if( c->move_backward ) { c->position += c->zAxis * ( speed * dt ); }
    }
}

struct VIEW_BOUND {
    boo32 isValid;
    vec3  center;
    vec2  dim;
};

internal VIEW_BOUND
getBoundOfPoints( vec3 * point, uint32 nPoints, CAMERA * camera ) {
    VIEW_BOUND result = {};
    
    vec3 min = Vec3(  1000000.0f );
    vec3 max = Vec3( -1000000.0f );
    
    vec3 axis[ 3 ] = {
        camera->xAxis,
        camera->yAxis,
        camera->zAxis,
    };
    
    vec3 cameraPos = getCameraPos( camera );
    for( uint32 iPoint = 0; iPoint < nPoints; iPoint++ ) {
        vec3 V = point[ iPoint ] - cameraPos;
        for( uint32 iAxis = 0; iAxis < 3; iAxis++ ) {
            flo32 t = dot( V, axis[ iAxis ] );
            min.elem[ iAxis ] = minValue( min.elem[ iAxis ], t );
            max.elem[ iAxis ] = maxValue( max.elem[ iAxis ], t );
        }
    }
    
    result.isValid = true;
    result.center  = ( min + max ) * 0.5f;
    result.dim     = Vec2( max.x - min.x, max.y - min.y );
    
    return result;
}

internal VIEW_BOUND
getBoundOfSelection( EDITOR_STATE * editorState, CAMERA * camera, MEMORY * tempMemory ) {
    VIEW_BOUND result = {};
    
    uint32 nSelect = getSelectedCount_vertex( editorState );
    if( nSelect > 0 ) {
        uint32 index = 0;
        vec3 * point = _pushArray( tempMemory, vec3, nSelect );
        
        for( int32 iPoint = 0; iPoint < editorState->point_count; iPoint++ ) {
            if( editorState->point_isSelected[ iPoint ] ) {
                point[ index++ ] = editorState->point[ iPoint ];
            }
        }
        Assert( index == nSelect );
        
        result = getBoundOfPoints( point, nSelect, camera );
        _popArray( tempMemory, vec3, nSelect );
    } else if( editorState->point_count > 0 ) {
        result = getBoundOfPoints( editorState->point, editorState->point_count, camera );
    }
    
    return result;
}

internal vec2
expandToAspectRatio( vec2 dim, flo32 aspectRatio ) {
    if( dim.x == 0.0f ) {
        dim.x = dim.y * aspectRatio;
    } else if( dim.y == 0.0f ) {
        dim.y = dim.x / aspectRatio;
    } else {
        flo32 a = dim.x / dim.y;
        if( a > aspectRatio ) {
            dim.y *= ( a / aspectRatio );
        } else if( a < aspectRatio ) {
            dim.x *= ( aspectRatio / a );
        }
    }
    
    vec2 result = dim;
    return result;
}

internal PLANE
getPlane( CAMERA * camera ) {
    PLANE result = {};
    result.normal         = camera->zAxis;
    result.distFromOrigin = dot( getCameraPos( camera ), camera->zAxis );
    return result;
}

internal vec3
closestPointOnPlane( vec3 P, PLANE plane ) {
    flo32 t = dot( P, plane.normal ) - plane.distFromOrigin;
    vec3 result = P - plane.normal * t;
    return result;
}

internal void
updateCameraInput( EDITOR_STATE * editorState, CAMERA * camera, KEYBOARD_STATE * keyboard, vec2 dim, vec2 app_dim, MEMORY * tempMemory ) {
    if( wasPressed ( keyboard, keyCode_num4 ) ) { camera->move_left     = true;  }
    if( wasReleased( keyboard, keyCode_num4 ) ) { camera->move_left     = false; }
    if( wasPressed ( keyboard, keyCode_num6 ) ) { camera->move_right    = true;  }
    if( wasReleased( keyboard, keyCode_num6 ) ) { camera->move_right    = false; }
    if( wasPressed ( keyboard, keyCode_num9 ) ) { camera->move_forward  = true;  }
    if( wasReleased( keyboard, keyCode_num9 ) ) { camera->move_forward  = false; }
    if( wasPressed ( keyboard, keyCode_num7 ) ) { camera->move_backward = true;  }
    if( wasReleased( keyboard, keyCode_num7 ) ) { camera->move_backward = false; }
    if( wasPressed ( keyboard, keyCode_num8 ) ) { camera->move_up       = true;  }
    if( wasReleased( keyboard, keyCode_num8 ) ) { camera->move_up       = false; }
    if( wasPressed ( keyboard, keyCode_num5 ) ) { camera->move_down     = true;  }
    if( wasReleased( keyboard, keyCode_num5 ) ) { camera->move_down     = false; }
    if( wasPressed ( keyboard, keyCode_num0 ) ) { camera->moveSlow_isActive = true;  }
    if( wasReleased( keyboard, keyCode_num0 ) ) { camera->moveSlow_isActive = false; }
    if( wasPressed ( keyboard, keyCode_numDiv ) ) { camera->isOrthographic = !camera->isOrthographic; }
    if( wasPressed ( keyboard, keyCode_numAdd ) ) {
        if( camera->isOrthographic ) {
            VIEW_BOUND bound = getBoundOfSelection( editorState, camera, tempMemory );
            if( bound.isValid ) {
                camera->ortho_dim = expandToAspectRatio( bound.dim, dim.x / dim.y ) * 1.05f;
                camera->position  = getCameraPos( camera );
                camera->position += camera->xAxis * bound.center.x;
                camera->position += camera->yAxis * bound.center.y;
                camera->position += camera->zAxis * bound.center.z;
                camera->dist      = 100.0f;
                
                // NOTE: Hack!! This adjusts the camera position as if we were drawing the 3d viewport to the actual display panel instead of the full window. If we ever include the ability to render to different viewports, this code should be removed.
                vec2 adjRatio = app_dim / dim;
                vec2 world_dim = camera->ortho_dim * adjRatio;
                vec2 adjPos    = ( ( world_dim - camera->ortho_dim ) * 0.5f );
                
                camera->ortho_dim *= adjRatio;
                camera->position  += camera->xAxis * adjPos.x;
                camera->position  -= camera->yAxis * adjPos.y;
            }
        } else {
            
        }
    }
    if( wasPressed( keyboard, keyCode_num1 ) ) {
        camera->hRadians = 0.0f;
        camera->vRadians = PI * 0.5f;
    }
    if( wasPressed( keyboard, keyCode_num2 ) ) {
        camera->hRadians = 0.0f;
        camera->vRadians = 0.0f;
    }
    if( wasPressed( keyboard, keyCode_num3 ) ) {
        camera->hRadians = PI * 0.5f;
        camera->vRadians = 0.0f;
    }
}


internal mat4
genCameraTransform( CAMERA * camera, vec2 view_dim ) {
    vec3 camera_position  = getCameraPos( camera );
    mat4 main_camera      = genCameraTransform( getCameraPos( camera ), camera->xAxis, camera->yAxis, camera->zAxis );
    
    rect view_bound = rectCD( camera->ortho_center, camera->ortho_dim );
    mat4 main_perspective = ( camera->isOrthographic ? mat4_orthographic( view_bound, camera->nearPlane, camera->farPlane ) :
                             mat4_perspective ( view_dim, camera->fovAngle, camera->nearPlane, camera->farPlane ) );
    
    mat4 result = main_perspective * main_camera;
    return result;
}

internal uint32
addSecureDoor( EDITOR_STATE * editorState, EDITOR__SECURE_DOOR door ) {
    Assert( editorState->nSecureDoor < EDITOR__SECURE_DOOR_COUNT );
    
    uint32 result = editorState->nSecureDoor;
    editorState->secureDoor[ editorState->nSecureDoor++ ] = door;
    return result;
};

internal void
outputEntity_begin( MEMORY * memory ) {
    // TODO: check if entity file was already created
    int32 version_number = 1;
    char  version_string[ 16 + 1 ] = {};
    sprintf( version_string, "[ENTITY_VERS%02d]", version_number );
    
    writeSegment( memory, version_string );
    writeSegment( memory, "\n\n" );
}

internal void
outputEntity_overhang( MEMORY * memory, EDITOR_STATE * editorState ) {
    for( int32 overhang_index = 0; overhang_index < editorState->overhang_count; overhang_index++ ) {
        OVERHANG * overhang = editorState->overhang + overhang_index;
        
        overhang->point[ 0 ] = editorState->point[ overhang->index[ 0 ] ];
        overhang->point[ 1 ] = editorState->point[ overhang->index[ 1 ] ];
    }
    
    if( editorState->overhang_count > 0 ) {
        writeSegment( memory, "[OVERHANG] {\n" );
        writeSegment( memory, "   " );
        writeS32( memory, editorState->overhang_count );
        writeSegment( memory, "\n" );
        for( int32 overhang_index = 0; overhang_index < editorState->overhang_count; overhang_index++ ) {
            OVERHANG overhang = editorState->overhang[ overhang_index ];
            
            writeSegment( memory, "   " );
            writeV3  ( memory, overhang.point[ 0 ] );
            writeSegment( memory, "   " );
            writeV3  ( memory, overhang.point[ 1 ] );
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal void
outputEntity_ledge( MEMORY * memory, EDITOR_STATE * e ) {
    if( e->nLedge > 0 ) {
        writeSegment( memory, "[LEDGE] {\n" );
        writeS32( memory, e->nLedge, 1, true );
        for( uint32 iLedge = 0; iLedge < e->nLedge; iLedge++ ) {
            LEDGE ledge = toLedge( e, e->ledge[ iLedge ] );
            
            writeV3( memory, ledge.point[ 0 ], 1, true );
            writeV3( memory, ledge.point[ 1 ], 1, true );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal void
outputEntity_pole( MEMORY * memory, EDITOR_STATE * editorState ) {
    for( int32 pole_index = 0; pole_index < editorState->pole_count; pole_index++ ) {
        POLE * pole = editorState->pole + pole_index;
        
        pole->point[ 0 ] = editorState->point[ pole->index[ 0 ] ];
        pole->point[ 1 ] = editorState->point[ pole->index[ 1 ] ];
    }
    
    if( editorState->pole_count > 0 ) {
        writeSegment( memory, "[POLE] {\n" );
        writeSegment( memory, "   " );
        writeS32( memory, editorState->pole_count );
        writeSegment( memory, "\n" );
        for( int32 pole_index = 0; pole_index < editorState->pole_count; pole_index++ ) {
            POLE pole = editorState->pole[ pole_index ];
            
            writeSegment( memory, "   " );
            writeV3  ( memory, pole.point[ 0 ] );
            writeSegment( memory, "   " );
            writeV3  ( memory, pole.point[ 1 ] );
            writeSegment( memory, "   " );
            writeV3  ( memory, pole.normal );
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal SECURE_DOOR
toSecureDoor( EDITOR_STATE * e, EDITOR__SECURE_DOOR eDoor ) {
    vec3 P = e->point[ eDoor.point_index ];
    vec3 O = P + Vec3( 0.0f, eDoor.height * 0.5f, 0.0f );
    vec3 Y = Vec3( 0.0f, 1.0f, 0.0f );
    vec3 Z = toVec3( eDoor.radians );
    vec3 X = getNormal( cross( Y, Z ) );
    
    RAY_INTERSECT i0 = doesIntersectAnyFace( e, Ray( O,  X ) );
    RAY_INTERSECT i1 = doesIntersectAnyFace( e, Ray( O, -X ) );
    
    flo32 w0 = SECURE_DOOR__MAX_HALF_WIDTH;
    flo32 w1 = SECURE_DOOR__MAX_HALF_WIDTH;
    if( ( i0.isValid ) && ( i0.t < w0 ) ) { w0 = i0.t; }
    if( ( i1.isValid ) && ( i1.t < w1 ) ) { w1 = i1.t; }
    flo32 shift = ( w1 - w0 ) * 0.5f;
    
    vec3 C = O + X * shift;
    
    flo32 hWidth  = ( w0 + w1 ) * 0.5f;
    flo32 hHeight = eDoor.height * 0.5f;
    flo32 hDepth_act = SECURE_DOOR__ACTIVATE__HALF_DEPTH;
    flo32 hDepth_ren = SECURE_DOOR__RENDER__HALF_DEPTH;
    
    SECURE_DOOR result = {};
    result.activate_bound  = OrientedBox( C, X, Y, Z, Vec3( hWidth, hHeight, hDepth_act ) );
    result.render_bound    = OrientedBox( C, X, Y, Z, Vec3( hWidth, hHeight, hDepth_ren ) );
    result.collision_bound = result.render_bound;
    for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
        result.pixie_cost[ iPixie ] = eDoor.pixie_cost[ iPixie ];
    }
    
    return result;
}

internal EDITOR__SECURE_DOOR
toEditorSecureDoor( EDITOR_STATE * e, SECURE_DOOR door, mat4 transform ) {
    ORIENTED_BOX b = transformOBox( transform, door.render_bound );
    
    flo32 radians    = getRadiansBetween( b.zAxis, Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, -1.0f, 0.0f ) );
    flo32 halfHeight = b.halfDim.y;
    vec3  base       = b.center + Vec3( 0.0f, -halfHeight, 0.0f );
    
    EDITOR__SECURE_DOOR result = {};
    result.point_index = getBestPoint( e, base );
    result.height      = halfHeight * 2.0f;
    result.radians     = radians;
    memcpy( result.pixie_cost, door.pixie_cost, sizeof( uint32 ) * 4 );
    
    return result;
}

internal void
outputEntity_secureDoor( MEMORY * memory, EDITOR_STATE * editorState ) {
    if( editorState->nSecureDoor > 0 ) {
        writeSegment( memory, "[SECURE_DOOR] {\n" );
        writeU32    ( memory, editorState->nSecureDoor, 1, true );
        for( uint32 iDoor = 0; iDoor < editorState->nSecureDoor; iDoor++ ) {
            EDITOR__SECURE_DOOR in = editorState->secureDoor[ iDoor ];
            
            SECURE_DOOR out = toSecureDoor( editorState, in );
            
            ORIENTED_BOX b = out.render_bound;
            writeV3( memory, b.center,  1, true );
            writeV3( memory, b.xAxis,   1, true );
            writeV3( memory, b.yAxis,   1, true );
            writeV3( memory, b.zAxis,   1, true );
            writeV3( memory, b.halfDim, 1, true );
            writeF32 ( memory, out.activate_bound.halfDim.z, 1, true );
            for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                writeU32( memory, out.pixie_cost[ iPixie ], 1, false );
            }
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal PIXIE
genPixie( vec3 position ) {
    ORIENTED_BOX b = {};
    b.halfDim = PIXIE__PICK_UP__HALF_DIM;
    b.xAxis   = Vec3( 1.0f, 0.0f, 0.0f );
    b.yAxis   = Vec3( 0.0f, 1.0f, 0.0f );
    b.zAxis   = Vec3( 0.0f, 0.0f, 1.0f );
    b.center  = position + Vec3( 0.0f, b.halfDim.y, 0.0f );
    
    PIXIE pixie = Pixie( pixieTypeID_grow, b );
    return pixie;
}

internal PIXIE
toPixie( EDITOR_STATE * e, EDITOR__PIXIE ePixie ) {
    vec3 P = e->point[ ePixie.point_index ];
    PIXIE result = genPixie( P );
    result.typeID = ePixie.typeID;
    return result;
}

internal EDITOR__PIXIE
toEditorPixie( EDITOR_STATE * e, PIXIE pixie, mat4 transform ) {
    EDITOR__PIXIE result = {};
    result.typeID = pixie.typeID;
    
    ORIENTED_BOX b = transformOBox( transform, pixie.pickUp_bound );
    vec3 P = b.center - b.yAxis * b.halfDim.y;
    result.point_index = getBestPoint( e, P );
    
    return result;
}

internal void
outputEntity_pixie( MEMORY * memory, EDITOR_STATE * editorState ) {
    if( editorState->nPixie > 0 ) {
        writeSegment( memory, "[PIXIE] {\n" );
        writeU32    ( memory, editorState->nPixie, 1, true );
        for( uint32 iPixie = 0; iPixie < editorState->nPixie; iPixie++ ) {
            EDITOR__PIXIE ePixie = editorState->pixie[ iPixie ];
            PIXIE pixie = toPixie( editorState, ePixie );
            
            ORIENTED_BOX b = pixie.pickUp_bound;
            writeV3( memory, b.center,  1, true );
            writeV3( memory, b.xAxis,   1, true );
            writeV3( memory, b.yAxis,   1, true );
            writeV3( memory, b.zAxis,   1, true );
            writeV3( memory, b.halfDim, 1, true );
            writeU32( memory, pixie.typeID, 1, true );
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal KILL_ZONE
genKillZone( vec3 position ) {
    ORIENTED_BOX b = {};
    b.halfDim = Vec3( 1.0f, 1.0f, 1.0f );
    b.xAxis   = Vec3( 1.0f, 0.0f, 0.0f );
    b.yAxis   = Vec3( 0.0f, 1.0f, 0.0f );
    b.zAxis   = Vec3( 0.0f, 0.0f, 1.0f );
    b.center  = position;
    
    KILL_ZONE killZone = {};
    killZone.bound = b;
    return killZone;
}

internal KILL_ZONE
toKillZone( EDITOR_STATE * e, EDITOR__KILL_ZONE eKillZone ) {
    vec3 P = e->point[ eKillZone.point_index ];
    
    ORIENTED_BOX bound = {};
    bound.center  = P;
    bound.xAxis   = getNormal( Vec3( cosf( eKillZone.radians ), 0.0f, -sinf( eKillZone.radians ) ) );
    bound.yAxis   = Vec3( 0.0f, 1.0f, 0.0f );
    bound.zAxis   = getNormal( cross( bound.xAxis, bound.yAxis ) );
    bound.halfDim = eKillZone.halfDim;
    
    KILL_ZONE result = {};
    result.bound = bound;
    return result;
}

internal EDITOR__KILL_ZONE
toEditorKillZone( EDITOR_STATE * e, KILL_ZONE killZone, mat4 transform ) {
    EDITOR__KILL_ZONE result = {};
    
    ORIENTED_BOX b = transformOBox( transform, killZone.bound );
    result.radians = getRadiansBetween( b.xAxis, Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, -1.0f, 0.0f ) );
    result.halfDim = b.halfDim;
    result.point_index = getBestPoint( e, b.center );
    
    return result;
}

internal void
outputEntity_killZone( MEMORY * memory, EDITOR_STATE * editorState ) {
    if( editorState->nKillZone > 0 ) {
        writeSegment( memory, "[KILL_ZONE] {\n" );
        writeU32    ( memory, editorState->nKillZone, 1, true );
        for( uint32 iKillZone = 0; iKillZone < editorState->nKillZone; iKillZone++ ) {
            EDITOR__KILL_ZONE eKillZone = editorState->killZone[ iKillZone ];
            KILL_ZONE killZone = toKillZone( editorState, eKillZone );
            
            ORIENTED_BOX b = killZone.bound;
            writeV3( memory, b.center,  1, true );
            writeV3( memory, b.xAxis,   1, true );
            writeV3( memory, b.yAxis,   1, true );
            writeV3( memory, b.zAxis,   1, true );
            writeV3( memory, b.halfDim, 1, true );
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal CHECKPOINT
genCheckpoint( vec3 position ) {
    ORIENTED_BOX b = {};
    b.halfDim = Vec3( 1.0f, 1.0f, 1.0f );
    b.xAxis   = Vec3( 1.0f, 0.0f, 0.0f );
    b.yAxis   = Vec3( 0.0f, 1.0f, 0.0f );
    b.zAxis   = Vec3( 0.0f, 0.0f, 1.0f );
    b.center  = position;
    
    CHECKPOINT checkpoint = {};
    checkpoint.bound = b;
    return checkpoint;
}

internal CHECKPOINT
toCheckpoint( EDITOR_STATE * e, EDITOR__CHECKPOINT eCheckpoint ) {
    vec3 P = e->point[ eCheckpoint.point_index ];
    
    ORIENTED_BOX bound = {};
    bound.center  = P;
    bound.xAxis   = getNormal( Vec3( cosf( eCheckpoint.radians ), 0.0f, -sinf( eCheckpoint.radians ) ) );
    bound.yAxis   = Vec3( 0.0f, 1.0f, 0.0f );
    bound.zAxis   = getNormal( cross( bound.xAxis, bound.yAxis ) );
    bound.halfDim = eCheckpoint.halfDim;
    
    CHECKPOINT result = {};
    result.bound      = bound;
    result.respawnPos = e->point[ eCheckpoint.respawn_index ];
    return result;
}

internal EDITOR__CHECKPOINT
toEditorCheckpoint( EDITOR_STATE * e, CHECKPOINT checkpoint, mat4 transform ) {
    EDITOR__CHECKPOINT result = {};
    ORIENTED_BOX b = transformOBox( transform, checkpoint.bound );
    result.radians = getRadiansBetween( b.xAxis, Vec3( 1.0f, 0.0f, 0.0f ), Vec3( 0.0f, -1.0f, 0.0f ) );
    result.halfDim = b.halfDim;
    result.point_index = getBestPoint( e, b.center );
    
    vec3 P = ( transform * Vec4( checkpoint.respawnPos, 1.0f ) ).xyz;
    result.respawn_index = getBestPoint( e, P );
    
    return result;
}

internal void
outputEntity_checkpoint( MEMORY * memory, EDITOR_STATE * editorState ) {
    if( editorState->nCheckpoint > 0 ) {
        writeSegment( memory, "[CHECKPOINT] {\n" );
        writeU32    ( memory, editorState->nCheckpoint, 1, true );
        for( uint32 iCheckpoint = 0; iCheckpoint < editorState->nCheckpoint; iCheckpoint++ ) {
            EDITOR__CHECKPOINT eCheckpoint = editorState->checkpoint[ iCheckpoint ];
            CHECKPOINT checkpoint = toCheckpoint( editorState, eCheckpoint );
            
            ORIENTED_BOX b = checkpoint.bound;
            writeV3( memory, b.center,  1, true );
            writeV3( memory, b.xAxis,   1, true );
            writeV3( memory, b.yAxis,   1, true );
            writeV3( memory, b.zAxis,   1, true );
            writeV3( memory, b.halfDim, 1, true );
            writeV3( memory, checkpoint.respawnPos, 1, true );
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal VIEW
genView( vec3 position ) {
    VIEW view = {};
    view.type = viewType_point;
    view.P    = position;
    
    view.lo = 1.0f;
    view.hi = 4.0f;
    
    return view;
}

internal VIEW
toView( EDITOR_STATE * e, EDITOR__VIEW eView ) {
    VIEW result = {};
    result.type = eView.type;
    result.P    = e->point[ eView.pIndex ];
    result.Q    = e->point[ eView.qIndex ];
    if( eView.type == viewType_box ) {
        InvalidCodePath;
    }
    
    result.lo       = eView.lo;
    result.hi       = eView.hi;
    result.maxDist  = eView.maxDist;
    result.hOffset  = eView.hOffset;
    result.vOffset  = eView.vOffset;
    result.fovAngle = eView.fovAngle;
    
    return result;
}

internal EDITOR__VIEW
toEditorView( EDITOR_STATE * e, VIEW view, mat4 transform ) {
    EDITOR__VIEW result = {};
    result.type   = view.type;
    
    vec3 P = ( transform * Vec4( view.P, 1.0f ) ).xyz;
    vec3 Q = ( transform * Vec4( view.Q, 1.0f ) ).xyz;
    
    result.pIndex = getBestPoint( e, P );
    result.qIndex = getBestPoint( e, Q );
    if( view.type == viewType_box ) {
        InvalidCodePath;  
    }
    
    result.lo       = view.lo;
    result.hi       = view.hi;
    result.maxDist  = view.maxDist;
    result.hOffset  = view.hOffset;
    result.vOffset  = view.vOffset;
    result.fovAngle = view.fovAngle;
    
    return result;
}

internal void
outputEntity_view( MEMORY * memory, EDITOR_STATE * editorState ) {
    if( editorState->nView > 0 ) {
        writeSegment( memory, "[VIEW] {\n" );
        writeU32    ( memory, editorState->nView, 1, true );
        for( uint32 iView = 0; iView < editorState->nView; iView++ ) {
            EDITOR__VIEW eView = editorState->view[ iView ];
            VIEW view = toView( editorState, eView );
            
            writeU32( memory, view.type, 1, true );
            writeV3 ( memory, view.P,    1, true );
            writeV3 ( memory, view.Q,    1, true );
            
            ORIENTED_BOX b = view.b;
            writeV3( memory, b.center,  1, true );
            writeV3( memory, b.xAxis,   1, true );
            writeV3( memory, b.yAxis,   1, true );
            writeV3( memory, b.zAxis,   1, true );
            writeV3( memory, b.halfDim, 1, true );
            
            writeF32( memory, view.lo,       1, true );
            writeF32( memory, view.hi,       1, true );
            writeF32( memory, view.maxDist,  1, true );
            writeF32( memory, view.hOffset,  1, true );
            writeF32( memory, view.vOffset,  1, true );
            writeF32( memory, view.fovAngle, 1, true );
            writeSegment( memory, "\n" );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal EDITOR__LEDGE
toEditorLedge( EDITOR_STATE * e, LEDGE ledge, mat4 transform ) {
    EDITOR__LEDGE result = {};
    vec3 P = ( transform * Vec4( ledge.point[ 0 ], 1.0f ) ).xyz;
    vec3 Q = ( transform * Vec4( ledge.point[ 1 ], 1.0f ) ).xyz;
    result.point_index[ 0 ] = getBestPoint( e, P );
    result.point_index[ 1 ] = getBestPoint( e, Q );
    return result;
}

#define COLOR_TABLE__COLOR_COUNT  ( 32 )
struct COLOR_TABLE {
    uint32 nColor;
    uint32  color[ COLOR_TABLE__COLOR_COUNT ];
};

internal COLOR_TABLE
genColorTable( EDITOR_STATE * editorState ) {
    COLOR_TABLE result = {};
    for( int32 iFace = 0; iFace < editorState->face_count; iFace++ ) {
        uint32 color = editorState->face_color[ iFace ];
        
        int32 index = -1;
        for( uint32 iColor = 0; ( index == -1 ) && ( iColor < result.nColor ); iColor++ ) {
            if( result.color[ iColor ] == color ) {
                index = iColor;
            }
        }
        
        if( index == -1 ) {
            Assert( result.nColor < COLOR_TABLE__COLOR_COUNT );
            result.color[ result.nColor++ ] = color;
        }
    }
    return result;
}

internal void
outputEntity_color( MEMORY * memory, EDITOR_STATE * editorState ) {
    if( editorState->face_count > 0 ) {
        writeSegment( memory, "[COLOR] {\n" );
        
        COLOR_TABLE colorTable = genColorTable( editorState );
        
        uint32 adjFaceCount = 0;
        for( int32 iFace = 0; iFace < editorState->face_count; iFace++ ) {
            if( isQuad( editorState->face[ iFace ] ) ) {
                adjFaceCount += 2;
            } else {
                adjFaceCount += 1;
            }
        }
        
        writeU32( memory, colorTable.nColor, 1, false );
        writeSegment( memory, " " );
        writeU32( memory, adjFaceCount, 0, true );
        
        for( uint32 iColor = 0; iColor < colorTable.nColor; iColor++ ) {
            uint32 color = colorTable.color[ iColor ];
            uint32 nFace0 = 0;
            for( int32 iFace = 0; iFace < editorState->face_count; iFace++ ) {
                if( editorState->face_color[ iFace ] == color ) {
                    if( isQuad( editorState->face[ iFace ] ) ) {
                        nFace0 += 2;
                    } else {
                        nFace0 += 1;
                    }
                }
            }
            
            writeU32Hex ( memory, color, 1, false );
            writeSegment( memory, " " );
            writeU32    ( memory, nFace0, 0, true );
            
            writeSegment( memory, "   " );
            
            uint32 nFace1 = 0;
            for( int32 iFace = 0; iFace < editorState->face_count; iFace++ ) {
                if( editorState->face_color[ iFace ] == color ) {
                    writeU32( memory, nFace1 );
                    writeSegment( memory, " " );
                    nFace1++;
                    
                    if( isQuad( editorState->face[ iFace ] ) ) {
                        writeU32( memory, nFace1 );
                        writeSegment( memory, " " );
                        nFace1++;
                    }
                }
            }
            writeSegment( memory, "\n" );
            
            Assert( nFace0 == nFace1 );
        }
        writeSegment( memory, "}\n\n" );
    }
}

internal void
outputEntity_end( MEMORY * memory, uint32 entityID ) {
    char entity_filePath[ 128 ] = {};
    sprintf( entity_filePath, "../../art/entity/ENTITY_%04d.ent", entityID );
    writeFile( entity_filePath, getSnapBase( memory ), ( uint32 )getSnapUsed( memory ) );
    _popSize( memory, ( uint32 )getSnapUsed( memory ) );
}

internal int32
getBestDoor( EDITOR_STATE * e, RAY select ) {
    int32 result = -1;
    
    RAY_INTERSECT best = {};
    best.t = 1000000.0f;
    
    for( uint32 iDoor = 0; iDoor < e->nSecureDoor; iDoor++ ) {
        EDITOR__SECURE_DOOR eDoor = e->secureDoor[ iDoor ];
        SECURE_DOOR door = toSecureDoor( e, eDoor );
        
        RAY_INTERSECT i = doesIntersectBox( select.origin, select.vector, door.collision_bound );
        if( ( i.isValid ) && ( i.t < best.t ) ) {
            result = iDoor;
            best   = i;
        }
    }
    
    return result;
}

internal void
processFloatInput( KEYBOARD_STATE * keyboard, INPUT_STRING * i ) {
    for( uint32 iNum = 0; iNum < 10; iNum++ ) {
        if( wasPressed( keyboard, ( KEY_STATE_CODE )( keyCode_0 + iNum ) ) ) {
            addChar( i, '0' + ( char )iNum );
        }
    }
    if( wasPressed( keyboard, keyCode_dash ) ) {
        addChar( i, '-' );
    }
    if( wasPressed( keyboard, keyCode_period ) ) {
        addChar( i, '.' );
    }
    if( wasPressed( keyboard, keyCode_backspace ) ) {
        deleteChar_backspace( i );
    }
}

internal void
processUIntInput( KEYBOARD_STATE * keyboard, INPUT_STRING * i ) {
    for( uint32 iNum = 0; iNum < 10; iNum++ ) {
        if( wasPressed( keyboard, ( KEY_STATE_CODE )( keyCode_0 + iNum ) ) ) {
            addChar( i, '0' + ( char )iNum );
        }
    }
    if( wasPressed( keyboard, keyCode_backspace ) ) {
        deleteChar_backspace( i );
    }
}

internal void
drawNumBox( RENDERER * renderer, RENDER_PASS_ID passID, rect bound, uint32 value, vec4 color ) {
    drawRect( renderer, passID, bound, color );
    drawRectOutline( renderer, passID, bound, COLOR_BLACK );
    
    char str[ 8 ] = {};
    sprintf( str, "%u", value );
    drawString( renderer, passID, assetID_font_default, str, getCenter( bound ), ALIGN_CENTER, COLOR_BLACK );
}

internal flo32
checkInputOrGridForScale( INPUT_STRING * i, UTILITY_PANEL * u, flo32 t ) {
    flo32 result = t;
    if( i->count > 0 ) {
        result = strtof( i->string, 0 );
    } else if( u->grid_isActive ) {
        result = roundTo( result, u->scale );
    }
    return result;
}

internal flo32
checkInputOrGridForRadians( INPUT_STRING * i, UTILITY_PANEL * u, flo32 radians ) {
    flo32 result = radians;
    if( i->count > 0 ) {
        result = toRadians( strtof( i->string, 0 ) );
    } else if( u->grid_isActive ) {
        // grid rotate snap is in degrees, not radians
        result = roundTo( radians, toRadians( u->degrees ) );
    }
    return result;
}

internal void
drawPixie( RENDERER * renderer, RENDER_PASS_ID passID, PIXIE pixie ) {
    vec4 color_array[ 4 ] = PIXIE__COLOR_HI__ARRAY;
    quat xOrient = Quat( Vec3( 1.0f, 0.0f, 0.0f ), pixie.xRadians );
    quat yOrient = Quat( Vec3( 0.0f, 1.0f, 0.0f ), pixie.yRadians );
    quat zOrient = Quat( Vec3( 0.0f, 0.0f, 1.0f ), pixie.zRadians );
    quat orientation = zOrient * yOrient * xOrient;
    mat3 transform = mat3_toRotation( orientation );
    
    ORIENTED_BOX bound = {};
    bound.center  = pixie.position;
    bound.xAxis   = transform.column[ 0 ];
    bound.yAxis   = transform.column[ 1 ];
    bound.zAxis   = transform.column[ 2 ];
    bound.halfDim = Vec3( 0.1f, 0.1f, 0.1f );
    
    drawBox       ( renderer, passID, bound, color_array[ pixie.typeID ] );
    drawBoxOutline( renderer, passID, pixie.pickUp_bound, COLOR_CYAN );
}

internal void
drawPixie( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position ) {
    PIXIE pixie = genPixie( position );
    drawPixie( renderer, passID, pixie );
}

internal int32
getFirstPixie( EDITOR_STATE * editorState, RAY select ) {
    int32 result = -1;
    
    RAY_INTERSECT best = {};
    best.t = 1000000.0f;
    for( uint32 iPixie = 0; iPixie < editorState->nPixie; iPixie++ ) {
        EDITOR__PIXIE ePixie = editorState->pixie[ iPixie ];
        PIXIE          pixie = toPixie( editorState, ePixie );
        RAY_INTERSECT intersect = doesIntersectBox( pixie.pickUp_bound, select );
        if( ( intersect.isValid ) && ( intersect.t < best.t ) ) {
            best   = intersect;
            result = iPixie;
        }
    }
    return result;
}

internal int32
addPixie( EDITOR_STATE * editorState, EDITOR__PIXIE ePixie ) {
    Assert( editorState->nPixie < EDITOR__PIXIE_COUNT );
    int32 result = editorState->nPixie;
    editorState->pixie[ editorState->nPixie++ ] = ePixie;
    return result;
}

internal int32
addPixie( EDITOR_STATE * e, int32 point_index ) {
    Assert( point_index > -1 );
    
    EDITOR__PIXIE ePixie = {};
    ePixie.typeID = pixieTypeID_grow;
    ePixie.point_index = point_index;
    int32 result = addPixie( e, ePixie );
    return result;
}

internal void
deletePixie( EDITOR_STATE * e, uint32 pixie_index ) {
    if( ( pixie_index >= 0 ) && ( pixie_index < e->nPixie ) ) {
        e->pixie[ pixie_index ] = e->pixie[ --e->nPixie ];
    }
}

internal void
drawKillZone( RENDERER * renderer, RENDER_PASS_ID passID, KILL_ZONE killZone ) {
    drawBoxOutline( renderer, passID, killZone.bound, COLOR_CYAN );
}

internal void
drawKillZone( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position ) {
    KILL_ZONE killZone = genKillZone( position );
    drawKillZone( renderer, passID, killZone );
}

internal int32
getFirstKillZone( EDITOR_STATE * editorState, RAY select ) {
    int32 result = -1;
    
    RAY_INTERSECT best = {};
    best.t = 1000000.0f;
    for( uint32 iKillZone = 0; iKillZone < editorState->nKillZone; iKillZone++ ) {
        EDITOR__KILL_ZONE eKillZone = editorState->killZone[ iKillZone ];
        KILL_ZONE          killZone = toKillZone( editorState, eKillZone );
        RAY_INTERSECT intersect = doesIntersectBox( killZone.bound, select );
        if( ( intersect.isValid ) && ( intersect.t < best.t ) ) {
            best   = intersect;
            result = iKillZone;
        }
    }
    return result;
}

internal int32
addKillZone( EDITOR_STATE * editorState, EDITOR__KILL_ZONE eKillZone ) {
    Assert( editorState->nKillZone < EDITOR__KILL_ZONE_COUNT );
    int32 result = editorState->nKillZone;
    editorState->killZone[ editorState->nKillZone++ ] = eKillZone;
    return result;
}

internal int32
addKillZone( EDITOR_STATE * e, int32 point_index ) {
    Assert( point_index > -1 );
    
    EDITOR__KILL_ZONE eKillZone = {};
    eKillZone.point_index = point_index;
    eKillZone.halfDim     = Vec3( 1.0f, 1.0f, 1.0f );
    eKillZone.radians     = 0.0f;
    
    int32 result = addKillZone( e, eKillZone );
    return result;
}

internal void
deleteKillZone( EDITOR_STATE * e, uint32 killZone_index ) {
    if( ( killZone_index >= 0 ) && ( killZone_index < e->nKillZone ) ) {
        e->killZone[ killZone_index ] = e->killZone[ --e->nKillZone ];
    }
}

internal void
drawCheckpoint( RENDERER * renderer, RENDER_PASS_ID passID, CHECKPOINT checkpoint ) {
    drawBoxOutline( renderer, passID, checkpoint.bound, COLOR_CYAN );
}

internal void
drawCheckpoint( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position ) {
    CHECKPOINT checkpoint = genCheckpoint( position );
    drawCheckpoint( renderer, passID, checkpoint );
}

internal int32
getFirstCheckpoint( EDITOR_STATE * editorState, RAY select ) {
    int32 result = -1;
    
    RAY_INTERSECT best = {};
    best.t = 1000000.0f;
    for( uint32 iCheckpoint = 0; iCheckpoint < editorState->nCheckpoint; iCheckpoint++ ) {
        EDITOR__CHECKPOINT eCheckpoint = editorState->checkpoint[ iCheckpoint ];
        CHECKPOINT          checkpoint = toCheckpoint( editorState, eCheckpoint );
        RAY_INTERSECT intersect = doesIntersectBox( checkpoint.bound, select );
        if( ( intersect.isValid ) && ( intersect.t < best.t ) ) {
            best   = intersect;
            result = iCheckpoint;
        }
    }
    return result;
}

internal int32
addCheckpoint( EDITOR_STATE * editorState, EDITOR__CHECKPOINT eCheckpoint ) {
    Assert( editorState->nCheckpoint < EDITOR__CHECKPOINT_COUNT );
    int32 result = editorState->nCheckpoint;
    editorState->checkpoint[ editorState->nCheckpoint++ ] = eCheckpoint;
    return result;
}

internal int32
addCheckpoint( EDITOR_STATE * e, int32 point_index ) {
    Assert( point_index > -1 );
    
    EDITOR__CHECKPOINT eCheckpoint = {};
    eCheckpoint.point_index = point_index;
    eCheckpoint.halfDim     = Vec3( 1.0f, 1.0f, 1.0f );
    eCheckpoint.radians     = 0.0f;
    eCheckpoint.respawn_index = point_index;
    
    int32 result = addCheckpoint( e, eCheckpoint );
    return result;
}

internal void
deleteCheckpoint( EDITOR_STATE * e, uint32 checkpoint_index ) {
    if( ( checkpoint_index >= 0 ) && ( checkpoint_index < e->nCheckpoint ) ) {
        e->checkpoint[ checkpoint_index ] = e->checkpoint[ --e->nCheckpoint ];
    }
}

internal void
drawView( RENDERER * renderer, RENDER_PASS_ID passID, VIEW view, boo32 isSelected ) {
    vec4 color0 = COLOR_MAGENTA;
    vec4 color1 = Vec4( 0.5f, 0.0f, 0.5f, 1.0f );
    if( isSelected ) {
        color0 = COLOR_YELLOW;
        color1 = Vec4( 0.5f, 0.5f, 0.0f, 1.0f );
    }
    
    if( view.type == viewType_point ) {
        //drawSphereOutline( renderer, passID, Sphere( view.P, view.lo ), color0 );
        //drawSphereOutline( renderer, passID, Sphere( view.P, view.hi ), color1 );
    } else if( view.type == viewType_line ) {
        //drawCapsuleOutline( renderer, passID, Capsule( view.P, view.Q, view.lo ), color0 );
        //drawCapsuleOutline( renderer, passID, Capsule( view.P, view.Q, view.hi ), color1 );
    }
}

internal void
drawView( RENDERER * renderer, RENDER_PASS_ID passID, vec3 position ) {
    VIEW view = genView( position );
    drawView( renderer, passID, view, false );
}

internal int32
getFirstView( EDITOR_STATE * editorState, RAY select ) {
    int32 result = -1;
    
    RAY_INTERSECT best = {};
    best.t = 1000000.0f;
    for( uint32 iView = 0; iView < editorState->nView; iView++ ) {
        EDITOR__VIEW eView = editorState->view[ iView ];
        VIEW          view = toView( editorState, eView );
        RAY_INTERSECT intersect = doesIntersectSphere( Sphere( view.P, view.hi ), select );
        if( ( intersect.isValid ) && ( intersect.t < best.t ) ) {
            best   = intersect;
            result = iView;
        }
    }
    return result;
}

internal int32
addView( EDITOR_STATE * editorState, EDITOR__VIEW eView ) {
    Assert( editorState->nView < EDITOR__VIEW_COUNT );
    int32 result = editorState->nView;
    editorState->view[ editorState->nView++ ] = eView;
    return result;
}

internal int32
addView( EDITOR_STATE * e, int32 point_index ) {
    Assert( point_index > -1 );
    
    EDITOR__VIEW eView = {};
    eView.type    = viewType_point;
    eView.pIndex  = point_index;
    eView.lo      = 1.0f;
    eView.hi      = 4.0f;
    eView.hOffset = 0.0f;
    eView.vOffset = 0.0f;
    eView.maxDist = 8.0f;
    eView.fovAngle = ( PI * 0.3333f );
    
    int32 result = addView( e, eView );
    return result;
}

internal void
deleteView( EDITOR_STATE * e, uint32 view_index ) {
    if( ( view_index >= 0 ) && ( view_index < e->nView ) ) {
        e->view[ view_index ] = e->view[ --e->nView ];
    }
}

internal void
updateEntityPanel( EDITOR_STATE * editorState, MOUSE_STATE * mouse ) {
    PROFILE_FUNCTION();
    
    ENTITY_PANEL * e = &editorState->entityPanel;
    
    e->hover = -1;
    if( isInBound( mouse->position, e->bound ) ) {
        for( int32 iButton = 0; iButton < e->nButton; iButton++ ) {
            if( isInBound( mouse->position, e->button_bound[ iButton ] ) ) {
                e->hover = iButton;
            }
        }
        
        if( wasPressed( mouse, mouseButton_left ) ) {
            if( e->hover == e->active ) {
                e->active = -1;
                editorState->entity_select = -1;
                editorState->editorMode = editorMode_default;
            } else if( e->hover != -1 ) {
                e->active = e->hover;
                editorState->entity_select = -1;
                editorState->entityMode = ( ENTITY_MODE )e->active;
                editorState->editorMode = editorMode_placeEntity;
            }
        }
    }
}

internal void
selectRange( boo32 * isSelected, uint32 bIndex, uint32 nIndex ) {
    memset( isSelected + bIndex, 1, sizeof( boo32 ) * nIndex );
}

internal void
deselectRange( boo32 * isSelected, uint32 bIndex, uint32 nIndex ) {
    memset( isSelected + bIndex, 0, sizeof( boo32 ) * nIndex );
}

internal void
updateLevelPanel( EDITOR_STATE * e, KEYBOARD_STATE * keyboard, MOUSE_STATE * mouse ) {
    if( e->levelEditor_isActive ) {
        LEVEL_PANEL * l = &e->levelPanel;
        
        int32 hover = -1;
        vec2  atPos = getTL( l->bound );
        
        rect addButton_bound = addRadius( rectTLD( atPos, l->addButton_dim ), -2.0f );
        atPos.y -= l->addButton_dim.y;
        
        for( uint32 iButton = 0; iButton < l->nLevel; iButton++ ) {
            rect bound = addRadius( rectTLD( atPos, l->entry_dim ), -1.0f );
            if( isInBound( mouse->position, bound ) ) {
                hover = ( int32 )iButton;
            }
            atPos.y -= l->entry_dim.y;
        }
        
        // DISPLAY_VALUE( int32, hover );
        if( wasPressed( mouse, mouseButton_left ) ) {
            if( isInBound( mouse->position, addButton_bound ) ) {
                e->editorMode = editorMode_addLevel;
                init( &e->input_string, 4 );
            }
            if( hover != -1 ) {
                if( !( keyboard->flags & KEYBOARD_FLAGS__SHIFT ) ) {
                    memset( l->level_isSelected, 0, sizeof( boo32 ) * l->nLevel );
                    clearAllSelected( e );
                }
                LEVEL level = l->level[ hover ];
                
                l->level_isSelected[ hover ] = !l->level_isSelected[ hover ];
                if( l->level_isSelected[ hover ] ) {
                    selectRange( e->point_isSelected, level.bPoint, level.nPoint );
                    selectRange( e->face_isSelected,  level.bFace,  level.nFace  );
                } else {
                    deselectRange( e->point_isSelected, level.bPoint, level.nPoint );
                    deselectRange( e->face_isSelected,  level.bFace,  level.nFace  );
                }
            }
        }
    }
}

internal boo32
facesAreAdjacent( FACE f0, FACE f1 ) {
    uint32 nShared = 0;
    uint32 count0 = getElemCount( f0 );
    uint32 count1 = getElemCount( f1 );
    for( uint32 e0 = 0; e0 < count0; e0++ ) {
        for( uint32 e1 = 0; e1 < count1; e1++ ) {
            if( f0.index[ e0 ] == f1.index[ e1 ] ) {
                nShared++;
            }
        }
    }
    Assert( nShared <= 2 );
    
    boo32 result = ( nShared == 2 );
    return result;
}

internal void
selectAllConnectedToSelection( EDITOR_STATE * e, MEMORY * memory ) {
    SnapMemory( memory );
    
    if( e->selectMode == selectMode_face ) {
        QUEUE  _queue = Queue( memory, e->face_count );
        QUEUE * queue = &_queue;
        
        for( int32 iFace = 0; iFace < e->face_count; iFace++ ) {
            if( e->face_isSelected[ iFace ] ) {
                enqueue( queue, iFace );
            }
        }
        
        while( !queue->isEmpty ) {
            uint32 i = dequeue( queue );
            
            FACE f0 = e->face[ i ];
            for( uint32 iter = 0; iter < ( uint32 )e->face_count; iter++ ) {
                if( ( !e->face_isSelected[ iter ] ) && ( i != iter ) ) {
                    FACE f1 = e->face[ iter ];
                    if( facesAreAdjacent( f0, f1 ) ) {
                        e->face_isSelected[ iter ] = true;
                        enqueue( queue, iter );
                    }
                }
            }
        }
        
        updateSelectedVertices( e );
        popToSnap( memory );
    }
}

internal boo32
isValidLedge( EDITOR_STATE * e, EDGE__ edge ) {
    vec3 P0 = e->point[ edge.index[ 0 ] ];
    vec3 P1 = e->point[ edge.index[ 1 ] ];
    vec3 V = getNormal( P1 - P0 );
    
    flo32 dSq = getLengthSq( V );
    flo32 dY  = fabsf( dot( V, Vec3( 0.0f, 1.0f, 0.0f ) ) );
    boo32 result = ( dSq >= DOT__DEGENERATE_EPSILON ) && ( dY <= ( 1.0f - DOT__COLLINEAR_EPSILON ) );
    return result;
}

internal void
drawLedge( RENDERER * renderer, RENDER_PASS_ID passID, EDITOR_STATE * e, EDITOR__LEDGE ledge ) {
    ORIENTED_BOX b = toOBox( e, ledge );
    drawBoxOutline( renderer, passID, b, COLOR_MAGENTA );
}

struct STRING_BUFFER {
    uint32 nStr;
    uint32 nStrMax;
    char ** str;
    MEMORY  memory;
};

internal STRING_BUFFER
StringBuffer( MEMORY * parent_memory, uint32 maxString, uint32 maxSize ) {
    STRING_BUFFER result = {};
    result.str     = _pushArray_clear( parent_memory, char *, maxString );
    result.memory  = subArena( parent_memory, maxSize );
    result.nStrMax = maxString;
    return result;
}

internal void
add( STRING_BUFFER * s, char * string ) {
    Assert( s->nStr < s->nStrMax );
    
    uint32 length = ( uint32 )strlen( string );
    char * dest = ( char * )_pushSize( &s->memory, length + 1 );
    strcpy( dest, string );
    
    s->str[ s->nStr++ ] = dest;
}

internal char *
getFileExtension( char * filename ) {
    char * ptr = filename;
    while( ptr[ 0 ]        ) { ptr++; }
    while( ptr[ 0 ] != '.' ) { ptr--; }
    char * result = ptr + 1;
    return result;
}

internal boo32
fileExtensionIs( char * filename, char * extension ) {
    char * fExt = getFileExtension( filename );
    boo32 result = matchString( fExt, extension );
    return result;
}

internal boo32
prefixIs( char * filename, char * prefix ) {
    boo32 result = matchSegment( filename, prefix, ( uint32 )strlen( prefix ) );
    return result;
}

internal void
getListOfMeshFiles( STRING_BUFFER * s ) {
    WIN32_FIND_DATA file_data = {};
    
    SetCurrentDirectory( "../../art/mesh/" );
    
    HANDLE handle = FindFirstFile( "*", &file_data );
    FindNextFile( handle, &file_data );
    
    while( FindNextFile( handle, &file_data ) ) {
        boo32 fExt_isValid   = fileExtensionIs( file_data.cFileName, "x" );
        boo32 prefix_isValid = prefixIs       ( file_data.cFileName, "MESH_" );
        if( fExt_isValid && prefix_isValid ) {
            add( s, file_data.cFileName );
        }
    }
}

internal boo32
containsSubString( char * str, char * sub ) {
    boo32 result = false;
    
    uint32 length0 = ( uint32 )strlen( str );
    uint32 length1 = ( uint32 )strlen( sub );
    if( length1 < length0 ) {
        uint32 left = length0 - length1;
        for( uint32 iSub = 0; ( !result ) && ( iSub < left ); iSub++ ) {
            if( matchSegment( str + iSub, sub, length1 ) ) {
                result = true;
            }
        }
    }
    
    return result;
}

internal uint32
filterStringsByInput( STRING_BUFFER * s, INPUT_STRING * i, uint32 * output ) {
    uint32 result = 0;
    
    if( i->count > 0 ) {
        for( uint32 iString = 0; iString < s->nStr; iString++ ) {
            if( containsSubString( s->str[ iString ], i->string ) ) {
                output[ result++ ] = iString;
            }
        }
    } else {
        for( uint32 iter = 0; iter < s->nStr; iter++ ) {
            output[ result++ ] = iter;
        }
    }
    return result;
}

internal void
resetEditorState( EDITOR_STATE * e ) {
    memset( e->point_isSelected,    0, sizeof( boo32 ) * EDITOR__POINT_COUNT    );
    memset( e->face_isSelected,     0, sizeof( boo32 ) * EDITOR__FACE_COUNT     );
    memset( e->overhang_isSelected, 0, sizeof( boo32 ) * EDITOR__OVERHANG_COUNT );
    memset( e->pole_isSelected,     0, sizeof( boo32 ) * EDITOR__POLE_COUNT     );
    
    e->edge_count = 0;
    
    e->entity_select = -1;
    
    e->levelPanel.nLevel = 0;
    
    e->point_count    = 0;
    e->face_count     = 0;
    e->overhang_count = 0;
    e->nLedge         = 0;
    e->pole_count     = 0;
    e->nSecureDoor    = 0;
    e->nPixie         = 0;
    e->nKillZone      = 0;
    e->nCheckpoint    = 0;
    e->nView          = 0;
}

internal void
drawU32( RENDERER * renderer, RENDER_PASS_ID passID, uint32 value, vec2 pos, TEXT_ALIGNMENT align, vec4 color ) {
    char str[ 32 ] = {};
    sprintf( str, "%u", value );
    drawString( renderer, passID, assetID_font_default, str, pos, align, color );
}