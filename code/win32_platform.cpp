
#ifdef	STD_INCLUDE_DECL

struct FILE_DATA {
	uint32 size;
	void * contents;
};

typedef boo32     ( WRITE_FILE )( const char * saveDir, const char * filename, void * data, uint32 size );
typedef FILE_DATA ( READ_FILE )( MEMORY * memory, char * saveDir, char * filename );
typedef boo32     ( READ_FILE_CHUNK  )( uint64 fileHandle, uint32 offset, uint32 size, void * buffer );
typedef boo32     ( READ_ASSET_CHUNK )( uint64 fileHandle, uint32 offset, uint32 size, void * buffer );
typedef void      ( END_APPLICATION  )( void * platform, void * appState );

struct PLATFORM {
	MOUSE_STATE      mouse;
	KEYBOARD_STATE   keyboard;
    CONTROLLER_STATE controller;
	
	MEMORY permMemory;
	MEMORY tempMemory;
	
    WRITE_FILE       * writeFile;
    READ_FILE        * readFile;
	READ_FILE_CHUNK  * readFile_chunk;
	READ_ASSET_CHUNK * readAsset_chunk;
    END_APPLICATION  * endApplication;
	
    AUDIO_SYSTEM     audioSystem;
    MUSIC_SYSTEM     musicSystem;
	ASSET_SYSTEM     assetSystem;
    
    RENDERER      renderer;
	// RENDER_SYSTEM renderSystem;
	// VERTEX_BUFFER vertexBuffer;
    
	flo32 targetSec;
};

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

internal boo32
win32_writeFile( const char * saveDir, const char * filename, void * data, uint32 size ) {
	boo32 result = false;
	
	char filePath[ 2048 ] = {};
	if( saveDir ) {
		sprintf( filePath, "%s/%s", saveDir, filename );
	} else {
		sprintf( filePath, "%s", filename );
	}
    
	HANDLE fileHandle = CreateFile( filePath, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
	if( fileHandle != INVALID_HANDLE_VALUE ) {
		DWORD bytesWritten;
		if( WriteFile( fileHandle, data, size, &bytesWritten, 0 ) ) {
			result = ( bytesWritten == size );
		}
        
		CloseHandle( fileHandle );
	}
    
	return result;
}

internal FILE_DATA
win32_readFile( MEMORY * memory, char * saveDir, char * filename ) {
	FILE_DATA result = {};
	
	char filePath[ 2048 ] = {};
	if( saveDir ) {
		sprintf( filePath, "%s/%s", saveDir, filename );
	} else {
		sprintf( filePath, "%s", filename );
	}
	
	HANDLE fileHandle = CreateFile( filePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if( fileHandle != INVALID_HANDLE_VALUE ) {
		LARGE_INTEGER fileSize;
		if( GetFileSizeEx( fileHandle, &fileSize ) ) {
			result.size = truncToU32( fileSize.QuadPart );
			result.contents = _pushSize_clear( memory, result.size );
			
			DWORD bytesRead;
			if( ReadFile( fileHandle, result.contents, result.size, &bytesRead, 0 ) && ( result.size == bytesRead ) ) {
			} else {
				_popSize( memory, result.size );
				result.contents = 0;
				OutputDebugString( "ERROR. Unable to read file contents.\n" );
			}
			CloseHandle( fileHandle );
		} else {
			OutputDebugString( "ERROR. File does not contain any data.\n" );
		}
	} else {
		char debugStr[128] = {};
		sprintf( debugStr, "ERROR. Unable to open file: %s\n", filename );
		OutputDebugString( debugStr );
	}
	
	return result;
}

internal boo32
win32_readFile_chunk( uint64 fileHandle, uint32 offset, uint32 size, void * buffer ) {
	boo32 result = false;
	if( fileHandle ) {
		// TODO: check that file handle is still open
		DWORD error = SetFilePointer( ( HANDLE )fileHandle, offset, 0, FILE_BEGIN );
		if( error != INVALID_SET_FILE_POINTER ) {
			DWORD bytesRead = 0;
			ReadFile( ( HANDLE )fileHandle, buffer, size, &bytesRead, 0 );
			if( bytesRead == size ) {
				result = true;
			} else {
				CONSOLE_STRING( "ERROR. Error reading in requested file chunk." );
			}
		} else {
			CONSOLE_STRING( "ERROR. Unable to set file pointer in requested file." );
		}
	} else {
		CONSOLE_STRING( "ERROR. fileHandle is not valid." );
	}
	return result;
}

internal int64
win32_getPerfCount() {
	LARGE_INTEGER perfCounter = {};
	QueryPerformanceCounter( &perfCounter );
	
	int64 result = *( ( int64 * )&perfCounter );
	return result;
}

internal flo32
win32_getMSElapsed( int64 startCounter, int64 endCounter, int64 perfFrequency ) {
	LARGE_INTEGER counterA  = *( ( LARGE_INTEGER * )&startCounter  );
	LARGE_INTEGER counterB  = *( ( LARGE_INTEGER * )&endCounter    );
	LARGE_INTEGER frequency = *( ( LARGE_INTEGER * )&perfFrequency );
	flo32 result = 1000.0f * ( ( flo32 )( counterB.QuadPart - counterA.QuadPart ) / ( flo32 )( frequency.QuadPart ) );
	return result;
}

global_variable boo32 global_mouseFocus = 0;

LRESULT CALLBACK
win32_WindowProc( HWND window, uint32 message, WPARAM wParam, LPARAM lParam ) {
	LRESULT result = 0;
	switch( message ) {
        case WM_ACTIVATEAPP: {
            if( wParam == TRUE ) {
                global_mouseFocus = true;
            } else {
                global_mouseFocus = false;
            }
        } break;
        
		case WM_CLOSE: {
		} break;
        
		case WM_DESTROY: {
		} break;
		
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP: {
			Assert( !"ERROR! Keyboard message was passed directly to WindowProc! All keyboard messages should be handled directly by the game!" );
		} break;
		
		default: {
			result = DefWindowProc( window, message, wParam, lParam );
		} break;
	}
	return result;
}

internal void
win32_processPendingMessages( PLATFORM * platform, HWND windowHandle ) {
	// PROFILE_FUNCTION();
	
	KEYBOARD_STATE * keyboard = &platform->keyboard;
	keyboard->event_count = 0;
	
	MOUSE_STATE * mouse = &platform->mouse;
	MOUSE_endOfFrame( mouse );
	
	MSG message;
	while( PeekMessage( &message, 0, 0, 0, PM_REMOVE ) ) {
		switch( message.message ) {
            case WM_ACTIVATEAPP: {
                if( message.wParam == TRUE ) {
                    global_mouseFocus = true;
                } else {
                    global_mouseFocus = false;
                }
            } break;
            
			case WM_MOUSEWHEEL: {
				int16 wParam_hi = ( ( message.wParam >> 16 ) & 0xFFFF );
				int32 wheelClicks = wParam_hi / 120;
				mouse->wheelClicks = wheelClicks;
			} break;
			
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP: {
				platform->mouse.control_isDown = ( message.wParam & MK_CONTROL );
				platform->mouse.shift_isDown = ( message.wParam & MK_SHIFT );
				processButtonEvent( mouse, mouseButton_left, ( message.wParam & MK_LBUTTON ) );
			} break;
			
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP: {
				platform->mouse.control_isDown = ( message.wParam & MK_CONTROL );
				platform->mouse.shift_isDown = ( message.wParam & MK_SHIFT );
				processButtonEvent( mouse, mouseButton_middle, ( message.wParam & MK_MBUTTON ) );
			} break;
			
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN: {
				platform->mouse.control_isDown = ( message.wParam & MK_CONTROL );
				platform->mouse.shift_isDown = ( message.wParam & MK_SHIFT );
				processButtonEvent( mouse, mouseButton_right, ( message.wParam & MK_RBUTTON ) );
			} break;
			
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP: {
                boo32 wasDown = ( ( message.lParam & ( 1 << 30 ) ) != 0 );
				boo32 isDown = ( ( message.lParam & ( 1 << 31 ) ) == 0 );
				if( isDown != wasDown ) {
                    switch( message.wParam ) {
						case VK_BACK: 		{ addKeyEvent( keyboard, keyCode_backspace, 	isDown, keyboard->flags ); } break;
						case VK_TAB: 		{ addKeyEvent( keyboard, keyCode_tab, 			isDown, keyboard->flags ); } break;
						case VK_RETURN:	{ addKeyEvent( keyboard, keyCode_enter, 		isDown, keyboard->flags ); } break;
						case VK_SHIFT: 	{
                            if( isDown ) {
                                keyboard->flags |=  KEYBOARD_FLAGS__SHIFT;
                            } else {
                                keyboard->flags &= ~KEYBOARD_FLAGS__SHIFT;
                            }
							addKeyEvent( keyboard, keyCode_shift, isDown, keyboard->flags );
						} break;
						case VK_CONTROL: 	{
                            if( isDown ) {
                                keyboard->flags |=  KEYBOARD_FLAGS__CONTROL;
                            } else {
                                keyboard->flags &= ~KEYBOARD_FLAGS__CONTROL;
                            }
							addKeyEvent( keyboard, keyCode_control, isDown, keyboard->flags );
						} break;
						case VK_MENU: 		{
                            if( isDown ) {
                                keyboard->flags |=  KEYBOARD_FLAGS__ALT;
                            } else {
                                keyboard->flags &= ~KEYBOARD_FLAGS__ALT;
                            }
                            addKeyEvent( keyboard, keyCode_alt, isDown, keyboard->flags );
                        } break;
						case VK_CAPITAL: 	{ addKeyEvent( keyboard, keyCode_capsLock, 	isDown, keyboard->flags ); } break;
						case VK_ESCAPE: 	{ addKeyEvent( keyboard, keyCode_escape, 		isDown, keyboard->flags ); } break;
						case VK_SPACE: 	{ addKeyEvent( keyboard, keyCode_space, 		isDown, keyboard->flags ); } break;
						case VK_PRIOR: 	{ addKeyEvent( keyboard, keyCode_pageUp, 		isDown, keyboard->flags ); } break;
						case VK_NEXT: 		{ addKeyEvent( keyboard, keyCode_pageDown, 	isDown, keyboard->flags ); } break;
						case VK_END: 		{ addKeyEvent( keyboard, keyCode_end, 			isDown, keyboard->flags ); } break;
						case VK_HOME: 		{ addKeyEvent( keyboard, keyCode_home, 		isDown, keyboard->flags ); } break;
						case VK_LEFT: 		{ addKeyEvent( keyboard, keyCode_left, 		isDown, keyboard->flags ); } break;
						case VK_RIGHT: 	{ addKeyEvent( keyboard, keyCode_right, 		isDown, keyboard->flags ); } break;
						case VK_UP: 		{ addKeyEvent( keyboard, keyCode_up, 			isDown, keyboard->flags ); } break;
						case VK_DOWN: 		{ addKeyEvent( keyboard, keyCode_down,       isDown, keyboard->flags ); } break;
						case VK_INSERT: 	{ addKeyEvent( keyboard, keyCode_insert, 		isDown, keyboard->flags ); } break;
						case VK_DELETE: 	{ addKeyEvent( keyboard, keyCode_delete, 		isDown, keyboard->flags ); } break;
						case 0x30: { addKeyEvent( keyboard, keyCode_0, isDown, keyboard->flags ); } break;
						case 0x31: { addKeyEvent( keyboard, keyCode_1, isDown, keyboard->flags ); } break;
						case 0x32: { addKeyEvent( keyboard, keyCode_2, isDown, keyboard->flags ); } break;
						case 0x33: { addKeyEvent( keyboard, keyCode_3, isDown, keyboard->flags ); } break;
						case 0x34: { addKeyEvent( keyboard, keyCode_4, isDown, keyboard->flags ); } break;
						case 0x35: { addKeyEvent( keyboard, keyCode_5, isDown, keyboard->flags ); } break;
						case 0x36: { addKeyEvent( keyboard, keyCode_6, isDown, keyboard->flags ); } break;
						case 0x37: { addKeyEvent( keyboard, keyCode_7, isDown, keyboard->flags ); } break;
						case 0x38: { addKeyEvent( keyboard, keyCode_8, isDown, keyboard->flags ); } break;
						case 0x39: { addKeyEvent( keyboard, keyCode_9, isDown, keyboard->flags ); } break;
						case 0x41: { addKeyEvent( keyboard, keyCode_a, isDown, keyboard->flags ); } break;
						case 0x42: { addKeyEvent( keyboard, keyCode_b, isDown, keyboard->flags ); } break;
						case 0x43: { addKeyEvent( keyboard, keyCode_c, isDown, keyboard->flags ); } break;
						case 0x44: { addKeyEvent( keyboard, keyCode_d, isDown, keyboard->flags ); } break;
						case 0x45: { addKeyEvent( keyboard, keyCode_e, isDown, keyboard->flags ); } break;
						case 0x46: { addKeyEvent( keyboard, keyCode_f, isDown, keyboard->flags ); } break;
						case 0x47: { addKeyEvent( keyboard, keyCode_g, isDown, keyboard->flags ); } break;
						case 0x48: { addKeyEvent( keyboard, keyCode_h, isDown, keyboard->flags ); } break;
						case 0x49: { addKeyEvent( keyboard, keyCode_i, isDown, keyboard->flags ); } break;
						case 0x4A: { addKeyEvent( keyboard, keyCode_j, isDown, keyboard->flags ); } break;
						case 0x4B: { addKeyEvent( keyboard, keyCode_k, isDown, keyboard->flags ); } break;
						case 0x4C: { addKeyEvent( keyboard, keyCode_l, isDown, keyboard->flags ); } break;
						case 0x4D: { addKeyEvent( keyboard, keyCode_m, isDown, keyboard->flags ); } break;
						case 0x4E: { addKeyEvent( keyboard, keyCode_n, isDown, keyboard->flags ); } break;
						case 0x4F: { addKeyEvent( keyboard, keyCode_o, isDown, keyboard->flags ); } break;
						case 0x50: { addKeyEvent( keyboard, keyCode_p, isDown, keyboard->flags ); } break;
						case 0x51: { addKeyEvent( keyboard, keyCode_q, isDown, keyboard->flags ); } break;
						case 0x52: { addKeyEvent( keyboard, keyCode_r, isDown, keyboard->flags ); } break;
						case 0x53: { addKeyEvent( keyboard, keyCode_s, isDown, keyboard->flags ); } break;
						case 0x54: { addKeyEvent( keyboard, keyCode_t, isDown, keyboard->flags ); } break;
						case 0x55: { addKeyEvent( keyboard, keyCode_u, isDown, keyboard->flags ); } break;
						case 0x56: { addKeyEvent( keyboard, keyCode_v, isDown, keyboard->flags ); } break;
						case 0x57: { addKeyEvent( keyboard, keyCode_w, isDown, keyboard->flags ); } break;
						case 0x58: { addKeyEvent( keyboard, keyCode_x, isDown, keyboard->flags ); } break;
						case 0x59: { addKeyEvent( keyboard, keyCode_y, isDown, keyboard->flags ); } break;
						case 0x5A: { addKeyEvent( keyboard, keyCode_z, isDown, keyboard->flags ); } break;
						case VK_OEM_1: { addKeyEvent( keyboard, keyCode_semicolon, isDown, keyboard->flags ); } break;
						case VK_OEM_PLUS: { addKeyEvent( keyboard, keyCode_equal, isDown, keyboard->flags ); } break;
						case VK_OEM_COMMA: { addKeyEvent( keyboard, keyCode_comma, isDown, keyboard->flags ); } break;
						case VK_OEM_MINUS: { addKeyEvent( keyboard, keyCode_dash, isDown, keyboard->flags ); } break;
						case VK_OEM_PERIOD: { addKeyEvent( keyboard, keyCode_period, isDown, keyboard->flags ); } break;
						case VK_OEM_2: { addKeyEvent( keyboard, keyCode_forwardSlash, isDown, keyboard->flags ); } break;
						case VK_OEM_3: { addKeyEvent( keyboard, keyCode_tilde, isDown, keyboard->flags ); } break;
						case VK_OEM_4: { addKeyEvent( keyboard, keyCode_openBracket, isDown, keyboard->flags ); } break;
						case VK_OEM_5: { addKeyEvent( keyboard, keyCode_backslash, isDown, keyboard->flags ); } break;
						case VK_OEM_6: { addKeyEvent( keyboard, keyCode_closeBracket, isDown, keyboard->flags ); } break;
						case VK_OEM_7: { addKeyEvent( keyboard, keyCode_quote, isDown, keyboard->flags ); } break;
						case VK_NUMPAD0: { addKeyEvent( keyboard, keyCode_num0, isDown, keyboard->flags ); } break;
						case VK_NUMPAD1: { addKeyEvent( keyboard, keyCode_num1, isDown, keyboard->flags ); } break;
						case VK_NUMPAD2: { addKeyEvent( keyboard, keyCode_num2, isDown, keyboard->flags ); } break;
						case VK_NUMPAD3: { addKeyEvent( keyboard, keyCode_num3, isDown, keyboard->flags ); } break;
						case VK_NUMPAD4: {addKeyEvent( keyboard, keyCode_num4, isDown, keyboard->flags ); } break;
						case VK_NUMPAD5: { addKeyEvent( keyboard, keyCode_num5, isDown, keyboard->flags ); } break;
						case VK_NUMPAD6: { addKeyEvent( keyboard, keyCode_num6, isDown, keyboard->flags ); } break;
						case VK_NUMPAD7: { addKeyEvent( keyboard, keyCode_num7, isDown, keyboard->flags ); } break;
						case VK_NUMPAD8: { addKeyEvent( keyboard, keyCode_num8, isDown, keyboard->flags ); } break;
						case VK_NUMPAD9: { addKeyEvent( keyboard, keyCode_num9, isDown, keyboard->flags ); } break;
						case VK_MULTIPLY: { addKeyEvent( keyboard, keyCode_numMul, isDown, keyboard->flags ); } break;
						case VK_ADD: { addKeyEvent( keyboard, keyCode_numAdd, isDown, keyboard->flags ); } break;
						case VK_DECIMAL: { addKeyEvent( keyboard, keyCode_numPeriod, isDown, keyboard->flags ); } break;
						case VK_SUBTRACT: { addKeyEvent( keyboard, keyCode_numSub, isDown, keyboard->flags ); } break;
						case VK_DIVIDE: { addKeyEvent( keyboard, keyCode_numDiv, isDown, keyboard->flags ); } break;
						case VK_F1:      { addKeyEvent( keyboard, keyCode_F1,   isDown, keyboard->flags ); } break;
						case VK_F2:      { addKeyEvent( keyboard, keyCode_F2,   isDown, keyboard->flags ); } break;
						case VK_F3:      { addKeyEvent( keyboard, keyCode_F3,   isDown, keyboard->flags ); } break;
						case VK_F4:      { addKeyEvent( keyboard, keyCode_F4,   isDown, keyboard->flags ); } break;
						case VK_F5:      { addKeyEvent( keyboard, keyCode_F5,   isDown, keyboard->flags ); } break;
						case VK_F6:      { addKeyEvent( keyboard, keyCode_F6,   isDown, keyboard->flags ); } break;
						case VK_F7:      { addKeyEvent( keyboard, keyCode_F7,   isDown, keyboard->flags ); } break;
						case VK_F8:      { addKeyEvent( keyboard, keyCode_F8,   isDown, keyboard->flags ); } break;
						case VK_F9:      { addKeyEvent( keyboard, keyCode_F9,   isDown, keyboard->flags ); } break;
						case VK_F10:     { addKeyEvent( keyboard, keyCode_F10,  isDown, keyboard->flags ); } break;
						case VK_F11:     { addKeyEvent( keyboard, keyCode_F11,  isDown, keyboard->flags ); } break;
						case VK_F12:     { addKeyEvent( keyboard, keyCode_F12,  isDown, keyboard->flags ); } break;
						case VK_F13:     { addKeyEvent( keyboard, keyCode_F13,  isDown, keyboard->flags ); } break;
						case VK_F14:     { addKeyEvent( keyboard, keyCode_F14,  isDown, keyboard->flags ); } break;
						case VK_F15:     { addKeyEvent( keyboard, keyCode_F15,  isDown, keyboard->flags ); } break;
						case VK_F16:     { addKeyEvent( keyboard, keyCode_F16,  isDown, keyboard->flags ); } break;
						case VK_F17:     { addKeyEvent( keyboard, keyCode_F17,  isDown, keyboard->flags ); } break;
						case VK_F18:     { addKeyEvent( keyboard, keyCode_F18,  isDown, keyboard->flags ); } break;
						case VK_F19:     { addKeyEvent( keyboard, keyCode_F19,  isDown, keyboard->flags ); } break;
						case VK_F20:     { addKeyEvent( keyboard, keyCode_F20,  isDown, keyboard->flags ); } break;
						case VK_F21:     { addKeyEvent( keyboard, keyCode_F21,  isDown, keyboard->flags ); } break;
						case VK_F22:     { addKeyEvent( keyboard, keyCode_F22,  isDown, keyboard->flags ); } break;
						case VK_F23:     { addKeyEvent( keyboard, keyCode_F23,  isDown, keyboard->flags ); } break;
						case VK_F24:     { addKeyEvent( keyboard, keyCode_F24,  isDown, keyboard->flags ); } break;
						default: {
							char string[ 128 ] = {};
							sprintf( string, "Key message received, but not processed: %lu %s\n", (uint32)message.wParam, ( isDown ? "PRESSED" : "RELEASED" ) );
							OutputDebugString( string );
						} break;
					};
				}
			} break;
			
			default: {
				TranslateMessage( &message );
				DispatchMessage( &message );
			} break;
		}
	}
}

internal void
win32_endApplication( void * platform, void * appState ) {
	APP_STATE * myAppState = ( APP_STATE * )appState;
	myAppState->isRunning = false;
}

typedef HRESULT WINAPI DIRECT_SOUND_CREATE( LPCGUID lpcGuidDevice, LPDIRECTSOUND * ppDS, LPUNKNOWN lpUnkOuter );

typedef DWORD XINPUT_GET_STATE( DWORD controllerIndex, XINPUT_STATE     * controllerState     );
typedef DWORD XINPUT_SET_STATE( DWORD controllerIndex, XINPUT_VIBRATION * controllerVibration );

int32 CALLBACK
WinMain( HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int32 windowShowCode ) {
    // #if DEBUG_BUILD
    // AttachConsole( ( DWORD )( -1 ) );
    // freopen( "CON", "w", stdout );
    // printf( "\n" );
    // #endif // DEBUG_BUILD
    
	srand( ( uint32 )time( 0 ) );
	
	boo32 sleepFreqIsHiRes = ( timeBeginPeriod( 1 ) == TIMERR_NOERROR );
	
	LARGE_INTEGER perfFrequency;
	QueryPerformanceFrequency( &perfFrequency );
    
    MEMORY platformMemory = {};
    platformMemory.size = ( sizeof( APP_STATE ) + sizeof( EDITOR_STATE ) + sizeof( PLATFORM ) + APP_permMemorySize + APP_tempMemorySize );
    platformMemory.base = VirtualAlloc( 0, platformMemory.size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE );
    
    APP_STATE * appState = _pushType( &platformMemory, APP_STATE );
    appState->isRunning = true;
    
    EDITOR_STATE * editorState = _pushType( &platformMemory, EDITOR_STATE );
    
    PLATFORM * win32_platform = _pushType( &platformMemory, PLATFORM );
    win32_platform->permMemory = { _pushSize( &platformMemory, APP_permMemorySize ), APP_permMemorySize };
    win32_platform->tempMemory = { _pushSize( &platformMemory, APP_tempMemorySize ), APP_tempMemorySize };
    win32_platform->writeFile       = win32_writeFile;
    win32_platform->readFile        = win32_readFile;
    win32_platform->readFile_chunk  = win32_readFile_chunk;
    win32_platform->readAsset_chunk = win32_readFile_chunk;
    win32_platform->endApplication  = win32_endApplication;
    win32_platform->targetSec = 1.0f / APP_targetFPS;
    
    RENDERER      * renderer     = &win32_platform->renderer;
    ASSET_SYSTEM  * assetSystem  = &win32_platform->assetSystem;
    AUDIO_SYSTEM  * audioSystem  = &win32_platform->audioSystem;
    MUSIC_SYSTEM  * musicSystem  = &win32_platform->musicSystem;
    renderer->assetSystem      = assetSystem;
    audioSystem->assetSystem   = assetSystem;
    musicSystem->assetSystem   = assetSystem;
	
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc   = win32_WindowProc;
	windowClass.hInstance     = instance;
	windowClass.lpszClassName = "WindowClass";
    windowClass.hCursor       = LoadCursor( 0, IDC_ARROW ); 
	
	if( RegisterClass( &windowClass ) ) {
        int32 screen_width  = GetSystemMetrics( SM_CXSCREEN );
		int32 screen_height = GetSystemMetrics( SM_CYSCREEN );
        
        DISPLAY_SETTINGS displaySettings = getDisplayAdapterMatch( &win32_platform->tempMemory, screen_width, screen_height, APP_targetFPS );
        
        uint32 window_flags = ( APP_isFullscreen ) ? ( WS_VISIBLE | WS_EX_TOPMOST | WS_POPUP ) : ( WS_VISIBLE );
		HWND window = CreateWindowEx( 0, "WindowClass", "Caption", window_flags, 0, 0, displaySettings.width, displaySettings.height, 0, 0, instance, 0 );
        
        // #if !DEBUG_BUILD
        // ShowCursor( FALSE );
        // #endif  // !DEBUG_BUILD
        
		if( window ) {
            RECT client_rect = {};
            GetClientRect( window, &client_rect );
			
            int32 client_width  = client_rect.right  - client_rect.left;
            int32 client_height = client_rect.bottom - client_rect.top;
            if( ( client_width  % 2 ) == 1 ) { client_width  += 1; }
            if( ( client_height % 2 ) == 1 ) { client_height += 1; }
			
            vec2 client_dim = Vec2( ( flo32 )client_width, ( flo32 )client_height );
            vec2 client_halfDim = client_dim * 0.5f;
            rect client_bound = Rect( ( flo32 )client_rect.left, ( flo32 )client_rect.top, ( flo32 )client_rect.right, ( flo32 )client_rect.bottom );
            
            if( !APP_isFullscreen ) {
                displaySettings.width  = client_width;
                displaySettings.height = client_height;
            }
            
            HANDLE assetFile_handle = CreateFile( "assets.pak", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
            if( assetFile_handle != INVALID_HANDLE_VALUE ) {
                LARGE_INTEGER _file_size = {};
                GetFileSizeEx( assetFile_handle, &_file_size );
                Assert( _file_size.HighPart == 0 );
                uint32 file_size = _file_size.LowPart;
                
                uint32 header_size = _arrayBytes( assetSystem->entry );
                uint32  asset_size = file_size - header_size;
                
                DWORD bytesRead = 0;
                ReadFile( assetFile_handle, assetSystem->entry, header_size, &bytesRead, 0 );
                Assert( bytesRead == header_size );
                
                assetSystem->asset_data = ( uint8 * )_pushSize( &win32_platform->permMemory, asset_size );
                ReadFile( assetFile_handle, assetSystem->asset_data, asset_size, &bytesRead, 0 );
                Assert( bytesRead == asset_size );
                
                boo32 result = CloseHandle( assetFile_handle );
                Assert( result );
            } else {
#if DEBUG_BUILD
                OutputDebugString( "ERROR: Unable to open asset file: assets.pak" );
#else          
                MessageBox( window, "The game is unable to find the file holding all the art!\n\nPlease, make sure the file 'assets.pak' is located in the same folder as the game file 'hexagon.exe'. Otherwise, you may need to reinstall the game!", "ERROR", MB_OK );
#endif // DEBUG_BUILD
                win32_endApplication( win32_platform, appState );
            }
            
			HDC win32_deviceContext = GetDC( window );
			
			vec2 app_dim     = Vec2( APP_width, APP_height );
            app_dim     = client_dim;
			vec2 app_halfDim = app_dim * 0.5f;
			rect app_bound   = rectBLD( Vec2( 0.0f, 0.0f ), app_dim );
			appState->app_dim     = app_dim;
			appState->app_halfDim = app_halfDim;
			appState->app_bound   = app_bound;
			editorState->app_dim     = app_dim;
			editorState->app_halfDim = app_halfDim;
			editorState->app_bound   = app_bound;
            
            boo32 success_initRenderer = init( renderer, window, displaySettings, APP_isFullscreen );
            if( !success_initRenderer ) {
                MessageBox( window, "ERROR! No renderer!", ERROR, MB_OK );
                win32_endApplication( win32_platform, appState );
            }
            
            initShader1( renderer );
            initShader2( renderer );
            
            MEMORY * permMemory = &win32_platform->permMemory;
            initVertexBuffer( renderer, 0, permMemory, sizeof( DEBUG_VERTEX ), 65536  );
            initVertexBuffer( renderer, 1, permMemory, sizeof( VERTEX1 ),      1048576 );
            initRenderPass( renderer, renderPassID_debug,           permMemory, 1024 );
            initRenderPass( renderer, renderPassID_main,            permMemory, 8192 );
            initRenderPass( renderer, renderPassID_main_debug,      permMemory, 512 );
            initRenderPass( renderer, renderPassID_main_noLighting, permMemory, 512 );
            initRenderPass( renderer, renderPassID_overlay,         permMemory, 512 );
            initRenderPass( renderer, renderPassID_scissor0,        permMemory, 512 );
            initRenderPass( renderer, renderPassID_scissor1,        permMemory, 512 );
            initRenderPass( renderer, renderPassID_scissor2,        permMemory, 512 );
            initRenderPass( renderer, renderPassID_scissor3,        permMemory, 512 );
            initRenderPass( renderer, renderPassID_scissor4,        permMemory, 512 );
            initRenderPass( renderer, renderPassID_scissor5,        permMemory, 512 );
            
            mat4 client_transform  = mat4_orthographic( 0.0f, 0.0f, client_dim.x, client_dim.y, 0.0f, 1.0f );
            
			uint32 audioBuffer_size = AUDIO__SAMPLES_PER_SECOND * AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT * AUDIO__BUFFER_SECONDS;
			LPDIRECTSOUNDBUFFER audioBuffer = {};
			HMODULE audioBuffer_lib = LoadLibrary( "dsound.dll" );
			if( audioBuffer_lib ) {
                DIRECT_SOUND_CREATE * DirectSoundCreate = ( DIRECT_SOUND_CREATE * )GetProcAddress( audioBuffer_lib, "DirectSoundCreate" );
                
                LPDIRECTSOUND DirectSound;
                if( DirectSoundCreate && SUCCEEDED( DirectSoundCreate( 0, &DirectSound, 0 ) ) ) {
                    WAVEFORMATEX format = {};
                    format.wFormatTag      = WAVE_FORMAT_PCM;
                    format.nChannels       = ( WORD )AUDIO__CHANNEL_COUNT;
                    format.nSamplesPerSec  = AUDIO__SAMPLES_PER_SECOND;
                    format.wBitsPerSample  = ( WORD )( AUDIO__BYTES_PER_SAMPLE * 8 );
                    format.nBlockAlign     = ( WORD )( AUDIO__CHANNEL_COUNT * AUDIO__BYTES_PER_SAMPLE );
                    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
                    
                    if( SUCCEEDED( DirectSound->SetCooperativeLevel( window, DSSCL_PRIORITY ) ) ) {
                        DSBUFFERDESC primaryDesc = { sizeof( DSBUFFERDESC ) };
                        primaryDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
                        
                        LPDIRECTSOUNDBUFFER primaryBuffer;
                        if(   SUCCEEDED( DirectSound->CreateSoundBuffer( &primaryDesc, &primaryBuffer, 0 ) )
                           && SUCCEEDED( primaryBuffer->SetFormat( &format ) ) ) {
                            OutputDebugString( "Primary Sound Buffer was created successfully.\n" );
                        }
                    }
                    
                    DSBUFFERDESC secondaryDesc = { sizeof( DSBUFFERDESC ) };
                    secondaryDesc.dwBufferBytes = audioBuffer_size;
                    secondaryDesc.lpwfxFormat = &format;
                    if( SUCCEEDED( DirectSound->CreateSoundBuffer( &secondaryDesc, &audioBuffer, 0 ) ) ) {
                        audioBuffer->Play( 0, 0, DSBPLAY_LOOPING );
                        OutputDebugString( "Secondary Sound Buffer was created successfully.\n" );
                    }
                }
			}
            
            int32 audio_bytesPerFrame   = ( AUDIO__SAMPLES_PER_SECOND / 60 ) * AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT;
            int32 audio_writeSize       = AUDIO__WRITE_FRAMES * audio_bytesPerFrame;
            audioSystem->mixBuffer      = ( uint8 * )_pushSize_clear( &win32_platform->permMemory, audio_writeSize );
            audioSystem->mixBuffer_size = audio_writeSize;
            
            musicSystem->mixBuffer      = audioSystem->mixBuffer;
            musicSystem->mixBuffer_size = audio_writeSize;
            
            DWORD audio_ignore    = 0;
            DWORD audio_prevWrite = 0;
            audioBuffer->GetCurrentPosition( &audio_ignore, &audio_prevWrite );
			
            { // gen default texture
                uint8 defaultTexture[] = { 80, 80, 80, 255,   255, 0, 255, 255,   255, 0, 255, 255,   80, 80, 80, 255 };
                ID3D11Texture2D          * texture_default  = loadTexture( renderer->device, 2, 2, &defaultTexture );
                ID3D11ShaderResourceView * texture_resource = loadShaderResource( renderer->device, texture_default );
                addResource( renderer, assetID_texture_default, texture_resource );
            }
            
            { // gen default font
                DEFAULT_FONT__TEXTURE defaultFont_texture = genDefaultFont( assetSystem, &win32_platform->tempMemory );
                ID3D11Texture2D          * fontTexture      = loadTexture( renderer->device, defaultFont_texture.width, defaultFont_texture.height, defaultFont_texture.data );
                ID3D11ShaderResourceView * texture_resource = loadShaderResource( renderer->device, fontTexture );
                addResource( renderer, assetID_font_default, texture_resource );
                _popArray( &win32_platform->tempMemory, uint32, defaultFont_texture.width * defaultFont_texture.height );
            }
            
            { // gen white texture
                uint8 whiteTexture[] = { 255, 255, 255, 255 };
                ID3D11Texture2D          * texture_whiteTexture = loadTexture       ( renderer->device, 1, 1, &whiteTexture );
                ID3D11ShaderResourceView * texture_resource     = loadShaderResource( renderer->device, texture_whiteTexture );
                addResource( renderer, assetID_texture_whiteTexture, texture_resource );
            }
            
            { // gen box texture
                MEMORY * tempMemory = &win32_platform->tempMemory;
                int32 width  = 100;
                int32 height = 100;
                uint32 * boxTexture = _pushArray_clear( tempMemory, uint32, width * height );
                for( int32 iter = 0; iter < width * height; iter++ ) { boxTexture[ iter ] = 0xFF7F7F7F; }
                for( int32 row = 1; row < height - 1; row++ ) {
                    for( int32 column = 1; column < width - 1; column++ ) {
                        boxTexture[ row * width + column ] = 0xFFFFFFFF;
                    }
                }
                ID3D11Texture2D          * texture_boxTexture = loadTexture       ( renderer->device, width, height, boxTexture );
                ID3D11ShaderResourceView * texture_resource   = loadShaderResource( renderer->device, texture_boxTexture );
                addResource( renderer, assetID_texture_boxTexture, texture_resource );
                _popArray( tempMemory, uint32, width * height );
            }
            
            // // TODO: determine texture_count
            // // for( int32 texture_index = 0; texture_index < 8; texture_index++ ) {
            // // ASSET_ID assetID = ( ASSET_ID )( assetID_texture_whiteTexture + texture_index );
            
            // // ASSET_ENTRY entry = assetSystem->entry[ assetID ];
            // // Assert( entry.size > 0 );
            
            // // ASSET_TEXTURE * texture = ( ASSET_TEXTURE * )( assetSystem->asset_data + entry.offset );
            // // ID3D11Texture2D          * d3d_texture  = loadTexture( renderer->device, texture->width, texture->height, ( texture + 1 ) );
            // // ID3D11ShaderResourceView * d3d_resource = loadShaderResource( renderer->device, d3d_texture );
            // // addResource( renderer, assetID, d3d_resource );
            // // }
			
#if DEBUG_BUILD
            globalVar_debugSystem.renderer    = renderer;
            globalVar_debugSystem.defaultFont = &assetSystem->defaultFont;
            globalVar_advanceHeight           = assetSystem->defaultFont.advanceHeight;
			
			int32 	 profileSystem_sizeInBytes = PROFILE_SYSTEM__SIZE_IN_BYTES;
			MEMORY	_profileSystem_memory = Memory( VirtualAlloc( 0, profileSystem_sizeInBytes, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE ), profileSystem_sizeInBytes );
			MEMORY *	 profileSystem_memory = &_profileSystem_memory;
			globalVar_profileSystem.getMSElapsed = win32_getMSElapsed;
			globalVar_profileSystem.eventDatabase.eventList 		= _pushArray( profileSystem_memory, PROFILE_EVENT_LIST, PROFILE_EVENT_DATABASE_MAX_COUNT );
			globalVar_profileSystem.eventDatabase.maxCount 			= PROFILE_EVENT_DATABASE_MAX_COUNT;
			globalVar_profileSystem.eventDatabase.selectIndex 		= -1;
			globalVar_profileSystem.eventList_currentFrame			= _pushType( profileSystem_memory, PROFILE_EVENT_LIST );
			globalVar_profileSystem.nodeList_currentFrame			= _pushType( profileSystem_memory, PROFILE_NODE_LIST );
			globalVar_profileSystem.eventList_currentFrame->frameClockStart = __rdtsc();
			
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.frameStart 	= __rdtsc();
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.platformStart	= __rdtsc();
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.appStart 		= __rdtsc();
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.renderStart 	= __rdtsc();
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.frameEnd 		= __rdtsc();
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.counter_frameStart = win32_getPerfCount();
			globalVar_profileSystem.eventDatabase_clocks_currentFrame.counter_frameEnd   = win32_getPerfCount();
			
			globalVar_profileSystem.eventDatabase_clocks = _pushArray( profileSystem_memory, PROFILE_EVENT_DATABASE_CLOCKS, PROFILE_EVENT_DATABASE_MAX_COUNT );
			
			globalVar_profileSystem.eventDatabase_posOffset 				= 4.0f;
			globalVar_profileSystem.eventDatabase_width 						= client_width - 2.0f * globalVar_profileSystem.eventDatabase_posOffset;
			globalVar_profileSystem.eventDatabase_height 					= 300.0f;
			globalVar_profileSystem.eventDatabase_scrollActive 			= false;
			globalVar_profileSystem.frameBreakdown_scrollActive_bar 		= false;
			globalVar_profileSystem.frameBreakdown_scrollActive_left 	= false;
			globalVar_profileSystem.frameBreakdown_scrollActive_right 	= false;
			globalVar_profileSystem.frameBreakdown_scrollX_left  			= globalVar_profileSystem.eventDatabase_posOffset;
			globalVar_profileSystem.frameBreakdown_scrollX_right 			= client_width - globalVar_profileSystem.eventDatabase_posOffset;
#endif
			
			MOUSE_STATE    * mouse        = &win32_platform->mouse;
			KEYBOARD_STATE * keyboard     = &win32_platform->keyboard;
            CONTROLLER_STATE * controller = &win32_platform->controller;
            
            HMODULE XInputLib = LoadLibraryA( "xinput1_3.dll" );
            XINPUT_GET_STATE * XInputGetState = 0;
            XINPUT_SET_STATE * XInputSetState = 0;
            if( XInputLib ) {
                XInputGetState = ( XINPUT_GET_STATE * )GetProcAddress( XInputLib, "XInputGetState" );
                XInputSetState = ( XINPUT_SET_STATE * )GetProcAddress( XInputLib, "XInputSetState" );
            } else {
                InvalidCodePath;
            }
            
            boo32 showEditor = true;
            // showEditor = false;
            while( appState->isRunning ) {
#if DEBUG_BUILD
				globalVar_profileSystem.eventDatabase_clocks_previousFrame 							= globalVar_profileSystem.eventDatabase_clocks_currentFrame;
				globalVar_profileSystem.eventDatabase_clocks_currentFrame.frameStart 		  	= __rdtsc();
				globalVar_profileSystem.eventDatabase_clocks_currentFrame.counter_frameStart 	= win32_getPerfCount();
				globalVar_profileSystem.eventList_currentFrame->frameClockStart 					= globalVar_profileSystem.eventDatabase_clocks_currentFrame.frameStart;
                
				globalVar_atPos = getTL( client_bound ) + Vec2( 10.0f, -10.0f );
				
                if( wasPressed( keyboard, keyCode_p ) ) {
                    globalVar_profileSystem.showProfile = !globalVar_profileSystem.showProfile;
                }
                
				updateProfile( &globalVar_profileSystem, win32_platform, *( ( int64 * )&perfFrequency ), globalVar_debugSystem.renderer, client_bound );
#endif // DEBUG_BUILD
				win32_processPendingMessages( win32_platform, window );
                
                XINPUT_STATE controller_state  = {};
                DWORD        controller_status = XInputGetState( 0, &controller_state );
                boo32        controller_buttonDown[ 14 ] = {};
                for( uint32 controllerButton_index = 0; controllerButton_index < controllerButton_count; controllerButton_index++ ) {
                    controller->button[ controllerButton_index ].halfTransitionCount = 0;
                }
                
                int16  leftThumbX = 0;
                int16  leftThumbY = 0;
                int16 rightThumbX = 0;
                int16 rightThumbY = 0;
                
                if( controller_status == ERROR_SUCCESS ) {
                    updateButton( controller, controllerButton_dPad_up,        ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP        ) );
                    updateButton( controller, controllerButton_dPad_down,      ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN      ) );
                    updateButton( controller, controllerButton_dPad_left,      ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT      ) );
                    updateButton( controller, controllerButton_dPad_right,     ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT     ) );
                    updateButton( controller, controllerButton_start,          ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_START          ) );
                    updateButton( controller, controllerButton_back,           ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK           ) );
                    updateButton( controller, controllerButton_thumb_left,     ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB     ) );
                    updateButton( controller, controllerButton_thumb_right,    ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB    ) );
                    updateButton( controller, controllerButton_shoulder_left,  ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER  ) );
                    updateButton( controller, controllerButton_shoulder_right, ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) );
                    updateButton( controller, controllerButton_A,              ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_A              ) );
                    updateButton( controller, controllerButton_B,              ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_B              ) );
                    updateButton( controller, controllerButton_X,              ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_X              ) );
                    updateButton( controller, controllerButton_Y,              ( controller_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y              ) );
                    
                    leftThumbX  = controller_state.Gamepad.sThumbLX;
                    leftThumbY  = controller_state.Gamepad.sThumbLY;
                    rightThumbX = controller_state.Gamepad.sThumbRX;
                    rightThumbY = controller_state.Gamepad.sThumbRY;
                }
                
                // // DISPLAY_VALUE( int16, leftThumbX );
                // // DISPLAY_VALUE( int16, leftThumbY );
                
                appState->input_ratio = Vec2( 0.0f, 0.0f );
                appState->input_speed = 0;
                if( ( leftThumbX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) || ( leftThumbX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ||
                   ( leftThumbY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) || ( leftThumbY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ) ) {
                    int16 input_range = 3000;
                    if( leftThumbX < -input_range ) { appState->input_ratio.x = ( flo32 )( leftThumbX + input_range ) / ( flo32 )( 32768 - input_range ); }
                    if( leftThumbX >  input_range ) { appState->input_ratio.x = ( flo32 )( leftThumbX - input_range ) / ( flo32 )( 32767 - input_range ); }
                    if( leftThumbY < -input_range ) { appState->input_ratio.y = ( flo32 )( leftThumbY + input_range ) / ( flo32 )( 32768 - input_range ); }
                    if( leftThumbY >  input_range ) { appState->input_ratio.y = ( flo32 )( leftThumbY - input_range ) / ( flo32 )( 32767 - input_range ); }
                    
                    flo32 xSpeedSq = ( flo32 )leftThumbX * ( flo32 )leftThumbX;
                    flo32 ySpeedSq = ( flo32 )leftThumbY * ( flo32 )leftThumbY;
                    flo32 speed = sqrtf( xSpeedSq + ySpeedSq );
                    // DISPLAY_VALUE( flo32, speed );
                    if( ( speed > 0.0f     ) && ( speed <= 18000.0f ) ) { appState->input_speed = 1; }
                    if( ( speed > 18000.0f ) && ( speed <= 26000.0f ) ) { appState->input_speed = 2; }
                    if( ( speed > 26000.0f ) && ( speed <= 30000.0f ) ) { appState->input_speed = 3; }
                    if(   speed > 30000.0f                            ) { appState->input_speed = 4; }
                    
                    flo32 xSpeed = sqrtf( xSpeedSq );
                    if( ( xSpeed > 0.0f     ) && ( xSpeed <= 18000.0f ) ) { appState->input_speedX = 1; }
                    if( ( xSpeed > 18000.0f ) && ( xSpeed <= 26000.0f ) ) { appState->input_speedX = 2; }
                    if( ( xSpeed > 26000.0f ) && ( xSpeed <= 30000.0f ) ) { appState->input_speedX = 3; }
                    if(   xSpeed > 30000.0f                             ) { appState->input_speedX = 4; }
                    
                    flo32 ySpeed = sqrtf( ySpeedSq );
                    if( ( ySpeed > 0.0f     ) && ( ySpeed <= 18000.0f ) ) { appState->input_speedY = 1; }
                    if( ( ySpeed > 18000.0f ) && ( ySpeed <= 26000.0f ) ) { appState->input_speedY = 2; }
                    if( ( ySpeed > 26000.0f ) && ( ySpeed <= 30000.0f ) ) { appState->input_speedY = 3; }
                    if(   ySpeed > 30000.0f                             ) { appState->input_speedY = 4; }
                }
                
                appState->camera_left  = false;
                appState->camera_right = false;
                appState->camera_up    = false;
                appState->camera_down  = false;
                if( rightThumbX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) { appState->camera_left     = true; }
                if( rightThumbX >  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) { appState->camera_right    = true; }
                if( rightThumbY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) { appState->camera_up       = true; }
                if( rightThumbY >  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) { appState->camera_down     = true; }
                
                if( rightThumbX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) {
                    appState->camera_horRatio = ( flo32 )( rightThumbX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / ( flo32 )( 32768 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
                }
                if( rightThumbX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) {
                    appState->camera_horRatio = ( flo32 )( rightThumbX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / ( flo32 )( 32768 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
                }
                if( rightThumbY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) {
                    appState->camera_verRatio = ( flo32 )( rightThumbY + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / ( flo32 )( 32768 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
                }
                if( rightThumbY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) {
                    appState->camera_verRatio = ( flo32 )( rightThumbY - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ) / ( flo32 )( 32768 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
                }
                //appState->camera_verRatio *= -1.0f;
                
                for( int32 button_index = 0; button_index < 14; button_index++ ) {
                    if( controller_buttonDown[ button_index ] ) {
                        // DISPLAY_VALUE( int32, button_index );
                    }
                }
				
                mat4 main_transform = mat4_identity();
				POINT mouse_position = {};
				GetCursorPos( &mouse_position );
                mouse->position = Vec2( ( flo32 )mouse_position.x, ( flo32 )mouse_position.y );
                
#if DEBUG_BUILD
				globalVar_profileSystem.eventDatabase_clocks_currentFrame.appStart = __rdtsc();
				if( globalVar_profileSystem.showProfile ) {
                    ScreenToClient( window, &mouse_position );
                    mouse->position = Vec2( ( flo32 )mouse_position.x, ( flo32 )( client_height - 1 - mouse_position.y ) );
                } else {
#endif // DEBUG_BUILD
                    if( wasPressed( keyboard, keyCode_F9 ) ) {
                        if( showEditor ) {
                            appState->spawn_useSpawnPoint = editorState->spawn_useSpawnPoint;
                            appState->spawn_spawnPoint    = editorState->spawn_spawnPoint;
                        } else {
                            editorState->modCamera_maxDist  = appState->modCamera_maxDist;
                            editorState->modCamera_hOffset  = appState->modCamera_hOffset;
                            editorState->modCamera_vOffset  = appState->modCamera_vOffset;
                            editorState->modCamera_fovAngle = appState->modCamera_fovAngle;
                        }
                        showEditor = !showEditor;
                    }
                    if( showEditor ) {
                        ScreenToClient( window, &mouse_position );
                        mouse->position = Vec2( ( flo32 )mouse_position.x, ( flo32 )( client_height - 1 - mouse_position.y ) );
                        
                        editorState->mouse_position = mouse->position;
                        editorState->mouseLeft_wasPressed    = wasPressed ( mouse, mouseButton_left   );
                        editorState->mouseLeft_wasReleased   = wasReleased( mouse, mouseButton_left   );
                        editorState->mouseRight_wasPressed   = wasPressed ( mouse, mouseButton_right  );
                        editorState->mouseRight_wasReleased  = wasReleased( mouse, mouseButton_right  );
                        editorState->mouseMiddle_wasPressed  = wasPressed ( mouse, mouseButton_middle );
                        editorState->mouseMiddle_wasReleased = wasReleased( mouse, mouseButton_middle );
                        editorState->mouseMiddle_wheelClicks = mouse->wheelClicks;
                        
                        updateEditor( editorState, win32_platform );
                        
                        main_transform = genCameraTransform( &editorState->camera, client_dim );
                    } else {
                        ScreenToClient( window, &mouse_position );
                        mouse->position = Vec2( ( flo32 )mouse_position.x, ( flo32 )( client_height - 1 - mouse_position.y ) );
                        appState->mouse_position = mouse->position;
                        appState->mouseLeft_wasPressed    = wasPressed ( mouse, mouseButton_left   );
                        appState->mouseLeft_wasReleased   = wasReleased( mouse, mouseButton_left   );
                        appState->mouseRight_wasPressed   = wasPressed ( mouse, mouseButton_right  );
                        appState->mouseRight_wasReleased  = wasReleased( mouse, mouseButton_right  );
                        appState->mouseMiddle_wasPressed  = wasPressed ( mouse, mouseButton_middle );
                        appState->mouseMiddle_wasReleased = wasReleased( mouse, mouseButton_middle );
                        appState->mouseMiddle_wheelClicks = mouse->wheelClicks;
                        
                        PLAYER_CONTROL * playerControl = &appState->playerControl;
                        // if( wasPressed ( keyboard, keyCode_a ) ) { playerControl->move_left     = true;  }
                        // if( wasReleased( keyboard, keyCode_a ) ) { playerControl->move_left     = false; }
                        // if( wasPressed ( keyboard, keyCode_d ) ) { playerControl->move_right    = true;  }
                        // if( wasReleased( keyboard, keyCode_d ) ) { playerControl->move_right    = false; }
                        // if( wasPressed ( keyboard, keyCode_w ) ) { playerControl->move_forward  = true;  }
                        // if( wasReleased( keyboard, keyCode_w ) ) { playerControl->move_forward  = false; }
                        // if( wasPressed ( keyboard, keyCode_s ) ) { playerControl->move_backward = true;  }
                        // if( wasReleased( keyboard, keyCode_s ) ) { playerControl->move_backward = false; }
                        
                        if( wasPressed ( keyboard, keyCode_left  ) ) { appState->camera_moveLeft     = true;  }
                        if( wasReleased( keyboard, keyCode_left  ) ) { appState->camera_moveLeft     = false; }
                        if( wasPressed ( keyboard, keyCode_right ) ) { appState->camera_moveRight    = true;  }
                        if( wasReleased( keyboard, keyCode_right ) ) { appState->camera_moveRight    = false; }
                        if( wasPressed ( keyboard, keyCode_up    ) ) { appState->camera_moveForward  = true;  }
                        if( wasReleased( keyboard, keyCode_up    ) ) { appState->camera_moveForward  = false; }
                        if( wasPressed ( keyboard, keyCode_down  ) ) { appState->camera_moveBackward = true;  }
                        if( wasReleased( keyboard, keyCode_down  ) ) { appState->camera_moveBackward = false; }
                        
                        // playerControl->move_left     = appState->moveLeft;
                        // playerControl->move_right    = appState->moveRight;
                        // playerControl->move_backward = appState->moveBackward;
                        // playerControl->move_forward  = appState->moveForward;
                        
                        playerControl->jump_wasPressed  = wasPressed ( mouse, mouseButton_right );
                        playerControl->jump_wasReleased = wasReleased( mouse, mouseButton_right );
                        
                        playerControl->crouch_wasPressed  = wasPressed ( keyboard, keyCode_space );
                        playerControl->crouch_wasReleased = wasReleased( keyboard, keyCode_space );
                        
                        playerControl->jump_wasPressed    = wasPressed ( controller, controllerButton_A );
                        playerControl->jump_wasReleased   = wasReleased( controller, controllerButton_A );
                        playerControl->crouch_wasPressed  = wasPressed ( controller, controllerButton_B );
                        playerControl->crouch_wasReleased = wasReleased( controller, controllerButton_B );
                        playerControl->respawn_wasPressed = wasPressed ( controller, controllerButton_Y );
                        playerControl->respawn_wasReleased = wasReleased( controller, controllerButton_Y );
                        
                        APP_updateAndRender( appState, win32_platform );
                        
                        //if( global_mouseFocus ) {
                        //POINT center = { ( LONG )app_halfDim.x, ( LONG )app_halfDim.y };
                        //SetCursorPos( center.x, center.y );
                        //}
                        
                        vec3 camera_position  = appState->camera_position + appState->camera_zAxis * appState->camera_dist;
                        camera_position += appState->camera_xAxis * appState->camera_hOffset;
                        camera_position += appState->camera_yAxis * appState->camera_vOffset;
                        
                        mat4 main_camera      = genCameraTransform( camera_position, appState->camera_xAxis, appState->camera_yAxis, appState->camera_zAxis );
                        mat4 main_perspective = mat4_perspective( client_dim, appState->camera_fovAngle, 0.001f, 2000.0f );
                        main_transform   = main_perspective * main_camera;
                    }
#if DEBUG_BUILD
                }
				globalVar_profileSystem.eventDatabase_clocks_currentFrame.renderStart = __rdtsc();
#endif // DEBUG_BUILD
                
                void * audio_regionA = 0;
                void * audio_regionB = 0;
                DWORD audio_sizeA = 0;
                DWORD audio_sizeB = 0;
                DWORD audio_currRead  = 0;
                DWORD audio_currWrite = 0;
                if( ( audioBuffer->GetCurrentPosition( &audio_currRead, &audio_currWrite ) == DS_OK ) &&
                   ( audio_currWrite != audio_prevWrite ) &&
                   ( audioBuffer->Lock( audio_currWrite, audio_writeSize, &audio_regionA, &audio_sizeA, &audio_regionB, &audio_sizeB, 0 ) == DS_OK ) ) {
                    uint32   bytesToAdvance = minValue( ( ( audio_currWrite - audio_prevWrite ) + audioBuffer_size ) % audioBuffer_size, audio_writeSize );
                    uint32 samplesToAdvance = bytesToAdvance / ( AUDIO__BYTES_PER_SAMPLE * AUDIO__CHANNEL_COUNT );
                    
                    memset( audioSystem->mixBuffer, 0, audioSystem->mixBuffer_size );
                    // #if DEBUG_BUILD
                    // if( !globalVar_profileSystem.showProfile ) {
                    // #endif // DEBUG_BUILD
                    advanceMusic( musicSystem, samplesToAdvance );
                    advanceAudio( audioSystem, samplesToAdvance );
                    
                    mixMusic( musicSystem, samplesToAdvance );
                    mixAudio( audioSystem, samplesToAdvance );
                    // #if DEBUG_BUILD
                    // }
                    // #endif // DEBUG_BUILD
                    memcpy( audio_regionA, audioSystem->mixBuffer,               audio_sizeA );
                    memcpy( audio_regionB, audioSystem->mixBuffer + audio_sizeA, audio_sizeB );
                    
                    audioBuffer->Unlock( audio_regionA, audio_sizeA, audio_regionB, audio_sizeB );
                    audio_prevWrite = audio_currWrite;
                }
				
                if( wasPressed( &win32_platform->keyboard, keyCode_F4, KEYBOARD_FLAGS__ALT ) ) {
                    win32_endApplication( win32_platform, appState );
                }
                
                clearRender( renderer, COLOR_WHITE );
                
                setShader( renderer, shaderID_main_lighting, vertexBufferID_vertex1 );
                setDepth( renderer, true );
                setBlend( renderer, true );
                pushVertices( renderer, 1 );
                
                setCamera( renderer, main_transform );
                renderPass( renderer, renderPassID_main );
                
                setShader ( renderer, shaderID_main_noLighting, vertexBufferID_vertex1 );
                renderPass( renderer, renderPassID_main_noLighting );
                
                setDepth( renderer, false );
                renderPass( renderer, renderPassID_main_debug );
                
                setShader( renderer, shaderID_debug, vertexBufferID_debug );
                setBlend( renderer, true );
                pushVertices( renderer, 0 );
                
                mat4 camera_transform = mat4_orthographic( client_bound.left, client_bound.bottom, client_bound.right, client_bound.top, 0.0f, 1.0f );
                setCamera( renderer, camera_transform );
				
                renderPass( renderer, renderPassID_overlay );
                
                // enableScissor( renderer, client_bound, addDim( editorState->pointPanel.bound, 1.0f, 0.0f, 0.0f, 1.0f ) );
                // renderPass   ( renderer, renderPassID_scissor0 );
                
                // enableScissor( renderer, client_bound, addDim( editorState->facePanel.bound,  1.0f, 0.0f, 0.0f, 1.0f ) );
                // renderPass   ( renderer, renderPassID_scissor1 );
                
                // enableScissor( renderer, client_bound, addDim( editorState->cornerPanel.bound,  1.0f, 0.0f, 0.0f, 1.0f ) );
                // renderPass   ( renderer, renderPassID_scissor2 );
                
                // enableScissor( renderer, client_bound, addDim( editorState->overhangPanel.bound,  1.0f, 0.0f, 0.0f, 1.0f ) );
                // renderPass   ( renderer, renderPassID_scissor3 );
                
                // enableScissor( renderer, client_bound, addDim( editorState->ledgePanel.bound,  1.0f, 0.0f, 0.0f, 1.0f ) );
                // renderPass   ( renderer, renderPassID_scissor4 );
                
                // enableScissor( renderer, client_bound, addDim( editorState->polePanel.bound,  1.0f, 0.0f, 0.0f, 1.0f ) );
                // renderPass   ( renderer, renderPassID_scissor5 );
#if DEBUG_BUILD
                disableScissor( renderer );
                renderPass( renderer, renderPassID_debug );
#endif // DEBUG_BUILD
                
                presentRender( renderer );
				
#if DEBUG_BUILD
				globalVar_profileSystem.eventDatabase_clocks_currentFrame.frameEnd 			= __rdtsc();
				globalVar_profileSystem.eventDatabase_clocks_currentFrame.counter_frameEnd = win32_getPerfCount();
				
				if( wasPressed( &win32_platform->keyboard, keyCode_b ) ) {
                    InvalidCodePath;
				}
#endif // DEBUG_BUILD
			}
            
			audioBuffer->Stop();
            end( renderer );
		}
	}
    
	return 0;
}

#endif	// STD_INCLUDE_FUNC