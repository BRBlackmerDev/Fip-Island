

struct READ_FILE_RESULT {
    uint32 size;
    void * contents;
};

internal READ_FILE_RESULT
readFile( MEMORY * memory, const char * filename ) {
    READ_FILE_RESULT result = {};
    
    HANDLE file_handle = CreateFile( filename, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    if( file_handle == INVALID_HANDLE_VALUE ) {
        // TODO: logger
        char error_string[ 256 ] = {};
        sprintf( error_string, "ERROR! Unable to open file for reading: %s\n", filename );
        OutputDebugString( error_string );
    } else {
        uint32  file_size = ( uint32 )GetFileSize( file_handle, 0 );
        uint8 * file_data = ( uint8 * )_pushSize( memory, file_size );
        
        DWORD file_bytesRead = 0;
        boo32 file_success   = ReadFile( file_handle, file_data, file_size, &file_bytesRead, 0 );
        if( file_success ) {
            result.size     = file_size;
            result.contents = file_data;
            CloseHandle( file_handle );
        } else {
            // TODO: logger
            char error_string[ 256 ] = {};
            sprintf( error_string, "ERROR! Error reading file: %s\n", filename );
            OutputDebugString( error_string );
        }
    }
    
    return result;
}

internal boo32
writeFile( const char * filename, void * data, uint32 size ) {
	boo32 result = false;
    
	HANDLE file_handle = CreateFile( filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
	if( file_handle == INVALID_HANDLE_VALUE ) {
        // TODO: logger
        char error_string[ 256 ] = {};
        sprintf( error_string, "ERROR! Unable to open file for writing: %s\n", filename );
        OutputDebugString( error_string );
    }
    
    DWORD file_bytesWritten = 0;
    boo32 file_success      = WriteFile( file_handle, data, size, &file_bytesWritten, 0 );
    if( file_success ) {
        result = true;
		CloseHandle( file_handle );
    } else {
        // TODO: logger
        char error_string[ 256 ] = {};
        sprintf( error_string, "ERROR! Error writing file: %s\n", filename );
        OutputDebugString( error_string );
    }
    
	return result;
}

struct X_FILE_DATA {
    vec3 * position;
    uint32 position_count;
    
    uint32 * face;
    uint32   face_count;
};

struct EDGE__ {
    int32 index[ 2 ];
};

internal EDGE__
Edge__( int32 index0, int32 index1 ) {
    Assert( index0 != index1 );
    
    EDGE__ result = {};
    result.index[ 0 ] = ( index0 < index1 ) ? index0 : index1;
    result.index[ 1 ] = ( index0 < index1 ) ? index1 : index0;
    return result;
}

struct FACE {
    int32 index[ 4 ];
};

internal FACE
Face( int32 index0, int32 index1, int32 index2, int32 index3 = -1 ) {
    FACE result = {};
    result.index[ 0 ] = index0;
    result.index[ 1 ] = index1;
    result.index[ 2 ] = index2;
    result.index[ 3 ] = index3;
    return result;
}

struct X_FILE_DATA__QUAD {
    vec3 * position;
    uint32 position_count;
    
    FACE   * face;
    uint32   face_count;
};

struct TEX_FILE_DATA {
    vec2 * texCoord;
    uint32 texCoord_count;
    boo32 texFileExists;
    boo32 texDataExistsInXFile;
};

internal int32
getDataSize( X_FILE_DATA file_data ) {
    int32 result = ( sizeof( vec3 ) * file_data.position_count + sizeof( uint32 ) * file_data.face_count * 3 );
    return result;
}

internal int32
getDataSize( X_FILE_DATA__QUAD file_data ) {
    int32 result = ( sizeof( vec3 ) * file_data.position_count + sizeof( FACE ) * file_data.face_count );
    return result;
}

internal READ_FILE_RESULT
readXFile( MEMORY * memory, uint32 entityID ) {
    char filename[ 128 ] = {};
    sprintf( filename, "../../art/mesh/MESH_%04u.x", entityID );
    
    READ_FILE_RESULT result = readFile( memory, filename );
    return result;
}

internal READ_FILE_RESULT
readEntityFile( MEMORY * memory, uint32 entityID ) {
    char filename[ 128 ] = {};
    sprintf( filename, "../../art/entity/ENTITY_%04u.ent", entityID );
    
    READ_FILE_RESULT result = readFile( memory, filename );
    return result;
}

internal X_FILE_DATA
parseXFile( MEMORY * memory, READ_FILE_RESULT file_mesh, TEX_FILE_DATA * texFile_data ) {
    FILE_PARSER  _parser = FileParser( file_mesh.contents, file_mesh.size );
    FILE_PARSER * parser = &_parser;
    
    // verify header
    // TODO: better header verification
    Assert( file_mesh.size > 16 );
    if( ( parser->at[ 0 ] == 'x' ) && ( parser->at[ 1 ] == 'o' ) && ( parser->at[ 2 ] == 'f' ) && ( parser->at[ 3 ] == ' ' ) ) { parser->at += 4; } else { InvalidCodePath; }
    if( ( parser->at[ 0 ] == '0' ) && ( parser->at[ 1 ] == '3' ) && ( parser->at[ 2 ] == '0' ) && ( parser->at[ 3 ] == '3' ) ) { parser->at += 4; } else { InvalidCodePath; }
    if( ( parser->at[ 0 ] == 't' ) && ( parser->at[ 1 ] == 'x' ) && ( parser->at[ 2 ] == 't' ) && ( parser->at[ 3 ] == ' ' ) ) { parser->at += 4; } else { InvalidCodePath; }
    if( ( parser->at[ 0 ] == '0' ) && ( parser->at[ 1 ] == '0' ) && ( parser->at[ 2 ] == '3' ) && ( parser->at[ 3 ] == '2' ) ) { parser->at += 4; } else { InvalidCodePath; }
    
    eatWhiteSpace( parser );
    
    // clear comments
    // clear token separators ',' ';'
    char * start = parser->start;
    char * temp  = parser->at;
    boo32 isComment = false;
    while( ( temp - start ) < parser->size ) {
        if( isComment ) {
            if( temp[ 0 ] == '\n' ) {
                isComment = false;
            } else {
                temp[ 0 ] = ' ';
            }
        } else {
            if(   ( temp[ 0 ] == '#' ) || 
               ( ( temp[ 0 ] == '/' ) && ( ( temp - start ) < ( parser->size - 1 ) ) && ( temp[ 1 ] == '/' ) ) ) {
                isComment = true;  
                temp[ 0 ] = ' ';
            }
            if( ( temp[ 0 ] == ';' ) || ( temp[ 0 ] == ',' ) ) {
                temp[ 0 ] = ' ';
            }
        }
        temp++;
    }
    
    X_FILE_DATA result = {};
    
    // parse file
    // TODO: better error checking (like, actually check for errors)
    uint32 position_count = 0;
    
    int32 nest_level = 0;
    mat4  transform  = mat4_identity();
    while( hasTextRemaining( parser ) ) {
        char * token0 = parseToken( parser );
        if( matchString( token0, "Frame" ) ) {
            char * token1 = parseToken( parser );
            if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
            
            nest_level++;
        } else if( matchString( token0, "FrameTransformMatrix" ) ) {
            char * token1 = parseToken( parser );
            if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
            
            mat4 temp_transform = {};
            for( int32 elem_index = 0; elem_index < 16; elem_index++ ) {
                flo32 value = parseF32( parser );
                temp_transform.elem[ elem_index ] = value;
            }
            
            transform = transform * temp_transform;
            
            char * end_token = parseToken( parser );
            Assert( end_token[ 0 ] == '}' );
        } else if( matchString( token0, "Mesh" ) ) {
            char * token1 = parseToken( parser );
            if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
            
            position_count  = parseU32( parser );
            vec3 * position = _pushArray_clear( memory, vec3, position_count );
            
            for( uint32 position_index = 0; position_index < position_count; position_index++ ) {
                flo32 x = parseF32( parser );
                flo32 y = parseF32( parser );
                flo32 z = parseF32( parser );
                
                vec3 pos = ( transform * Vec4( x, y, z, 1.0f ) ).xyz;
                position[ position_index ] = pos;
            }
            
            result.position       = position;
            result.position_count = position_count;
            
            uint32 face_count = parseU32( parser );
            
            uint32 face_counter   = 0;
            uint32 face_tempTotal = face_count * 2;
            
            uint32 * face = _pushArray_clear( memory, uint32, face_tempTotal * 3 );
            for( uint32 face_index = 0; face_index < face_count; face_index++ ) {
                uint32 elem_count = parseU32( parser );
                if( elem_count == 3 ) {
                    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
                        face[ face_counter * 3 + elem_index ] = parseU32( parser );
                    }
                    
                    face_counter++;
                } else if( elem_count == 4 ) {
                    uint32 elem[ 4 ] = {};
                    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
                        elem[ elem_index ] = parseU32( parser );
                    }
                    
                    face[ face_counter * 3 + 0 ] = elem[ 0 ];
                    face[ face_counter * 3 + 1 ] = elem[ 1 ];
                    face[ face_counter * 3 + 2 ] = elem[ 3 ];
                    
                    face[ face_counter * 3 + 3 ] = elem[ 2 ];
                    face[ face_counter * 3 + 4 ] = elem[ 3 ];
                    face[ face_counter * 3 + 5 ] = elem[ 1 ];
                    
                    face_counter += 2;
                } else {
                    uint32 elem[ 12 ] = {};
                    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
                        elem[ elem_index ] = parseU32( parser );
                    }
                    InvalidCodePath;
                }
            }
            
            uint32 pop_count = face_tempTotal - face_counter;
            _popArray( memory, uint32, pop_count * 3 );
            face_count = face_counter;
            
            result.face       = face;
            result.face_count = face_count;
        } else if( matchString( token0, "MeshTextureCoords" ) ) {
            if( texFile_data->texFileExists ) {
                char * token1 = parseToken( parser );
                if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
                
                uint32 texCoord_count = parseU32( parser );
                Assert( texCoord_count == position_count );
                
                vec2 * texCoord = _pushArray( memory, vec2, texCoord_count );
                for( uint32 texCoord_index = 0; texCoord_index < texCoord_count; texCoord_index++ ) {
                    flo32 u = parseF32( parser );
                    flo32 v = parseF32( parser );
                }
                
                char * end_token = parseToken( parser );
                Assert( end_token[ 0 ] == '}' );
            } else  {
                char * token1 = parseToken( parser );
                if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
                
                uint32 texCoord_count = parseU32( parser );
                Assert( texCoord_count == position_count );
                Assert( texCoord_count == ( result.face_count * 3 ) );
                Assert( position_count == ( result.face_count * 3 ) );
                
                vec2 * texCoord = _pushArray( memory, vec2, texCoord_count );
                for( uint32 texCoord_index = 0; texCoord_index < texCoord_count; texCoord_index++ ) {
                    flo32 u = parseF32( parser );
                    flo32 v = parseF32( parser );
                    texCoord[ texCoord_index ] = Vec2( u, v );
                }
                
                texFile_data->texDataExistsInXFile = true;
                texFile_data->texCoord       = texCoord;
                texFile_data->texCoord_count = texCoord_count;
                
                char * end_token = parseToken( parser );
                Assert( end_token[ 0 ] == '}' );
            }
        } else if( matchString( token0, "}" ) ) {
            nest_level--;
        } else {
            InvalidCodePath;
        }
        uint32 breakHere = 10;
    }
    
    return result;
}

internal X_FILE_DATA__QUAD
parseXFile_quad( MEMORY * memory, READ_FILE_RESULT file_mesh, TEX_FILE_DATA * texFile_data ) {
    FILE_PARSER  _parser = FileParser( file_mesh.contents, file_mesh.size );
    FILE_PARSER * parser = &_parser;
    
    // verify header
    // TODO: better header verification
    Assert( file_mesh.size > 16 );
    if( ( parser->at[ 0 ] == 'x' ) && ( parser->at[ 1 ] == 'o' ) && ( parser->at[ 2 ] == 'f' ) && ( parser->at[ 3 ] == ' ' ) ) { parser->at += 4; } else { InvalidCodePath; }
    if( ( parser->at[ 0 ] == '0' ) && ( parser->at[ 1 ] == '3' ) && ( parser->at[ 2 ] == '0' ) && ( parser->at[ 3 ] == '3' ) ) { parser->at += 4; } else { InvalidCodePath; }
    if( ( parser->at[ 0 ] == 't' ) && ( parser->at[ 1 ] == 'x' ) && ( parser->at[ 2 ] == 't' ) && ( parser->at[ 3 ] == ' ' ) ) { parser->at += 4; } else { InvalidCodePath; }
    if( ( parser->at[ 0 ] == '0' ) && ( parser->at[ 1 ] == '0' ) && ( parser->at[ 2 ] == '3' ) && ( parser->at[ 3 ] == '2' ) ) { parser->at += 4; } else { InvalidCodePath; }
    
    eatWhiteSpace( parser );
    
    // clear comments
    // clear token separators ',' ';'
    char * start = parser->start;
    char * temp  = parser->at;
    boo32 isComment = false;
    while( ( temp - start ) < parser->size ) {
        if( isComment ) {
            if( temp[ 0 ] == '\n' ) {
                isComment = false;
            } else {
                temp[ 0 ] = ' ';
            }
        } else {
            if(   ( temp[ 0 ] == '#' ) || 
               ( ( temp[ 0 ] == '/' ) && ( ( temp - start ) < ( parser->size - 1 ) ) && ( temp[ 1 ] == '/' ) ) ) {
                isComment = true;  
                temp[ 0 ] = ' ';
            }
            if( ( temp[ 0 ] == ';' ) || ( temp[ 0 ] == ',' ) ) {
                temp[ 0 ] = ' ';
            }
        }
        temp++;
    }
    
    X_FILE_DATA__QUAD result = {};
    
    // parse file
    // TODO: better error checking (like, actually check for errors)
    uint32 position_count = 0;
    
    int32 nest_level = 0;
    mat4  transform  = mat4_identity();
    while( hasTextRemaining( parser ) ) {
        char * token0 = parseToken( parser );
        if( matchString( token0, "Frame" ) ) {
            char * token1 = parseToken( parser );
            if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
            
            nest_level++;
        } else if( matchString( token0, "FrameTransformMatrix" ) ) {
            char * token1 = parseToken( parser );
            if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
            
            mat4 temp_transform = {};
            for( int32 elem_index = 0; elem_index < 16; elem_index++ ) {
                flo32 value = parseF32( parser );
                temp_transform.elem[ elem_index ] = value;
            }
            
            transform = transform * temp_transform;
            
            char * end_token = parseToken( parser );
            Assert( end_token[ 0 ] == '}' );
        } else if( matchString( token0, "Mesh" ) ) {
            char * token1 = parseToken( parser );
            if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
            
            position_count  = parseU32( parser );
            vec3 * position = _pushArray_clear( memory, vec3, position_count );
            
            for( uint32 position_index = 0; position_index < position_count; position_index++ ) {
                flo32 x = parseF32( parser );
                flo32 y = parseF32( parser );
                flo32 z = parseF32( parser );
                
                vec3 pos = ( transform * Vec4( x, y, z, 1.0f ) ).xyz;
                position[ position_index ] = pos;
            }
            
            result.position       = position;
            result.position_count = position_count;
            
            uint32 face_count = parseU32( parser );
            FACE * face       = _pushArray_clear( memory, FACE, face_count );
            for( uint32 face_index = 0; face_index < face_count; face_index++ ) {
                uint32 elem_count = parseU32( parser );
                if( elem_count == 3 ) {
                    FACE face0 = {};
                    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
                        face0.index[ elem_index ] = parseU32( parser );
                    }
                    face0.index[ 3 ] = -1;
                    face[ face_index ] = face0;
                } else if( elem_count == 4 ) {
                    FACE face0 = {};
                    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
                        face0.index[ elem_index ] = parseU32( parser );
                    }
                    _swap( int32, face0.index[ 2 ], face0.index[ 3 ] );
                    face[ face_index ] = face0;
                } else {
                    uint32 elem[ 12 ] = {};
                    for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
                        elem[ elem_index ] = parseU32( parser );
                    }
                    InvalidCodePath;
                }
            }
            
            result.face       = face;
            result.face_count = face_count;
        } else if( matchString( token0, "MeshTextureCoords" ) ) {
            if( texFile_data->texFileExists ) {
                char * token1 = parseToken( parser );
                if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
                
                uint32 texCoord_count = parseU32( parser );
                Assert( texCoord_count == position_count );
                
                vec2 * texCoord = _pushArray( memory, vec2, texCoord_count );
                for( uint32 texCoord_index = 0; texCoord_index < texCoord_count; texCoord_index++ ) {
                    flo32 u = parseF32( parser );
                    flo32 v = parseF32( parser );
                }
                
                char * end_token = parseToken( parser );
                Assert( end_token[ 0 ] == '}' );
            } else  {
                char * token1 = parseToken( parser );
                if( token1[ 0 ] != '{' ) { token1 = parseToken( parser ); }
                
                uint32 texCoord_count = parseU32( parser );
                Assert( texCoord_count == position_count );
                Assert( texCoord_count == ( result.face_count * 3 ) );
                Assert( position_count == ( result.face_count * 3 ) );
                
                vec2 * texCoord = _pushArray( memory, vec2, texCoord_count );
                for( uint32 texCoord_index = 0; texCoord_index < texCoord_count; texCoord_index++ ) {
                    flo32 u = parseF32( parser );
                    flo32 v = parseF32( parser );
                    texCoord[ texCoord_index ] = Vec2( u, v );
                }
                
                texFile_data->texDataExistsInXFile = true;
                texFile_data->texCoord       = texCoord;
                texFile_data->texCoord_count = texCoord_count;
                
                char * end_token = parseToken( parser );
                Assert( end_token[ 0 ] == '}' );
            }
        } else if( matchString( token0, "}" ) ) {
            nest_level--;
        } else {
            InvalidCodePath;
        }
        uint32 breakHere = 10;
    }
    
    return result;
}

internal void
writeSegment( MEMORY * memory, const char * string, uint32 nTabs = 0, boo32 addNewLine = false ) {
	int32 length = ( int32 )strlen( string );
    length += ( nTabs * 3 );
    if( addNewLine ) { length += 1; }
    
	char * src  = ( char * )string;
	char * dest = ( char * )_pushSize( memory, length );
	
    for( uint32 iTab = 0; iTab < nTabs; iTab++ ) {
        *dest++ = ' ';
        *dest++ = ' ';
        *dest++ = ' ';
    }
	while( *src ) { *dest++ = *src++; }
    if( addNewLine ) {
        *dest++ = '\n';
    }
}

internal void
writeS32( MEMORY * memory, int32 value, uint32 nTabs = 0, boo32 addNewLine = false ) {
    char string[ 32 ] = {};
    sprintf( string, "%d", value );
    writeSegment( memory, string, nTabs, addNewLine );
}

internal void
writeU32( MEMORY * memory, uint32 value, uint32 nTabs = 0, boo32 addNewLine = false ) {
    char string[ 32 ] = {};
    sprintf( string, "%u", value );
    writeSegment( memory, string, nTabs, addNewLine );
}

internal void
writeU32Hex( MEMORY * memory, uint32 value, uint32 nTabs = 0, boo32 addNewLine = false ) {
    char string[ 32 ] = {};
    sprintf( string, "0x%X", value );
    writeSegment( memory, string, nTabs, addNewLine );
}

internal void
writeF32( MEMORY * memory, flo32 value, uint32 nTabs = 0, boo32 addNewLine = false ) {
    char string[ 32 ] = {};
    sprintf( string, "%.06f", value );
    writeSegment( memory, string, nTabs, addNewLine );
}

internal void
write_boo32( MEMORY * memory, boo32 value ) {
    char string[ 32 ] = {};
    sprintf( string, value ? "TRUE" : "FALSE" );
    writeSegment( memory, string );
}

internal void
writeV2( MEMORY * memory, vec2 value ) {
    writeF32( memory, value.x );
    writeSegment( memory, " " );
    writeF32( memory, value.y );
}

internal void
writeV3( MEMORY * memory, vec3 value, uint32 nTabs = 0, boo32 addNewLine = false ) {
    writeF32( memory, value.x, nTabs, false );
    writeF32( memory, value.y, 1,     false );
    writeF32( memory, value.z, 1,     addNewLine );
}

internal void
outputXFile( MEMORY * tempMemory, int32 entityID, vec3 * position, uint32 position_count, FACE * face, uint32 face_count ) {
    Assert( ( entityID >= 0 ) && ( entityID <= 9999 ) );
    
    MEMORY  _memory      = subArena( tempMemory, _MB( 2 ) );
    MEMORY * memory      = &_memory;
    
    writeSegment( memory, "xof 0303txt 0032\n" );
    writeSegment( memory, "\n" );
    writeSegment( memory, "Frame Root {\n" );
    writeSegment( memory, "\tFrame Mesh {\n" );
    
    writeSegment( memory, "\t\tFrameTransformMatrix {\n" );
    mat4 transform = mat4_identity();
    for( uint32 row_index = 0; row_index < 4; row_index++ ) {
        writeSegment( memory, "\t\t\t" );
        for( uint32 column_index = 0; column_index < 4; column_index++ ) {
            writeF32    ( memory, transform.elem[ row_index * 4 + column_index ] );
            writeSegment( memory, ", " );
        }
        if( row_index == 3 ) {
            memory->used -= 2;
            writeSegment( memory, ";;" );
        }
        writeSegment( memory, "\n" );
    }
    writeSegment( memory, "\t\t}\n" );
    
    writeSegment( memory, "\t\tMesh { // Mesh mesh\n" );
    writeSegment( memory, "\t\t\t" );
    writeU32    ( memory, position_count );
    writeSegment( memory, ";\n" );
    for( uint32 position_index = 0; position_index < position_count; position_index++ ) {
        vec3 P = position[ position_index ];
        writeSegment( memory, "\t\t\t" );
        writeF32    ( memory, P.x );
        writeSegment( memory, ";" );
        writeF32    ( memory, P.y );
        writeSegment( memory, ";" );
        writeF32    ( memory, P.z );
        writeSegment( memory, ";" );
        if( position_index < ( position_count - 1 ) ) {
            writeSegment( memory, "," );
        } else {
            writeSegment( memory, ";" );
        }
        writeSegment( memory, "\n" );
    }
    writeSegment( memory, "\t\t\t" );
    writeU32    ( memory, face_count );
    writeSegment( memory, ";\n" );
    for( uint32 face_index = 0; face_index < face_count; face_index++ ) {
        FACE F = face[ face_index ];
        
        uint32 elem_count = 0;
        for( uint32 elem_index = 0; elem_index < 4; elem_index++ ) {
            if( F.index[ elem_index ] != -1 ) {
                elem_count++;
            }
        }
        
        if( elem_count == 4 ) {
            _swap( int32, F.index[ 2 ], F.index[ 3 ] );
        }
        
        writeSegment( memory, "\t\t\t" );
        writeU32    ( memory, elem_count );
        writeSegment( memory, ";" );
        for( uint32 elem_index = 0; elem_index < elem_count; elem_index++ ) {
            writeU32( memory, F.index[ elem_index ] );
            if( elem_index < ( elem_count - 1 ) ) {
                writeSegment( memory, "," );
            } else {
                writeSegment( memory, ";" );
            }
        }
        if( face_index < ( face_count - 1 ) ) {
            writeSegment( memory, "," );
        } else {
            writeSegment( memory, ";" );
        }
        writeSegment( memory, "\n" );
    }
    writeSegment( memory, "\t\t} // End of Mesh mesh\n" );
    
    writeSegment( memory, "\t} // End of Mesh\n" );
    writeSegment( memory, "} // End of Root\n" );
    
    char entity_filePath[ 128 ] = {};
    sprintf( entity_filePath, "../../art/mesh/MESH_%04d.x", entityID );
    writeFile( entity_filePath, memory->base, ( uint32 )memory->used );
    
    _popSize( tempMemory, memory->size );
}

struct OVERHANG {
    int32 index[ 2 ];
    vec3  point[ 2 ];
};

internal OVERHANG
Overhang( vec3 point0, vec3 point1 ) {
    OVERHANG result = {};
    result.point[ 0 ] = point0;
    result.point[ 1 ] = point1;
    return result;
}

struct LEDGE {
    vec3 point[ 2 ];
};

internal LEDGE
Ledge( vec3 point0, vec3 point1 ) {
    LEDGE result = {};
    result.point[ 0 ] = point0;
    result.point[ 1 ] = point1;
    return result;
}

struct POLE {
    int32 index[ 2 ];
    vec3  point[ 2 ];
    vec3  normal;
};

internal POLE
Pole( vec3 point0, vec3 point1, vec3 normal ) {
    POLE result = {};
    result.point[ 0 ] = point0;
    result.point[ 1 ] = point1;
    result.normal     = normal;
    return result;
}

struct ORIENTED_BOX {
    vec3 center;
    vec3 xAxis;
    vec3 yAxis;
    vec3 zAxis;
    vec3 halfDim;
};

internal ORIENTED_BOX
OrientedBox( vec3 center, vec3 xAxis, vec3 yAxis, vec3 zAxis, vec3 halfDim ) {
    ORIENTED_BOX result = {};
    result.center  = center;
    result.xAxis   = xAxis;
    result.yAxis   = yAxis;
    result.zAxis   = zAxis;
    result.halfDim = halfDim;
    return result;
}

internal ORIENTED_BOX
parseOBox( FILE_PARSER * parser ) {
    ORIENTED_BOX result = {};
    result.center  = parseV3( parser );
    result.xAxis   = parseV3( parser );
    result.yAxis   = parseV3( parser );
    result.zAxis   = parseV3( parser );
    result.halfDim = parseV3( parser );
    return result;
}

struct SPHERE {
    vec3  center;
    flo32 radius;
};

internal SPHERE
Sphere( vec3 center, flo32 radius ) {
    SPHERE result = {};
    result.center = center;
    result.radius = radius;
    return result;
}

struct CAPSULE {
    vec3 P;
    vec3 Q;
    flo32 radius;
};

internal CAPSULE
Capsule( vec3 P, vec3 Q, flo32 radius ) {
    CAPSULE result = {};
    result.P = P;
    result.Q = Q;
    result.radius = radius;
    return result;
}

#define SECURE_DOOR__TARGET_TIME__ACTIVATE  ( 1.0f )
#define SECURE_DOOR__TARGET_TIME__OPEN      ( 1.0f )
struct SECURE_DOOR {
    ORIENTED_BOX activate_bound;
    ORIENTED_BOX render_bound;
    
    boo32        collision_isActive;
    ORIENTED_BOX collision_bound;
    
    uint32 pixie_cost[ 4 ];
    
    flo32 activate_timer;
};

enum PIXIE_TYPE_ID {
    pixieTypeID_grow,
    pixieTypeID_sight,
    pixieTypeID_memory,
    pixieTypeID_empty,
    
    pixieTypeID_count,
};

struct PIXIE {
    PIXIE_TYPE_ID typeID;
    
    vec3  position;
    
    flo32 xRadians;
    flo32 yRadians;
    flo32 zRadians;
    
    ORIENTED_BOX pickUp_bound;
};

internal PIXIE
Pixie( PIXIE_TYPE_ID typeID, ORIENTED_BOX pickUp_bound ) {
    PIXIE result = {};
    result.typeID       = typeID;
    result.position     = pickUp_bound.center;
    result.pickUp_bound = pickUp_bound;
    return result;
}

struct KILL_ZONE {
    ORIENTED_BOX bound;
};

struct CHECKPOINT {
    ORIENTED_BOX bound;
    vec3         respawnPos;
};

enum VIEW_TYPE {
    viewType_point,
    viewType_line,
    viewType_box,
};

struct VIEW {
    VIEW_TYPE type;
    
    vec3 P;
    vec3 Q;
    ORIENTED_BOX b;
    
    flo32 lo;
    flo32 hi;
    flo32 maxDist;
    flo32 hOffset;
    flo32 vOffset;
    flo32 fovAngle;
};

struct ENTITY_FILE_DATA {
    OVERHANG * overhang;
    uint32     overhang_count;
    
    LEDGE * ledge;
    uint32  nLedge;
    
    POLE * pole;
    uint32 pole_count;
    
    SECURE_DOOR * secureDoor;
    uint32       nSecureDoor;
    
    PIXIE * pixie;
    uint32  nPixie;
    
    KILL_ZONE * killZone;
    uint32     nKillZone;
    
    CHECKPOINT * checkpoint;
    uint32      nCheckpoint;
    
    VIEW *  view;
    uint32 nView;
    
    uint32   nColor;
    uint32 *  color;
    uint32 * bColor0;
    uint32 * nColor0;
    uint32 *  color_face;
};

internal ENTITY_FILE_DATA
parseEntityFile( MEMORY * memory, READ_FILE_RESULT file_entity ) {
    FILE_PARSER  _parser = FileParser( file_entity.contents, file_entity.size );
    FILE_PARSER * parser = &_parser;
    
    eatWhiteSpace( parser );
    char * heading = parseToken( parser );
    if( !matchString( heading, "[ENTITY_VERS01]" ) ) {
        OutputDebugString( "ERROR! File is not valid entity file!\n" );
    }
    
    // TODO: better error checking (like, actually check for errors)
    ENTITY_FILE_DATA result = {};
    while( hasTextRemaining( parser ) ) {
        char * token0 = parseToken( parser );
        char * token1 = parseToken( parser );
        if( token1[ 0 ] != '{' ) {
            OutputDebugString( "ERROR! File is not valid entity file!\n" );
        }
        
        if( matchString( token0, "[OVERHANG]" ) ) {
            result.overhang_count = parseU32( parser );
            result.overhang       = _pushArray_clear( memory, OVERHANG, result.overhang_count );
            for( uint32 overhang_index = 0; overhang_index < result.overhang_count; overhang_index++ ) {
                vec3 point0 = parseV3( parser );
                vec3 point1 = parseV3( parser );
                result.overhang[ overhang_index ] = Overhang( point0, point1 );
            }
        } else if( matchString( token0, "[LEDGE]" ) ) {
            result.nLedge = parseU32( parser );
            result.ledge  = _pushArray_clear( memory, LEDGE, result.nLedge );
            for( uint32 iLedge = 0; iLedge < result.nLedge; iLedge++ ) {
                vec3 point0 = parseV3( parser );
                vec3 point1 = parseV3( parser );
                result.ledge[ iLedge ] = Ledge( point0, point1 );
            }
        } else if( matchString( token0, "[POLE]" ) ) {
            result.pole_count = parseU32( parser );
            result.pole       = _pushArray_clear( memory, POLE, result.pole_count );
            for( uint32 pole_index = 0; pole_index < result.pole_count; pole_index++ ) {
                vec3 point0 = parseV3( parser );
                vec3 point1 = parseV3( parser );
                vec3 normal = parseV3( parser );
                result.pole[ pole_index ] = Pole( point0, point1, normal );
            }
        } else if( matchString( token0, "[SECURE_DOOR]" ) ) {
            result.nSecureDoor = parseU32( parser );
            result.secureDoor  = _pushArray_clear( memory, SECURE_DOOR, result.nSecureDoor );
            for( uint32 iDoor = 0; iDoor < result.nSecureDoor; iDoor++ ) {
                ORIENTED_BOX b = {};
                b.center = parseV3( parser );
                b.xAxis  = parseV3( parser );
                b.yAxis  = parseV3( parser );
                b.zAxis  = parseV3( parser );
                b.halfDim = parseV3( parser );
                
                SECURE_DOOR door = {};
                door.render_bound    = b;
                door.collision_bound = b;
                door.activate_bound  = b;
                door.activate_bound.halfDim.z = parseF32( parser );
                
                for( uint32 iPixie = 0; iPixie < 4; iPixie++ ) {
                    door.pixie_cost[ iPixie ] = parseU32( parser );
                }
                
                result.secureDoor[ iDoor ] = door;
            }
        } else if( matchString( token0, "[PIXIE]" ) ) {
            result.nPixie = parseU32( parser );
            result.pixie  = _pushArray_clear( memory, PIXIE, result.nPixie );
            for( uint32 iPixie = 0; iPixie < result.nPixie; iPixie++ ) {
                ORIENTED_BOX b = {};
                b.center  = parseV3( parser );
                b.xAxis   = parseV3( parser );
                b.yAxis   = parseV3( parser );
                b.zAxis   = parseV3( parser );
                b.halfDim = parseV3( parser );
                
                PIXIE_TYPE_ID type = ( PIXIE_TYPE_ID )parseU32( parser );
                PIXIE pixie = Pixie( type, b );
                
                result.pixie[ iPixie ] = pixie;
            }
        } else if( matchString( token0, "[KILL_ZONE]" ) ) {
            result.nKillZone = parseU32( parser );
            result.killZone  = _pushArray_clear( memory, KILL_ZONE, result.nKillZone );
            for( uint32 iKillZone = 0; iKillZone < result.nKillZone; iKillZone++ ) {
                ORIENTED_BOX b = {};
                b.center  = parseV3( parser );
                b.xAxis   = parseV3( parser );
                b.yAxis   = parseV3( parser );
                b.zAxis   = parseV3( parser );
                b.halfDim = parseV3( parser );
                
                KILL_ZONE killZone = {};
                killZone.bound = b;
                
                result.killZone[ iKillZone ] = killZone;
            }
        } else if( matchString( token0, "[CHECKPOINT]" ) ) {
            result.nCheckpoint = parseU32( parser );
            result.checkpoint  = _pushArray_clear( memory, CHECKPOINT, result.nCheckpoint );
            for( uint32 iCheckpoint = 0; iCheckpoint < result.nCheckpoint; iCheckpoint++ ) {
                ORIENTED_BOX b = {};
                b.center  = parseV3( parser );
                b.xAxis   = parseV3( parser );
                b.yAxis   = parseV3( parser );
                b.zAxis   = parseV3( parser );
                b.halfDim = parseV3( parser );
                
                CHECKPOINT checkpoint = {};
                checkpoint.bound      = b;
                checkpoint.respawnPos = parseV3( parser );
                
                result.checkpoint[ iCheckpoint ] = checkpoint;
            }
        } else if( matchString( token0, "[VIEW]" ) ) {
            result.nView = parseU32( parser );
            result.view  = _pushArray_clear( memory, VIEW, result.nView );
            for( uint32 iView = 0; iView < result.nView; iView++ ) {
                VIEW view = {};
                view.type = ( VIEW_TYPE )parseU32( parser );
                
                view.P    = parseV3( parser );
                view.Q    = parseV3( parser );
                view.b    = parseOBox( parser );
                
                view.lo       = parseF32( parser );
                view.hi       = parseF32( parser );
                view.maxDist  = parseF32( parser );
                view.hOffset  = parseF32( parser );
                view.vOffset  = parseF32( parser );
                view.fovAngle = parseF32( parser );
                
                result.view[ iView ] = view;
            }
        } else if( matchString( token0, "[COLOR]" ) ) {
            uint32 nColor = parseU32( parser );
            uint32 nFace  = parseU32( parser );
            
            result.nColor  = nColor;
            result.color   = _pushArray_clear( memory, uint32, result.nColor );
            result.bColor0 = _pushArray_clear( memory, uint32, result.nColor );
            result.nColor0 = _pushArray_clear( memory, uint32, result.nColor );
            result.color_face = _pushArray_clear( memory, uint32, nFace );
            
            uint32 bIndex = 0;
            for( uint32 iColor = 0; iColor < nColor; iColor++ ) {
                result.color[ iColor ] = parseU32( parser );
                
                result.bColor0[ iColor ] = bIndex;
                result.nColor0[ iColor ] = parseU32( parser );
                
                uint32 * face = result.color_face + bIndex;
                for( uint32 iFace = 0; iFace < nFace; iFace++ ) {
                    face[ iFace ] = parseU32( parser );
                }
                
                bIndex += nFace;
            }
        } else {
            InvalidCodePath;
        }
        
        token1 = parseToken( parser );
        Assert( token1[ 0 ] == '}' );
    }
    
    return result;
}

struct RENDER_DATA {
    VERTEX1 * vertex;
    int32     vertex_count;
    vec4      modColor;
};

internal void
drawRenderData( RENDERER * renderer, RENDER_PASS_ID passID, RENDER_DATA renderData ) {
    VERTEX1_TASK vertexTask = Vertex1Task( renderer );
    for( int32 vertex_index = 0; vertex_index < renderData.vertex_count; vertex_index += 3 ) {
        addTri( &vertexTask, renderData.vertex + vertex_index );
    }
    
    addVertex( renderer, 1, renderData.vertex_count );
    addObject( renderer, passID, RenderObject( meshDrawType_triangles, vertexTask.vertex_index, renderData.vertex_count, assetID_texture_boxTexture, renderData.modColor ) );
}

internal void
bubbleSort( vec3 * elem, uint32 elem_count ) {
    boo32 resort = true;
    while( resort ) {
        resort = false;
        
        for( uint32 iter = 0; iter < elem_count - 1; iter++ ) {
            uint32 index0 = iter;
            uint32 index1 = iter + 1;
            
            vec3 * elem0 = elem + index0;
            vec3 * elem1 = elem + index1;
            
            if( elem0->x > elem1->x ) {
                resort = true;
                _swap( vec3, elem0[ 0 ], elem1[ 0 ] );
            } else if( ( elem0->x == elem1->x ) && ( elem0->y > elem1->y ) ) {
                resort = true;
                _swap( vec3, elem0[ 0 ], elem1[ 0 ] );
            } else if( ( elem0->x == elem1->x ) && ( elem0->y == elem1->y ) && ( elem0->z > elem1->z ) ) {
                resort = true;
                _swap( vec3, elem0[ 0 ], elem1[ 0 ] );
            }
            uint32 breakHere = 10;
        }
    }
}

internal uint32
removeDuplicates( vec3 * elem, uint32 elem_count ) {
    uint32 atIndex = 1;
    vec3   elem0   = elem[ 0 ];
    for( uint32 elem_index = 1; elem_index < elem_count; elem_index++ ) {
        vec3 elem1 = elem[ elem_index ];
        if( elem0 != elem1 ) {
            elem[ atIndex++ ] = elem1;
            elem0 = elem1;
        } 
    }
    return atIndex;
}

internal boo32
testIsWalkable( vec3 normal ) {
    flo32 radians = acosf( dot( normal, Vec3( 0.0f, 1.0f, 0.0f ) ) );
    flo32 degrees = radians / ( 2.0f * PI ) * 360.0f;
    // // DISPLAY_VALUE( flo32, degrees );
    
    boo32 result = ( degrees <= 25.0f );
    return result;
}