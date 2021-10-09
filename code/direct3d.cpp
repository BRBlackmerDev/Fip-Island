#ifdef	STD_INCLUDE_DECL

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

struct DEBUG_VERTEX {
    vec2 position;
    vec2 texCoord;
};

enum VERTEX_BUFFER_ID {
    vertexBufferID_debug,
    vertexBufferID_vertex1,
    
    vertexBufferID_count,
};

enum RENDER_PASS_ID {
    renderPassID_debug,
    renderPassID_main,
    renderPassID_main_debug,
    
    renderPassID_main_noLighting,
    
    renderPassID_overlay,
    
    renderPassID_scissor0,
    renderPassID_scissor1,
    renderPassID_scissor2,
    renderPassID_scissor3,
    renderPassID_scissor4,
    renderPassID_scissor5,
    
    renderPassID_count,
};

enum SHADER_ID {
    shaderID_debug,
    
    shaderID_main_lighting,
    shaderID_main_noLighting,
    
    shaderID_count,
};

struct RENDERER {
    ASSET_SYSTEM * assetSystem;
    
    ID3D11Device           * device;
    ID3D11DeviceContext    * deviceContext;
    IDXGISwapChain         * swapChain;
    ID3D11RenderTargetView * renderTargetView;
    ID3D11DepthStencilView * depthStencilView;
    boo32 isFullscreen;
    
#define RENDERER__SHADER_COUNT  ( shaderID_count )
    ID3D11VertexShader * vertexShader[ RENDERER__SHADER_COUNT ];
    ID3D11PixelShader  * pixelShader [ RENDERER__SHADER_COUNT ];
    ID3D11Buffer       * vertexBuffer[ RENDERER__SHADER_COUNT ];
    ID3D11InputLayout  * vertexLayout[ RENDERER__SHADER_COUNT ];
    
    uint32 vertex_stride  [ RENDERER__SHADER_COUNT ];
    void * vertex_ptr     [ RENDERER__SHADER_COUNT ];
    int32  vertex_count   [ RENDERER__SHADER_COUNT ];
    int32  vertex_maxCount[ RENDERER__SHADER_COUNT ];
    
    ID3D11DepthStencilState * depthState_enable;
    ID3D11DepthStencilState * depthState_disable;
    
    ID3D11BlendState * blendState_enable;
    ID3D11BlendState * blendState_disable;
    
#define RENDERER__MAX_LOADED_TEXTURE_COUNT  ( 128 )
    int32 loadedTexture_count;
    ASSET_ID                   loadedTexture_id      [ RENDERER__MAX_LOADED_TEXTURE_COUNT ];
    ID3D11ShaderResourceView * loadedTexture_resource[ RENDERER__MAX_LOADED_TEXTURE_COUNT ];
    
    ID3D11Buffer * buffer_cameraTransform;
    ID3D11Buffer * buffer_modColor;
    
    RENDER_OBJECT * object         [ renderPassID_count ];
    int32           object_count   [ renderPassID_count ];
    int32           object_maxCount[ renderPassID_count ];
    
#if DEBUG_BUILD
#define DEBUG__MAX_VERTEX_COUNT  ( 4096 )
    // int32 debug_vertexCount;
    // DEBUG_VERTEX debug_vertex[ DEBUG__MAX_VERTEX_COUNT ];
    
    // #define DEBUG__MAX_OBJECT_COUNT  ( 512 )
    // int32         debug_objectCount;
    // RENDER_OBJECT debug_object[ DEBUG__MAX_OBJECT_COUNT ];
#endif // DEBUG_BUILD
};
#endif	// STD_INCLUDE_DECL

#ifdef	STD_INCLUDE_FUNC

internal ID3D11DepthStencilState *
DepthState( ID3D11Device * device, boo32 enableDepth ) {
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable                  = enableDepth;
    depthStencilDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc                    = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable                = false;
    depthStencilDesc.StencilReadMask              = 0xFF;
    depthStencilDesc.StencilWriteMask             = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
    
    ID3D11DepthStencilState * depthStencilState = 0;
    uint32 status = device->CreateDepthStencilState( &depthStencilDesc, &depthStencilState );
    Assert( status == S_OK );
    
    return depthStencilState;
}

internal void
setDepth( RENDERER * renderer, boo32 enable ) {
    renderer->deviceContext->OMSetDepthStencilState( enable ? renderer->depthState_enable : renderer->depthState_disable, 1 );
}

internal ID3D11BlendState *
BlendState( ID3D11Device * device, boo32 enableBlend ) {
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[ 0 ].BlendEnable           = enableBlend;
    
    blendDesc.RenderTarget[ 0 ].SrcBlend              = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[ 0 ].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].BlendOp               = D3D11_BLEND_OP_ADD;
    
    blendDesc.RenderTarget[ 0 ].SrcBlendAlpha         = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[ 0 ].DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[ 0 ].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    
    blendDesc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
    ID3D11BlendState * blendState = 0;
    uint32 status = device->CreateBlendState( &blendDesc, &blendState );
    Assert( status == S_OK );
    
    return blendState;
}

internal void
setBlend( RENDERER * renderer, boo32 enable ) {
    renderer->deviceContext->OMSetBlendState( enable ? renderer->blendState_enable : renderer->blendState_disable, 0, 0xFFFFFFFF );
}

internal void
enableScissor( RENDERER * renderer, rect client_bound, rect scissor_bound ) {
    // D3D11_RECT scissor_rect = { ( int32 )bound.left, ( int32 )bound.top, ( int32 )bound.right, ( int32 )bound.bottom };
    D3D11_RECT scissor_rect = {
        ( int32 )scissor_bound.left,  ( int32 )( client_bound.top    - scissor_bound.top    ),
        ( int32 )scissor_bound.right, ( int32 )( getHeight( client_bound ) - scissor_bound.bottom ),
    };
    renderer->deviceContext->RSSetScissorRects( 1, &scissor_rect );
}

internal void
disableScissor( RENDERER * renderer ) {
    renderer->deviceContext->RSSetScissorRects( 0, 0 );
}

internal void
setShader( RENDERER * renderer, uint32 shaderID, uint32 vertexBufferID ) {
    renderer->deviceContext->VSSetShader( renderer->vertexShader[ shaderID ], 0, 0 );
    renderer->deviceContext->PSSetShader( renderer->pixelShader[  shaderID ], 0, 0 );
    renderer->deviceContext->IASetInputLayout( renderer->vertexLayout[ shaderID ] );
    
    uint32 stride = renderer->vertex_stride[ shaderID ];
    uint32 offset = 0;
    renderer->deviceContext->IASetVertexBuffers( 0, 1, &renderer->vertexBuffer[ vertexBufferID ], &stride, &offset );
}

internal void
setCamera( RENDERER * renderer, mat4 transform ) {
    transform = mat4_transpose( transform );
    
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    int32 status = renderer->deviceContext->Map( renderer->buffer_cameraTransform, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to map the transform buffer for the shader!\n" ); }
    
    mat4 * shader_transform = ( mat4 * )mappedResource.pData;
    shader_transform[ 0 ] = transform;
    renderer->deviceContext->Unmap( renderer->buffer_cameraTransform, 0 );
    renderer->deviceContext->VSSetConstantBuffers( 0, 1, &renderer->buffer_cameraTransform );
}

struct RENDER_DATA__MOD_COLOR {
    vec4 color;
};

internal void
setModColor( RENDERER * renderer, vec4 color ) {
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    int32 status = renderer->deviceContext->Map( renderer->buffer_modColor, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to map the color buffer for the shader!\n" ); }
    RENDER_DATA__MOD_COLOR * data = ( RENDER_DATA__MOD_COLOR * )mappedResource.pData;
    data->color  = color;
    renderer->deviceContext->Unmap( renderer->buffer_modColor, 0 );
    renderer->deviceContext->PSSetConstantBuffers( 0, 1, &renderer->buffer_modColor );
}

internal void
pushVertices( RENDERER * renderer, uint32 vertexID ) {
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    int32 status = renderer->deviceContext->Map( renderer->vertexBuffer[ vertexID ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to map the vertex buffer for the debug vertices!\n" ); }
    memcpy( mappedResource.pData, renderer->vertex_ptr[ vertexID ], renderer->vertex_stride[ vertexID ] * renderer->vertex_count[ vertexID ] );
    renderer->deviceContext->Unmap( renderer->vertexBuffer[ vertexID ], 0 );
    
    renderer->vertex_count[ vertexID ] = 0;
}

internal int32
findLoadedTexture( ASSET_ID textureID, RENDERER * renderer ) {
	int32 result = -1;
	
	int32 index 	= 0;
	boo32 wasFound = false;
	while( ( !wasFound ) && ( index < renderer->loadedTexture_count ) ) {
		ASSET_ID currID = renderer->loadedTexture_id[ index ];
		if( textureID == currID ) {
			wasFound = true;
			result = index;
		}
		index++;
	}
	
	return result;
}

internal void
initRenderPass( RENDERER * renderer, uint32 passID, MEMORY * memory, int32 maxCount ) {
    renderer->object         [ passID ] = _pushArray( memory, RENDER_OBJECT, maxCount );
    renderer->object_maxCount[ passID ] = maxCount;
}

internal void
initVertexBuffer( RENDERER * renderer, uint32 bufferID, MEMORY * memory, int32 stride, int32 maxCount ) {
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth      = stride * maxCount;
    vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    ID3D11Buffer * vertexBuffer = 0;
    int32 status = renderer->device->CreateBuffer( &vertexBufferDesc, 0, &vertexBuffer );
    if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to create a vertex buffer!\n" ); }
    
    renderer->vertexBuffer   [ bufferID ] = vertexBuffer;
    renderer->vertex_ptr     [ bufferID ] = _pushSize( memory, stride * maxCount );
    renderer->vertex_maxCount[ bufferID ] = maxCount;
}

internal void
renderPass( RENDERER * renderer, uint32 passID ) {
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};
    int32                    status         = 0;
    
    RENDER_OBJECT * objectList   = renderer->object[ passID ];
    int32           object_count = renderer->object_count[ passID ];
    
    for( int32 object_index = 0; object_index < object_count; object_index++ ) {
        RENDER_OBJECT * object = objectList + object_index;
        
        D3D11_PRIMITIVE_TOPOLOGY drawType = {};
        switch( object->drawType ) {
            case meshDrawType_triangles:     { drawType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;  } break;
            case meshDrawType_triangleStrip: { drawType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; } break;
            case meshDrawType_lines:         { drawType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;      } break;
            case meshDrawType_lineStrip:     { drawType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;     } break;
            
            default: { InvalidCodePath; } break;
        }
        
        int32 loadedTexture_index = findLoadedTexture( object->textureID, renderer );
        Assert( loadedTexture_index > -1 );
        
        ID3D11ShaderResourceView * object_resource = renderer->loadedTexture_resource[ loadedTexture_index ];
        
        setModColor( renderer, object->color );
        
        renderer->deviceContext->IASetPrimitiveTopology( drawType );
        renderer->deviceContext->PSSetShaderResources( 0, 1, &object_resource );
        renderer->deviceContext->Draw( object->vertex_count, object->vertex_index );
    }
    
    renderer->object_count[ passID ] = 0;
}

struct DISPLAY_SETTINGS {
    int32 width;
    int32 height;
    int32 refresh_numer;
    int32 refresh_denom;
};

internal DISPLAY_SETTINGS
getDisplayAdapterMatch( MEMORY * tempMemory, uint32 target_width, uint32 target_height, flo32 target_refreshRate ) {
    int32 status = 0;
    
    IDXGIFactory * factory = 0;
    status = CreateDXGIFactory( __uuidof( IDXGIFactory ), ( void ** )&factory );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create DXGI factory to enumerate adapters!\n" );
    }
    
    IDXGIAdapter * adapter = 0;
    status = factory->EnumAdapters( 0, &adapter );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to enumerate adapters!\n" );
    }
    
    IDXGIOutput * adapterOutput = 0;
    status = adapter->EnumOutputs( 0, &adapterOutput );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to enumerate adapter outputs!\n" );
    }
    
    uint32 numModes = 0;
    status = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, 0 );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to get count of display modes!\n" );
    }
    
    DXGI_MODE_DESC * displayModeList = _pushArray_clear( tempMemory, DXGI_MODE_DESC, numModes );
    status = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to get the list of display modes!\n" );
    }
    
    flo32  best_refreshRate  = 0.0f;
    uint32 best_width        = 0;
    uint32 best_height       = 0;
    flo32  best_dAspectRatio = 1000.0f;
    
    DISPLAY_SETTINGS bestMatch = {};
    
    flo32 target_aspectRatio  = ( flo32 )target_width / ( flo32 )target_height;
    flo32 target_dAspectRatio = 1000.0f;
    
    boo32 matchedTarget = false;
    for( uint32 mode_index = 0; ( !matchedTarget ) && ( mode_index < numModes ); mode_index++ ) {
        DXGI_MODE_DESC * displayMode = displayModeList + mode_index;
        
        DXGI_RATIONAL ratio = displayMode->RefreshRate;
        
        flo32 refreshRate = ( flo32 )ratio.Numerator / ( flo32 )ratio.Denominator;
        flo32 aspectRatio = ( flo32 )displayMode->Width / ( flo32 )displayMode->Height;
        
        if( ( refreshRate == target_refreshRate ) && ( displayMode->Width == target_width ) && ( displayMode->Height == target_height ) ) {
            matchedTarget = true;
            
            best_refreshRate = refreshRate;
            best_width       = displayMode->Width;
            best_height      = displayMode->Height;
            
            bestMatch.width         = best_width;
            bestMatch.height        = best_height;
            bestMatch.refresh_numer = ratio.Numerator;
            bestMatch.refresh_denom = ratio.Denominator;
        } else {
            flo32 dAspectRatio = fabsf( target_aspectRatio - aspectRatio );
            if(   ( dAspectRatio <  best_dAspectRatio ) ||
               ( ( dAspectRatio == best_dAspectRatio ) && ( refreshRate > best_refreshRate ) ) || 
               ( ( dAspectRatio == best_dAspectRatio ) && ( refreshRate == best_refreshRate ) && ( ( displayMode->Width > best_width ) || ( displayMode->Height > best_height ) ) ) )
            {
                best_dAspectRatio = dAspectRatio;
                best_refreshRate  = refreshRate;
                best_width        = displayMode->Width;
                best_height       = displayMode->Height;
                
                bestMatch.width         = best_width;
                bestMatch.height        = best_height;
                bestMatch.refresh_numer = ratio.Numerator;
                bestMatch.refresh_denom = ratio.Denominator;
            }
        }
    }
    
    DXGI_ADAPTER_DESC adapterDesc = {};
    status = adapter->GetDesc( &adapterDesc );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to get the adapter descriptor!\n" );
    }
    
    adapterOutput->Release();
    adapter->Release();
    factory->Release();
    
    _popArray( tempMemory, DXGI_MODE_DESC, numModes );
    
    return bestMatch;
}

internal boo32
init( RENDERER * renderer, HWND window, DISPLAY_SETTINGS displaySettings, boo32 isFullscreen ) {
    // TODO: is HWND the correct type to use for window? multiple platforms?
    boo32 result = true;
    
    int32 status = 0;
    
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width  = displaySettings.width;
    swapChainDesc.BufferDesc.Height = displaySettings.height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    
    swapChainDesc.BufferDesc.RefreshRate.Numerator   = displaySettings.refresh_numer;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = displaySettings.refresh_denom;
    
    swapChainDesc.SampleDesc.Count  = 1;
    swapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount       = 1;
    swapChainDesc.OutputWindow      = window;
    swapChainDesc.Windowed          = !isFullscreen;
    swapChainDesc.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;
    
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    
    ID3D11Device        * device        = 0;
    ID3D11DeviceContext * deviceContext = 0;
    IDXGISwapChain      * swapChain     = 0;
    
    status = D3D11CreateDeviceAndSwapChain( 0, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, 0, &deviceContext );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create device and swap chain!\n" );
    }
    
    ID3D11Texture2D * backBufferPtr = 0;
    status = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID * )&backBufferPtr );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER is unable to locate backbuffer in swap chain!\n" );
    }
    
    ID3D11RenderTargetView * renderTargetView = 0;
    status = device->CreateRenderTargetView( backBufferPtr, 0, &renderTargetView );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create render target view from backbuffer!\n" );
    }
    
    backBufferPtr->Release();
    
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    depthBufferDesc.Width            = displaySettings.width;
    depthBufferDesc.Height           = displaySettings.height;
    depthBufferDesc.MipLevels        = 1;
    depthBufferDesc.ArraySize        = 1;
    depthBufferDesc.Format           = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.Usage            = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags        = D3D11_BIND_DEPTH_STENCIL;
    
    ID3D11Texture2D * depthStencilBuffer = 0;
    status = device->CreateTexture2D( &depthBufferDesc, 0, &depthStencilBuffer );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create texture for depth buffer!\n" );
    }
    
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format        = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    
    ID3D11DepthStencilView * depthStencilView = 0;
    status = device->CreateDepthStencilView( depthStencilBuffer, &depthStencilViewDesc, &depthStencilView );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create depth buffer view!\n" );
    }
    
    deviceContext->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );
    
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode        = D3D11_FILL_SOLID;
    // rasterDesc.CullMode        = D3D11_CULL_FRONT;
    rasterDesc.CullMode        = D3D11_CULL_NONE;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.ScissorEnable   = false;
    
    ID3D11RasterizerState * rasterState = 0;
    status = device->CreateRasterizerState( &rasterDesc, &rasterState );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create a rasterizer state!\n" );
    }
    
    deviceContext->RSSetState( rasterState );
    
    // D3D11_RECT scissor_rect = { ( int32 )scissor_bound.left, displaySettings.height - ( int32 )scissor_bound.top, ( int32 )scissor_bound.right, displaySettings.height - ( int32 )scissor_bound.bottom };
    // deviceContext->RSSetScissorRects( 1, &scissor_rect );
    
    // deviceContext->OMSetBlendState( renderer->blendState_enable,  0, 0xFFFFFFFF );
    
    D3D11_VIEWPORT viewport = {};
    viewport.Width    = ( flo32 )displaySettings.width;
    viewport.Height   = ( flo32 )displaySettings.height;
    viewport.MaxDepth = 1.0f;
    
    deviceContext->RSSetViewports( 1, &viewport );
    
    // const char * shader_vertexCode = "\
 // cbuffer MatrixBuffer {\
    // matrix camera_transform;\
 // };\
 // \
 // cbuffer MatrixBuffer {\
    // matrix bone_transform[ 128 ];\
 // };\
 // \
 // struct VertexInputType {\
    // float4 position    : POSITION;\
    // float2 tex         : TEXCOORD0;\
    // uint4  bone_index  : BLENDINDICES;\
    // float4 bone_weight : BLENDWEIGHT;\
 // };\
 // \
 // struct PixelInputType {\
    // float4 position : SV_POSITION;\
    // float2 tex      : TEXCOORD0;\
 // };\
 // \
 // PixelInputType TextureVertexShader( VertexInputType input ) {\
    // PixelInputType output;\
    // input.position.w = 1.0f;\
    // float4 pos0 = mul( input.position, bone_transform[ input.bone_index[ 0 ] ] ) * input.bone_weight[ 0 ];\
    // float4 pos1 = mul( input.position, bone_transform[ input.bone_index[ 1 ] ] ) * input.bone_weight[ 1 ];\
    // float4 pos2 = mul( input.position, bone_transform[ input.bone_index[ 2 ] ] ) * input.bone_weight[ 2 ];\
    // float4 pos3 = mul( input.position, bone_transform[ input.bone_index[ 3 ] ] ) * input.bone_weight[ 3 ];\
    // float4 pos = pos0 + pos1 + pos2 + pos3;\
    // pos.w = 1.0f;\
    // output.position = mul( pos, camera_transform );\
    // output.tex = input.tex;\
    // return output;\
 // }";
    
    // const char * shader_pixelCode = "\
 // Texture2D    shaderTexture;\
 // SamplerState SampleType;\
 // \
 // cbuffer ColorBuffer {\
    // float4 modColor;\
 // };\
 // \
 // struct PixelInputType {\
    // float4 position : SV_POSITION;\
    // float2 tex      : TEXCOORD0;\
 // };\
 // \
 // float4 TexturePixelShader( PixelInputType input ) : SV_TARGET {\
    // float4 textureColor = shaderTexture.Sample( SampleType, input.tex );\
    // textureColor.r *= modColor.r;\
    // textureColor.g *= modColor.g;\
    // textureColor.b *= modColor.b;\
    // return textureColor;\
 // }";
    
    ID3D10Blob * errorMessage = 0;
    // ID3D10Blob * vertexShaderBuffer = 0;
    // status = D3DCompile( shader_vertexCode, strlen( shader_vertexCode ), 0, 0, 0, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage );
    // if( status != S_OK ) {
    // result = false;
    // CONSOLE_STRING( "ERROR! RENDERER failed to compile the vertex shader!\n" );
    // char * error = ( char * )( errorMessage->GetBufferPointer() );
    // InvalidCodePath;
    // }
    
    // ID3D10Blob * pixelShaderBuffer  = 0;
    // status = D3DCompile( shader_pixelCode, strlen( shader_pixelCode ), 0, 0, 0, "TexturePixelShader",  "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage );
    // if( status != S_OK ) {
    // result = false;
    // CONSOLE_STRING( "ERROR! RENDERER failed to compile the pixel shader!\n" );
    // char * error = ( char * )( errorMessage->GetBufferPointer() );
    // InvalidCodePath;
    // }
    
    // ID3D11VertexShader * vertexShader = 0;
    // status = device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 0, &vertexShader );
    // if( status != S_OK ) {
    // result = false;
    // CONSOLE_STRING( "ERROR! RENDERER failed to create the vertex shader!\n" );
    // }
    
    // ID3D11PixelShader * pixelShader = 0;
    // status = device->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), 0, &pixelShader );
    // if( status != S_OK ) {
    // result = false;
    // CONSOLE_STRING( "ERROR! RENDERER failed to create the pixel shader!\n" );
    // }
    
    // deviceContext->VSSetShader( vertexShader, 0, 0 );
    // deviceContext->PSSetShader(  pixelShader, 0, 0 );
    
    // D3D11_INPUT_ELEMENT_DESC polygonLayout[ 4 ] = {};
    // polygonLayout[ 0 ].SemanticName   = "POSITION";
    // polygonLayout[ 0 ].Format         = DXGI_FORMAT_R32G32B32_FLOAT;
    // polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    
    // polygonLayout[ 1 ].SemanticName      = "TEXCOORD";
    // polygonLayout[ 1 ].Format            = DXGI_FORMAT_R32G32_FLOAT;
    // polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    // polygonLayout[ 1 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    // polygonLayout[ 2 ].SemanticName      = "BLENDINDICES";
    // polygonLayout[ 2 ].Format            = DXGI_FORMAT_R32G32B32A32_UINT;
    // polygonLayout[ 2 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    // polygonLayout[ 2 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    // polygonLayout[ 3 ].SemanticName      = "BLENDWEIGHT";
    // polygonLayout[ 3 ].Format            = DXGI_FORMAT_R32G32B32A32_FLOAT;
    // polygonLayout[ 3 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    // polygonLayout[ 3 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    // ID3D11InputLayout * layout      = 0;
    // status = device->CreateInputLayout( polygonLayout, 4, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout );
    // if( status != S_OK ) {
    // result = false;
    // CONSOLE_STRING( "ERROR! RENDERER failed to create an input layout for the vertex shader!\n" );
    // }
    
    // vertexShaderBuffer->Release();
    // pixelShaderBuffer->Release();
    
    D3D11_BUFFER_DESC buffer_cameraTransformDesc = {};
    buffer_cameraTransformDesc.Usage          = D3D11_USAGE_DYNAMIC;
    buffer_cameraTransformDesc.ByteWidth      = sizeof( mat4 );
    buffer_cameraTransformDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    buffer_cameraTransformDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    ID3D11Buffer * buffer_cameraTransform = 0;
    status = device->CreateBuffer( &buffer_cameraTransformDesc, 0, &buffer_cameraTransform );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create the transform buffer for the shader!\n" );
    }
    
    D3D11_BUFFER_DESC colorBufferDesc = {};
    colorBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
    colorBufferDesc.ByteWidth      = sizeof( RENDER_DATA__MOD_COLOR );
    colorBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    ID3D11Buffer * buffer_modColor = 0;
    status = device->CreateBuffer( &colorBufferDesc, 0, &buffer_modColor );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create the color buffer!\n" );
    }
    
    // D3D11_BUFFER_DESC offsetBufferDesc = {};
    // offsetBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
    // offsetBufferDesc.ByteWidth      = sizeof( RENDER_DATA__OFFSET );
    // offsetBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    // offsetBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    // ID3D11Buffer * buffer_offset = 0;
    // status = device->CreateBuffer( &offsetBufferDesc, 0, &buffer_offset );
    // if( status != S_OK ) {
    // result = false;
    // CONSOLE_STRING( "ERROR! RENDERER failed to create the offset buffer!\n" );
    // }
    
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MaxAnisotropy  = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;
    
    ID3D11SamplerState * sampleState = 0;
    status = device->CreateSamplerState( &samplerDesc, &sampleState );
    if( status != S_OK ) {
        result = false;
        CONSOLE_STRING( "ERROR! RENDERER failed to create a sampler state for the pixel shader!\n" );
    }
    deviceContext->PSSetSamplers( 0, 1, &sampleState );
    
    renderer->device           = device;
    renderer->deviceContext    = deviceContext;
    renderer->swapChain        = swapChain;
    renderer->renderTargetView = renderTargetView;
    renderer->depthStencilView = depthStencilView;
    renderer->isFullscreen     = isFullscreen;
    
    renderer->depthState_enable  = DepthState( device, true  );
    renderer->depthState_disable = DepthState( device, false );
    renderer->blendState_enable  = BlendState( device, true  );
    renderer->blendState_disable = BlendState( device, false );
    
    renderer->buffer_cameraTransform = buffer_cameraTransform;
    renderer->buffer_modColor        = buffer_modColor;
    
    // renderer->vertexShader[ 1 ] = vertexShader;
    // renderer->pixelShader[ 1 ]  = pixelShader;
    // renderer->vertexLayout[ 1 ]       = layout;
    // renderer->vertex_stride[ 1 ] = sizeof( VERTEX_TEXMESH );
    
#if DEBUG_BUILD
    const char * debug_vertexCode = "\
        cbuffer MatrixBuffer {\
        matrix transform;\
    };\
    \
    struct VertexInputType {\
        float4 position : POSITION;\
        float2 tex      : TEXCOORD0;\
    };\
    \
    struct PixelInputType {\
        float4 position : SV_POSITION;\
        float2 tex      : TEXCOORD0;\
    };\
    \
    PixelInputType TextureVertexShader( VertexInputType input ) {\
        PixelInputType output;\
        input.position.w = 1.0f;\
        output.position = mul( input.position, transform );\
        output.tex = input.tex;\
        return output;\
    }";
        
        const char * debug_pixelCode = "\
        Texture2D    shaderTexture;\
    SamplerState SampleType;\
    \
    cbuffer ColorBuffer {\
        float4 modColor;\
    };\
    \
    struct PixelInputType {\
        float4 position : SV_POSITION;\
        float2 tex      : TEXCOORD0;\
    };\
    \
    float4 TexturePixelShader( PixelInputType input ) : SV_TARGET {\
        float4 textureColor = shaderTexture.Sample( SampleType, input.tex );\
        textureColor.r *= modColor.r;\
        textureColor.g *= modColor.g;\
        textureColor.b *= modColor.b;\
        textureColor.a *= modColor.a;\
        return textureColor;\
    }";
        
        ID3D10Blob * debug_vertexShaderBuffer = 0;
    status = D3DCompile( debug_vertexCode, strlen( debug_vertexCode ), 0, 0, 0, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &debug_vertexShaderBuffer, &errorMessage );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to compile the debug vertex shader!\n" );
    }
    
    ID3D10Blob * debug_pixelShaderBuffer  = 0;
    status = D3DCompile( debug_pixelCode, strlen( debug_pixelCode ), 0, 0, 0, "TexturePixelShader",  "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &debug_pixelShaderBuffer, &errorMessage );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to compile the debug pixel shader!\n" );
    }
    
    ID3D11VertexShader * debug_vertexShader = 0;
    status = device->CreateVertexShader( debug_vertexShaderBuffer->GetBufferPointer(), debug_vertexShaderBuffer->GetBufferSize(), 0, &debug_vertexShader );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create the debug vertex shader!\n" );
    }
    
    ID3D11PixelShader * debug_pixelShader = 0;
    status = device->CreatePixelShader( debug_pixelShaderBuffer->GetBufferPointer(), debug_pixelShaderBuffer->GetBufferSize(), 0, &debug_pixelShader );
    if( status != S_OK ) {
        CONSOLE_STRING( "ERROR! RENDERER failed to create the debug pixel shader!\n" );
    }
    
    D3D11_INPUT_ELEMENT_DESC debug_polygonLayout[ 2 ] = {};
    debug_polygonLayout[ 0 ].SemanticName   = "POSITION";
    debug_polygonLayout[ 0 ].Format         = DXGI_FORMAT_R32G32_FLOAT;
    debug_polygonLayout[ 0 ].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    
    debug_polygonLayout[ 1 ].SemanticName      = "TEXCOORD";
    debug_polygonLayout[ 1 ].Format            = DXGI_FORMAT_R32G32_FLOAT;
    debug_polygonLayout[ 1 ].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    debug_polygonLayout[ 1 ].InputSlotClass    = D3D11_INPUT_PER_VERTEX_DATA;
    
    ID3D11InputLayout * debug_layout = 0;
    status = device->CreateInputLayout( debug_polygonLayout, 2, debug_vertexShaderBuffer->GetBufferPointer(), debug_vertexShaderBuffer->GetBufferSize(), &debug_layout );
    if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to create an debug input layout for the debug vertex shader!\n" ); }
    
    debug_vertexShaderBuffer->Release();
    debug_pixelShaderBuffer->Release();
    
    renderer->vertexShader[ 0 ]    = debug_vertexShader;
    renderer->pixelShader[ 0 ]     = debug_pixelShader;
    renderer->vertexLayout[ 0 ]          = debug_layout;
    renderer->vertex_stride[ 0 ] = sizeof( DEBUG_VERTEX );
#endif // DEBUG_BUILD
    return result;
}

internal ID3D11Texture2D *
loadTexture( ID3D11Device * device, int32 texture_width, int32 texture_height, void * texture_data ) {
    ID3D11Texture2D * result = 0;
    
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width            = texture_width;
    textureDesc.Height           = texture_height;
    textureDesc.MipLevels        = 1;
    textureDesc.ArraySize        = 1;
    textureDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage            = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
    
    D3D11_SUBRESOURCE_DATA subresourceData = {};
    subresourceData.pSysMem          = texture_data;
    subresourceData.SysMemPitch      = texture_width * 4;
    subresourceData.SysMemSlicePitch = texture_width * texture_height * 4;
    
    HRESULT status = device->CreateTexture2D( &textureDesc, &subresourceData, &result );
    Assert( status == S_OK );
    
    return result;
}

internal void
addResource( RENDERER * renderer, ASSET_ID textureID, ID3D11ShaderResourceView * texture ) {
    Assert( renderer->loadedTexture_count < RENDERER__MAX_LOADED_TEXTURE_COUNT );
    renderer->loadedTexture_id[       renderer->loadedTexture_count ] = textureID;
    renderer->loadedTexture_resource[ renderer->loadedTexture_count ] = texture;
    renderer->loadedTexture_count++;
}

internal ID3D11ShaderResourceView *
loadShaderResource( ID3D11Device * device, ID3D11Texture2D * texture ) {
    ID3D11ShaderResourceView * result = 0;
    
    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
    resourceDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
    resourceDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceDesc.Texture2D.MostDetailedMip = 0;
    resourceDesc.Texture2D.MipLevels       = 1;
    
    HRESULT status = device->CreateShaderResourceView( texture, &resourceDesc, &result );
    Assert( status == S_OK );
    
    return result;
}

// internal void
// render( RENDERER * renderer, ASSET_SYSTEM * assetSystem, RENDER_OBJECT * object_array, int32 object_count ) {
// for( int32 object_index = 0; object_index < object_count; object_index++ ) {
// RENDER_OBJECT * object = object_array + object_index;

// D3D11_PRIMITIVE_TOPOLOGY drawType = {};
// switch( object->drawType ) {
// case meshDrawType_triangles:     { drawType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;  } break;
// case meshDrawType_triangleStrip: { drawType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; } break;
// case meshDrawType_lines:         { drawType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;      } break;
// case meshDrawType_lineStrip:     { drawType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;     } break;

// default: { InvalidCodePath; } break;
// }

// int32 loadedTexture_index = findLoadedTexture( object->textureID, renderer );
// Assert( loadedTexture_index > -1 );

// ID3D11ShaderResourceView * object_resource = renderer->loadedTexture_resource[ loadedTexture_index ];

// D3D11_MAPPED_SUBRESOURCE mappedResource = {};

// int32 status = renderer->deviceContext->Map( renderer->buffer_modColor, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
// if( status != S_OK ) {
// CONSOLE_STRING( "ERROR! RENDERER failed to map the transform buffer for the shader!\n" );
// }

// vec4 * modColor = ( vec4 * )mappedResource.pData;
// modColor[ 0 ] = object->color;
// renderer->deviceContext->Unmap( renderer->buffer_modColor, 0 );
// renderer->deviceContext->PSSetConstantBuffers( 0, 1, &renderer->buffer_modColor );

// if( object->bone_count > 0 ) {
// setModelTransform( renderer, object->bone_transform, object->bone_count );
// }

// renderer->deviceContext->IASetPrimitiveTopology( drawType );
// renderer->deviceContext->PSSetShaderResources( 0, 1, &object_resource );
// renderer->deviceContext->Draw( object->vertex_count, object->vertex_index );
// }
// }

// internal void
// render( RENDER_SYSTEM * renderSystem, RENDERER * renderer, ASSET_SYSTEM * assetSystem ) {
// VERTEX_BUFFER * vertexBuffer = renderSystem->vertexBuffer;
// renderer->deviceContext->VSSetShader( renderer->vertexShader[ 1 ], 0, 0 );
// renderer->deviceContext->PSSetShader( renderer->pixelShader[ 1 ],  0, 0 );
// renderer->deviceContext->OMSetDepthStencilState( renderer->depthState_enable, 1 );
// renderer->deviceContext->OMSetBlendState( renderer->blendState_enable, 0, 0xFFFFFFFF );

// uint32 stride = sizeof( VERTEX_TEXMESH );
// uint32 offset = 0;
// renderer->deviceContext->IASetVertexBuffers( 0, 1, &renderer->vertexBuffer[ 1 ], &stride, &offset );
// renderer->deviceContext->IASetInputLayout( renderer->vertexLayout[ 1 ] );

// setCamera( renderer, renderSystem->camera_transform );
// render( renderer, assetSystem, renderSystem->object, renderSystem->object_count );   renderSystem->object_count = 0;

// renderer->deviceContext->OMSetDepthStencilState( renderer->depthState_disable, 1 );
// render( renderer, assetSystem, renderSystem->debug,  renderSystem->debug_count  );   renderSystem->debug_count  = 0;

// vertexBuffer->temp_count  = 0;
// vertexBuffer->temp_select = ( vertexBuffer->temp_select + 1 ) % 3;
// }

internal void
clearRender( RENDERER * renderer, vec4 clearColor ) {
    renderer->deviceContext->ClearRenderTargetView( renderer->renderTargetView, clearColor.elem );
    renderer->deviceContext->ClearDepthStencilView( renderer->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

internal void
presentRender( RENDERER * renderer ) {
    renderer->swapChain->Present( 1, 0 );
}

internal void
end( RENDERER * renderer ) {
    if( renderer->isFullscreen ) {
        renderer->swapChain->SetFullscreenState( false, 0 );
        renderer->isFullscreen = false;
    }
}

// internal void
// debug_render( RENDERER * renderer, ASSET_SYSTEM * assetSystem, rect client_bound ) {
// // TODO: remove asset loading
// // TODO: preload any needed assets and store pointers to the shader resources in RENDERER ( ie. defaultFont, defaultTexture, whiteTexture, etc. )
// renderer->deviceContext->VSSetShader( renderer->debug_vertexShader, 0, 0 );
// renderer->deviceContext->PSSetShader( renderer->debug_pixelShader,  0, 0 );
// renderer->deviceContext->OMSetDepthStencilState( renderer->depthState_disable, 1 );
// renderer->deviceContext->OMSetBlendState( renderer->blendState_enable, 0, 0xFFFFFFFF );

// uint32 stride = sizeof( DEBUG_VERTEX );
// uint32 offset = 0;
// renderer->deviceContext->IASetVertexBuffers( 0, 1, &renderer->debug_vertexBuffer, &stride, &offset );
// renderer->deviceContext->IASetInputLayout( renderer->debug_layout );

// D3D11_MAPPED_SUBRESOURCE mappedResource = {};
// int32                    status         = 0;

// status = renderer->deviceContext->Map( renderer->debug_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
// if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to map the vertex buffer for the debug vertices!\n" ); }
// memcpy( mappedResource.pData, renderer->debug_vertex, sizeof( DEBUG_VERTEX ) * renderer->debug_vertexCount );
// renderer->deviceContext->Unmap( renderer->debug_vertexBuffer, 0 );

// mat4 camera_transform = mat4_orthographic( client_bound.left, client_bound.bottom, client_bound.right, client_bound.top, 0.0f, 1.0f );
// camera_transform = mat4_transpose( camera_transform );

// status = renderer->deviceContext->Map( renderer->debug_transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
// if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to map the transform buffer for the debug shader!\n" ); }
// *( ( mat4 * )mappedResource.pData ) = camera_transform;
// renderer->deviceContext->Unmap( renderer->debug_transformBuffer, 0 );
// renderer->deviceContext->VSSetConstantBuffers( 0, 1, &renderer->debug_transformBuffer );

// for( int32 object_index = 0; object_index < renderer->debug_objectCount; object_index++ ) {
// DEBUG_OBJECT * object = renderer->debug_object + object_index;

// D3D11_PRIMITIVE_TOPOLOGY drawType = {};
// switch( object->drawType ) {
// case meshDrawType_triangles:     { drawType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;  } break;
// case meshDrawType_triangleStrip: { drawType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; } break;
// case meshDrawType_lines:         { drawType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;      } break;
// case meshDrawType_lineStrip:     { drawType = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;     } break;

// default: { InvalidCodePath; } break;
// }

// ID3D11ShaderResourceView * object_resource = 0;
// if( object->textureID == assetID_font_default ) {
// object_resource = renderer->resource_defaultFont;
// } else if( object->textureID == assetID_texture_default ) {
// object_resource = renderer->resource_defaultTexture;
// } else {
// int32 loadedTexture_index = findLoadedTexture( object->textureID, renderer );
// if( loadedTexture_index == -1 ) {
// ASSET_ENTRY entry = assetSystem->entry[ object->textureID ];
// Assert( entry.size > 0 );

// ASSET_TEXTURE * texture = ( ASSET_TEXTURE * )( assetSystem->asset_data + entry.offset );
// ID3D11Texture2D          * d3d_texture  = loadTexture( renderer->device, texture->width, texture->height, ( texture + 1 ) );
// ID3D11ShaderResourceView * d3d_resource = loadShaderResource( renderer->device, d3d_texture );

// renderer->loadedTexture_id[       renderer->loadedTexture_count ] = object->textureID;
// renderer->loadedTexture_resource[ renderer->loadedTexture_count ] = d3d_resource;
// object_resource = d3d_resource;
// renderer->loadedTexture_count++;
// } else {
// object_resource = renderer->loadedTexture_resource[ loadedTexture_index ];
// }
// }

// status = renderer->deviceContext->Map( renderer->debug_colorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
// if( status != S_OK ) { CONSOLE_STRING( "ERROR! RENDERER failed to map the color buffer for the debug shader!\n" ); }
// *( ( vec4 * )mappedResource.pData ) = object->color;
// renderer->deviceContext->Unmap( renderer->debug_colorBuffer, 0 );
// renderer->deviceContext->PSSetConstantBuffers( 0, 1, &renderer->debug_colorBuffer );

// renderer->deviceContext->IASetPrimitiveTopology( drawType );
// renderer->deviceContext->PSSetShaderResources( 0, 1, &object_resource );
// renderer->deviceContext->Draw( object->vertex_count, object->vertex_index );
// }
// renderer->debug_objectCount = 0;
// renderer->debug_vertexCount = 0;
// }
#endif	// STD_INCLUDE_FUNC