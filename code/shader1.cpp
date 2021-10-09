
struct VERTEX1 {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

internal VERTEX1
Vertex1( vec3 position, vec3 normal, vec2 texCoord ) {
    VERTEX1 result = { position, normal, texCoord };
    return result;
}

struct VERTEX1_TASK {
    int32 vertex_index;
    int32 vertex_count;
    VERTEX1 * vertex;
};

internal VERTEX1_TASK
Vertex1Task( RENDERER * renderer ) {
    int32 bufferID = 1;
    
    VERTEX1_TASK result = {};
    result.vertex_index = renderer->vertex_count[ bufferID ];
    result.vertex_count = 0;
    result.vertex       = ( VERTEX1 * )( ( uint8 * )renderer->vertex_ptr[ bufferID ] + ( renderer->vertex_count[ bufferID ] * renderer->vertex_stride[ bufferID ] ) );
    return result;
}

internal void
addPoint( VERTEX1_TASK * vertexTask, vec3 pos0, vec3 normal ) {
    *vertexTask->vertex++ = Vertex1( pos0, normal, Vec2( 0.0f, 0.0f ) );
    vertexTask->vertex_count++;
}

internal void
addEdge( VERTEX1_TASK * vertexTask, vec3 pos0, vec3 pos1, vec3 normal ) {
    *vertexTask->vertex++ = Vertex1( pos0, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos1, normal, Vec2( 0.0f, 0.0f ) );
    vertexTask->vertex_count += 2;
}

internal void
addTri( VERTEX1_TASK * vertexTask, vec3 pos0, vec3 pos1, vec3 pos2, vec3 normal ) {
    *vertexTask->vertex++ = Vertex1( pos0, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos1, normal, Vec2( 1.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos2, normal, Vec2( 0.0f, 1.0f ) );
    vertexTask->vertex_count += 3;
}

internal void
addTri( VERTEX1_TASK * vertexTask, VERTEX1 * vertex ) {
    *vertexTask->vertex++ = vertex[ 0 ];
    *vertexTask->vertex++ = vertex[ 1 ];
    *vertexTask->vertex++ = vertex[ 2 ];
    vertexTask->vertex_count += 3;
}

internal void
addQuad( VERTEX1_TASK * vertexTask, vec3 pos0, vec3 pos1, vec3 pos2, vec3 pos3, vec3 normal ) {
    *vertexTask->vertex++ = Vertex1( pos0, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos1, normal, Vec2( 1.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos2, normal, Vec2( 0.0f, 1.0f ) );
    *vertexTask->vertex++ = Vertex1( pos3, normal, Vec2( 1.0f, 1.0f ) );
    *vertexTask->vertex++ = Vertex1( pos2, normal, Vec2( 0.0f, 1.0f ) );
    *vertexTask->vertex++ = Vertex1( pos1, normal, Vec2( 1.0f, 0.0f ) );
    vertexTask->vertex_count += 6;
}

internal void
addQuadOutline( VERTEX1_TASK * vertexTask, vec3 pos0, vec3 pos1, vec3 pos2, vec3 pos3, vec3 normal ) {
    *vertexTask->vertex++ = Vertex1( pos0, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos1, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos1, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos2, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos2, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos3, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos3, normal, Vec2( 0.0f, 0.0f ) );
    *vertexTask->vertex++ = Vertex1( pos0, normal, Vec2( 0.0f, 0.0f ) );
    vertexTask->vertex_count += 8;
}

internal void
initShader1( RENDERER * renderer ) {
    
    const char * shader_vertexCode = "\
 cbuffer MatrixBuffer : register( b0 ) {\
    matrix camera_transform;\
 };\
 \
 struct VertexInputType {\
    float4 position    : POSITION;\
    float4 normal      : NORMAL;\
    float2 tex         : TEXCOORD0;\
 };\
 \
 struct PixelInputType {\
    float4 position : SV_POSITION;\
    float4 normal   : NORMAL;\
    float2 tex      : TEXCOORD0;\
 };\
 \
 PixelInputType TextureVertexShader( VertexInputType input ) {\
    PixelInputType output;\
    input.position.w = 1.0f;\
    input.normal.w   = 0.0f;\
    output.position = mul( input.position, camera_transform );\
    output.normal   = input.normal;\
    output.tex      = input.tex;\
    return output;\
 }";
    
    const char * shader_pixelCode = "\
 Texture2D    shaderTexture;\
 SamplerState SampleType;\
 \
 cbuffer ColorBuffer : register( b0 ) {\
    float4 modColor;\
 };\
 \
 struct PixelInputType {\
    float4 position : SV_POSITION;\
    float4 normal   : NORMAL;\
    float2 tex      : TEXCOORD0;\
 };\
 \
 float4 TexturePixelShader( PixelInputType input ) : SV_TARGET {\
    float4 lightDir = normalize( float4( -0.2f, 1.0f, 0.5f, 0.0f ) );\
    float  lightDot = clamp( dot( input.normal, lightDir ), 0.0f, 1.0f ) * 0.6f + 0.4f;\
    float4 texel = shaderTexture.Sample( SampleType, input.tex );\
    texel.r *= ( modColor.r * lightDot );\
    texel.g *= ( modColor.g * lightDot );\
    texel.b *= ( modColor.b * lightDot );\
    return texel;\
 }";
    
    ID3D10Blob * errorMessage = 0;
    ID3D10Blob * vertexShaderBuffer = 0;
    int32 status = D3DCompile( shader_vertexCode, strlen( shader_vertexCode ), 0, 0, 0, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to compile the vertex shader!\n" );
        char * error = ( char * )( errorMessage->GetBufferPointer() );
        InvalidCodePath;
    }
    
    ID3D10Blob * pixelShaderBuffer  = 0;
    status = D3DCompile( shader_pixelCode, strlen( shader_pixelCode ), 0, 0, 0, "TexturePixelShader",  "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to compile the pixel shader!\n" );
        char * error = ( char * )( errorMessage->GetBufferPointer() );
        InvalidCodePath;
    }
    
    ID3D11VertexShader * vertexShader = 0;
    status = renderer->device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 0, &vertexShader );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create the vertex shader!\n" );
    }
    
    ID3D11PixelShader * pixelShader = 0;
    status = renderer->device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &pixelShader );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create the pixel shader!\n" );
    }
    
    renderer->deviceContext->VSSetShader( vertexShader, 0, 0 );
    renderer->deviceContext->PSSetShader(  pixelShader, 0, 0 );
    
    D3D11_INPUT_ELEMENT_DESC polygonLayout[ 3 ] = {};
    polygonLayout[ 0 ].SemanticName   = "POSITION";
    polygonLayout[ 0 ].Format         = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    
    polygonLayout[ 1 ].SemanticName      = "NORMAL";
    polygonLayout[ 1 ].Format            = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[ 1 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    polygonLayout[ 2 ].SemanticName      = "TEXCOORD";
    polygonLayout[ 2 ].Format            = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[ 2 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    ID3D11InputLayout * layout = 0;
    status = renderer->device->CreateInputLayout( polygonLayout, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create an input layout for the vertex shader!\n" );
    }
    
    vertexShaderBuffer->Release();
    pixelShaderBuffer->Release();
    
    int32 shaderID = shaderID_main_lighting;
    renderer->vertexShader [ shaderID ] = vertexShader;
    renderer->pixelShader  [ shaderID ] = pixelShader;
    renderer->vertexLayout [ shaderID ] = layout;
    renderer->vertex_stride[ shaderID ] = sizeof( VERTEX1 );
}

internal void
initShader2( RENDERER * renderer ) {
    const char * shader_vertexCode = "\
 cbuffer MatrixBuffer : register( b0 ) {\
    matrix camera_transform;\
 };\
 \
 struct VertexInputType {\
    float4 position    : POSITION;\
    float4 normal      : NORMAL;\
    float2 tex         : TEXCOORD0;\
 };\
 \
 struct PixelInputType {\
    float4 position : SV_POSITION;\
    float4 normal   : NORMAL;\
    float2 tex      : TEXCOORD0;\
 };\
 \
 PixelInputType TextureVertexShader( VertexInputType input ) {\
    PixelInputType output;\
    input.position.w = 1.0f;\
    input.normal.w   = 0.0f;\
    output.position = mul( input.position, camera_transform );\
    output.normal   = input.normal;\
    output.tex      = input.tex;\
    return output;\
 }";
    
    const char * shader_pixelCode = "\
 Texture2D    shaderTexture;\
 SamplerState SampleType;\
 \
 cbuffer ColorBuffer : register( b0 ) {\
    float4 modColor;\
 };\
 \
 struct PixelInputType {\
    float4 position : SV_POSITION;\
    float4 normal   : NORMAL;\
    float2 tex      : TEXCOORD0;\
 };\
 \
 float4 TexturePixelShader( PixelInputType input ) : SV_TARGET {\
    float4 lightDir = normalize( float4( -0.2f, 1.0f, 0.5f, 0.0f ) );\
    float  lightDot = clamp( dot( input.normal, lightDir ), 0.0f, 1.0f ) * 0.8f + 0.2f;\
           lightDot = 1.0f;\
    float4 texel = shaderTexture.Sample( SampleType, input.tex );\
    texel.r *= ( modColor.r * lightDot );\
    texel.g *= ( modColor.g * lightDot );\
    texel.b *= ( modColor.b * lightDot );\
    return texel;\
 }";
    
    ID3D10Blob * errorMessage = 0;
    ID3D10Blob * vertexShaderBuffer = 0;
    int32 status = D3DCompile( shader_vertexCode, strlen( shader_vertexCode ), 0, 0, 0, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to compile the vertex shader!\n" );
        char * error = ( char * )( errorMessage->GetBufferPointer() );
        InvalidCodePath;
    }
    
    ID3D10Blob * pixelShaderBuffer  = 0;
    status = D3DCompile( shader_pixelCode, strlen( shader_pixelCode ), 0, 0, 0, "TexturePixelShader",  "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to compile the pixel shader!\n" );
        char * error = ( char * )( errorMessage->GetBufferPointer() );
        InvalidCodePath;
    }
    
    ID3D11VertexShader * vertexShader = 0;
    status = renderer->device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 0, &vertexShader );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create the vertex shader!\n" );
    }
    
    ID3D11PixelShader * pixelShader = 0;
    status = renderer->device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &pixelShader );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create the pixel shader!\n" );
    }
    
    renderer->deviceContext->VSSetShader( vertexShader, 0, 0 );
    renderer->deviceContext->PSSetShader(  pixelShader, 0, 0 );
    
    D3D11_INPUT_ELEMENT_DESC polygonLayout[ 3 ] = {};
    polygonLayout[ 0 ].SemanticName   = "POSITION";
    polygonLayout[ 0 ].Format         = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    
    polygonLayout[ 1 ].SemanticName      = "NORMAL";
    polygonLayout[ 1 ].Format            = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[ 1 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    polygonLayout[ 2 ].SemanticName      = "TEXCOORD";
    polygonLayout[ 2 ].Format            = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[ 2 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    ID3D11InputLayout * layout = 0;
    status = renderer->device->CreateInputLayout( polygonLayout, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create an input layout for the vertex shader!\n" );
    }
    
    vertexShaderBuffer->Release();
    pixelShaderBuffer->Release();
    
    int32 shaderID = shaderID_main_noLighting;
    renderer->vertexShader [ shaderID ] = vertexShader;
    renderer->pixelShader  [ shaderID ] = pixelShader;
    renderer->vertexLayout [ shaderID ] = layout;
    renderer->vertex_stride[ shaderID ] = sizeof( VERTEX1 );
}